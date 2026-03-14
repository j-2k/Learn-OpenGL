// Fragment Shader
#version 330 core
out vec4 FragColor;

in vec3 vertexPos;

uniform float _time;
uniform vec3 objectColor;
uniform vec3 lightColor;


void main()
{
   FragColor = vec4(lightColor * objectColor, 1.0);
}