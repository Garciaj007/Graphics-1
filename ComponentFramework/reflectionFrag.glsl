#version 410
out vec4 fragColor;

in vec3 normal;
in vec3 position;

uniform samplerCube skybox;
uniform mat4 viewMatrix;

void main () 
{
	vec3 I = normalize(position);
	vec3 N = normalize(normal);
	vec3 R = reflect(I, N);
	R = vec3(inverse(viewMatrix) * vec4(R, 0.0));
	fragColor = texture(skybox, R);
}