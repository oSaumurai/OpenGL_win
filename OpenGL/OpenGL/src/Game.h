#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Camera/Camera.h"

#include "tests/TestClearColor.h"
#include "tests/TestTexture2D.h"
#include "tests/TestTexture3D.h"
#include "tests/TestLoader.h"
class Game :public Window
{
public:
	Game();
	~Game();

	MouseController* mouseController;
	KeyboardController* keyboardController;
	test::Test* currentTest;
	test::TestMenu* testMenu;

	void Init();
	void LoadContent();
	void UnLoadContent();
	void OnWindowUpdate() override;
	void Update() override;
	void Draw() override;

private:
	void initController();
	void initTestMember();
};

