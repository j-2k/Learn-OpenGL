//window.h
#pragma once

#include "pch.h"

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

		//Tell GLFW to capture our mouse (disable cursor)
		glfwSetInputMode(w_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		//(Initialize GLAD) GLAD: load all OpenGL function pointers
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize GLAD" << std::endl;
			return;
		}
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