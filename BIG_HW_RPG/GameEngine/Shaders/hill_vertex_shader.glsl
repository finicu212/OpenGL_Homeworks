#version 400

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normals;
layout (location = 2) in vec2 texCoord;

out vec2 textureCoord;
out vec3 norm;
out vec3 fragPos;

uniform mat4 MVP;
uniform mat4 model;

void main()
{
	textureCoord = texCoord;

	// fragPos calculation - wave-like mountains
	vec3 posCopy = pos * 2.5;

	float amplitude = 27.0f;
	float freq = 0.025f;
	vec2 direction = vec2(0.325, 0.675);

	float dotProductResult = dot(direction, vec2(posCopy.x, posCopy.z));
	posCopy.y += 2 * amplitude * pow( (sin(dotProductResult * freq + 50) + 1) / 2, 2);

	fragPos = vec3(model * vec4(posCopy, 1.0f));
	// end fragPos calculation

	norm = mat3(transpose(inverse(model)))*normals;
	gl_Position = MVP * vec4(posCopy, 1.0f);
}