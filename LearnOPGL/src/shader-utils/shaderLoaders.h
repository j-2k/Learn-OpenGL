//shaderloaders.h
#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
public:
	unsigned int programID;

	//constructor must take the input path for vert & frag shader source code
	Shader(const char* vertexPath, const char* fragmentPath);

	//=======================Added when desctructor was added to shader class=================
	~Shader();
	// Prevent copies
	Shader(const Shader&) = delete;
	Shader& operator=(const Shader&) = delete;

	// Allow moves
	Shader(Shader&& other) noexcept : programID(other.programID) {
		other.programID = 0;
	}
	Shader& operator=(Shader&& other) noexcept {
		if (this != &other) {
			glDeleteProgram(programID);
			programID = other.programID;
			other.programID = 0;
		}
		return *this;
	}
	//=======================Added when desctructor was added to shader class=================

	//activate the shader
	void use();

	//utils for sending uniform variables to the shader
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;    
	void setVec2(const std::string& name, const glm::vec2& value) const;
	void setVec2(const std::string& name, float x, float y) const;
	void setVec3(const std::string& name, const glm::vec3& value) const;
	void setVec3(const std::string& name, float x, float y, float z) const;
	void setVec4(const std::string& name, const glm::vec4& value) const;
	void setVec4(const std::string& name, float x, float y, float z, float w) const;
	void setMat2(const std::string& name, const glm::mat2& mat) const;
	void setMat3(const std::string& name, const glm::mat3& mat) const;
	void setMat4(const std::string& name, const glm::mat4& mat) const;

private:
	void checkCompileErrors(GLuint shader, std::string type);
};