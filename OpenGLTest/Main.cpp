#include "Model.h"
#include <GLFW/glfw3.h>


void InitWindow(); // Initial GLFW
int InitGlad(); //Initialize Glad. Glad manages function pointers for OpenGL
void Framebuffer_size_callback(GLFWwindow* window, int width, int height); //Callback function for when we resize the window
void mouse_callback(GLFWwindow* window, double xpos, double ypos); //callback function for mouse inputs. Mouse X and Y 
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset); // Callback function for mouse scroll
void ProcessInput(GLFWwindow* window); 
void SetupLights(Shader& shader, Camera& camera); //Light parameters are set here
void EnableOutline(Model& mod, Shader& modShader, Shader& outlineShader, glm::mat4& projection, glm::mat4& view, glm::mat4& model);
bool IsCursorOnModel(double xPos, double yPos, const glm::vec3& modPos, float tolerance);

//Screen dimensions
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_LENGTH= 600;

//Textures directory
static const std::string textureDirectory = "D:/Personal Project Repos/OpenGL Test/OpenGLTest/OpenGLTest/Resources/Textures";

////Vertices of a cube with pos, normals, color and tex
//Vertex vertices[] = {
//	// positions			  //Normals				// colors         // texture 
////																		// coords
//	// Front face
//	{glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
//	{glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
//	{glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
//	{glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
//	// Back face
//	{glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
//	{glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
//	{glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
//	{glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
//	// Left face
//	{glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
//	{glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
//	{glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
//	{glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
//	// Right face
//	{glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
//	{glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
//	{glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
//	{glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
//	// Top face
//	{glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
//	{glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
//	{glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
//	{glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
//	// Bottom face
//	{glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
//	{glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
//	{glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
//	{glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)}
//};
//
////indices of the vertices defined above
//GLuint indices[] = 
//{  
//	// Front face
//		0, 1, 2,
//		2, 3, 0,
//		// Back face
//		4, 5, 6,
//		6, 7, 4,
//		// Left face
//		8, 9, 10,
//		10, 11, 8,
//		// Right face
//		12, 13, 14,
//		14, 15, 12,
//		// Top face
//		16, 17, 18,
//		18, 19, 16,
//		// Bottom face
//		20, 21, 22,
//		22, 23, 20
//};

