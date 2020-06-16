#include "Camera.h"

Camera::Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
    float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), 
    MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM),
    Velocity(0.4f)
{
    Position = position;
    WorldUp = up;
    Yaw = yaw;
    Pitch = pitch;
    mouseController = MouseController::getInstance();
    updateCameraVectors();
}

Camera::Camera(glm::vec3 position)
    :MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM), 
    WorldUp(glm::vec3(0.0f, 1.0f, 0.0f)), Front(glm::vec3(0.0f, 0.0f, -1.0f)),
    Yaw(YAW), Pitch(PITCH), Position(position), Velocity(0.4f)
{
   mouseController = MouseController::getInstance();
   updateCameraVectors();
}

Camera::~Camera()
{
}

glm::mat4 Camera::GetViewMartix()
{
    return glm::lookAt(Position, Position + Front, Up);
}

glm::vec3 Camera::GetPosition()
{
    return Position;
}

void Camera::MoveBack()
{
    Position -= Front * Velocity;
}

void Camera::MoveForward()
{
    Position += Front * Velocity;
}

void Camera::MoveUp()
{
    Position += Up * Velocity;
}

void Camera::MoveDown()
{
    Position -= Up * Velocity;
}

void Camera::MoveLeft()
{
    Position -= Right * Velocity;
}

void Camera::MoveRight()
{
    Position += Right * Velocity;
}

void Camera::updateCameraVectors()
{
    mouseController->GetMouseOffset();
    Yaw += mouseController->mouse_offset_x / 4;
    Pitch += mouseController->mouse_offset_y / 4;

    if (Pitch > 89.0f)
        Pitch = 89.0f;
    if (Pitch < -89.0f)
        Pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);
    // also re-calculate the Right and Up vector
    Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    Up = glm::normalize(glm::cross(Right, Front));
}

void Camera::ResetPosition()
{
    Position = glm::vec3(0.0f, 0.0f, 0.0f);
    Yaw = YAW;
    Pitch = PITCH;
}

/*void Camera::ProcessKeyBoard(Camera_Movement direction, float deltaTime)
{
    float velocity = MovementSpeed * deltaTime;
    if (direction == FORWARD)
        Position += Front * velocity;
    if (direction == BACKWARD)
        Position -= Front * velocity;
    if (direction == LEFT)
        Position -= Right * velocity;
    if (direction == RIGHT)
        Position += Right * velocity;
}*/
