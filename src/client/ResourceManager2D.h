#pragma once
#include <map>
#include <string>
#include "Texture2D.h"
#include "SpriteShader.h"
#include <fstream>
#include<sstream>
#include<SDL_image.h>
class ResourceManager2D
{
public:
	// Resource storage
	static std::map<std::string, SpriteShader>    Shaders;
	static std::map<std::string, Texture2D> Textures;
	// Loads (and generates) a shader program from file loading vertex, fragment (and geometry) shader's source code. If gShaderFile is not nullptr, it also loads a geometry shader
	static SpriteShader   LoadShader(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile, std::string name);
	// Retrieves a stored sader
	static SpriteShader GetShader(std::string name);
	// Loads (and generates) a texture from file
	static Texture2D LoadTexture(const GLchar *file, GLboolean alpha, std::string name);
	// Retrieves a stored texture
	static Texture2D GetTexture(std::string name);
	// Properly de-allocates all loaded resources
	static void      Clear();
private:
	// Private constructor, that is we do not want any actual resource manager objects. Its members and functions should be publicly available (static).
	ResourceManager2D() { }
	// Loads and generates a shader from file
	static SpriteShader    loadShaderFromFile(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile = nullptr);
	// Loads a single texture from file
	static Texture2D loadTextureFromFile(const GLchar *file, GLboolean alpha);
};

