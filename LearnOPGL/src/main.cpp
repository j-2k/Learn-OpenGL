//https://github.com/j-2k/Learn-OpenGL
//main.cpp

#include "pch.h"

#include "window.h"
#include "camera.h"
#include "texture.h"
#include "renderer.h"
#include "geometry.h"
#include "shader-utils/shaderLoaders.h"
#include "time.h"
#include "inputhandler.h"


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
	Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
	InputHandler inputHandler(window.getWindow(), camera);

	initGLStates();	//Depth testing, face culling, etc.

	Shader shader("src/shaders/basic/basic-vs.glsl","src/shaders/basic/basic-fs.glsl"); //Shader Management
	Shader lightShader("src/shaders/lightsource/lightsource-vs.glsl", "src/shaders/lightsource/lightsource-fs.glsl"); //Shader Management
	Shader litShader("src/shaders/lit/lit-vs.glsl", "src/shaders/lit/lit-fs.glsl"); //Shader Management
	
	unsigned int ichiseTex = loadTexture("./assets/ichise_ran_optimized.png");			//Texture Loading
	MeshBuffers cubeMesh = setupMesh(createCubeEBO());							// Mesh Setup (Setup vertices and buffers and configure vertex attribute_
	MeshBuffers lightCubeMesh = setupMesh33(createCubeNormals());						// Mesh Setup (Setup vertices and buffers and configure vertex attribute_

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
		Time::getInstance().update(); // update deltaTime and totalTime

		inputHandler.processInput();

		glClearColor(0.2f, 0.3f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// commons
		// pass projection matrix to shader (note that in this case it could change every frame)
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), static_cast<float>(WINDOW_WIDTH) / static_cast<float>(WINDOW_HEIGHT), 0.1f, 100.0f);
		float t = Time::getInstance().getTotalTime();
		
		//MUST ALWAYS USE THE SHADER FIRST BEFORE SETTING UNIFORMS, OTHERWISE THE UNIFORM SETTING CALLS WILL BE A NO-OP SINCE THEY AFFECT THE CURRENTLY ACTIVE SHADER, WHICH WOULD BE WRONG IF WE DIDNT ACTIVATE THE SHADER FIRST.		
		{	//Light source cube
			lightShader.use();

			//Shader Uniforms usage, example of sending a uniform variable to the shader;
			lightShader.setFloat("_time", t);
			//MVP matrices
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 2.0f, -3.0f));
			transform = glm::rotate(transform, t * glm::radians(40.0f), glm::vec3(1.0, 1.0, 1.0));
			lightShader.setMat4("model", transform);
			lightShader.setMat4("view", camera.GetViewMatrix());
			lightShader.setMat4("projection", projection);

			lightShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
			lightShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);

			drawMesh(cubeMesh);
		}


		//Slightly confused right now on how textured objs need to be activated?
		{	//Textured cubes
			shader.use();
			shader.setMat4("view", camera.GetViewMatrix());
			shader.setMat4("projection", projection);

			// Below showcases how to bind textures in the case there are multiple textures in cases of when there are different meshes or multiple textures for one mesh.
			// For this demo we just have one texture, but the process is the same regardless of how many textures you have, you just have to bind them to different texture units and then tell the shader which texture unit each sampler belongs to.
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, ichiseTex);
			shader.setInt("tex0", 0);

			//Multi Draw
			for (int i = 0; i < 10; i++)
			{
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), cubePositions[i]);
				transform = glm::scale(transform, glm::vec3(0.2f));
				transform = glm::rotate(transform, t * glm::radians(20.0f) * (i + 1), glm::vec3(0.5f, 1.0f, 0.0f));
				shader.setMat4("model", transform);
				drawMesh(cubeMesh);
			}
		}

		{
			litShader.use();
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -4.0f, 0.0f));
			//transform = glm::rotate(transform, t * glm::radians(40.0f), glm::vec3(1.0, 1.0, 1.0));
			transform = glm::scale(transform, glm::vec3(3.0f));
			litShader.setMat4("model", transform);
			litShader.setMat4("view", camera.GetViewMatrix());
			litShader.setMat4("projection", projection);

			drawMesh(lightCubeMesh);
		}





		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		window.swapAndPoll();
	}

	//optional here but do know these exist
	cubeMesh.destroy();
	glDeleteTextures(1, &ichiseTex);
	glDeleteProgram(shader.programID);
	glDeleteProgram(lightShader.programID);

	return 0;
}