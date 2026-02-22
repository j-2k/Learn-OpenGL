#include <glad/glad.h>  // GLAD goes first!
#include <GLFW/glfw3.h>

#include <iostream>	

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// Engine Consts
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Window Title
const char* WINDOW_TITLE = "CircusClown";


// Vertex Shader
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

// Fragment Shader
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";




int main()
{
	std::cout << "Hello, OpenGL!" << std::endl;

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

	//First Triangle Steps ==========================================

	//1. Vertex shader creation stage ---------------------------------
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);

	//Attach shader source code to the shader object and compile the shader
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	//Check for shader compile errors
	int  success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	//Vertex shader creation end ---------------------------------

	//2. Fragment shader creation stage ---------------------------------
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	//Attach shader source code to the shader object and compile the shader
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	//Check for shader compile errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	//Fragment shader creation end ---------------------------------

	//3. Shader Program & Linking Stage ---------------------------------


	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};

	unsigned int VBO, VAO;
	//Generate Vertex Array Object and bind it
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//Copy our vertices array in a buffer for OpenGL to use
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);



	//Render loop
	while (!glfwWindowShouldClose(window)) {

		processInput(window);

		glClearColor(0.2f, 0.3f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

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


}

// (GLFW) Whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}