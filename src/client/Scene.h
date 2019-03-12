#pragma once
#include <forward_list>
#include "Entity.h"
#include "LightObject.h"
#include "Camera.h"
#include "Shader.h"
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
	void DrawObjects();
private:
	std::forward_list<Entity*> objects; //docelowo lista list dla kazdego znanego typu
	LightObject* light;
	Camera* camera;
	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;
};

