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

	vec3 posCopy = pos;

	float amplitude = 2.5f;
	float freq = 1.0f;
	vec2 direction = vec2(0.75, 0.25);

	float dotProductResult = dot(direction, vec2(posCopy.x, posCopy.z));
	posCopy.y += 2 * amplitude * pow( (sin(dotProductResult * freq) + 1) / 2, 2);

	fragPos = vec3(model * vec4(posCopy, 1.0f));
	norm = mat3(transpose(inverse(model)))*normals;
	gl_Position = MVP * vec4(posCopy, 1.0f);
}