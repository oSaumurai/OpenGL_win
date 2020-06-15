#include "Controller.h"

Controller::Controller() {
	// Nothing
}

Controller::Controller(GLFWwindow* window) {
	this->window = window;
}

Controller::~Controller() {
	// Nothing
}

void Controller::AttachWindowInput(GLFWwindow* window)
{
	this->window = window;
}
