#include "Texture.h"


Texture::~Texture()
{
}

void Texture::SetShineDamper(float _ShineDamper)
{
	this->_ShineDamper = _ShineDamper;
}

void Texture::SetReflectivity(float _Reflectivity)
{
	this->_Reflectivity = _Reflectivity;
}
