#include "SpriteTexture.h"



SpriteTexture::SpriteTexture():Width(0),Height(0),InternalFormat(GL_RGB),ImageFormat(GL_RGB),WrapS(GL_REPEAT),WrapT(GL_REPEAT)
,FilterMax(GL_LINEAR),FilterMin(GL_LINEAR)
{
	glGenTextures(1, &this->ID);
}

void SpriteTexture::Bind() const
{
	glBindTexture(GL_TEXTURE_2D, this->ID);
}


SpriteTexture::~SpriteTexture()
{
}

 void SpriteTexture::Generate(unsigned int width, unsigned int height, SDL_Surface*image) {
	this->Height = height;
	this->Width = width;

	glBindTexture(GL_TEXTURE_2D, this->ID);
	glTexImage2D(GL_TEXTURE_2D, 0, this->InternalFormat, width, height, 0, this->ImageFormat, GL_UNSIGNED_BYTE, image->pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->WrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->WrapT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->FilterMin);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->FilterMax);

	glBindTexture(GL_TEXTURE_2D, 0);
}
