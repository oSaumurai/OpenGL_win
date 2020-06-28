#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Renderer.h"

class Texture_Depthmap
{
public:
	Texture_Depthmap();
	~Texture_Depthmap();

	void Bind();
	void UnBind();
private:
	unsigned int id;
};

