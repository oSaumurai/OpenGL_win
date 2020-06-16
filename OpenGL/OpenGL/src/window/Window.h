#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "tests/TestClearColor.h"
#include "tests/TestTexture2D.h"
#include "tests/TestTexture3D.h"
#include "tests/TestLoader.h"

#include "Shader.h"

#include "controller/Mouse_Controller.h"
#include "controller/KeyboardController.h"
class Window
{
private:
	int window_height, window_witdth;

	void initGLfw();
	void initOpenGLOption();
	void initGLew();
	void initGUI();

	void initController();
public:
	bool shouldClose();

	GLFWwindow* window;
	MouseController* mouseController;
	KeyboardController* keyboardController;
	test::Test* currentTest;
	test::TestMenu* testMenu;

public:
	Window(int height, int width);
	~Window();
	
	void Update();
	void Draw();
};

