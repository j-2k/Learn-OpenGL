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

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// Engine Consts
const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;

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
		// Position            // Color           // UV
		// Front face (z = 0.5)
		-0.5f, -0.5f,  0.5f,  1,0,0,  0.f, 0.f,
		 0.5f, -0.5f,  0.5f,  1,0,0,  1.f, 0.f,
		 0.5f,  0.5f,  0.5f,  1,0,0,  1.f, 1.f,
		-0.5f,  0.5f,  0.5f,  1,0,0,  0.f, 1.f,

		// Back face (z = -0.5)
		-0.5f, -0.5f, -0.5f,  0,1,0,  1.f, 0.f,
		 0.5f, -0.5f, -0.5f,  0,1,0,  0.f, 0.f,
		 0.5f,  0.5f, -0.5f,  0,1,0,  0.f, 1.f,
		-0.5f,  0.5f, -0.5f,  0,1,0,  1.f, 1.f,

		// Left face (x = -0.5)
		-0.5f, -0.5f, -0.5f,  0,0,1,  0.f, 0.f,
		-0.5f, -0.5f,  0.5f,  0,0,1,  1.f, 0.f,
		-0.5f,  0.5f,  0.5f,  0,0,1,  1.f, 1.f,
		-0.5f,  0.5f, -0.5f,  0,0,1,  0.f, 1.f,

		// Right face (x = 0.5)
		 0.5f, -0.5f,  0.5f,  1,1,0,  0.f, 0.f,
		 0.5f, -0.5f, -0.5f,  1,1,0,  1.f, 0.f,
		 0.5f,  0.5f, -0.5f,  1,1,0,  1.f, 1.f,
		 0.5f,  0.5f,  0.5f,  1,1,0,  0.f, 1.f,

		 // Top face (y = 0.5)
		 -0.5f,  0.5f,  0.5f,  1,0,1,  0.f, 0.f,
		  0.5f,  0.5f,  0.5f,  1,0,1,  1.f, 0.f,
		  0.5f,  0.5f, -0.5f,  1,0,1,  1.f, 1.f,
		 -0.5f,  0.5f, -0.5f,  1,0,1,  0.f, 1.f,

		 // Bottom face (y = -0.5)
		 -0.5f, -0.5f, -0.5f,  0,1,1,  0.f, 0.f,
		  0.5f, -0.5f, -0.5f,  0,1,1,  1.f, 0.f,
		  0.5f, -0.5f,  0.5f,  0,1,1,  1.f, 1.f,
		 -0.5f, -0.5f,  0.5f,  0,1,1,  0.f, 1.f,
	};

	unsigned int indices[] = {
		0,1,2,  0,2,3,      // front
		6,5,4,  7,6,4,      // back (flipped)
		8,9,10, 8,10,11,    // left
		12,13,14, 12,14,15, // right
		16,17,18, 16,18,19, // top
		20,21,22, 20,22,23  // bottom
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

	glBindVertexArray(0);			// stop recording, VAO is saved DONT NEED TO PUT IN RENDER LOOP LIKE BEFORE, ITS UNNECESSARY TO BIND/UNBIND EVERY FRAME. When changing VAO VBO EBOS etc they are overwritten, so this is not needed.

	//Since we have 1 shader with the rgb verts that is constant we just set it once outisde the loop.
	shader.use();

	glActiveTexture(GL_TEXTURE0); // active proper texture unit before binding
	glBindTexture(GL_TEXTURE_2D, texture);

	//MVP (Model View Projection) setup
	glm::mat4 model, view, projection;
	model = glm::mat4(1.0f);
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
	projection = glm::perspective(glm::radians(45.0f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);

	//M
	model = glm::translate(model, glm::vec3(0.75f, 0.75f, 0));
	model = glm::scale(model, glm::vec3(0.5f));
	shader.setMat4("model", model);

	//Camera: +Z is behind, while forward is -Z, here we go back 3 units on the Z axis.
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget); //Pointing towards the camera from the target (z).

	//Get Up and Right Vectors for the camera's coordinate system
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection)); //Right vector is perpendicular to the up and camDirection vectors.
	glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight); //Up vector is perpendicular to the camDirection and cameraRight vectors.
	const float radius = 10.0f;

	shader.setMat4("view", view); 

	shader.setMat4("projection", projection);




	//OpenGL State Management
	glEnable(GL_DEPTH_TEST);	// enable depth testing for correct z-ordering	
	glEnable(GL_CULL_FACE);		// enable culling
	glCullFace(GL_BACK);		// cull back faces //glCullFace(GL_FRONT); to see front face culling & test!
	glFrontFace(GL_CCW);		// define front face as counter-clockwise (this is default)

	//FPS - Console Output
	double previousTime = glfwGetTime();
	bool showFPS = false;

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
		if (showFPS)
		{
			double currentTime = glfwGetTime();
			double deltaTime = currentTime - previousTime;
			previousTime = currentTime;
			std::cout << "FPS: " << 1 / deltaTime << std::endl;
		}

		processInput(window);

		glClearColor(0.2f, 0.3f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Shader Uniforms usage, example of sending a uniform variable to the shader;
		shader.setFloat("_time", (float)glfwGetTime());

		//Update Transformations
		model = glm::rotate(model, glm::radians(45.0f)*0.0001f, glm::vec3(1.0, 1.0, 1.0));
		shader.setMat4("model", model);

		float camX = (float)sin(glfwGetTime()) * radius;
		float camZ = (float)cos(glfwGetTime()) * radius;
		glm::mat4 view;
		view = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
		shader.setMat4("view", view);

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
			transform = glm::rotate(transform, (float)glfwGetTime() * glm::radians(20.0f) * (i + 1), glm::vec3(0.5f, 1.0f, 0.0f));
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