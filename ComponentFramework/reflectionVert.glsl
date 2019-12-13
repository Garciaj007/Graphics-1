#version 410
layout (location = 0) in vec4 vVertex;
layout (location = 1) in vec4 vNormal;

out vec3 normal; 
out vec3 position;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

void main()
{
	position = vec3(viewMatrix * modelMatrix * vVertex);
	normal = vec3(viewMatrix * modelMatrix * vec4(vNormal.xyz, 0.0));
	//normal = vec3(viewMatrix * modelMatrix * vNormal);
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vVertex;
}