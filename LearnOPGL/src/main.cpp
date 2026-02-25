//https://github.com/j-2k/Learn-OpenGL
//===================================================
#include <glad/glad.h>  // GLAD goes first!
#include <GLFW/glfw3.h>

#include <iostream>	

#include "./shader-utils/shaderLoaders.h"

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
	Shader shader(vertexShaderSource, fragmentShaderSource);
	if (shader.programID == 0)
	{
		std::cout << "Failed to create shader program" << std::endl;
		return -1;
	}



	// Setup vertices and buffers and configure vertex attributes ---------------------------------
	/*
	float vertices[] = {
		 0.5f,  0.5f, 0.0f,  // top right
		 0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  // bottom left
		-0.5f,  0.5f, 0.0f   // top left 
	};
	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};
	*/

	float vertices[] = {
		// positions         // colors
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
		 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top 
	};

	unsigned int VBO, VAO, EBO;
	//glGenBuffers(1, &EBO);			// reserve an EBO ID
	glGenVertexArrays(1, &VAO);		// reserve a VAO ID
	glGenBuffers(1, &VBO);			// reserve a VBO ID

	glBindVertexArray(VAO);			// start recording into VAO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);												// select VBO as the active buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);		// upload vertex data to GPU
	
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);	// select EBO as the active buffer
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);	// upload vertex data to GPU
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);	// describe layout: slot 0, 3 floats, stride 12 bytes, offset 0
	glEnableVertexAttribArray(0);	// enable attribute slot 0 so the shader can read it

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));	// layout is now : slot 1, 3 floats, stride 12 bytes, offset 12 bytes (after the position data)
	glEnableVertexAttribArray(1);	// enable attribute slot 0 so the shader can read it
	glBindVertexArray(0);			// stop recording, VAO is saved

	//Since we have 1 shader with the rgb verts that is constant we just set it once outisde the loop.
	glUseProgram(shaderProgram);

	//Render loop
	while (!glfwWindowShouldClose(window)) {

		processInput(window);

		glClearColor(0.2f, 0.3f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//Draw our first triangle
			
		//Shader Uniforms
		/*
		double  timeValue = glfwGetTime();
		float c = static_cast<float>(cos(timeValue) / 2.0f + 0.5f);
		int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
		glUseProgram(shaderProgram);
		glUniform4f(vertexColorLocation, c + 0.2f, c, c - 0.4f, 1.0f);
		*/

		glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		glDrawArrays(GL_TRIANGLES, 0, 3);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // using EBO instead of VBO for glDrawElements
		glBindVertexArray(0); // no need to unbind it every time

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//optional here but do know these exist
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

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