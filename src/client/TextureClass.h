#pragma once

enum ETextureFiltering
{
	TEXTURE_FILTER_MAG_NEAREST = 0, // Nearest criterion for magnification
	TEXTURE_FILTER_MAG_BILINEAR, // Bilinear criterion for magnification
	TEXTURE_FILTER_MIN_NEAREST, // Nearest criterion for minification
	TEXTURE_FILTER_MIN_BILINEAR, // Bilinear criterion for minification
	TEXTURE_FILTER_MIN_NEAREST_MIPMAP, // Nearest criterion for minification, but on closest mipmap
	TEXTURE_FILTER_MIN_BILINEAR_MIPMAP, // Bilinear criterion for minification, but on closest mipmap
	TEXTURE_FILTER_MIN_TRILINEAR, // Bilinear criterion for minification on two closest mipmaps, then averaged
};

/********************************

Class:		CTexture

Purpose:	Wraps OpenGL texture
			object and performs
			their loading.

********************************/


#include <algorithm>
#include <cmath>
#include <cstring>
#include <vector>
#include <sstream>
#include <queue>
#include <map>
#include <set>
#include<Windows.h>
using namespace std;

#include <gl/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include"SDL_image.h"
#include"SDL.h"


class CTexture
{
public:
	void CreateEmptyTexture(int a_iWidth, int a_iHeight, GLenum format);
	void CreateFromData(BYTE* bData, int a_iWidth, int a_iHeight, int a_iBPP, GLenum format, bool bGenerateMipMaps = false);

	//bool ReloadTexture();

	bool LoadTexture2D(string path, const string &directory, bool  bGenerateMipMaps);
	void BindTexture(int iTextureUnit = 0);

	void SetFiltering(int a_tfMagnification, int a_tfMinification);

	void SetSamplerParameter(GLenum parameter, GLenum value);

	int GetMinificationFilter();
	int GetMagnificationFilter();

	int GetWidth();
	int GetHeight();
	int GetBPP();

	UINT GetTextureID();

	string GetPath();

	void DeleteTexture();

	CTexture();

	static int LoadCubeMap(vector<string>faces);
private:

	int iWidth, iHeight, iBPP; // Texture width, height, and bytes per pixel
	UINT uiTexture;// Texture name
	UINT uiSampler; // Sampler name
	bool bMipMapsGenerated;

	int tfMinification, tfMagnification;
	vector<std::string> faces
	{
		("res/img/right.jpg"),
		("res/img/left.jpg"),
		("res/img/top.jpg"),
		("res/img/bottom.jpg"),
		("res/img/front.jpg"),
		("res/img/back.jpg"),
	};

	string sPath;
};
#define NUMTEXTURES 3
extern CTexture tTextures[NUMTEXTURES];
void LoadAllTextures();