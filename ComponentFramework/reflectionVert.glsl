#version 410
layout (location = 0) in vec4 vVertex;

out vec3 Normal; 
out vec3 Position;

uniform mat4 normalMatrix;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{
	Normal = vec3(normalMatrix * vVertex);
	Position = vec3(modelMatrix * vVertex);
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vVertex;
}