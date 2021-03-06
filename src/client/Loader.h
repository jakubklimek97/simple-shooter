#pragma once
#include "Model.h"
#include "Shader.h"
#include <string>
#include <iostream>

class Loader {
public:
	static bool loadModels();
	static bool loadShaders();
	static bool unloadShaders();
	static bool unloadModels();
	static bool loadTextures2D();
	static bool unloadTextures2D();
	enum LoadedModels {
		CUBE,
		GUN,
		PLAYER,
		MODELS_COUNT
	};
	enum LoadedShaders {
		OUR,
		LIGHT,
		BOUNDING_BOX,
		SIMPLE,
		BULLET,
		IMAGE,
		SKYBOX,
		SHADER2D,
		SHADER2D2,
		MENUBOX,
		SHADERS_COUNT
	};
	enum LoadedTextures2D {
		JA_WON,
		PRZECIWNIK_WON,
		TEXTURES2D_COUNT
	};
	static Model& getModel(enum LoadedModels modelId);
	static Shader& getShader(enum LoadedShaders shaderId);
	static unsigned int getTexture2D(enum LoadedTextures2D textureId);
private:
	Loader() = default;
	~Loader() = default;
	static std::string modelPath[MODELS_COUNT];
	static std::string vertexShaderPath[SHADERS_COUNT];
	static std::string fragmentShaderPath[SHADERS_COUNT];
	static std::string textures2DPath[TEXTURES2D_COUNT];
	static std::string textures2DName[TEXTURES2D_COUNT];

	static std::vector<Model> models;
	static std::vector<Shader> shaders;
	static std::vector<unsigned int> textures2D;


};
