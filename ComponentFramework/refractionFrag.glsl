#version 410
out vec4 fragColor;

in vec3 normal;
in vec3 position;

uniform samplerCube skybox;

void main () 
{
	float ratio = 1.00/ 1.52;
	vec3 direction = normalize(position);
	vec3 R = refract(-direction, normal, ratio);
	fragColor = vec4(texture(skybox, R).rgb, 1.0);
}