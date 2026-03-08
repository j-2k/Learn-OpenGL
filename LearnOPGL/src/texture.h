//texture.h
#pragma once

#include "pch.h"

#include "../external/stb_image.h"


// Loads a texture from a file path and returns its OpenGL ID.
// Returns 0 on failure.
unsigned int loadTexture(const std::string& path) {

	//Texture Loading
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// tell stb_image.h to flip loaded texture's on the y-axis, must be before load.
	stbi_set_flip_vertically_on_load(true);

	// load and generate the texture
	int width, height, nrChannels;
	unsigned char* data = stbi_load("./assets/ichise_ran_optimized.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		//GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
		//glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "[Texture] Failed to load: " << path << std::endl;
		glDeleteTextures(1, &textureID);
		textureID = 0;
	}

	//After we're done generating the texture and its corresponding mipmaps, it is good practice to free the image memory
	stbi_image_free(data);
	return textureID;
}

