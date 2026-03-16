// Fragment Shader
#version 330 core
out vec4 FragColor;

in vec3 vertexPos;
in vec3 normalPos;

uniform float _time;

uniform vec3 lightColor;


void main()
{
	float ambientStr = 0.1;
	vec3 ambient = ambientStr * lightColor;

	vec3 fc = ambient * lightColor;
	FragColor = vec4(normalPos, 1.0);
}