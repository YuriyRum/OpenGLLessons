#include "App.h"
#include "string"
#include <sstream>
#include <iostream>
#include "ShaderProgram.h"
#include "Texture.h"
#include "glm/gtc/matrix_transform.hpp"
#include "Camera.h"
#include "Mesh.h"

const double ZOOM_SENSIVITY = -3.0f;
const float MOVE_SPEED = 5.0f;
float MOUSE_SENSITIVITY = 0.1f;

bool App::bFullScreen = false;
GLFWwindow* App::m_pWindow = nullptr;

FPSCamera fpsCamera(glm::vec3(0.0f, 3.0f, 10.0f));

void App::Init()
{
	/// initialize GLFW
	if (!glfwInit())
	{		
		throw std::string("Failed to initialize GLFW");
	}	
}

void App::CreateWindow(int& width, int& height, const char* title, void(*pOnKey)(GLFWwindow*, int, int, int, int), void(*pOnMove)(GLFWwindow*, double, double), void(*pScroll)(GLFWwindow*, double, double))
{	
	/// hints for window creation
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	bFullScreen = false;
	if (bFullScreen)
	{
		GLFWmonitor* pMonitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* pMode = glfwGetVideoMode(pMonitor);
		if (pMode != nullptr)
		{
			glfwCreateWindow(pMode->width, pMode->height, title, pMonitor, NULL);
		}
	}
	else
	{
		m_pWindow = glfwCreateWindow(width, height, title, NULL, NULL);
	}
	if (m_pWindow == nullptr)
	{
		throw std::string("Failed to create window");
	}

	glfwSetKeyCallback(m_pWindow, pOnKey);
	glfwSetCursorPosCallback(m_pWindow, pOnMove);
	glfwMakeContextCurrent(m_pWindow);
	
	void(*pScrollCallback)(GLFWwindow*, double, double) = [](GLFWwindow*, double deltaX, double deltaY)
	{
		double fov = fpsCamera.GetFOV() + deltaY * ZOOM_SENSIVITY;
		fov = glm::clamp(fov, 1.0, 120.0);
		fpsCamera.SetFOV(float(fov));
	};
		
	glfwSetScrollCallback(m_pWindow, pScrollCallback);

	glfwSetInputMode(m_pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPos(m_pWindow, width / 2.0, height / 2.0);

	/// initialize GLEW
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		throw std::string("Failed to initialize GLEW");
	};
	glClearColor(0.23f, 0.38f, 0.47f, 1.0f);
	glEnable(GL_DEPTH_TEST);
}

void App::Run(objects(*run)(), int& width, int& height, float& radius, float& yaw, float& pitch)
{
	ShaderProgram shaderProgram;

	if (!shaderProgram.CreateProgram())
	{
		return;
	};

	shaderProgram.DeleteShaders();		

	// Model position
	glm::vec3 modelPos[] = 
	{
		glm::vec3(-2.5f, 1.0f, 0.0f), // crate
		glm::vec3(2.5f, 1.0f, 0.0f), // wood crate
		glm::vec3(0.0f, 1.0f, -2.0f), // robot
		glm::vec3(0.0f, 0.0f, 0.0f)  // floor
	};

	//Model scale
	glm::vec3 modelScale[] =
	{
		glm::vec3(1.0f, 1.0f, 1.0f), // crate
		glm::vec3(1.0f, 1.0f, 1.0f), // wood crate
		glm::vec3(1.0f, 1.0f, 1.0f), // robot
		glm::vec3(10.0f, 0.0f, 10.0f)  // floor
	};
	//init meshes
	Mesh mesh[4];
	Texture texture[4];

	mesh[0].LoadObj(".\\models\\crate.obj");
	mesh[1].LoadObj(".\\models\\woodcrate.obj");
	mesh[2].LoadObj(".\\models\\robot.obj");
	mesh[3].LoadObj(".\\models\\floor.obj");

	texture[0].LoadTexture(".\\textures\\crate.jpg", true);
	texture[1].LoadTexture(".\\textures\\woodcrate_diffuse.jpg", true);
	texture[2].LoadTexture(".\\textures\\robot_diffuse.jpg", true);
	texture[3].LoadTexture(".\\textures\\tile_floor.jpg", true);

	double lastTime = glfwGetTime();	
	
	while (!glfwWindowShouldClose(m_pWindow))
	{
		showFPS(m_pWindow);

		double currentTime = glfwGetTime();
		double deltaTime = currentTime - lastTime;

		glfwPollEvents();		
		update(deltaTime, width, height);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);				

		glm::mat4 model, view, projection;			
		
		view = fpsCamera.GetViewMatrix();

		projection = glm::perspective(glm::radians(fpsCamera.GetFOV()), float(width/height), 0.1f, 100.0f);

		shaderProgram.Use();			
		shaderProgram.SetUniform("view", view);
		shaderProgram.SetUniform("projection", projection);							

		for (int i = 0; i < 4; i++)
		{
			model = glm::translate(glm::mat4(), modelPos[i]) * glm::scale(glm::mat4(), modelScale[i]);
			shaderProgram.SetUniform("model", model);

			texture[i].Bind(0);
			mesh[i].Draw();
			texture[i].Unbind(0);
		}				
		glfwSwapBuffers(m_pWindow);
		lastTime = currentTime;
	}
	shaderProgram.DeleteProgram();		
}

