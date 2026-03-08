//https://github.com/j-2k/Learn-OpenGL
//===================================================
#include "pch.h"

#include "window.h"
#include "camera.h"
#include "texture.h"
#include "renderer.h"
#include "geometry.h"
#include "shader-utils/shaderLoaders.h"

// Engine Consts
const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;

// Window Title
const char* WINDOW_TITLE = "Circus Clown";

int main()
{
	std::cout << "Hello, OpenGL!" << std::endl;
	std::cout << "Press WASD to see input detection in action, press 1 to toggle wireframe mode" << std::endl;

	//Window Management
	Window window(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);

	//Shader Management
	Shader shader("src/shaders/basic/basic-vs.glsl", "src/shaders/basic/basic-fs.glsl");

	//Texture Loading
	unsigned int texture = loadTexture("./assets/ichise_ran_optimized.png");

	// Mesh Setup (Setup vertices and buffers and configure vertex attribute_
	MeshBuffers cubeMesh = setupMesh(createCubeEBO());

	//Since we have 1 shader with the rgb verts that is constant we just set it once outisde the loop.
	shader.use();

	//Active proper texture unit before binding
	glActiveTexture(GL_TEXTURE0); 
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
	glm::vec3 cubePositions[10];
	for (int i = 0; i < 10; i++)
	{
		//rand currently has no set seed, thus the results will be the same when running the program multiple times.
		float x = ((rand() % 50) - 35) / 10.0f;  // -3.5  to  1.4
		float y = ((rand() % 50) - 35) / 10.0f;  // -3.5  to  1.4
		float z = -5.0f;
		cubePositions[i] = glm::vec3(x, y, z);
	}

	//Render loop
	while (!window.shouldClose()) {

		//FPS - Console Output
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		//std::cout << "FPS: " << 1 / deltaTime << std::endl;

		processInput(window.getWindow());

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
		drawMesh(cubeMesh);

		//Multi Draw
		for (int i = 0; i < 10; i++)
		{
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), cubePositions[i]);
			transform = glm::scale(transform, glm::vec3(0.2f));
			transform = glm::rotate(transform, static_cast<float>(glfwGetTime()) * glm::radians(20.0f) * (i + 1), glm::vec3(0.5f, 1.0f, 0.0f));
			shader.setMat4("model", transform);
			drawMesh(cubeMesh);
		}

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		window.swapAndPoll();
	}

	//optional here but do know these exist
	cubeMesh.destroy();
	glDeleteTextures(1, &texture);
	glDeleteProgram(shader.programID);

	return 0;
}