// Vertices coordinates
Vertex vertices[] =
{ 
	{glm::vec3(-1.0f, 0.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
	{glm::vec3(-1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
	{glm::vec3(1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
	{glm::vec3(1.0f, 0.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)}
};

// Indices for vertices order
GLuint indices[] =
{
	0, 1, 2,
	0, 2, 3
};

//Camera object with initial pos
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

//Mouse Inputs
float lastX = SCR_WIDTH / 2.0;
float lastY = SCR_LENGTH / 2.0;
bool firstMouse = true;

//Time
float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

//Light Cube
Vertex lightVertices[] =
{ //     COORDINATES     //
	Vertex{glm::vec3(-0.5f, -0.5f,  0.5f)},
	Vertex{glm::vec3(0.5f, -0.5f,  0.5f)},
	Vertex{glm::vec3(0.5f,  0.5f,  0.5f)},
	Vertex{glm::vec3(-0.5f,  0.5f,  0.5f)},

	Vertex{glm::vec3(-0.5f, -0.5f, -0.5f)},
	Vertex{glm::vec3(0.5f, -0.5f, -0.5f)},
	Vertex{glm::vec3(0.5f,  0.5f, -0.5f)},	
	Vertex{glm::vec3(-0.5f,  0.5f, -0.5f)}
};

GLuint lightIndices[] =
{
	//Front face
	0, 1, 2,
	0, 2, 3,

	//Left Face
	4, 0, 3,
	4, 3, 7,

	//Back face
	5, 4, 7,
	5, 7, 6,

	//Top Face
	3, 2, 6,
	3, 6, 7,

	//Right face
	1, 5, 6,
	1, 6, 2,

	//Bottom face
	1, 0, 4,
	1, 4, 5
};

// positions of the point lights
glm::vec3 pointLightPositions[] = {
	glm::vec3(0.7f,  0.2f,  2.0f),
	glm::vec3(2.3f, -3.3f, -4.0f),
	glm::vec3(-4.0f,  2.0f, -12.0f),
	glm::vec3(0.0f,  0.0f, -3.0f)
};

glm::vec3 Pos = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 CurrentVelocity = glm::vec3(0.0f);
float acceleration = 5.0f;
float deceleration = 2.5f;
float maxspeed = 60.0f;

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

	//Set the current window as context for OpenGL
	glfwMakeContextCurrent(window);
	//Allow cursor but keep it disabled
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//On Window Resize, callback to update viewport
	glfwSetFramebufferSizeCallback(window, Framebuffer_size_callback);
	//On mouse movement, callback to update camera
	glfwSetCursorPosCallback(window, mouse_callback);
	//On scroll input, callback to update camera zoom (FOV)
	glfwSetScrollCallback(window, scroll_callback);

	//Initialize GLAD
	InitGlad();
	
	//Enable Depth Buffer
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST); //Enable Stencil buffer
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE); //stencil fail, stencil and depth fail, stencil and depth pass

	//Face Culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	//glFrontFace(GL_CCW);

	camera.SetScreenDimensions(SCR_WIDTH, SCR_LENGTH);

	//Initialize our default shaders
	Shader ourShader("D:/Personal Project Repos/OpenGL Test/OpenGLTest/OpenGLTest/VertexShader.vs", "D:/Personal Project Repos/OpenGL Test/OpenGLTest/OpenGLTest/FragmentShader.fs");

	//Model loading
	Model mod("D:/Personal Project Repos/OpenGL Test/OpenGLTest/OpenGLTest/Resources/Textures/backpack/backpack.obj", Pos);

	Texture textures[]
	{
		Texture(textureDirectory, "container2.png", "diffuse", 0, GL_UNSIGNED_BYTE),
		Texture(textureDirectory, "container2_specular.png", "specular", 1, GL_UNSIGNED_BYTE)
	};
	
	Texture lightTextures[]
	{
		Texture(textureDirectory, "rcube/ColorBase-1001.png", "diffuse", 0, GL_UNSIGNED_BYTE)
	};

	// Store mesh data in vectors for the mesh
	std::vector <Vertex> verts(vertices, vertices + sizeof(vertices) / sizeof(Vertex));
	std::vector <GLuint> ind(indices, indices + sizeof(indices) / sizeof(GLuint));
	std::vector <Texture> tex(textures, textures + sizeof(textures) / sizeof(Texture));
	Mesh floor(verts, ind, tex);

#pragma region Light Cube
	////Initialize Light Shader
	Shader lightShader("D:/Personal Project Repos/OpenGL Test/OpenGLTest/OpenGLTest/LightShader.vs", "D:/Personal Project Repos/OpenGL Test/OpenGLTest/OpenGLTest/LightShader.fs");

	// Store mesh data in vectors for the mesh
	std::vector <Vertex> lightVerts(lightVertices, lightVertices + sizeof(lightVertices) / sizeof(Vertex));
	std::vector <GLuint> lightInd(lightIndices, lightIndices + sizeof(lightIndices) / sizeof(GLuint));
	std::vector <Texture> lightTex(lightTextures, lightTextures + sizeof(lightTextures) / sizeof(Texture));

	Mesh lightCube(lightVerts, lightInd, lightTex);

#pragma endregion Light Cube
	
	Shader outlineShader("D:/Personal Project Repos/OpenGL Test/OpenGLTest/OpenGLTest/Outline.vs", "D:/Personal Project Repos/OpenGL Test/OpenGLTest/OpenGLTest/Outline.fs");

	//Set Light Color, the value is fed into LightShader.fs
	glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);

	float radius = 5.0f;
	float speed = 2.0f;
	float tolerance = 0.95f;

	//Render Loop
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//Input
		ProcessInput(window);
		mod.HandleVehicleInputs(window, deltaTime);
		
		//Render Call
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		//Activate Shader
		ourShader.Activate();
		ourShader.setVec3("viewPos", camera.Position); //Update view position with the current camera position
		
		//Setup lights
		SetupLights(ourShader, camera);

		//// pass projection matrix to shader (note that in this case it could change every frame)
		//glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_LENGTH, 0.1f, 100.0f);
		//ourShader.setMat4("projection", projection);

		//// camera/view transformation
		//glm::mat4 view = camera.GetViewMatrix();
		//ourShader.setMat4("view", view);

		//glm::mat4 model = glm::mat4(1.0f);
		//model = glm::translate(model, Pos);
		//model = glm::scale(model, glm::vec3(0.025f));
		//ourShader.setMat4("model", model);

		//glm::mat4 gVPM = projection * view * model;
		//glm::vec3 vpm_vec = gVPM[3];

		/*if (IsCursorOnModel(lastX / SCR_WIDTH, lastY / SCR_LENGTH, vpm_vec, 0.5f))
		{
			EnableOutline(mod, ourShader, outlineShader, projection, view, model);
		}

		else 
		{
			mod.Draw(ourShader);
		}*/
		
		//mod.PrintData();
		
		//Draw w/o outline
		mod.Draw(ourShader, camera);
		//mod.Position = Pos;

		camera.UpdateCameraMatrix(ourShader);

		glm::mat4 floorModel = glm::mat4(1.0f);
		floorModel = glm::translate(floorModel, glm::vec3(0.0f));
		floorModel = glm::scale(floorModel, glm::vec3(3.0f));
		ourShader.setMat4("model", floorModel);
		floor.Draw(ourShader);
			

#pragma region Light Cube draw
		//Activate light shader
		lightShader.Activate();
		
		/*lightShader.setMat4("projection", projection);
		lightShader.setMat4("view", view);*/

		camera.UpdateCameraMatrix(lightShader);

		for (unsigned int i = 0; i < 1; i++)
		{
			glm::mat4 lightModel = glm::mat4(1.0f);

			pointLightPositions[i].x = radius * cos(speed * currentFrame);
			pointLightPositions[i].y = radius * sin(speed * currentFrame);

			lightModel = glm::translate(lightModel, pointLightPositions[i]);
			lightModel = glm::scale(lightModel, glm::vec3(0.2f)); // Make it a smaller cube
			lightShader.setMat4("model", lightModel);
			lightShader.setVec3("lightColor", lightColor);
			lightCube.Draw(lightShader);
		}
#pragma endregion Light Cube draw

		//check and call events and swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	
	ourShader.Delete();
	lightShader.Delete();

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
	camera.SetScreenDimensions(width, height);
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
	
	//float velocity = 1.0f * deltaTime;
	//if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	//{	
	//	//camera.ProcessKeyboard(FORWARD, deltaTime);
	//	Pos += glm::vec3(0.0f, 0.0f, -velocity);
	//}
	//
	//if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	//{
	//	//camera.ProcessKeyboard(BACKWARD, deltaTime);
	//	Pos += glm::vec3(0.0f, 0.0f, velocity);
	//}

	//if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	//{	
	//	//camera.ProcessKeyboard(LEFT, deltaTime);
	//	Pos += glm::vec3(-velocity, 0.0f, 0.0f);
	//}

	//if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	//{	
	//	//camera.ProcessKeyboard(RIGHT, deltaTime);
	//	Pos += glm::vec3(velocity, 0.0f, 0.0f);
	//}
}

void SetupLights(Shader& shader, Camera& camera) 
{
	//ourShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
	shader.setFloat("material.shininess", 32.0f);

	//Directional Light
	shader.setVec3("dirLight.direction", 0.0f, -1.0f, 0.0f);
	shader.setVec3("dirLight.ambient", 0.25f, 0.25f, 0.25f);
	shader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
	shader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

	// point light 1
	shader.setVec3("pointLights[0].position", pointLightPositions[0]);
	shader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
	shader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
	shader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
	shader.setFloat("pointLights[0].constant", 1.0f);
	shader.setFloat("pointLights[0].linear", 0.09f);
	shader.setFloat("pointLights[0].quadratic", 0.032f);
	// point light 2
	shader.setVec3("pointLights[1].position", pointLightPositions[1]);
	shader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
	shader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
	shader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
	shader.setFloat("pointLights[1].constant", 1.0f);
	shader.setFloat("pointLights[1].linear", 0.09f);
	shader.setFloat("pointLights[1].quadratic", 0.032f);
	// point light 3
	shader.setVec3("pointLights[2].position", pointLightPositions[2]);
	shader.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
	shader.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
	shader.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
	shader.setFloat("pointLights[2].constant", 1.0f);
	shader.setFloat("pointLights[2].linear", 0.09f);
	shader.setFloat("pointLights[2].quadratic", 0.032f);
	// point light 4
	shader.setVec3("pointLights[3].position", pointLightPositions[3]);
	shader.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
	shader.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
	shader.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
	shader.setFloat("pointLights[3].constant", 1.0f);
	shader.setFloat("pointLights[3].linear", 0.09f);
	shader.setFloat("pointLights[3].quadratic", 0.032f);
	// spotLight
	shader.setVec3("spotLight.position", camera.Position);
	shader.setVec3("spotLight.direction", camera.Front);
	shader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
	shader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
	shader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
	shader.setFloat("spotLight.constant", 1.0f);
	shader.setFloat("spotLight.linear", 0.09f);
	shader.setFloat("spotLight.quadratic", 0.032f);
	shader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
	shader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));
}

