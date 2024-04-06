#include <iostream>
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "stb_image.h"

#include "Shader.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Texture.h"
#include "Camera.h"

void InitWindow();
int InitGlad();
void Framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void ProcessInput(GLFWwindow* window);


const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_LENGTH= 600;

float vertices[] = {
	// positions			  //Normals				// colors         // texture 
																		// coords
	   -0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,  1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,  1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
	   -0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	   -0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
												  
	   -0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,  1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,  1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
	   -0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	   -0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
												  
	   -0.5f,  0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
	   -0.5f,  0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
	   -0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	   -0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	   -0.5f, -0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	   -0.5f,  0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
												  
		0.5f,  0.5f,  0.5f,   1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,   1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,   1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,   1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,   1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,   1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
												  
	   -0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,  1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
	   -0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	   -0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
												  
	   -0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,  1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
	   -0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	   -0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
};

unsigned int indices[] = 
{  
	0, 1, 2,
	0, 2, 3,
	0, 4, 7,
	0, 7, 3,
	3, 7, 6,
	3, 6, 2,
	2, 6, 5,
	2, 5, 1,
	1, 5, 4,
	1, 4, 0,
	4, 5, 6,
	4, 6, 7
};

glm::vec3 cubePositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
};

//Texutre alpha blend
float blend = 0.9f;

//Camera 
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

//Mouse Inputs
float lastX = SCR_LENGTH / 2.0;
float lastY = SCR_WIDTH / 2.0;
bool firstMouse = true;

//Time
float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

//Light Cube
float lightVertices[] = {
	-0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,

	-0.5f, -0.5f,  0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	-0.5f, -0.5f,  0.5f,

	-0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,

	 0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,

	-0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f, -0.5f,  0.5f,
	-0.5f, -0.5f,  0.5f,
	-0.5f, -0.5f, -0.5f,

	-0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f, -0.5f
};

unsigned int lightIndices[] =
{
	0, 1, 2,
	0, 2, 3,
	0, 4, 7,
	0, 7, 3,
	3, 7, 6,
	3, 6, 2,
	2, 6, 5,
	2, 5, 1,
	1, 5, 4,
	1, 4, 0,
	4, 5, 6,
	4, 6, 7
};

// positions of the point lights
glm::vec3 pointLightPositions[] = {
	glm::vec3(0.7f,  0.2f,  2.0f),
	glm::vec3(2.3f, -3.3f, -4.0f),
	glm::vec3(-4.0f,  2.0f, -12.0f),
	glm::vec3(0.0f,  0.0f, -3.0f)
};


