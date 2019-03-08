#include "LightObject.h"

glm::vec3 LightObject::GetPosition() const
{
	return position;
}

glm::vec3 LightObject::GetColor() const
{
	return lightColor;
}
LightObject::LightObject(Model & model, glm::vec3 position, float rotation, glm::vec3 scale, Shader *lightShader, glm::vec3 lightColor): Entity(model, position, rotation, scale), lightColor(lightColor)
{
	this->setShader(lightShader);
}

LightObject::~LightObject()
{
}

void LightObject::Draw(glm::mat4 & projectionMatrix, glm::mat4 & viewMatrix)
{
	prepareModelMatrix();
	shader->use();
	shader->setMat4("projection", projectionMatrix);
	shader->setMat4("view", viewMatrix);
	shader->setMat4("model", modelMatrix);
	shader->setVec3("color", lightColor);
	model.Draw(*shader);
}


