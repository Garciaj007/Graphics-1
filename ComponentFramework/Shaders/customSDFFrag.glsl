#version 410

out vec4 fragColor;

in vec3 worldPosition;
in vec3 cameraPosition;

uniform float STEPS = 50;
uniform float STEP_SIZE = 1.0;
uniform float MIN_DISTANCE = 0.01;

uniform vec3 CENTRE = vec3(0,0,0);
uniform float RADIUS = 3.0;

float SDFSphere(vec3 p)
{
	return distance(p, CENTRE) - RADIUS;
}

float SDFBox(vec3 p, vec3 b)
{
	return distance(max(abs(p) - b, 0.0), CENTRE);
}

vec4 Raymarch(vec3 position, vec3 direction)
{
	for(int i = 0; i < STEPS; ++i)
	{
		float dist = SDFSphere(position);
		//float dist = SDFBox(position, vec3(2,2,1));

		if(dist < MIN_DISTANCE)
			return vec4(1.0 - float(i)/float(STEPS), 0, 0, 1);

		position += direction * dist;
	}

	return vec4(1,1,1,1);
}

void main()
{
	fragColor = Raymarch(worldPosition, normalize(worldPosition - cameraPosition));
}