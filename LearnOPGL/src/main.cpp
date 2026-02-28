//https://github.com/j-2k/Learn-OpenGL
//===================================================
#include <glad/glad.h>  // GLAD goes first!
#include <GLFW/glfw3.h>

#include <iostream>	

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f); //Testing glm

#include "./shader-utils/shaderLoaders.h"
#include "../external/stb_image.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// Engine Consts
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Window Title
const char* WINDOW_TITLE = "CircusClown";



bool wireframe = false;
bool key1WasPressed = false; // track previous frame state


int main()
{
	std::cout << "Hello, OpenGL!" << std::endl;
	std::cout << "Press WASD to see input detection in action, press 1 to toggle wireframe mode" << std::endl;

	glfwInit();

	//OpenGL version 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	//Core profile = No backwards compatibility
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Window creation
	GLFWwindow* window = glfwCreateWindow(800, 600, WINDOW_TITLE, NULL, NULL);
	if(window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	//callback for window resizing
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//(Initialize GLAD) GLAD: load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//Shader Management
	Shader shader("src/shaders/basic/basic-vs.glsl", "src/shaders/basic/basic-fs.glsl");
	
	if (shader.programID == 0)
	{
		std::cout << "Failed to create shader program" << std::endl;
		return -1;
	}

	//Texture Loading
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// load and generate the texture
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis, must be before load.
	unsigned char* data = stbi_load("./assets/ichise_ran_optimized.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture! check path!" << std::endl;
	}

	//After we're done generating the texture and its corresponding mipmaps, it is good practice to free the image memory
	stbi_image_free(data);




	// Setup vertices and buffers and configure vertex attributes ---------------------------------

	//Using EBO to to reduce verts for multiple tris
	float vertices[] = {
		//Position          // Color		  //Texture coords
		 0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.f, 1.f,		// top right
		 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.f, 0.f,		// bottom right
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.f, 0.f,		// bottom left
		-0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.f, 1.f,		// top left 
	};
	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};


	/* //triangles without EBO
	float vertices[] = {
		// positions         // colors
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
		 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top 
	};
	*/

	unsigned int VBO, VAO, EBO;
	glGenBuffers(1, &EBO);			// reserve an EBO ID
	glGenVertexArrays(1, &VAO);		// reserve a VAO ID
	glGenBuffers(1, &VBO);			// reserve a VBO ID

	glBindVertexArray(VAO);			// start recording into VAO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);												// select VBO as the active buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);		// upload vertex data to GPU
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);	// select EBO as the active buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);	// upload vertex data to GPU

	//glVertexAttribPointer (SLOT, SIZE, TYPE, NORMALIZED, STRIDE, OFFSET) <<< this is how we tell OpenGL how to interpret the vertex data we just uploaded. We have to do this for each attribute in our vertex data (position, color, texture coords)

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);	// POSITION layout: slot 0, 3 floats, stride 12 bytes, offset 0
	glEnableVertexAttribArray(0);	// enable attribute slot 0 so the shader can read it

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3* sizeof(float)));	// COLOR layout is now : slot 1, 3 floats, stride 12 bytes, offset 12 bytes (after the position data)
	glEnableVertexAttribArray(1);	// enable attribute slot 1 so the shader can read it

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));	// TEXTURE layout is now : slot 2, 2 floats, stride 8 bytes, offset 12 bytes (after the color data)
	glEnableVertexAttribArray(2);	// enable attribute slot 2 so the shader can read it

	glBindVertexArray(0);			// stop recording, VAO is saved

	//Since we have 1 shader with the rgb verts that is constant we just set it once outisde the loop.
	shader.use();

	glActiveTexture(GL_TEXTURE0); // active proper texture unit before binding
	glBindTexture(GL_TEXTURE_2D, texture);

	//Render loop
	while (!glfwWindowShouldClose(window)) {

		processInput(window);

		glClearColor(0.2f, 0.3f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//Shader Uniforms usage, example of sending a uniform variable to the shader;
		shader.setFloat("_time", glfwGetTime());

		//Draw our first triangle
		glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		//glDrawArrays(GL_TRIANGLES, 0, 3); //triangles without EBO
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // using EBO instead of VBO for glDrawElements
		glBindVertexArray(0); // no need to unbind it every time

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//optional here but do know these exist
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shader.programID);

	//cleanup and exit
	glfwTerminate();
	return 0;
}




// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	//Print if WASD keys are pressed
	if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		std::cout << "W";
	if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		std::cout << "A";
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		std::cout << "S";
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		std::cout << "D";


	bool key1IsPressed = glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS;

	if (key1IsPressed && !key1WasPressed) // only triggers on the first frame of the press
	{
		std::cout << "1 was pressed\n";
		wireframe = !wireframe;
		if (wireframe)
		{
			std::cout << "Wireframe ON\n";
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else
		{
			std::cout << "Wireframe OFF\n";
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
	}

	key1WasPressed = key1IsPressed; // update at end of loop
}

// (GLFW) Whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}