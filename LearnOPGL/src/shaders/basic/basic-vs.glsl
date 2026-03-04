// Vertex Shader
#version 330 core
layout (location = 0) in vec3 aPos;		// the vertex position variable has attribute position 0
layout (location = 1) in vec3 aColor;	// the color variable has attribute position 1
layout (location = 2) in vec2 aTexCoord;	// the texture coordinate variable has attribute position 2
out vec3 vertexColor;
out vec3 vertexPos;
out vec2 uv;

uniform float _time;

uniform mat4 transform;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	vertexColor = aColor;
	vertexPos = aPos;
	uv = aTexCoord;



	//gl_Position = transform * vec4(aPos,1.);//vec4(aPos.x,aPos.y + (sin(_time * 3)*0.5),aPos.z, 1.0);
	gl_Position = projection * view * model * vec4(aPos, 1.0);
}
