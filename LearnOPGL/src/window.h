#pragma once

#include "pch.h"
#include "camera.h"
#include "time.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// Toggles
bool wireframe = false;
bool key1WasPressed = false; // show/hide wireframe mode, track previous frame state to prevent multiple toggles per key press
bool key2WasPressed = false; // show/hide cursor, track previous frame state to prevent multiple toggles per key press

//Camera setup
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = 0;
float lastY = 0;
bool firstMouse = true;

class Window
{
public:
	Window(int width, int height, const char* title) : w_width(width), w_height(height) 
	{
		glfwInit();

		//OpenGL version 3.3
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

		//Core profile = No backwards compatibility
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		//Window creation
		w_window = glfwCreateWindow(width, height, title, NULL, NULL);
		if (w_window == NULL)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			return;
		}
		glfwMakeContextCurrent(w_window);

		//glfw callbacks for input handling
		glfwSetFramebufferSizeCallback(w_window, framebuffer_size_callback);
		glfwSetCursorPosCallback(w_window, mouse_callback);
		glfwSetScrollCallback(w_window, scroll_callback);

		//Tell GLFW to capture our mouse (disable cursor)
		glfwSetInputMode(w_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		//(Initialize GLAD) GLAD: load all OpenGL function pointers
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize GLAD" << std::endl;
			return;
		}

		lastX = width / 2.0f;
		lastY = height / 2.0f;
	}

	~Window() 
	{
		glfwTerminate();
	}

	bool shouldClose() const {
		return glfwWindowShouldClose(w_window);
	}

	void swapAndPoll() const {
		glfwSwapBuffers(w_window);
		glfwPollEvents();
	}

	GLFWwindow* getWindow() const { return w_window; }
	int getWidth() const { return w_width; }
	int getHeight() const { return w_height; }
	float getAspect() const { return static_cast<float>(w_width) / static_cast<float>(w_height); }

private:
    GLFWwindow* w_window = nullptr;
    int         w_width;
    int         w_height;
};



// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow* window)
{
	const float dt = Time::getInstance().getDeltaTime();

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);


	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, dt);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, dt);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, dt);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, dt);

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