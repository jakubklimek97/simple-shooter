#include "Camera.h"





Camera::Camera(glm::vec3 position, glm::vec3 front, glm::vec3 up, float sensitivity, float moveSpeed): cameraPos(position), cameraFront(front), cameraUp(up), sensitivity(sensitivity), moveSpeed(moveSpeed)
{
	updateCameraVectors();
}

Camera::~Camera()
{
}

void Camera::turnCamera(SDL_MouseMotionEvent & event)
{
	float xoffset = event.xrel * sensitivity;
	float yoffset = event.yrel * sensitivity;
	yaw += xoffset;
	pitch -= yoffset;
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;
	updateCameraVectors();
}

void Camera::moveCamera(Movement move, float deltaTime)
{
	if (move.none()) return;
	if (move[Camera::MovementBits::MOVE_FORWARD]) {
		cameraPos += deltaTime * moveSpeed * movementFront;
	}
	if (move[Camera::MovementBits::MOVE_BACKWARD]) {
		cameraPos -= deltaTime * moveSpeed * movementFront;
	}
	if (move[Camera::MovementBits::STRAFE_LEFT]) {
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * deltaTime * moveSpeed;
	}
	if (move[Camera::MovementBits::STRAFE_RIGHT]) {
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * deltaTime * moveSpeed;
	}
}

glm::mat4 Camera::getViewMatrix() const
{
	return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

void Camera::updateCameraVectors()
{
	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);
	movementFront.x = cameraFront.x;
	movementFront.y = 0.0f;
	movementFront.z = cameraFront.z;
}
