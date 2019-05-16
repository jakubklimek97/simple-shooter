#pragma once

#include "TextureClass.h"
#include<iostream>




CTexture::CTexture()
{
	bMipMapsGenerated = false;
}

void CTexture::CreateEmptyTexture(int a_iWidth, int a_iHeight, GLenum format)
{
	glGenTextures(1, &uiTexture);
	glBindTexture(GL_TEXTURE_2D, uiTexture);
	if (format == GL_RGBA || format == GL_BGRA)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, a_iWidth, a_iHeight, 0, format, GL_UNSIGNED_BYTE, NULL);
	// We must handle this because of internal format parameter
	else if (format == GL_RGB || format == GL_BGR)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, a_iWidth, a_iHeight, 0, format, GL_UNSIGNED_BYTE, NULL);
	else
		glTexImage2D(GL_TEXTURE_2D, 0, format, a_iWidth, a_iHeight, 0, format, GL_UNSIGNED_BYTE, NULL);

}



void CTexture::CreateFromData(BYTE* bData, int a_iWidth, int a_iHeight, int a_iBPP, GLenum format, bool bGenerateMipMaps)
{
	// Generate an OpenGL texture ID for this texture
    glGenTextures(1, &uiTexture);
	glBindTexture(GL_TEXTURE_2D, uiTexture);
	if (format == GL_RGBA || format == GL_BGRA)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, a_iWidth, a_iHeight, 0, format, GL_UNSIGNED_BYTE, bData);
	// We must handle this because of internal format parameter
	else if (format == GL_RGB || format == GL_BGR)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, a_iWidth, a_iHeight, 0, format, GL_UNSIGNED_BYTE, bData);
	else
		glTexImage2D(GL_TEXTURE_2D, 0, format, a_iWidth, a_iHeight, 0, format, GL_UNSIGNED_BYTE, bData);
	if (bGenerateMipMaps)glGenerateMipmap(GL_TEXTURE_2D);
	glGenSamplers(1, &uiSampler);

	sPath = "";
	bMipMapsGenerated = bGenerateMipMaps;
	iWidth = a_iWidth;
	iHeight = a_iHeight;
	iBPP = a_iBPP;
}



bool CTexture::LoadTexture2D(std::string path, const std::string &directory, bool bGenerateMipMaps)
{

	std::string filename = path;

	filename = directory + '/' + filename;


	int width = 1, height = 1, nrComponents = 1;
	

	SDL_Surface* ptr = IMG_Load(filename.c_str());

	iWidth = ptr->w;
	iHeight = ptr->h;

	nrComponents = ptr->format->BytesPerPixel;

	unsigned char *data = (unsigned char*)ptr->pixels;
	unsigned char* bDataPointer = data;// Retrieve the image data
	iBPP = ptr->format->BitsPerPixel;
	nrComponents = ptr->format->BytesPerPixel;


	GLenum format;
	if (nrComponents == 1)
		format = GL_RED;
	else if (nrComponents == 3)
		format = GL_RGB;
	else if (nrComponents == 4)
		format = GL_RGBA;

  	glGenTextures(1, &uiTexture);
	glBindTexture(GL_TEXTURE_2D, uiTexture);

	int iFormat = iBPP == 24 ? GL_BGR : iBPP == 8 ? GL_LUMINANCE : 0;
	int iInternalFormat = iBPP == 24 ? GL_RGB : GL_DEPTH_COMPONENT;

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, iWidth, iHeight, 0, iFormat, GL_UNSIGNED_BYTE, bDataPointer);

	if (bGenerateMipMaps)glGenerateMipmap(GL_TEXTURE_2D);

    //	glGenSamplers(1, &uiSampler);  // BEZ TEGO DZIALA SKYBOXA, MOZLIWE ZE Samplery na siebie na chodza
	// Jak zarzadzic struktura, zeby tego nie robil? (dziala jak stos z tego co widze)

	sPath = filename;

	bMipMapsGenerated = bGenerateMipMaps;


	return true; 
}

void CTexture::SetSamplerParameter(GLenum parameter, GLenum value)
{
	glSamplerParameteri(uiSampler, parameter, value);
}


void CTexture::SetFiltering(int a_tfMagnification, int a_tfMinification)
{
	glBindSampler(0, uiSampler);

	// Set magnification filter
	if (a_tfMagnification == TEXTURE_FILTER_MAG_NEAREST)
		glSamplerParameteri(uiSampler, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	else if (a_tfMagnification == TEXTURE_FILTER_MAG_BILINEAR)
		glSamplerParameteri(uiSampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Set minification filter
	if (a_tfMinification == TEXTURE_FILTER_MIN_NEAREST)
		glSamplerParameteri(uiSampler, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	else if (a_tfMinification == TEXTURE_FILTER_MIN_BILINEAR)
		glSamplerParameteri(uiSampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	else if (a_tfMinification == TEXTURE_FILTER_MIN_NEAREST_MIPMAP)
		glSamplerParameteri(uiSampler, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	else if (a_tfMinification == TEXTURE_FILTER_MIN_BILINEAR_MIPMAP)
		glSamplerParameteri(uiSampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	else if (a_tfMinification == TEXTURE_FILTER_MIN_TRILINEAR)
		glSamplerParameteri(uiSampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	tfMinification = a_tfMinification;
	tfMagnification = a_tfMagnification;
}



void CTexture::BindTexture(int iTextureUnit)
{
	glActiveTexture(GL_TEXTURE0 + iTextureUnit);
	glBindTexture(GL_TEXTURE_2D, uiTexture);
	glBindSampler(iTextureUnit, uiSampler);
}


void CTexture::DeleteTexture()
{
	glDeleteSamplers(1, &uiSampler);
	glDeleteTextures(1, &uiTexture);
}




int CTexture::GetMinificationFilter()
{
	return tfMinification;
}

int CTexture::GetMagnificationFilter()
{
	return tfMagnification;
}

int CTexture::GetWidth()
{
	return iWidth;
}

int CTexture::GetHeight()
{
	return iHeight;
}

int CTexture::GetBPP()
{
	return iBPP;
}

UINT CTexture::GetTextureID()
{
	return uiTexture;
}

string CTexture::GetPath()
{
	return sPath;
}


CTexture tTextures[NUMTEXTURES];

void LoadAllTextures()
{
	// Load textures

	string sTextureNames[] = { "sand.jpg", "fungus.jpg", "sand.jpg","water.jpg" };

     for(int i =0;i<NUMTEXTURES;++i)
	{
		tTextures[i].LoadTexture2D(sTextureNames[i], "res/img", true);
		tTextures[i].SetFiltering(TEXTURE_FILTER_MAG_BILINEAR, TEXTURE_FILTER_MIN_BILINEAR_MIPMAP);
	}
}


