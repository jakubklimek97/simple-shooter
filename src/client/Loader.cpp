#include "Loader.h"
#include "Model.h"
#include <vector>
#include <string>
std::string Loader::modelPath[Loader::MODELS_COUNT] = {
	"res/models/kostka/kos.obj",
	"res/models/pistolet/pistolet.obj",
	"res/models/char/char.obj"
};
std::vector<Model> Loader::models;
bool Loader::loadModels() {
	for (int i = 0; i < Loader::LoadedModels::MODELS_COUNT; ++i) {
		models.push_back(Model(Loader::modelPath[i]));
	}
	return true;
}

bool Loader::loadShaders()
{
	for (int i = 0; i < Loader::LoadedShaders::SHADERS_COUNT; ++i) {
		shaders.push_back(Shader(vertexShaderPath[i].c_str(), fragmentShaderPath[i].c_str()));
	}
	return true;
}

Model & Loader::getModel(LoadedModels modelId)
{
	return Loader::models[modelId];
}

Shader & Loader::getShader(LoadedShaders shaderId)
{
	return Loader::shaders[shaderId];
}
