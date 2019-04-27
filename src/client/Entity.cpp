#include "Entity.h"
#include "LightObject.h"

Entity::Entity(Model & model, glm::vec3 position, float rotation, glm::vec3 scale): model(model), position(position), rotationAngle(rotation), scale(scale)
{
	this->rotation = glm::vec3(0.0f, 1.0f, 0.0f);
	modelMatrix = glm::mat4(1.0f);
}

void Entity::rotateX(float radians)
{
	rotationAngle.x += radians;
}

void Entity::rotateY(float radians)
{
	rotationAngle.y += radians;
}

void Entity::rotateZ(float radians)
{
	rotationAngle.z += radians;
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

void Entity::Draw(glm::mat4 & projectionMatrix, glm::mat4 & viewMatrix, const LightObject& lightObject, const Camera& camera) 
{
	prepareModelMatrix();
	shader->use();
	shader->setMat4("projection", projectionMatrix);
	shader->setMat4("view", viewMatrix);
	shader->setMat4("model", modelMatrix);
	shader->setVec3("lightColor", lightObject.GetColor());
	shader->setVec3("lightPos", lightObject.GetPosition());
	shader->setVec3("viewPos", camera.cameraPos);
	model.Draw(*shader);
}

Shader * Entity::GetShader()
{
	return shader;
}

glm::vec3 Entity::getPosition()
{
	return position;
}

void Entity::setPosition(const glm::vec3& pos)
{
	position = pos;
}

void Entity::prepareModelMatrix()
{
	modelMatrix = glm::translate(glm::mat4(1.0f), position);
	modelMatrix = glm::rotate(modelMatrix, rotationAngle.x, glm::vec3(1.0f, 0.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, rotationAngle.y, glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, rotationAngle.z, glm::vec3(0.0f, 0.0f, 1.0f));
	modelMatrix = glm::scale(modelMatrix, scale);
}
