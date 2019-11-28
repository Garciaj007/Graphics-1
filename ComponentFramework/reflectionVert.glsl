#version 410
layout (location = 0) in vec4 vVertex;
layout (location = 1) in vec4 vNormal;

out vec3 normal; 
out vec3 position;

uniform mat3 normalMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

void main()
{
	normal = normalMatrix * vNormal.xyz;
	position = vec3(modelMatrix * vVertex);
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vVertex;
}