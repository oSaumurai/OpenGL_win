#include "Mouse_Controller.h"
MouseController* MouseController::instance = NULL;

MouseController* MouseController::getInstance()
{
	if (!instance)
	{
		instance = new MouseController();
	}
	return instance;
}

MouseController::MouseController()
{
	last_mouse_x = 0.0;
	last_mouse_y = 0.0;
	mouse_x = 0.0;
	mouse_y = 0.0;
	mouse_offset_x = 0.0;
	mouse_offset_y = 0.0;
	zoom = 45.0f;
	//is_first_mouse = true;
}

glm::vec2 MouseController::GetMouseOffset()
{
	return glm::vec2(mouse_offset_x,mouse_offset_y);
}

void MouseController::ProcessMouseMovement()
{
	if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS)
	{
		mouse_offset_x = mouse_x - last_mouse_x;
		mouse_offset_y = mouse_y - last_mouse_y;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_RELEASE)
	{
		mouse_offset_x = 0;
		mouse_offset_y = 0;
	}
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

void MouseController::updateInput() {
	glfwGetCursorPos(this->window, &mouse_x, &mouse_y);

	ProcessMouseMovement();
	//ProcessMouseScroll();
}