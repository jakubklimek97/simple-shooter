#pragma once
#include "Entity.h"
#include <glm\glm.hpp>
class LightObject :
	public Entity
{
public:
	LightObject(Model& model,glm::vec3 position, float rotation, Shader *lightShader);
	~LightObject();
};

