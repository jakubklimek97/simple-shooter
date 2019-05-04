#include "ResourceManager.h"
#include <sstream>
#include <fstream>

map<string, SpriteTexture > ResourceManager::Textures;
map<string, Shader> ResourceManager::Shaders;

Shader ResourceManager::LoadShader(const char * vShaderFile, const char * fShaderFile, string name)
{
	Shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile);
	return Shaders[name];
}

Shader ResourceManager::GetShader(string name)
{
	return Shaders[name];
}

SpriteTexture ResourceManager::LoadTexture(const char * file, bool alpha, string name)
{
	Textures[name] = loadTextureFromFile(file, alpha);
	return Textures[name];
}

SpriteTexture ResourceManager::GetTexture(string name)
{
	return Textures[name];
}

void ResourceManager::Clear()
{
	for (auto iter : Shaders)
		glDeleteProgram(iter.second.ID);
	for (auto iter : Textures)
		glDeleteTextures(1, &iter.second.ID);
}

ResourceManager::ResourceManager()
{
	
}


ResourceManager::~ResourceManager()
{
}

Shader ResourceManager::loadShaderFromFile(const char * vShaderFile, const char * fShaderFile)
{
	// 1. Retrieve the vertex/fragment source code from filePath
	std::string vertexCode;
	std::string fragmentCode;
	try
	{
		// Open files
		std::ifstream vertexShaderFile(vShaderFile);
		std::ifstream fragmentShaderFile(fShaderFile);
		std::stringstream vShaderStream, fShaderStream;
		// Read file's buffer contents into streams
		vShaderStream << vertexShaderFile.rdbuf();
		fShaderStream << fragmentShaderFile.rdbuf();
		// close file handlers
		vertexShaderFile.close();
		fragmentShaderFile.close();
		// Convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
		// If geometry shader path is present, also load a geometry shader
		
	}
	catch (std::exception e)
	{
		cout<< "ERROR::SHADER: Failed to read shader files" << endl;
	}
	const GLchar *vShaderCode = vertexCode.c_str();
	const GLchar *fShaderCode = fragmentCode.c_str();
	// 2. Now create shader object from source code
	Shader shader;
	shader.Compile(vShaderCode, fShaderCode);
	return shader;
}

SpriteTexture ResourceManager::loadTextureFromFile(const char * file, bool alpha)
{
	// Create Texture object
	SpriteTexture texture;
	if (alpha)
	{
		texture.InternalFormat = GL_RGBA;
		texture.ImageFormat = GL_RGBA;
	}
	// Load image
	
	SDL_Surface*image = IMG_Load(file);
	if (!image) {
		cout << "ERROR::TEXTURE_LOADER:: " << IMG_GetError() << endl;
	}

	//unsigned char *ptr = reinterpret_cast<unsigned char*>(&image);
	int width = image->w;
	int height = image->h;
	// Now generate texture
	texture.Generate(width, height,image);
	// And finally free image data
	SDL_FreeSurface(image);
	return texture;
}

