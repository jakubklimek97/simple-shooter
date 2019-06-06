#pragma once
#include <string>
#include "Entity.h"
#include "Model.h"
#include "Shader.h"
#include "Camera.h"
class Terrain
{
public:
	Terrain() = default;
	~Terrain();
	void loadTerrain(std::string modelPath, std::string heightMapPath, Shader& shader);
	float getHeight(glm::vec3 pos);
	void Draw(glm::mat4& projectionMatrix, glm::mat4& viewMatrix, const LightObject& lightObject, const Camera& camera);

private:
	Model* model = nullptr;
	Entity* entity = nullptr;
	SDL_Surface* heightMap = nullptr;
};
