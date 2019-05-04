#pragma once
#include"Shader.h"
#include"SpriteTexture.h"
#include<string>
#include<map>
#include<SDL_image.h>
#include<SDL.h>
using namespace std;
class ResourceManager
{
public:
	static map<string, Shader > Shaders;
	static map<string, SpriteTexture> Textures;
	static Shader LoadShader(const char *vShaderFile, const char*fShaderFile, string name);
	static Shader GetShader(string name);
	static SpriteTexture LoadTexture(const char *file, bool alpha, string name);
	static SpriteTexture GetTexture(string name);
	static void Clear();
private:
	ResourceManager();
	~ResourceManager();
	static Shader loadShaderFromFile(const char *vShaderFile, const char *fShaderFile);
	static SpriteTexture loadTextureFromFile(const char *file, bool alpha);
};

