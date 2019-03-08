#pragma once
#include "Model.h"
#include "Shader.h"
#include <glm/glm.hpp>
class Entity
{
public:
	

	Entity(Model& model, glm::vec3 position, float rotation, glm::vec3 scale);
	void rotate(float radians);
	void setShader(Shader& shader);
	void setShader(Shader* shader);
	void Draw(glm::mat4 &projectionMatrix, glm::mat4& viewMatrix);
	void Draw(glm::mat4 &projectionMatrix, glm::mat4& viewMatrix, glm::vec3 lightColor);
	Shader* GetShader();
private:
	Model& model;
	void prepareModelMatrix();
	glm::mat4 modelMatrix;
	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 rotation;
	float rotationAngle;
	Shader* shader;
};

