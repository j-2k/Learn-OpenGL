// Fragment Shader
#version 330 core
out vec4 FragColor;

in vec3 vertexPos;
in vec3 vertexColor;

uniform float _time;

void main()
{
   FragColor = vec4(vertexPos, 1.0);
}