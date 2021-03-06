#pragma once
#include <SDL.h>
#include <SDL_opengl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <bitset>
class Camera
{
public:
	typedef std::bitset<4> Movement;
	enum MovementBits {
		MOVE_FORWARD = 0,
		MOVE_BACKWARD = 1,
		STRAFE_LEFT = 2,
		STRAFE_RIGHT = 3
	};
	Camera(glm::vec3 position, glm::vec3 front, glm::vec3 up, float sensitivity, float moveSpeed);
	~Camera();
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 cameraRight;
	glm::vec3 movementFront = glm::vec3(0.0f, 0.0f, 1.0f);
	float yaw = -90.0f;
	float pitch = 0.0f;
	float sensitivity = 0.1f;
	float moveSpeed = 1.0f;
	void turnCamera(SDL_MouseMotionEvent &event);
	void moveCamera(Movement move, float deltaTime);
	glm::mat4 getViewMatrix() const;
	void updateCameraVectors();
};

