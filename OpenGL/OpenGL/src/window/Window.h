#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Renderer.h"
class Window
{
private:
	int window_height, window_witdth;
	const char* window_name;

	void initGLfw();
	void initOpenGLOption();
	void initGLew();

public:
	GLFWwindow* window;
	Window(int height, int width);
	~Window();
	
	void Update();
	void Draw();
};

