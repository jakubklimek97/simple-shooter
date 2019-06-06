#include "Camera.h"


Camera::Camera(glm::vec3 position, glm::vec3 front, glm::vec3 up, float sensitivity, float moveSpeed): cameraPos(position), cameraFront(front), cameraUp(up), sensitivity(sensitivity), moveSpeed(moveSpeed)
{
	updateCameraVectors();
}

Camera::~Camera()
{
}

void Camera::SetPitch1(float value)
{
	pitch1 = value;
}

void Camera::SetPitch2(float value)
{
	pitch2 = value;
}

void Camera::turnCamera(SDL_MouseMotionEvent & event)
{
	float xoffset = event.xrel * sensitivity;
	float yoffset = event.yrel * sensitivity;
	yaw += xoffset;
	pitch -= yoffset;
	if (pitch > pitch1)
		pitch = pitch1;
	if (pitch < pitch2)
		pitch = pitch2;
	updateCameraVectors();
}

void Camera::moveCamera(Movement move, float deltaTime)
{
	if (move.none()) return;
	if (move[Camera::MovementBits::MOVE_FORWARD]) {
		cameraPos += 10*deltaTime * moveSpeed * cameraFront;
	}
	if (move[Camera::MovementBits::MOVE_BACKWARD]) {
		cameraPos -= 10 * deltaTime * moveSpeed * cameraFront;
	}
	if (move[Camera::MovementBits::STRAFE_LEFT]) {
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * deltaTime * moveSpeed;
	}
	if (move[Camera::MovementBits::STRAFE_RIGHT]) {
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp))  * deltaTime * moveSpeed;
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

	//nie wiem dlaczego obraca kamera. Trzeba przemyslec
	//cameraRight = glm::normalize(glm::cross(cameraFront, cameraUp)); 
	//cameraUp = glm::normalize(glm::cross(cameraRight, cameraFront));
}
