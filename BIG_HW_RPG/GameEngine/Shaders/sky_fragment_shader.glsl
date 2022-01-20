#version 400

in vec2 textureCoord; 
in vec3 norm;
in vec3 fragPos;

out vec4 fragColor;

uniform sampler2D texture1;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
	//Ambient light
	float ambientStrength = 1.25;
    vec3 ambient = ambientStrength * lightColor;
//	vec3 objectColor = vec3(1.0f, 0.5f, 0.31f);

	vec3 result = ambient;
	//fragColor = vec4(result, 1.0f);
	fragColor = vec4(result, 1.0f);
	fragColor = fragColor * texture(texture1, textureCoord);
}