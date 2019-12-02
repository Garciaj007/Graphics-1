#version 410
out vec4 fragColor;

in vec3 normal;
in vec3 position;

uniform samplerCube skybox;

void main () 
{
	vec3 direction = normalize(position);
	vec3 R = reflect(-direction, normal);
	fragColor = vec4(texture(skybox, R).rgb, -1.0);
}