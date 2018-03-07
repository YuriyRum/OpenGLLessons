#include <iostream>
#include <string>
#include "App.h"

#define WIDTH 800
#define HEIGHT 600

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
			/// qurd            // texture coordinates  
			-0.5f,  0.5f, 0.0,  0.0f, 1.0f,
			0.5f, 0.5f, 0.0,    1.0f, 1.0f,
			0.5f,-0.5f, 0.0,    1.0f, 0.0f
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

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(0);

		glGenBuffers(1, &ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indeces), indeces, GL_STATIC_DRAW);

		objects obj{ vbo, ibo, vao };

		return obj;
	};

	try 
	{
		App::Init();
		App::CreateWindow(WIDTH, HEIGHT, pAppTitle, pCallback);
		App::Run(pSquad);
		App::Utilize();
	}	
	catch (std::string e)
	{
		App::Utilize();
		std::cout << e << std::endl;
	}
	return 0;
}