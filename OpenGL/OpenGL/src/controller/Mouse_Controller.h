#ifndef MOUSE_CONTROLLER_H
#define MOUSE_CONTROLLER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "controller.h"
#include "player/Player.h"

class MouseController : public Controller {
private:
	Player* player;

	double last_mouse_x;
	double last_mouse_y;
	double mouse_x;
	double mouse_y;
	double mouse_offset_x;
	double mouse_offset_y;
	float zoom;
	bool is_first_mouse;

	void initParams();

public:
	MouseController(GLFWwindow* window, Player* player);

	void ProcessMouseMovement();
	void ProcessMouseScroll();
	void updateInput(float dt);
};

#endif