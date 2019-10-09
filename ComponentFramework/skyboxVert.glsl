#version 410
layout (location = 0) in vec4 vVertex;

out vec3 texCoords;

uniform mat4 modelMatrix;
uniform mat4 projectionMatrix;

void main()
{
    texCoords = vec3(vVertex);
    gl_Position = projectionMatrix * modelMatrix * vVertex;
}