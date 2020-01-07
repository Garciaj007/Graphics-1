#version 410

out vec4 fragColor;

uniform vec2 resolution;
uniform vec2 mouse;
uniform float time;

#define EPSILON 0.00001

float SDFSphere(vec3 p, float radius)
{
	return length(p) - radius;
}

float SDFBox(vec3 p, vec3 b)
{
	vec3 q = abs(p) - b;
	return length(max(q, 0.0)) + min(max(q.x, max(q.y, q.z)), 0.0);
}

float SDFPlane(vec3 p, vec4 n)
{
	vec4 norm = normalize(n);
	return dot(p, n.xyz) + n.w;
}

float SDFWaves(vec3 p, vec2 d)
{
	return p.y + d.x * sin(-time + length(p.xz) * d.y); 
}

float SDFHexPrism( vec3 p, vec2 h )
{
  const vec3 k = vec3(-0.8660254, 0.5, 0.57735);
  p = abs(p);
  p.xy -= 2.0*min(dot(k.xy, p.xy), 0.0)*k.xy;
  vec2 d = vec2(
       length(p.xy-vec2(clamp(p.x,-k.z*h.x,k.z*h.x), h.x))*sign(p.y-h.x),
       p.z-h.y );
  return min(max(d.x,d.y),0.0) + length(max(d,0.0));
}

float SDFTriPrism( vec3 p, vec2 h )
{
    const float k = sqrt(3.0);
    h.x *= 0.5*k;
    p.xy /= h.x;
    p.x = abs(p.x) - 1.0;
    p.y = p.y + 1.0/k;
    if( p.x+k*p.y>0.0 ) p.xy=vec2(p.x-k*p.y,-k*p.x-p.y)/2.0;
    p.x -= clamp( p.x, -2.0, 0.0 );
    float d1 = length(p.xy)*sign(-p.y)*h.x;
    float d2 = abs(p.z)-h.y;
    return length(max(vec2(d1,d2),0.0)) + min(max(d1,d2), 0.);
}

float SDFCappedCylinder( vec3 p, float h, float r )
{
  vec2 d = abs(vec2(length(p.xz),p.y)) - vec2(h,r);
  return min(max(d.x,d.y),0.0) + length(max(d,0.0));
}

float SDFTorus( vec3 p, vec2 t )
{
  vec2 q = vec2(length(p.xz)-t.x,p.y);
  return length(q)-t.y;
}

float SDFOctahedron( vec3 p, float s)
{
  p = abs(p);
  float m = p.x+p.y+p.z-s;
  vec3 q;
       if( 3.0*p.x < m ) q = p.xyz;
  else if( 3.0*p.y < m ) q = p.yzx;
  else if( 3.0*p.z < m ) q = p.zxy;
  else return m*0.57735027;
    
  float k = clamp(0.5*(q.z-q.y+s),0.0,s); 
  return length(vec3(q.x,q.y-s+k,q.z-k)); 
}

float SDFLink( vec3 p, float le, float r1, float r2 )
{
  vec3 q = vec3( p.x, max(abs(p.y)-le,0.0), p.z );
  return length(vec2(length(q.xy)-r1,q.z)) - r2;
}

float SDFOpRound(float prim, float r)
{
	return prim - r;
}

float SDFOpUnion( float d1, float d2 ) { return min(d1,d2); }

float SDFOpSubtraction( float d1, float d2 ) { return max(-d1,d2); }

float SDFOpIntersection( float d1, float d2 ) { return max(d1,d2); }

float SDFOpSmoothUnion( float d1, float d2, float k ) {
    float h = clamp( 0.5 + 0.5*(d2-d1)/k, 0.0, 1.0 );
    return mix( d2, d1, h ) - k*h*(1.0-h); }

float SDFOpSmoothSubtraction( float d1, float d2, float k ) {
    float h = clamp( 0.5 - 0.5*(d2+d1)/k, 0.0, 1.0 );
    return mix( d2, -d1, h ) + k*h*(1.0-h); }

float SDFOpSmoothIntersection( float d1, float d2, float k ) {
    float h = clamp( 0.5 - 0.5*(d2-d1)/k, 0.0, 1.0 );
    return mix( d2, d1, h ) + k*h*(1.0-h); }

vec3 SDFOpRepetition( in vec3 p, in vec3 c )
{
    return mod(p+0.5*c,c)-0.5*c;
}

float smin(float a, float b, float k)
{
	float h = clamp(0.5 + 0.5*(b - a) / k, 0.0, 1.0);
	return mix(b, a, h) - k * h * (1.0-h);
}

//NO SCALE!
vec3 transform(in vec3 p, in mat4 t)
{
	return vec3(t * vec4(p, 1.0));
}

float map(vec3 p)
{
	float box = SDFOpRound(SDFOctahedron(vec3(p.x, p.y + sin(time) * 1.5, p.z), 1.0), 0.1);
	float waves = SDFWaves(p, vec2(0.05, 10));
	return smin(waves, box, 0.5);
}

float raymarch(vec3 origin, vec3 direction)
{
	float dist = 0.0;
	for(int i = 0; i < 256; i++)
	{
		vec3 p = origin + direction * dist;
		float d = map(p);
		if(d <= 0.0)
			break;
		dist += d;
	}
	return dist;
}

vec3 normal(vec3 p)
{
	return normalize(vec3(
	map(vec3(p.x + EPSILON, p.y, p.z)) - map(vec3(p.x - EPSILON, p.y, p.z)),
	map(vec3(p.x, p.y + EPSILON, p.z)) - map(vec3(p.x, p.y - EPSILON, p.z)),
	map(vec3(p.x, p.y, p.z + EPSILON)) - map(vec3(p.x, p.y, p.z - EPSILON))
	));
}

void main ()
{
	vec2 coord = 2.0 * gl_FragCoord.xy / resolution - vec2(1.0);
	coord.y *= resolution.y / resolution.x;
	vec2 mouseN = mouse / resolution;

	vec3 light = vec3(sin(time) * 5.0, 1.0, -2.0 + cos(time) * 4.0);

	vec3 direction = normalize(vec3(coord, 1.0));
	vec3 origin = vec3(0.0, 1.0, -5.0);
	float dist = raymarch(origin, direction);

	vec3 p = origin + dist * direction;
	vec3 norm = normal(p);
	vec3 reflection = direction - 2.0 * dot(direction, norm) * norm;

	vec3 color = vec3(0.2, 0.6, 1.0) * max(0.0, dot(norm, normalize(light - p)));
	float specular = pow(max(0.0, dot(reflection, normalize(light - p))), 14.0);
	vec3 ambient = vec3(0.1, 0.2, 0.3);

	color += specular * vec3(1.0) + ambient;

	fragColor = vec4(color, 1.0);
}