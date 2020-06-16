#ifndef CAMERA_H
#define CAMERA_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "controller/Mouse_Controller.h"
enum Camera_Movement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 4.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

class Camera 
{
public:
	// camera options
	float MovementSpeed;
	float Velocity;
	float MouseSensitivity;
	float Zoom;

private:
	float Yaw;
	float Pitch;
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;

	MouseController* mouseController;
	
public:
	Camera(glm::vec3 position, glm::vec3 worldUp,float yaw,float pitch);
	Camera(glm::vec3 position);
	~Camera();

	glm::mat4 GetViewMartix();	
	glm::vec3 GetPosition();

	void MoveBack();
	void MoveForward();
	void MoveUp();
	void MoveDown();
	void MoveLeft();
	void MoveRight();

	void updateCameraVectors();
	void ResetPosition();
private:
	void ProcessKeyBoard(Camera_Movement direction, float deltaTime);

};



#endif
