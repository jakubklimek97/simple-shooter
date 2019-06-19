#pragma once
#include<glm/common.hpp>
class Camera;
class Collision
{
protected:
	virtual float TerrainCollide(float WorldX,float WorldZ) = 0; // Calculates collision for terrain


	float Collide;

};

