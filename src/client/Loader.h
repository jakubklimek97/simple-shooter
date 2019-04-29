#pragma once
#include "Model.h"
#include "Shader.h"
#include <string>
#include <iostream>

class Loader {
public:
	static bool loadModels();
	static bool loadShaders();
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
		SHADERS_COUNT
	};
	static Model& getModel(enum LoadedModels modelId);
	static Shader& getShader(enum LoadedShaders shaderId);
private:
	Loader() = default;
	~Loader() = default;
	static std::string modelPath[MODELS_COUNT];
	static std::string vertexShaderPath[SHADERS_COUNT];
	static std::string fragmentShaderPath[SHADERS_COUNT];

	static std::vector<Model> models;
	static std::vector<Shader> shaders;


};
