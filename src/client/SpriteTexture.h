#pragma once
#include <GL/glew.h>
#include <SDL_image.h>
class SpriteTexture
{
public:
	unsigned int ID;
	unsigned int Width, Height;
	unsigned int InternalFormat;
	unsigned int ImageFormat;

	unsigned int WrapS;
	unsigned int WrapT;
	unsigned int FilterMin;
	unsigned int FilterMax;
	
	SpriteTexture();
	~SpriteTexture();

	void Generate(unsigned int width, unsigned int height, SDL_Surface*image);

	void Bind() const;
};

