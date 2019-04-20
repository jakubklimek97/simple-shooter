#include "Scene.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cassert>
Scene::Scene(glm::mat4 projectionMatrix, Camera* camera): projectionMatrix(projectionMatrix), camera(camera)
{
	light = nullptr;
	terrain = nullptr;
}

Scene::~Scene()
{
	objects.clear();
	if (light) delete light;
	delete camera;
}

LightObject* Scene::SetLight(LightObject * light)
{
	if (this->light) delete this->light;
	this->light = light;
	return light;
}

Entity* Scene::addObject(Entity* object)
{
	assert(reinterpret_cast<LightObject*>(object) != nullptr); //If you caught this assert it means you tried to add light source using this method. Use SetLight(LightObject& light) to do it instead.
	objects.push_front(object);
	return object;
}

void Scene::removeObject(Entity * object)
{
	objects.remove_if([&](const Entity* obj){return obj == object;});
}

Camera* Scene::getCamera()
{
	return camera;
}

glm::mat4 & Scene::GetViewMatrix()
{
	return viewMatrix;
}

glm::mat4 & Scene::GetProjectionMatrix()
{
	return projectionMatrix;
}

Terrain* Scene::addTerrain(Terrain* ptrTerrain)
{
	return terrain = ptrTerrain;
}

void Scene::DrawObjects()
{
	if(light == nullptr){
		assert(light != nullptr); //scene must have light
		return;
	}
	viewMatrix = camera->getViewMatrix();
	if (terrain) terrain->Draw(projectionMatrix, viewMatrix, *light, *camera);
	auto iter = objects.begin();
	auto end = objects.end();
	while (iter != end) {
		(*(iter++))->Draw(projectionMatrix, viewMatrix, *light, *camera);
	}
	light->Draw(projectionMatrix, viewMatrix);
}

void Scene::movePlayer(Camera::Movement move, float deltaTime)
{
	camera->moveCamera(move, deltaTime);
	camera->cameraPos.y = (int)(terrain->getHeight(camera->cameraPos) * 100) / 100.0f + 0.85f;
}