void App::Utilize()
{
	glfwTerminate();
}

void App::showFPS(GLFWwindow* window)
{
	static double previousSeconds{ 0 };
	static int framesCount{ 0 };
	double elapsedSeconds{ 0.0 };
	double currentSeconds = glfwGetTime();

	elapsedSeconds = currentSeconds - previousSeconds;

	if (elapsedSeconds > 0.25)
	{
		previousSeconds = currentSeconds;
		double fps = static_cast<double>(framesCount / elapsedSeconds);
		double msPerFrame = 1000.0 / fps;

		std::ostringstream outs;
		outs.precision(3);
		outs << std::fixed
			<< "FPS: " << fps
			<< " Frame time: " << msPerFrame << "(ms)";

		glfwSetWindowTitle(window, outs.str().c_str());

		framesCount = 0;
	}
	framesCount++;
};

void App::update(float elapsedTime, int& width, int& height)
{
	double mouseX, mouseY;

	glfwGetCursorPos(m_pWindow, &mouseX, &mouseY);
	fpsCamera.Rotate(float(width / 2.0 - mouseX) * MOUSE_SENSITIVITY, float(height / 2.0 - mouseY) * MOUSE_SENSITIVITY );
	glfwSetCursorPos(m_pWindow, width / 2.0, height / 2.0);

	/// Camera movement

	// Forward/Backward
	if (glfwGetKey(m_pWindow, GLFW_KEY_W) == GLFW_PRESS)
	{
		fpsCamera.Move(MOVE_SPEED * float(elapsedTime) * fpsCamera.GetLook());
	}
	else if (glfwGetKey(m_pWindow, GLFW_KEY_S) == GLFW_PRESS)
	{
		fpsCamera.Move(MOVE_SPEED * float(elapsedTime) * -fpsCamera.GetLook());
	}
	else if (glfwGetKey(m_pWindow, GLFW_KEY_A) == GLFW_PRESS)
	{
		fpsCamera.Move(MOVE_SPEED * float(elapsedTime) * -fpsCamera.GetRight());
	}
	else if (glfwGetKey(m_pWindow, GLFW_KEY_D) == GLFW_PRESS)
	{
		fpsCamera.Move(MOVE_SPEED * float(elapsedTime) * fpsCamera.GetRight());
	}
	else if (glfwGetKey(m_pWindow, GLFW_KEY_Z) == GLFW_PRESS)
	{
		fpsCamera.Move(MOVE_SPEED * float(elapsedTime) * fpsCamera.GetUp());
	}
	else if (glfwGetKey(m_pWindow, GLFW_KEY_X) == GLFW_PRESS)
	{
		fpsCamera.Move(MOVE_SPEED * float(elapsedTime) * -fpsCamera.GetUp());
	}
};