#include "Entity.h"

Entity::Entity(Model & model, glm::vec3 position, float rotation, glm::vec3 scale): model(model), position(position), rotationAngle(rotation), scale(scale)
{
	this->rotation = glm::vec3(0.0f, 1.0f, 0.0f);
	modelMatrix = glm::mat4(1.0f);
}

void Entity::rotate(float radians)
{
	rotationAngle += radians;
}

void Entity::setShader(Shader & shader)
{
	setShader(&shader);
}

void Entity::setShader(Shader * shader)
{
	this->shader = shader;
}

void Entity::Draw(glm::mat4 & projectionMatrix, glm::mat4 & viewMatrix)
{
	prepareModelMatrix();
	shader->use();
	shader->setMat4("projection", projectionMatrix);
	shader->setMat4("view", viewMatrix);
	shader->setMat4("model", modelMatrix);
	model.Draw(*shader);
}

void Entity::Draw(glm::mat4 & projectionMatrix, glm::mat4 & viewMatrix, glm::vec3 lightColor)
{
	prepareModelMatrix();
	shader->use();
	shader->setMat4("projection", projectionMatrix);
	shader->setMat4("view", viewMatrix);
	shader->setMat4("model", modelMatrix);
	shader->setVec3("lightColor", lightColor);
	shader->setVec3("lightPos", glm::vec3(2.5f, 1.0f, 2.0f));
	model.Draw(*shader);
}

Shader * Entity::GetShader()
{
	return shader;
}

void Entity::prepareModelMatrix()
{
	modelMatrix = glm::translate(glm::mat4(1.0f), position);
	modelMatrix = glm::rotate(modelMatrix, rotationAngle, rotation);
	modelMatrix = glm::scale(modelMatrix, scale);
}
