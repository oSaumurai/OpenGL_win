#ifndef CAMERA_H
#define CAMERA_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
	glm::vec3 direction_up;
	glm::vec3 camera_up;
	glm::vec3 camera_front;
	glm::vec3 camera_right;

private:
	float pitch;
	float yaw;
	float roll;
	float sensitivity;
	bool y_enable;
	bool y_reverse;

	void updateVectors();
public:
	glm::vec3* camera_position;

	Camera(glm::vec3* pos);
	~Camera();
	void setDirectionUp(float x, float y, float z);
	void setCameraFront(float x, float y, float z);
	void moveFront(float d);
	void moveBack(float d);
	void moveLeft(float d);
	void moveRight(float d);
	void moveUp(float d);
	void moveDown(float d);

	void setPosition(float px, float py, float pz);
	void updateByMouseInput(float dt, double offset_x, double offset_y);
	glm::mat4 getViewMatrix();
};

#endif
