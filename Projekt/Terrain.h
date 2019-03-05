#pragma once
#include <vector>
#include"GL/glew.h"
#include<SDL.h>
#include<SDL_image.h>
#include<iostream>
class Terrain
{
public:
	static void genTerrain(int wRes = 128, int lRes = 128);
	static int getPixel(SDL_Surface *surface, int x, int y);
	void loadHeighData();
	Terrain() = default;
	~Terrain() = default;

};

