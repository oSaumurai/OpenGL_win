#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "window/window.h"
#include "tests/TestLoader.h"

class Game
{
public:
	Window* window;
	
	void Init();
	void LoadContent();
	void UnLoadContent();
	void Update();
	void Draw();
private:
	void initController();
	//void loadObjects();
};

