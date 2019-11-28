#version 410
out vec4 FragColor;

in vec3 Normal;
in vec3 Position;

uniform vec3 cameraPos;
uniform samplerCube skybox;

void main () 
{
	vec3 incidence = normalize(Position - cameraPos);
	vec3 reflection = reflect(incidence, normalize(Normal));
	FragColor = vec4(texture(skybox, reflection).rgb, 1.0);
}