#pragma once

#define GLEW_STATIC

#include "GL/glew.h"
#include "GLFW/glfw3.h"

struct objects
{
	GLuint vbo;
	GLuint vbo1;
	GLuint vao;
};

class App
{
public:
	static bool bFullScreen;
	static GLFWwindow* m_pWindow;
public:
	static void Init();
	static void CreateWindow(int& width, int& height, const char* title, void(*pOnKey)(GLFWwindow*,int,int,int,int), void(*pOnMove)(GLFWwindow*, double, double), void(*pScroll)(GLFWwindow*, double, double));
	static void Run(objects(*run)(), int& width, int& height, float& radius, float& yaw, float& pitch);
	static void Utilize();
private:
	static void showFPS(GLFWwindow* window);
	static void update(float elapsedTime, int& width, int& height);
};