#version 410
in  vec3 vertNormal;
in  vec3 lightDir0;
in	vec3 lightDir1;
in  vec3 vertDir;
out vec4 fragColor;

void main() { 
	const vec4 ks = vec4(0.6, 0.8, 0.6, 0.0);
	const vec4 kd = vec4(0.6, 0.8, 0.6, 0.0); /// const means it cannot be changed just like C++
	const vec4 ks2 = vec4(0.8, 0.2, 0.6, 0.0);
	const vec4 kd2 = vec4(0.8, 0.2, 0.6, 0.0);
	const vec4 ka = 0.1 * kd;
	
	float diff = max(dot(vertNormal, lightDir0), 0.0);
	float diff2 = max(dot(vertNormal, lightDir1), 0.0);

	float spec = max(dot(vertNormal, normalize(lightDir0 + vertDir)), 0.0);
	float spec2 = max(dot(vertNormal, normalize(lightDir1 + vertDir)), 0.0);

	if(diff > 0.0){
		spec = pow(spec, 14.0);
	}

	if(diff2 > 0.0){
		spec2 = pow(spec2, 14.0);
	}

	fragColor =  ka + ((diff * kd) + (spec * ks)) + ((diff2 * kd2) + (spec2 * ks2));	
} 