int main()
{
	//Instantiate GLFW Window
	InitWindow();

	//Create Window
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_LENGTH, "Test", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//On Window Resize, callback to update viewport
	glfwSetFramebufferSizeCallback(window, Framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	//Initialize GLAD
	InitGlad();
	
	glEnable(GL_DEPTH_TEST);

	//Initialize our default shaders
	Shader ourShader("D:/Personal Project Repos/OpenGL Test/OpenGLTest/OpenGLTest/VertexShader.vs", "D:/Personal Project Repos/OpenGL Test/OpenGLTest/OpenGLTest/FragmentShader.fs");

	//Vertex Buffer Objects are created to store information that has to be rendered, on the GPU

	//Generate Vertex Array Object
	VAO m_VAO;
	m_VAO.Bind();

	//Generate Vertex Buffer Object and Element Buffer Object
	VBO m_VBO(vertices, sizeof(vertices));
	EBO m_EBO(indices, sizeof(indices));

	//Position attribute
	m_VAO.LinkAttrib(m_VBO, 0, 3, GL_FLOAT, 11 * sizeof(float), (void*)0);
	//Normal attribute
	m_VAO.LinkAttrib(m_VBO, 1, 3, GL_FLOAT, 11 * sizeof(float), (void*)(3 * sizeof(float)));
	//Color attribute
	m_VAO.LinkAttrib(m_VBO, 2, 3, GL_FLOAT, 11 * sizeof(float), (void*)(6 * sizeof(float)));
	//Texture attribute
	m_VAO.LinkAttrib(m_VBO, 3, 2, GL_FLOAT, 11 * sizeof(float), (void*)(9 * sizeof(float)));
	

	//Unbind when all is linked
	m_VAO.Unbind();
	m_VBO.Unbind();
	m_EBO.Unbind();

#pragma region Light Cube
	//Initialize Light Shader
	Shader lightShader("D:/Personal Project Repos/OpenGL Test/OpenGLTest/OpenGLTest/LightShader.vs", "D:/Personal Project Repos/OpenGL Test/OpenGLTest/OpenGLTest/LightShader.fs");

	//Generate Cube for Light
	VAO m_lightVAO;
	m_lightVAO.Bind();

	//Generate Vertex Buffer Object and Element Buffer Object
	VBO m_lightVBO(lightVertices, sizeof(lightVertices));
	EBO m_lightEBO(lightIndices, sizeof(lightIndices));

	//Position attribute
	m_lightVAO.LinkAttrib(m_lightVBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);

	//Unbind when all is linked
	m_lightVAO.Unbind();
	m_VBO.Unbind();
	m_lightEBO.Unbind();
#pragma endregion Light Cube
	
#pragma region Textures

	//Create Textures
	/*Texture containerTex("D:/Personal Project Repos/OpenGL Test/OpenGLTest/OpenGLTest/Resources/Textures/container.jpg", GL_TEXTURE_2D, GL_TEXTURE0, GL_UNSIGNED_BYTE);
	containerTex.TextureUnit(ourShader, "texture1", 0);
	containerTex.TextureUnit(ourShader, "material.diffuse", 0);
	
	Texture faceTex("D:/Personal Project Repos/OpenGL Test/OpenGLTest/OpenGLTest/Resources/Textures/face.png", GL_TEXTURE_2D, GL_TEXTURE1, GL_UNSIGNED_BYTE);
	faceTex.TextureUnit(ourShader, "texture2", 1);
	faceTex.TextureUnit(ourShader, "material.diffuse", 0);*/
	
	Texture container2Tex("D:/Personal Project Repos/OpenGL Test/OpenGLTest/OpenGLTest/Resources/Textures/container2.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_UNSIGNED_BYTE);
	//container2Tex.TextureUnit(ourShader, "texture1", 0);
	container2Tex.TextureUnit(ourShader, "material.diffuse", 0);

	Texture container2Tex_specular("D:/Personal Project Repos/OpenGL Test/OpenGLTest/OpenGLTest/Resources/Textures/container2_specular.png", GL_TEXTURE_2D, GL_TEXTURE1, GL_UNSIGNED_BYTE);
	//container2Tex_specular.TextureUnit(ourShader, "texture2", 1);
	container2Tex_specular.TextureUnit(ourShader, "material.specular", 0); 

#pragma endregion Textures
	
	glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(1.2f, 1.0f, 2.0f);

	//Render Loop
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//Input
		ProcessInput(window);

		//Render Call
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Activate Shader
		ourShader.Activate();		
		ourShader.setVec3("lightPos", lightPos);
		ourShader.setVec3("viewPos", camera.Position);

		ourShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
		ourShader.setFloat("material.shininess", 32.0f);

		//Directional Light
		ourShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
		ourShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
		ourShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
		ourShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

		// point light 1
		ourShader.setVec3("pointLights[0].position", pointLightPositions[0]);
		ourShader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
		ourShader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
		ourShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
		ourShader.setFloat("pointLights[0].constant", 1.0f);
		ourShader.setFloat("pointLights[0].linear", 0.09f);
		ourShader.setFloat("pointLights[0].quadratic", 0.032f);
		// point light 2
		ourShader.setVec3("pointLights[1].position", pointLightPositions[1]);
		ourShader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
		ourShader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
		ourShader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
		ourShader.setFloat("pointLights[1].constant", 1.0f);
		ourShader.setFloat("pointLights[1].linear", 0.09f);
		ourShader.setFloat("pointLights[1].quadratic", 0.032f);
		// point light 3
		ourShader.setVec3("pointLights[2].position", pointLightPositions[2]);
		ourShader.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
		ourShader.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
		ourShader.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
		ourShader.setFloat("pointLights[2].constant", 1.0f);
		ourShader.setFloat("pointLights[2].linear", 0.09f);
		ourShader.setFloat("pointLights[2].quadratic", 0.032f);
		// point light 4
		ourShader.setVec3("pointLights[3].position", pointLightPositions[3]);
		ourShader.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
		ourShader.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
		ourShader.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
		ourShader.setFloat("pointLights[3].constant", 1.0f);
		ourShader.setFloat("pointLights[3].linear", 0.09f);
		ourShader.setFloat("pointLights[3].quadratic", 0.032f);
		// spotLight
		ourShader.setVec3("spotLight.position", camera.Position);
		ourShader.setVec3("spotLight.direction", camera.Front);
		ourShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
		ourShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
		ourShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
		ourShader.setFloat("spotLight.constant", 1.0f);
		ourShader.setFloat("spotLight.linear", 0.09f);
		ourShader.setFloat("spotLight.quadratic", 0.032f);
		ourShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
		ourShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

		//Activate Textures
		//containerTex.Activate();
		//faceTex.Activate();
		container2Tex.Activate();
		container2Tex_specular.Activate();

		//Update blend percentage between two textures
		ourShader.setFloat("blend", blend);

		// pass projection matrix to shader (note that in this case it could change every frame)
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_LENGTH, 0.1f, 100.0f);
		ourShader.setMat4("projection", projection);

		// camera/view transformation
		glm::mat4 view = camera.GetViewMatrix();
		ourShader.setMat4("view", view);		

		m_VAO.Bind();

		for (unsigned int i = 0; i < 3; i++)
		{
			// calculate the model matrix for each object and pass it to shader before drawing
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			//float angle = 20.0f * i;
			//if (i % 3 == 0)  // every 3rd iteration (including the first) we set the angle using GLFW's time function.
			//	angle = glfwGetTime() * 25.0f;
			//model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			ourShader.setMat4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices)/sizeof(float));
			//glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(int), GL_UNSIGNED_INT, 0);
		}

		//Activate light shader
		lightShader.Activate();
		
		lightShader.setMat4("projection", projection);
		lightShader.setMat4("view", view);

		m_lightVAO.Bind();

		for (unsigned int i = 0; i < 4; i++)
		{
			glm::mat4 lightModel = glm::mat4(1.0f);
			lightModel = glm::translate(lightModel, pointLightPositions[i]);
			lightModel = glm::scale(lightModel, glm::vec3(0.2f)); // Make it a smaller cube
			lightShader.setMat4("model", lightModel);
			lightShader.setVec3("lightColor", lightColor);
			glDrawArrays(GL_TRIANGLES, 0, sizeof(lightVertices) / sizeof(float));
			//glDrawElements(GL_TRIANGLES, sizeof(lightIndices) / sizeof(int), GL_UNSIGNED_INT, 0);
		}

		//check and call events and swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	
	m_VAO.Delete();
	m_VBO.Delete();
	m_EBO.Delete();
	
	
	m_lightVAO.Delete();
	m_lightVBO.Delete();
	m_lightEBO.Delete();
	

	ourShader.Delete();
	lightShader.Delete();

	//containerTex.Delete();
	//faceTex.Delete();
	container2Tex.Delete();
	container2Tex_specular.Delete();

	//Terminate call to clean up all resources
	glfwTerminate();
	return 0;
}

void InitWindow() 
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

int InitGlad() 
{
	//GLAD manages functions pointers for OpenGL. We initialize GLAD before we call any OpenGL function
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
}

void Framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
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

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void ProcessInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		blend += 0.001f; // change this value accordingly (might be too slow or too fast based on system hardware)
		if (blend >= 1.0f)
			blend = 1.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		blend -= 0.001f; // change this value accordingly (might be too slow or too fast based on system hardware)
		if (blend <= 0.0f)
			blend = 0.0f;
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}
