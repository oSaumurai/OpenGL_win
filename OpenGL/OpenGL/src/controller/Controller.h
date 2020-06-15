#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Controller {
protected:
	GLFWwindow* window;

public:
	Controller();
	Controller(GLFWwindow* window);
	~Controller();

	virtual void AttachWindowInput(GLFWwindow* window);
	virtual void updateInput() {}
};

#endif