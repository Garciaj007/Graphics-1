#version 410

out vec4 fragColor;

uniform vec2 resolution;
uniform vec2 mouse;
uniform float time;

float smin(float a, float b, float k)
{
	float h = clamp(0.5 + 0.5*(b - a) / k, 0.0, 1.0);
	return mix(b, a, h) - k * h * (1.0-h);
}

float map(vec2 coord)
{
	vec2 mouseN = mouse / resolution;

	float circ1 = distance(coord, mouseN)-0.1;
	float circ2 = distance(coord, vec2(0.5, 0.7)) - 0.1;
	float rect = length(max(abs(coord - vec2(0.5, 0.4)) - vec2(0.1, 0.1), 0.0));

	return smin(smin(rect, circ2, 0.1), circ1, 0.1);
}

float circle(vec2 coord, vec2 center, float radius)
{
	return 1.0 - smoothstep(radius - 0.02, radius + 0.02, distance(coord, center));
}

void main ()
{
	vec2 coord = gl_FragCoord.xy / resolution;
	
	vec3 colorA = vec3(0.8, 0.2, 0.2);
	vec3 colorB = vec3(0.2, 0.8, 0.2);
	vec3 colorC = vec3(0.2, 0.8, 0.8);
	vec3 colorD = vec3(0.2, 0.8, 0.8);

	vec3 color = colorA * step(0.0, -map(coord));
	color += fract(max(0.0, map(coord)* 30.0));

	fragColor = vec4(color, 1.0);
}