#version 410

out vec4 fragColor;

uniform vec2 resolution;
uniform vec2 mouse;
uniform float time;

void main ()
{
	vec2 coord = gl_FragCoord.xy / resolution;
	vec2 mouseN = mouse / resolution; 
	fragColor = vec4(mouseN.x, coord.y, sin(time * 2.0) / 2 + 0.5,1);
}