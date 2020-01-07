#version 410

out vec4 fragColor;

uniform vec2 resolution;
uniform vec2 mouse;
uniform float time;

#define EPSILON 0.00001

float SDFSphere(vec3 p, float radius)
{
	return distance(p, vec3(0.0)) - radius;
}

float SDFBox(vec3 p, float b)
{
	return length(max(abs(p) - vec3(b), 0.0));
}

float RoundSDFBox(vec3 p, float b, float r)
{
	return length(max(abs(p) - vec3(b), 0.0)) - r;
}

float smin(float a, float b, float k)
{
	float h = clamp(0.5 + 0.5*(b - a) / k, 0.0, 1.0);
	return mix(b, a, h) - k * h * (1.0-h);
}

float map(vec3 p)
{
	float box = RoundSDFBox(p, sin(time) * 0.5, 0.1);
	float plane = p.y + 0.05 * sin(-time+length(p.xz) * 10);
	return smin(plane, box, 0.1);
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

	vec3 light = vec3(sin(time) * 5.0, 10.0, -2.0 + cos(time) * 4.0);

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