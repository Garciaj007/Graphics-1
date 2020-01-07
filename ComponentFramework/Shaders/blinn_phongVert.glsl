#version 410
layout (location = 0) in  vec4 vVertex;
layout (location = 1) in  vec4 vNormal;

out vec3 vertNormal;
out vec3 lightDir0;
out vec3 lightDir1;
out vec3 vertDir; 
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform mat3 normalMatrix;
uniform vec3 lightPos0;
uniform vec3 lightPos1;

void main() {
	vertNormal = normalMatrix * vNormal.xyz; /// Rotate the normal to the correct orientation 
	vec3 vertPos = vec3(viewMatrix * modelMatrix * vVertex); /// Creates the vertex position (-eyeDir) 
	lightDir0 = normalize(lightPos0 +  -vertPos); /// Create the light direction 
	lightDir1 = normalize(lightPos1 + -vertPos);
	vertDir = normalize(vertPos);
	gl_Position =  projectionMatrix * viewMatrix * modelMatrix * vVertex; 
}
