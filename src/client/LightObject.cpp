#include "LightObject.h"






LightObject::LightObject(Model & model, glm::vec3 position, float rotation, Shader *lightShader): Entity(model, position, rotation, glm::vec3(1.0f))
{
	this->setShader(lightShader);
}


LightObject::~LightObject()
{
}
