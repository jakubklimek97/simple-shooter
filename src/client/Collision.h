#pragma once
#include"glm/common.hpp"
class Camera;
class Collision
{
protected:
	virtual float TerrainCollide(float WorldX,float WorldZ) = 0; // Calculates collision for terrain
	//virtual void CollideChange(Camera position , float Collide) = 0; // Change position of camera according to y (height) value or x , z value 


	float Collide;

};

