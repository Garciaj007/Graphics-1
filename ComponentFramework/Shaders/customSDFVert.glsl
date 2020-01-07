#version 410
layout (location = 0) in vec4 vVertex;
layout (location = 1) in vec4 vNormal;
layout (location = 2) in vec2 vTexCoords;

out vec3 worldPosition;
out vec3 cameraPosition;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform vec3 cameraLocation;

void main()
{
	cameraPosition = inverse(viewMatrix)[0].xyz;
	//cameraPosition = cameraLocation;
	worldPosition = vec3(modelMatrix * vVertex);
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vVertex;
}