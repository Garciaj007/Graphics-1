#version 410

layout (location = 0) in  vec4 vVertex;
layout (location = 1) in  vec4 vNormal;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

void main() 
{
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vVertex;
}
