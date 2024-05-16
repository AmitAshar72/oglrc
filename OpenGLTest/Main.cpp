#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "RaceManager.h"
#include "Vehicle.h"
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
const unsigned int SCR_WIDTH = 1600;
const unsigned int SCR_LENGTH= 1200;

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
	2, 1, 0,
	3, 2, 0
};

Vertex trackVertices[] = {
	// Bottom surface
	// Triangle 1
	{glm::vec3(-10.0f, 0.0f, -10.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(0.0f, 0.0f)}, // Vertex 1
	{glm::vec3(-10.0f, 0.0f,  10.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(0.0f, 1.0f)}, // Vertex 2
	{glm::vec3(10.0f, 0.0f,  10.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(1.0f, 1.0f)}, // Vertex 3
	// Triangle 2
	{glm::vec3(-10.0f, 0.0f, -10.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(0.0f, 0.0f)}, // Vertex 4
	{glm::vec3(10.0f, 0.0f,  10.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(1.0f, 1.0f)}, // Vertex 5
	{glm::vec3(10.0f, 0.0f, -10.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(1.0f, 0.0f)}, // Vertex 6

	// Sidewalls
	// Left wall
	// Triangle 1
	{glm::vec3(-10.0f, 0.0f, -10.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec2(0.0f, 0.0f)}, // Vertex 1
	{glm::vec3(-10.0f, 2.0f, -10.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec2(0.0f, 1.0f)}, // Vertex 2
	{glm::vec3(-10.0f, 2.0f,  10.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec2(1.0f, 1.0f)}, // Vertex 3
	// Triangle 2					  
	{glm::vec3(-10.0f, 0.0f, -10.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec2(0.0f, 0.0f)}, // Vertex 4
	{glm::vec3(-10.0f, 2.0f,  10.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec2(1.0f, 1.0f)}, // Vertex 5
	{glm::vec3(-10.0f, 0.0f,  10.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec2(1.0f, 0.0f)}, // Vertex 6

	// Right wall
	// Triangle 1
	{glm::vec3(10.0f, 0.0f, -10.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec2(0.0f, 0.0f)}, // Vertex 1
	{glm::vec3(10.0f, 2.0f, -10.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec2(0.0f, 1.0f)}, // Vertex 2
	{glm::vec3(10.0f, 2.0f,  10.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec2(1.0f, 1.0f)}, // Vertex 3
	// Triangle 2
	{glm::vec3(10.0f, 0.0f, -10.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec2(0.0f, 0.0f)}, // Vertex 4
	{glm::vec3(10.0f, 2.0f,  10.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec2(1.0f, 1.0f)}, // Vertex 5
	{glm::vec3(10.0f, 0.0f,  10.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec2(1.0f, 0.0f)}  // Vertex 6
};


GLuint trackIndices[] = {
	// Bottom surface
	0, 1, 2,
	3, 4, 5,

	// Left wall
	6, 7, 8,
	9, 10, 11,

	// Right wall
	/*12, 13, 14,
	15, 16, 17*/

	14, 13, 12,
	17, 16, 15
};

//Camera object with initial pos
Camera camera(glm::vec3(0.0f, 1.0f, 3.0f));

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
	glm::vec3(9.95f,  2.1f,  -95.0f),
	glm::vec3(-9.95f,  2.1f, -25.0f),
	glm::vec3(9.95f,  2.1f, 25.0f),
	glm::vec3(-9.95f,  2.1f, 95.0f)
};

glm::vec3 Pos = glm::vec3(0.0f, 0.010f, 105.0f);
glm::vec3 CurrentVelocity = glm::vec3(0.0f);
float acceleration = 5.0f;
float deceleration = 2.5f;
float maxspeed = 60.0f;

bool ButtonPress = false;

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
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

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
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	camera.SetScreenDimensions(SCR_WIDTH, SCR_LENGTH);

	//Initialize our default shaders
	Shader ourShader("D:/Personal Project Repos/OpenGL Test/OpenGLTest/OpenGLTest/VertexShader.vs", "D:/Personal Project Repos/OpenGL Test/OpenGLTest/OpenGLTest/FragmentShader.fs");
	
	Shader colorShader("D:/Personal Project Repos/OpenGL Test/OpenGLTest/OpenGLTest/VertexShader.vs", "D:/Personal Project Repos/OpenGL Test/OpenGLTest/OpenGLTest/Color_FragmentShader.fs");


	//Model loading
	Vehicle mod("D:/Personal Project Repos/OpenGL Test/OpenGLTest/OpenGLTest/Resources/Textures/f1/Formula 1 mesh.obj", Pos, 1500, acceleration, deceleration, maxspeed);
	glm::vec3 modScale = glm::vec3(0.0025f);
	glm::vec3 modRotation = glm::vec3(0.0f, -90.0f, 0.0f);
	mod.SetScale(modScale);
	mod.SetRotation(modRotation);

	Texture textures[]
	{
		Texture(textureDirectory, "container2.png", "diffuse", 0, GL_UNSIGNED_BYTE),
		Texture(textureDirectory, "container2_specular.png", "specular", 1, GL_UNSIGNED_BYTE)
	};
	
	/*Texture lightTextures[]
	{
		Texture(textureDirectory, "rcube/ColorBase-1001.png", "diffuse", 2, GL_UNSIGNED_BYTE)
	};*/
	
	Texture raceLineTex[]
	{
		Texture(textureDirectory, "raceline.png", "diffuse", 0, GL_UNSIGNED_BYTE)
	};

	// Store mesh data in vectors for the mesh
	std::vector <Vertex> verts(trackVertices, trackVertices + sizeof(trackVertices) / sizeof(Vertex));
	std::vector <GLuint> ind(trackIndices, trackIndices + sizeof(trackIndices) / sizeof(GLuint));
	std::vector <Texture> tex(textures, textures + sizeof(textures) / sizeof(Texture));
	
	glm::vec3 trackPosition = glm::vec3(0.0f);
	glm::vec3 trackRotation = glm::vec3(0.0f);
	glm::vec3 trackScale = glm::vec3(1.0f, 1.0f, 10.0f);

	Mesh track(verts, ind, tex);

	track.UpdateBoundingBoxScale(trackScale);

	//Race Line
	std::vector <Vertex> lineVerts(vertices, vertices + sizeof(vertices) / sizeof(Vertex));
	std::vector <GLuint> lineInd(indices, indices + sizeof(indices) / sizeof(GLuint));
	std::vector <Texture> lineTex(raceLineTex, raceLineTex + sizeof(raceLineTex) / sizeof(Texture));
	glm::vec3 raceLineScale = glm::vec3(10.0f, 1.0f, 1.0f);
	glm::vec3 raceLineRotation = glm::vec3(0.0f, 0.0f, 0.0f);
	

	Mesh raceLine(lineVerts, lineInd, lineTex);
	glm::vec3 raceLinePosition = glm::vec3(0.0f, 0.01f, 95.0f);
	raceLine.UpdateBoundingBoxScale(raceLineScale);
	
	Mesh raceLine_finish(lineVerts, lineInd, lineTex);
	glm::vec3 raceLine_finishPosition = glm::vec3(0.0f, 0.01f, -95.0f);
	raceLine_finish.UpdateBoundingBoxScale(raceLineScale);
	

#pragma region Light Cube
	////Initialize Light Shader
	Shader lightShader("D:/Personal Project Repos/OpenGL Test/OpenGLTest/OpenGLTest/LightShader.vs", "D:/Personal Project Repos/OpenGL Test/OpenGLTest/OpenGLTest/LightShader.fs");

	// Store mesh data in vectors for the mesh
	std::vector <Vertex> lightVerts(lightVertices, lightVertices + sizeof(lightVertices) / sizeof(Vertex));
	std::vector <GLuint> lightInd(lightIndices, lightIndices + sizeof(lightIndices) / sizeof(GLuint));
	std::vector <Texture> lightTex;// (lightTextures, lightTextures + sizeof(lightTextures) / sizeof(Texture));
	
	//glm::vec3 lightPosition = glm::vec3(0.0f);
	glm::vec3 lightRotation = glm::vec3(0.0f);
	glm::vec3 lightScale = glm::vec3(0.2f);

	Mesh lightCube(lightVerts, lightInd, lightTex);

	lightCube.UpdateBoundingBoxScale(lightScale);

#pragma endregion Light Cube
	
	Shader outlineShader("D:/Personal Project Repos/OpenGL Test/OpenGLTest/OpenGLTest/Outline.vs", "D:/Personal Project Repos/OpenGL Test/OpenGLTest/OpenGLTest/Outline.fs");

	//Set Light Color, the value is fed into LightShader.fs
	glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);

	float radius = 5.0f;
	float speed = 2.0f;
	float tolerance = 0.95f;

	RaceManager rm(mod, track, raceLine, raceLine_finish);
	//double rpm = mod.CalculateRPM(50);
	double engineForce = mod.CalculateEngineForce(1200, mod.torqueCurve);
	mod.SetAcceleration(engineForce);

	//ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	//Render Loop
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;		

		//Input
		ProcessInput(window);
		mod.HandleVehicleInputs(window, deltaTime);

		camera.FollowModel(mod.ModelPosition, deltaTime);

		if (ButtonPress) 
		{
			mod.update(deltaTime);
		}

		//Render Call
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		
		//Let Imgui know that its a new frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

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
				
		raceLine.SetMeshProperties(ourShader, camera, raceLinePosition, raceLineRotation, raceLineScale);
		raceLine.Draw(ourShader);
		
		raceLine_finish.SetMeshProperties(ourShader, camera, raceLine_finishPosition, raceLinePosition, raceLineScale);
		raceLine_finish.Draw(ourShader);
		
		//SetupLights(colorShader, camera);
		track.SetMeshProperties(ourShader, camera, trackPosition, trackRotation, trackScale);
		track.Draw(ourShader);
			

#pragma region Light Cube draw
		
		for (unsigned int i = 0; i < 4; i++)
		{
			glm::mat4 lightModel = glm::mat4(1.0f);

			/*pointLightPositions[i].x = radius * cos(speed * currentFrame);
			pointLightPositions[i].y = radius * sin(speed * currentFrame);*/
			lightCube.SetMeshProperties(lightShader, camera, pointLightPositions[i], lightRotation, lightScale);
			lightShader.setVec3("lightColor", lightColor);
			lightCube.Draw(lightShader);
		}
#pragma endregion Light Cube draw

		//Imgui Window
		ImGui::Begin("Window, ImGui Window");
		ImGui::Text("Hello there adventurer");
		ImGui::Checkbox("Accelerate", &ButtonPress);
		ImGui::Text("Accelerate:  %.2f", mod.GetAcceleration());
		if (rm.getElapsedTime() != 0) 
		{
			ImGui::Text("Race time:  %.2f", rm.getElapsedTime());
		}
		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		//RaceManager
		rm.update(deltaTime);	
		rm.getElapsedTime();
		//check and call events and swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	
	ourShader.Delete();
	lightShader.Delete();
	colorShader.Delete();

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

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

	//camera.ProcessMouseMovement(xoffset, yoffset);
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

	if (!ButtonPress && glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
	{
		ButtonPress = true;
	}

	/*if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);*/
}

void SetupLights(Shader& shader, Camera& camera) 
{
	shader.Activate();

	//ourShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
	shader.setFloat("material.shininess", 32.0f);

	//Directional Light
	shader.setVec3("dirLight.direction", 1.0f, -1.0f, 0.0f);
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