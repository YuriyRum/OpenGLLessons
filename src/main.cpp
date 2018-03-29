#include <iostream>
#include <string>
#include "App.h"
#include "Camera.h"

#include "Math.h"

int WIDTH = 1024;
int HEIGHT = 768;

//float MOUSE_SENSITIVITY_ = 0.1f;
//const double ZOOM_SENSIVITY = -3.0f;
//const float MOVE_SPEED = 5.0f;


float yaw = 0.0f;
float pitch = 0.0f;
float radius = 10.0f;

int main()
{		
	const char* pAppTitle = "Test";

	void(*pCallback)(GLFWwindow*, int, int, int, int) = [](GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		static bool bWireFRame = false;
		if (action != GLFW_PRESS)
		{
			return;
		}
		if (key == GLFW_KEY_ESCAPE)
		{
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
	};

	void(*pWindowResize)(GLFWwindow*, int, int) = [](GLFWwindow* window, int width, int height)
	{
		WIDTH = width;
		HEIGHT = height;

		glViewport(0, 0, WIDTH, HEIGHT);
	};

	void(*pCursorCallback)(GLFWwindow *, double, double) = [](GLFWwindow * window, double x, double y)
	{

	};

	void(*pScrollCallback)(GLFWwindow*, double, double) = [](GLFWwindow*, double deltaX, double deltaY) 
	{
		
	};


	objects(*pNull)() = []() -> objects {return objects{}; };

	try 
	{
		App::Init();
		App::CreateWindow(WIDTH, HEIGHT, pAppTitle, pCallback, pCursorCallback, pScrollCallback);
		App::Run(pNull, WIDTH, HEIGHT, radius, yaw, pitch);
		App::Utilize();
	}	
	catch (std::string e)
	{
		App::Utilize();
		std::cout << e << std::endl;
	}
	return 0;
}