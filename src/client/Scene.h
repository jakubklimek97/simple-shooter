#pragma once
#include <forward_list>
#include "Entity.h"
#include "LightObject.h"
#include "Camera.h"
#include "Shader.h"
#include "Terrain.h"
#include <glm/glm.hpp>

class Scene
{
public:
	Scene(glm::mat4 projectionMatrix, Camera* camera);
	~Scene();
	LightObject* SetLight(LightObject* light);
	Entity* addObject(Entity* object);
	void removeObject(Entity* object);
	Camera* getCamera();
	glm::mat4& GetViewMatrix();
	glm::mat4& GetProjectionMatrix();
	Terrain* addTerrain(Terrain* ptrTerrain);
	void DrawObjects();
	void movePlayer(Camera::Movement move, float deltaTime);
	void playerJumpStart();
private:
	std::forward_list<Entity*> objects; //docelowo lista list dla kazdego znanego typu
	LightObject* light;
	Camera* camera;
	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;
	Terrain* terrain;
	float jumpTime;
	bool notJumping = true;
	bool notFalling = true;
	float jumpStartPos;
	float jumpHeight = 6.0f;
};

