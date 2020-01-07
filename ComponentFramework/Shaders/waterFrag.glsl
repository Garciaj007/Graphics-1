#version 410

#define PI 3.1415926535897932384626433832795

out vec4 fragColor;

in vec3 normal;
in vec3 position;
in vec2 texCoords;

uniform samplerCube skybox;
uniform mat4 viewMatrix;

uniform struct Water
{
	float roughness;
	float metallic;
	float rrLerpAmt;
	float colorLerpAmt;
	vec4 color;
} water;

uniform struct Light
{
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float power;
	float fresnelPower;
} light;

float ratio = 1.0 / 1.3333;
float fZero = pow((1.0 - ratio) / (1 + ratio), 2.0);

float phong(vec3 l, vec3 e, vec3 n, float power) {
    float nrm = (power + 8.0) / (PI * 8.0);
    return pow(max(dot(l,reflect(e,n)),0.0), power) * nrm;
}

void main () 
{
	vec3 I = normalize(position);
	vec3 N = normalize(normal);
	vec3 L = normalize(vec3(-0.5,1.0, 0.0));

	float metallic = 0.04;
	float roughness = 0.01;

	float fresnel = max(dot(N, -I), 0.0);
	fresnel = pow(fresnel, light.fresnelPower);

	vec3 refl = reflect(I, N);
	refl = vec3(inverse(viewMatrix) * vec4(refl, 0.0));
	refl = pow(texture(skybox, refl).rgb, vec3(2.2));

	vec3 refr = refract(I, N, ratio);
	refr = vec3(inverse(viewMatrix) * vec4(refr, 0.0));
	refr = texture(skybox, refr).rgb;

	vec3 ambient = light.ambient * water.color.xyz;

	float power = 1.0 / max(roughness * 0.4, 0.01);
	vec3 specular = light.specular * phong(L, I, N, power);
	refl -= specular;
	
	vec3 diffuse = light.diffuse * clamp(dot(L, N), 0, 1);
	diffuse = mix(diffuse, refl, fresnel);

	vec3 color = mix(diffuse, refl, metallic) + specular;

	//color = mix(texture(skybox, Rfl), color, water.rrLerpAmt);
	//vec4 color = vec4(light.ambient + diffuseReflection + fresnel * specularReflection, 1.0f);
	//fragColor = mix(mix(mix(texture(skybox, Rfr), texture(skybox, Rfl), water.rrLerpAmt), water.color, water.colorLerpAmt), color, 0.5f);
	fragColor = vec4(mix(mix(refr, water.color.xyz, 1.0 - fresnel), refl, 1.0 - fresnel) + color, 1.0f);
	//fragColor = vec4(1.0 - fresnel);
	//fragColor = vec4(pow(color, vec3(1.0 / 2.2)), 1.0f);
}