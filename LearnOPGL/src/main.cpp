//https://github.com/j-2k/Learn-OpenGL
//===================================================
#include <glad/glad.h>  // GLAD goes first!
#include <GLFW/glfw3.h>

#include <array>
#include <iostream>	

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "./shader-utils/shaderLoaders.h"
#include "../external/stb_image.h"
#include "./camera.h"
#include "./texture.h"
#include "./geometry.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// Engine Consts
const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;

// Window Title
const char* WINDOW_TITLE = "CircusClown";

// Toggles
bool wireframe = false;
bool key1WasPressed = false; // show/hide wireframe mode, track previous frame state to prevent multiple toggles per key press
bool key2WasPressed = false; // show/hide cursor, track previous frame state to prevent multiple toggles per key press

//Camera setup
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = WINDOW_WIDTH / 2.0f;
float lastY = WINDOW_HEIGHT / 2.0f;
bool firstMouse = true;

//deltaTime
float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame


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
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	//glfw callbacks for input handling
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	//Tell GLFW to capture our mouse (disable cursor)
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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
	unsigned int texture = loadTexture("./assets/ichise_ran_optimized.png");


	// Setup vertices and buffers and configure vertex attributes ---------------------------------
	GeometryData cubeData = createCubeEBO();

	unsigned int VBO, VAO, EBO;
	glGenBuffers(1, &EBO);			// reserve an EBO ID
	glGenVertexArrays(1, &VAO);		// reserve a VAO ID
	glGenBuffers(1, &VBO);			// reserve a VBO ID

	glBindVertexArray(VAO);			// start recording into VAO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);	// select VBO as the active buffer
	glBufferData(GL_ARRAY_BUFFER, cubeData.vertices.size() * sizeof(float), cubeData.vertices.data(), GL_STATIC_DRAW);		// upload vertex data to GPU

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);	// select EBO as the active buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, cubeData.indices.size() * sizeof(float), cubeData.indices.data(), GL_STATIC_DRAW);	// upload vertex data to GPU

	//glVertexAttribPointer (SLOT, SIZE, TYPE, NORMALIZED, STRIDE, OFFSET) <<< this is how we tell OpenGL how to interpret the vertex data we just uploaded. We have to do this for each attribute in our vertex data (position, color, texture coords)

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);	// POSITION layout: slot 0, 3 floats, stride 12 bytes, offset 0
	glEnableVertexAttribArray(0);	// enable attribute slot 0 so the shader can read it

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));	// COLOR layout is now : slot 1, 3 floats, stride 12 bytes, offset 12 bytes (after the position data)
	glEnableVertexAttribArray(1);	// enable attribute slot 1 so the shader can read it

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));	// TEXTURE layout is now : slot 2, 2 floats, stride 8 bytes, offset 12 bytes (after the color data)
	glEnableVertexAttribArray(2);	// enable attribute slot 2 so the shader can read it

	glBindVertexArray(0);			// stop recording, VAO is saved DONT NEED TO PUT IN RENDER LOOP LIKE BEFORE, ITS UNNECESSARY TO BIND/UNBIND EVERY FRAME. When changing VAO VBO EBOS etc they are overwritten, so this is not needed.

	//Since we have 1 shader with the rgb verts that is constant we just set it once outisde the loop.
	shader.use();

	glActiveTexture(GL_TEXTURE0); // active proper texture unit before binding
	glBindTexture(GL_TEXTURE_2D, texture);

	//Making Cubes
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.75f, 0.75f, 0));
	model = glm::scale(model, glm::vec3(0.5f));
	shader.setMat4("model", model);


	//OpenGL State Management
	glEnable(GL_DEPTH_TEST);	// enable depth testing for correct z-ordering	
	glEnable(GL_CULL_FACE);		// enable culling
	glCullFace(GL_BACK);		// cull back faces //glCullFace(GL_FRONT); to see front face culling & test!
	glFrontFace(GL_CCW);		// define front face as counter-clockwise (this is default)

	//Generate random position for cubes at init time
	std::array<glm::vec3, 10> cubePositions;
	for (int i = 0; i < 10; i++)
	{
		//rand currently has no set seed, thus the results will be the same when running the program multiple times.
		float x = ((rand() % 50) - 35) / 10.0f;  // -3.5  to  1.4
		float y = ((rand() % 50) - 35) / 10.0f;  // -3.5  to  1.4
		float z = -5.0f;
		cubePositions[i] = glm::vec3(x, y, z);
	}

	//Render loop
	while (!glfwWindowShouldClose(window)) {

		//FPS - Console Output
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//std::cout << "FPS: " << 1 / deltaTime << std::endl;


		processInput(window);

		glClearColor(0.2f, 0.3f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Shader Uniforms usage, example of sending a uniform variable to the shader;
		shader.setFloat("_time", static_cast<float>(glfwGetTime()));

		//MVP matrices
		model = glm::rotate(model, glm::radians(45.0f) * 0.0001f, glm::vec3(1.0, 1.0, 1.0));
		shader.setMat4("model", model);

		glm::mat4 view = camera.GetViewMatrix();
		shader.setMat4("view", view);

		// pass projection matrix to shader (note that in this case it could change every frame)
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), static_cast<float>(WINDOW_WIDTH) / static_cast<float>(WINDOW_HEIGHT), 0.1f, 100.0f);
		shader.setMat4("projection", projection);


		//Draw
		glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		//glDrawArrays(GL_TRIANGLES, 0, 3); //triangles without EBO
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0); // using EBO instead of VBO for glDrawElements
		//glBindVertexArray(0); // no need to unbind it every time?

		//Multi Draw
		for (int i = 0; i < 10; i++)
		{
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), cubePositions[i]);
			transform = glm::scale(transform, glm::vec3(0.2f));
			transform = glm::rotate(transform, static_cast<float>(glfwGetTime()) * glm::radians(20.0f) * (i + 1), glm::vec3(0.5f, 1.0f, 0.0f));
			shader.setMat4("model", transform);
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		}

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


	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);

	// toggle cursor on/off with key 2
	bool key2IsPressed = glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS;
	if (key2IsPressed && !key2WasPressed) // only triggers on the first frame of the press
	{
		std::cout << "2 was pressed\n";
		if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
		{
			std::cout << "Cursor Enabled\n";
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
		else
		{
			std::cout << "Cursor Disabled\n";
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			firstMouse = true; // reset mouse movement tracking for when we re-enable the cursor
			//Not correct behaviour, but keeping it for now!
		}
	}


	// toggle wireframe mode with key 1
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
	key2WasPressed = key2IsPressed; // update at end of loop
}

// (GLFW) Whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}