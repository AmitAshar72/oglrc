#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_LENGTH= 600;

int main()
{
	//Instantiate GLFW Window
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Create Window
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_LENGTH, "Test", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	//On Window Resize, callback to update viewport
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//Initialize GLAD
	//GLAD manages functions pointers for OpenGL. We initialize GLAD before we call any OpenGL function
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//Render Loop
	while (!glfwWindowShouldClose(window))
	{
		//Input
		processInput(window);

		//Render Call
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//check and call events and swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//Define Viewport
	glViewport(0, 0, 200, 200);

	//Terminate call to clean up all resources
	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}
