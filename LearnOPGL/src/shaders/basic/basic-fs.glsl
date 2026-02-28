// Fragment Shader
#version 330 core
out vec4 FragColor;

in vec3 vertexPos;
in vec3 vertexColor;
in vec2 uv;

uniform float _time;
uniform sampler2D tex0;

void main()
{
   FragColor = texture(tex0,uv) * vec4(vertexColor, 1.0); //vec4(vertexPos, 1.0);
}