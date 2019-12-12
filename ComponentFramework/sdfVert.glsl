#version 410
layout (location = 0) in vec4 vVertex;
layout (location = 1) in vec4 vNormal;
layout (location = 2) in vec2 vTexCoords;

out vec3 normal; 
out vec3 position;
out vec2 texCoords;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

void main()
{
	position = vec3(viewMatrix * modelMatrix * vVertex);
	normal = vec3(viewMatrix * modelMatrix * vec4(vNormal.xyz, 0.0));
	texCoords = vTexCoords;
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vVertex;
}