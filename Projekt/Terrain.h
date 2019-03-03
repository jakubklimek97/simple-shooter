#pragma once
#include <SDL.h>
#include <SDL_opengl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "BasicModel.h"
#include "Texture.h"
class Terrain
{
public:
	Terrain() = default;
	Terrain(int X, int Z);
	~Terrain();
private:
	float size = 800;
	int vertex_count = 128;
	int x; // na float!
	int z;// na float!
	BasicModel model;
	Texture texture;

};

	

