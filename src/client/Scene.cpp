#include "Scene.h"
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
	if (terrain) delete terrain;
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
	if (move.none() && notFalling && notJumping) return;
	glm::vec3 oldPos = camera->cameraPos;
	camera->moveCamera(move, deltaTime);
	float newHeight = (int)(terrain->getHeight(camera->cameraPos) * 100) / 100.0f + 0.85f;
	if (notJumping) {
		camera->cameraPos.y = newHeight;
		float heightDiff = camera->cameraPos.y - oldPos.y;
		if (heightDiff < -0.3f) {
			notFalling = false;
			camera->cameraPos.y = oldPos.y + (heightDiff * deltaTime * 8);
			return;
		}
		else if (heightDiff > 0.5f) {
			camera->cameraPos = oldPos;
		}
		notFalling = true;
	}
	else {
		jumpTime -= deltaTime;
		if (jumpTime <= 0.0f) {
			notJumping = true;
			float heightDiff = newHeight - oldPos.y;
			if (heightDiff < -0.3f) {
				notFalling = false;
				return;
			}
			else if (heightDiff > 0.5f) {
				camera->cameraPos.x = oldPos.x;
				camera->cameraPos.z = oldPos.z;
			}
		}
		camera->cameraPos.y = jumpStartPos + (0.3f - jumpTime)*jumpHeight;
	}
}

void Scene::playerJumpStart()
{
	if (notJumping && notFalling) {
		notJumping = false;
		jumpTime = 0.3f;
		jumpStartPos = camera->cameraPos.y;
	}
}

void Scene::InitScene()
{
}

void Scene::handleEvents(SDL_Event & e)
{
}

void Scene::render()
{
}
