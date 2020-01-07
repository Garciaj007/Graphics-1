#version 410
layout (location = 0) in vec4 vVertex;

out vec3 texCoords;

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{
    texCoords = vec3(-vVertex.x, vVertex.yz);
    gl_Position = projectionMatrix * mat4(mat3(viewMatrix)) * vVertex;
}