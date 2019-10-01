#version 410
layout (location = 0) in  vec4 vVertex;

out vec3 texCoords;

uniform mat4 modelMatrix;
unifomr mat4 projectionMatrix;

void main()
{
    TexCoords = vec3(aPos);
    gl_Position = projectionMatrix * modelMatrix * aPos;
}