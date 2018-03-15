#include "App.h"
#include "string"
#include <sstream>
#include <iostream>
#include "ShaderProgram.h"
#include "Texture.h"
#include "glm/gtc/matrix_transform.hpp"
#include "Camera.h"

bool App::bFullScreen = false;
GLFWwindow* App::m_pWindow = nullptr;

void App::Init()
{
	/// initialize GLFW
	if (!glfwInit())
	{		
		throw std::string("Failed to initialize GLFW");
	}	
}

void App::CreateWindow(int& width, int& height, const char* title, void(*pOnKey)(GLFWwindow*, int, int, int, int), void(*pOnMove)(GLFWwindow*, double, double))
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

	Texture texture;
	if (!texture.LoadTexture(".\\textures\\wooden_crate.jpg", true))
	{
		std::cerr << "Error while reading texture" << std::endl;
	};
	Texture texture2;
	if (!texture2.LoadTexture(".\\textures\\grid.jpg", true))
	{
		std::cerr << "Error while reading texture" << std::endl;
	};

	objects obj = run();//static buffer draw		

	//cube position
	glm::vec3 cubePos = glm::vec3(0.0f, 0.0f, -5.0f);

	//cube angle
	float cubeAngle = 0.0f;

	double lastTime = glfwGetTime();

	OrbitCamera orbitCamera;

	while (!glfwWindowShouldClose(m_pWindow))
	{
		showFPS(m_pWindow);

		double currentTime = glfwGetTime();
		double deltaTime = currentTime - lastTime;

		glfwPollEvents();		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		

		texture.Bind(0);		

		glm::mat4 model, view, projection;

		orbitCamera.SetLookAt(cubePos);		
		orbitCamera.SetRadius(radius);
		orbitCamera.Rotate(yaw, pitch);		
		model = glm::translate(model, cubePos);
		
		view = orbitCamera.GetViewMatrix();

		projection = glm::perspective(glm::radians(60.0f), float(width/height), 0.1f, 100.0f);

		shaderProgram.Use();	

		shaderProgram.SetUniform("model", model);
		shaderProgram.SetUniform("view", view);
		shaderProgram.SetUniform("projection", projection);			

		glBindVertexArray(obj.vao);
		glDrawArrays(GL_TRIANGLES, 0, 36);		

		texture2.Bind(0);

		glm::vec3 floorPos;
		floorPos.y = -1.0f;
		floorPos.z = 5.0f;
		model = glm::translate(model, floorPos) * glm::scale(model, glm::vec3(10.0f, 0.01f, 10.0f));
		shaderProgram.SetUniform("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(0);

		glfwSwapBuffers(m_pWindow);

		lastTime = currentTime;
	}

	shaderProgram.DeleteProgram();
	glDeleteVertexArrays(1, &obj.vao);
	glDeleteBuffers(1, &obj.vbo);
	if (&obj.vbo1 != NULL)
	{
		glDeleteBuffers(1, &obj.vbo1);
	}
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