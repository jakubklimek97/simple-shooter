#pragma once
#include "Entity.h"
#include <glm\glm.hpp>
class LightObject :
	public Entity
{
public:
	LightObject(Model& model,glm::vec3 position, float rotation, glm::vec3 scale, Shader *lightShader, glm::vec3 lightColor);
	~LightObject();
	void Draw(glm::mat4 &projectionMatrix, glm::mat4& viewMatrix) override;
	glm::vec3 GetPosition() const;
	glm::vec3 GetColor() const;
private:
	glm::vec3 lightColor;
};

