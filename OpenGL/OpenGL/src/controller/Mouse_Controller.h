#ifndef MOUSE_CONTROLLER_H
#define MOUSE_CONTROLLER_H

#include "Controller.h"
#include "player/Player.h"

#include "glm/glm.hpp"
class MouseController : public Controller {

public:
	double mouse_offset_x;
	double mouse_offset_y;

public:
	static MouseController* getInstance();
	~MouseController();
	
	glm::vec2 GetMouseOffset();
	void ProcessMouseMovement();
	void ProcessMouseScroll();
	void updateInput() override;

private:
	MouseController();
	static MouseController* instance;
	
private:
	double last_mouse_x;
	double last_mouse_y;
	double mouse_x;
	double mouse_y;
	float zoom;
	//bool is_first_mouse;
};

#endif