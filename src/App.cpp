#include "App.h"
#include "string"
#include <sstream>
#include <iostream>
#include "ShaderProgram.h"

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

void App::CreateWindow(int width, int height, const char* title, void(*pOnKey)(GLFWwindow*, int, int, int, int))
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
	glfwMakeContextCurrent(m_pWindow);

	/// initialize GLEW
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		throw std::string("Failed to initialize GLEW");
	};
	glClearColor(0.23f, 0.38f, 0.47f, 1.0f);
}

void App::Run(objects(*run)())
{
	ShaderProgram shaderProgram;

	if (!shaderProgram.CreateProgram())
	{
		return;
	};

	shaderProgram.DeleteShaders();

	objects obj = run();//static buffer draw		

	while (!glfwWindowShouldClose(m_pWindow))
	{
		showFPS(m_pWindow);
		glfwPollEvents();		
		glClear(GL_COLOR_BUFFER_BIT);		

		shaderProgram.Use();	

		GLfloat time = glfwGetTime();		
		
		glm::vec2 positionOffset;
		positionOffset.x = std::sin(time) / 2;
		positionOffset.y = std::cos(time) / 2;
		shaderProgram.SetUniform("positionOffset", positionOffset);

		shaderProgram.SetUniform("color", glm::vec4(0.0f, 0.5f, std::sin(time) / 2 + 0.5f, 1.0f));

		glBindVertexArray(obj.vao);
		//glDrawArrays(GL_TRIANGLES, 0, 6);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glfwSwapBuffers(m_pWindow);
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