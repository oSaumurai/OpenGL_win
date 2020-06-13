#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "tests/TestClearColor.h"
#include "tests/TestTexture2D.h"
#include "tests/TestTexture3D.h"

#include "player/Player.h"
#include "controller/Mouse_Controller.h"
class Window
{
private:
	int window_height, window_witdth;
	const char* window_name;

	void initGLfw();
	void initOpenGLOption();
	void initGLew();
	void initGUI();

public:
	bool shouldClose();

	GLFWwindow* window;

	Player* player;
	MouseController* mouseController;

	test::Test* currentTest;
	test::TestMenu* testMenu;

public:
	Window(int height, int width);
	~Window();
	
	void Update();
	void Draw();
};

