#pragma once
#include "Model.h"
#include "Entity.h"
class BoundingBox
{
public:
	BoundingBox(Entity & entity);
	~BoundingBox();
private:
	float minX, minY, minZ, maxX, maxY, maxZ;
	Entity& entityRef;
};

