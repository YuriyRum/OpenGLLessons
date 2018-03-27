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

		/*
		if (key == GLFW_KEY_W)
		{
			bWireFRame = !bWireFRame;

			if (bWireFRame)
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			}
			else
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}
		}
		*/
	};

	void(*pWindowResize)(GLFWwindow*, int, int) = [](GLFWwindow* window, int width, int height)
	{
		WIDTH = width;
		HEIGHT = height;

		glViewport(0, 0, WIDTH, HEIGHT);
	};

	void(*pCursorCallback)(GLFWwindow *, double, double) = [](GLFWwindow * window, double x, double y)
	{
		/*
		static glm::vec2 lastMousePos = glm::vec2(0, 0);

		/// Update angels based on Left Mouse Button input to orbit	
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == 1)
		{
			yaw -= (float(x) - lastMousePos.x) * MOUSE_SENSITIVITY;
			pitch += (float(y) - lastMousePos.y) * MOUSE_SENSITIVITY;
		}

		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == 1)
		{
			float dx = 0.01f * (float(x) - lastMousePos.x);
			float dy = 0.01f * (float(y) - lastMousePos.y);

			radius += dx - dy;
		}

		lastMousePos.x = float(x);
		lastMousePos.y = float(y);
		*/
	};

	void(*pScrollCallback)(GLFWwindow*, double, double) = [](GLFWwindow*, double deltaX, double deltaY) 
	{
		
	};

	/// interleaved
	objects (*pRun)() = []() -> objects
	{		
		GLfloat vertices[] =
		{
			// position       // color  
		    0.0f,  0.5f, 0.0, 1.0f, 0.0f, 0.0f, //top
			0.5f, -0.5f, 0.0, 0.0f, 1.0f, 0.0f, //right
			-0.5f,-0.5f, 0.0, 0.0f, 0.0f, 1.0f	//left
		};

		GLuint vbo;
		GLuint vao;

		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //GL_DYNAMIC_DRAW

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*6, NULL);		
		glEnableVertexAttribArray(0);		

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (GLvoid*)(sizeof(GLfloat)*3));
		glEnableVertexAttribArray(1);

		objects obj{ vbo, NULL, vao };

		return obj;
	};

	/// separate
	objects(*pRunSeparate)() = []() -> objects
	{		
		GLfloat vert_position[] =
		{
			// position      
			0.0f,  0.5f, 0.0,
			0.5f, -0.5f, 0.0,
			-0.5f,-0.5f, 0.0
		};

		GLfloat vert_color[] =
		{
			// color  
			1.0f, 0.0f, 0.0f, //top
			0.0f, 1.0f, 0.0f, //right
			0.0f, 0.0f, 1.0f  //left
		};

		GLuint vbo, vbo_color;
		GLuint vao;

		glGenBuffers(1, &vbo);		
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vert_position), vert_position, GL_STATIC_DRAW); //GL_DYNAMIC_DRAW

		glGenBuffers(1, &vbo_color);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_color);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vert_color), vert_color, GL_STATIC_DRAW);

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, vbo_color);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(1);

		objects obj{ vbo, vbo_color, vao };

		return obj;
	};

	/// interleaved
	objects(*pSquad)() = []() -> objects
	{		
		GLfloat vertices[] =
		{
			/// first triangle
			-0.5f,  0.5f, 0.0, 
			 0.5f, 0.5f, 0.0, 
			 0.5f,-0.5f, 0.0, 			 			 
			-0.5f,  -0.5f, 0.0,
		};

		GLuint indeces[] = 
		{
			0,1,2,
			0,2,3
		};

		GLuint vbo;
		GLuint vao;
		GLuint ibo;

		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //GL_DYNAMIC_DRAW

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(0);

		glGenBuffers(1, &ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indeces), indeces, GL_STATIC_DRAW);

		objects obj{ vbo, ibo, vao };

		return obj;
	};

	objects(*pTexture)() = []() -> objects
	{
		GLfloat vertices[] =
		{
			/// quard            // texture coordinates  
			-0.5f,  0.5f, 0.0,  0.0f, 1.0f,
			0.5f, 0.5f, 0.0,    1.0f, 1.0f,
			0.5f,-0.5f, 0.0,    1.0f, 0.0f,
		    -0.5f,  -0.5f, 0.0, 0.0f, 0.0f,
		};

		GLuint indeces[] =
		{
			0,1,2,
			0,2,3
		};

		GLuint vbo;
		GLuint vao;
		GLuint ibo;

		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //GL_DYNAMIC_DRAW

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (GLvoid*)(0));
		glEnableVertexAttribArray(0);
		
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);

		glGenBuffers(1, &ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indeces), indeces, GL_STATIC_DRAW);

		objects obj{ vbo, ibo, vao };

		return obj;
	};

	objects(*pCube)() = []() -> objects
	{
		GLfloat vertices[] = {
			// position		 // tex coords

			// front face
			-1.0f,  1.0f,  1.0f, 0.0f, 1.0f,
			1.0f, -1.0f,  1.0f, 1.0f, 0.0f,
			1.0f,  1.0f,  1.0f, 1.0f, 1.0f,
			-1.0f,  1.0f,  1.0f, 0.0f, 1.0f,
			-1.0f, -1.0f,  1.0f, 0.0f, 0.0f,
			1.0f, -1.0f,  1.0f, 1.0f, 0.0f,

			// back face
			-1.0f,  1.0f, -1.0f, 0.0f, 1.0f,
			1.0f, -1.0f, -1.0f, 1.0f, 0.0f,
			1.0f,  1.0f, -1.0f, 1.0f, 1.0f,
			-1.0f,  1.0f, -1.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -1.0f, 1.0f, 0.0f,

			// left face
			-1.0f,  1.0f, -1.0f, 0.0f, 1.0f,
			-1.0f, -1.0f,  1.0f, 1.0f, 0.0f,
			-1.0f,  1.0f,  1.0f, 1.0f, 1.0f,
			-1.0f,  1.0f, -1.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
			-1.0f, -1.0f,  1.0f, 1.0f, 0.0f,

			// right face
			1.0f,  1.0f,  1.0f, 0.0f, 1.0f,
			1.0f, -1.0f, -1.0f, 1.0f, 0.0f,
			1.0f,  1.0f, -1.0f, 1.0f, 1.0f,
			1.0f,  1.0f,  1.0f, 0.0f, 1.0f,
			1.0f, -1.0f,  1.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -1.0f, 1.0f, 0.0f,

			// top face
			-1.0f,  1.0f, -1.0f, 0.0f, 1.0f,
			1.0f,  1.0f,  1.0f, 1.0f, 0.0f,
			1.0f,  1.0f, -1.0f, 1.0f, 1.0f,
			-1.0f,  1.0f, -1.0f, 0.0f, 1.0f,
			-1.0f,  1.0f,  1.0f, 0.0f, 0.0f,
			1.0f,  1.0f,  1.0f, 1.0f, 0.0f,

			// bottom face
			-1.0f, -1.0f,  1.0f, 0.0f, 1.0f,
			1.0f, -1.0f, -1.0f, 1.0f, 0.0f,
			1.0f, -1.0f,  1.0f, 1.0f, 1.0f,
			-1.0f, -1.0f,  1.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -1.0f, 1.0f, 0.0f,
		};

		GLuint vbo;
		GLuint vao;
		
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //GL_DYNAMIC_DRAW

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0));
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);
		
		objects obj{ vbo, NULL, vao };

		return obj;
	};

	try 
	{
		App::Init();
		App::CreateWindow(WIDTH, HEIGHT, pAppTitle, pCallback, pCursorCallback, pScrollCallback);
		App::Run(pCube, WIDTH, HEIGHT, radius, yaw, pitch);
		App::Utilize();
	}	
	catch (std::string e)
	{
		App::Utilize();
		std::cout << e << std::endl;
	}
	return 0;
}