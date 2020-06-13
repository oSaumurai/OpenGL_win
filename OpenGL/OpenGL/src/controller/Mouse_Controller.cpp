#include "Mouse_Controller.h"

MouseController::MouseController(GLFWwindow* window, Player* player) : Controller(window)
{
	this->player = player;
	initParams();
}

void MouseController::ProcessMouseMovement()
{
	if (is_first_mouse) {
		last_mouse_x = mouse_x;
		last_mouse_y = mouse_y;
		is_first_mouse = false;
	}

	mouse_offset_x = mouse_x - last_mouse_x;
	mouse_offset_y = mouse_y - last_mouse_y;
	last_mouse_x = mouse_x;
	last_mouse_y = mouse_y;
}

void MouseController::ProcessMouseScroll()
{
	zoom -= (float)mouse_offset_y;
	if (zoom < 1.0f)
		zoom = 1.0f;
	if (zoom > 45.0f)
		zoom = 45.0f;
}

void MouseController::initParams() {
	last_mouse_x = 0.0;
	last_mouse_y = 0.0;
	mouse_x = 0.0;
	mouse_y = 0.0;
	mouse_offset_x = 0.0;
	mouse_offset_y = 0.0;
	zoom = 45.0f;
	is_first_mouse = true;
}

void MouseController::updateInput(float dt) {
	glfwGetCursorPos(this->window, &mouse_x, &mouse_y);
	// processMouse
	ProcessMouseMovement();
	ProcessMouseScroll();
	this->player->camera.updateByMouseInput(dt, mouse_offset_x, mouse_offset_y);
}