#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Renderer.h"

class FrameBuffer
{
public:
	FrameBuffer(unsigned int width, unsigned int height);
	~FrameBuffer();

	void Bind();
	void UnBind();
private:
	unsigned int id;

};