bool IsCursorOnModel(double xPos, double yPos, const glm::vec3& modPos, float tolerance) 
{
	return abs(modPos.x - xPos) <= tolerance && abs(modPos.y - yPos) <= tolerance;
}

//Stencil Buffer
void EnableOutline(Model& mod, Shader& modShader, Shader& outlineShader, glm::mat4& projection, glm::mat4& view, glm::mat4& model) 
{
	//Theory of Outlining models:
		//1. Render our object normally
		//2. Update Stencil Buffer with value 1 wherever we have a fragment of the object. And to 0 everywhere we do not have a fragment to draw
		//3. Disable writing to the stencil buffer so we dont accidentally modify it. 
		//   We also disable depth testing, so the next object we draw will completely be in front of the previous one. New object = Stencil. Previous = Color
		//4. We render a scaled up version of the new object with flat color and with the condition to draw its stencil value whereever fragment value is NOT 1
		//5. Restore writing to the stencil and enable depth buffer again.

		//Stencil Test always passes 
	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	//Enable modification of the stencil buffer
	glStencilMask(0xFF);

	//cube.Draw(ourShader);
	//Draw our model
	mod.Draw(modShader, camera);

	//Make it so only the pixels without the value 1 pass the test
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	//Disable stencil and depth 
	glStencilMask(0x00);
	glDisable(GL_DEPTH_TEST);

	outlineShader.Activate();
	outlineShader.setFloat("outlining", 0.01f);
	outlineShader.setMat4("projection", projection);
	outlineShader.setMat4("view", view);
	outlineShader.setMat4("model", model);
	mod.Draw(outlineShader, camera);

	//Enable stencil buffer modification again
	glStencilMask(0xFF);
	//This time only render pixel that are not 1 or are 0
	glStencilFunc(GL_ALWAYS, 0, 0xFF);
	//Enable depth buffer 
	glEnable(GL_DEPTH_TEST);
}