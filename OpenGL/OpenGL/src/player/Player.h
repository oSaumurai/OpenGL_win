#ifndef PLAYER_H
#define PLAYER_H

#include "glm/glm.hpp"
#include "Camera/Camera.h"
//#include "../camera/camera.h"
//#include "../utils/printer.h"

//class Camera;

class Player {
private:
	float moving_speed;

public:
	glm::vec3 pos;
	Camera camera;

	Player();
	Player(float px, float py, float pz);
	~Player();

	void moveFront(float dt);
	void moveBack(float dt);
	void moveLeft(float dt);
	void moveRight(float dt);
	void moveUp(float dt);
	void moveDown(float dt);

	glm::vec3 getFront();
	glm::mat4 getViewMatrix();
};

#endif