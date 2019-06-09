#pragma once
#include <glm/glm.hpp>
#include "Model.h"
#include "Entity.h"
#include "Shader.h"
class BoundingBox
{
public:
	BoundingBox(Entity & entity);
	~BoundingBox();
	void calculateBoundingBox();
	void Draw(glm::mat4 &projectionMatrix, glm::mat4& viewMatrix, Shader &shader);
private:
	void prepareMesh();
	float minX, minY, minZ, maxX, maxY, maxZ;
	unsigned int VAO, VBO, EBO;
	Entity& entityRef;
};

