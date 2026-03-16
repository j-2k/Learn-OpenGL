// Vertex Shader
#version 330 core
layout (location = 0) in vec3 aPos;		// the vertex position variable has attribute position 0
layout (location = 1) in vec3 aNormal;  

out vec3 vertexPos;
out vec3 normalPos;

uniform float _time;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	vertexPos = aPos;
	normalPos = aNormal;

	gl_Position = projection * view * model * vec4(aPos, 1.0);
}
