#include "shaderLoaders.h"
#include <iostream>

class Shader {
	unsigned int programID;

	Shader(const char* vertexPath, const char* fragmentPath)
	{
		//Pasted below the annoying part of shader file parsing (same idea in the webgl project)

		// 0. retrieve the vertex/fragment source code from filePath
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;
		// ensure ifstream objects can throw exceptions:
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			// open files
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			std::stringstream vShaderStream, fShaderStream;
			// read file's buffer contents into streams
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			// close file handlers
			vShaderFile.close();
			fShaderFile.close();
			// convert stream into string
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure e)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}
		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();


		//Start building the shader program from the source code strings
		int  success;
		char infoLog[512];
		unsigned int vertexShader;
		unsigned int fragmentShader;

		//1. Vertex shader creation stage ---------------------------------
		vertexShader = glCreateShader(GL_VERTEX_SHADER);

		//Attach shader source code to the shader object and compile the shader
		glShaderSource(vertexShader, 1, &vShaderCode, NULL);
		glCompileShader(vertexShader);

		//Check for shader compile errors
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		//2. Fragment shader creation stage ---------------------------------
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		//Attach shader source code to the shader object and compile the shader
		glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
		glCompileShader(fragmentShader);

		//Check for shader compile errors
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		//3. Shader Program & Linking Stage ---------------------------------
		programID = glCreateProgram();

		//A Shader program must have attached a vertex & fragment shader then linked for it to then be used in the renderer.
		glAttachShader(programID, vertexShader);
		glAttachShader(programID, fragmentShader);
		glLinkProgram(programID);

		//Check for linking errors
		glGetProgramiv(programID, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(programID, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}

		//Delete the shader objects once we've linked them into the program object; we no longer need them anymore.
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}

	void use()
	{
		glUseProgram(programID);
	}

	void setBool(const std::string& name, bool value) const
	{
		glUniform1i(glGetUniformLocation(programID, name.c_str()), (int)value);
	}
	void setInt(const std::string& name, int value) const
	{
		glUniform1i(glGetUniformLocation(programID, name.c_str()), value);
	}
	void setFloat(const std::string& name, float value) const
	{
		glUniform1f(glGetUniformLocation(programID, name.c_str()), value);
	}

};
