#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Shader.h"
#include "Renderer.h"

#include "controller/Mouse_Controller.h"
#include "controller/KeyboardController.h"
const int HEIGHT = 1080;
const int WIDTH = 1920;

class Window
{	
private:
	int window_height, window_witdth;

	void initGLfw();
	void initOpenGLOption();
	void initGLew();
	void initGUI();
public:
	bool shouldClose();

	GLFWwindow* window;

public:
	Window();
	~Window();
	
	//void RegisterTest(test::Test*& currentTest, test::TestMenu*& testMenu);
	virtual void OnWindowUpdate();
	virtual void Update();
	virtual void Draw();
	void OnFinished();

protected:
};

