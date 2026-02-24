#include "shaderLoaders.h"
#include <iostream>

unsigned int loadShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource)
{
	//1. Vertex shader creation stage ---------------------------------
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);

	//Attach shader source code to the shader object and compile the shader
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	//Check for shader compile errors
	int  success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		return 0;
	}
	//Vertex shader creation end ---------------------------------

	//2. Fragment shader creation stage ---------------------------------
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	//Attach shader source code to the shader object and compile the shader
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	//Check for shader compile errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		return 0;
	}
	//Fragment shader creation end ---------------------------------

	//3. Shader Program & Linking Stage ---------------------------------
	unsigned int shaderProgram = glCreateProgram();

	//A Shader program must have attached a vertex & fragment shader then linked for it to then be used in the renderer.
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	//Check for linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		return 0;
	}

	//Delete the shader objects once we've linked them into the program object; we no longer need them anymore.
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	//Shader Program & Linking Stage end ---------------------------------

	return shaderProgram;
}