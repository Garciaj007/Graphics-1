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
uniform float time;
uniform sampler3D perlinNoise;

void main()
{
	vec3 offset = sin(time) * vVertex.xyz;
	vec4 perlinVertex = texture(perlinNoise, offset);
	vec4 newVertex = mix(vVertex, perlinVertex, 0.5);
	position = vec3(viewMatrix * modelMatrix * newVertex);
	normal = vec3(viewMatrix * modelMatrix * vec4(vNormal.xyz, 0.0));
	texCoords = vTexCoords;
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * newVertex;
}