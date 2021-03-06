#include "Loader.h"
#include "Model.h"
#include <vector>
#include <string>
std::string Loader::modelPath[Loader::MODELS_COUNT] = {
	"res/models/kostka/kos.obj",
	"res/models/pistolet/pistolet.obj",
	"res/models/char/char.obj"
};
std::string Loader::vertexShaderPath[SHADERS_COUNT] = {
	"vertex.vs",
	"LightShader.vs",
	"boundingBox.vs",
	"simpleColorShader.vs",
	"bullet.vs",
	"image.vs",
	"skybox.vs",
	"texture.vs",
	"texture.vs",
	"menubox.vs",

};
std::string Loader::fragmentShaderPath[SHADERS_COUNT] = {
	"fragment.fs",
	"LightShader.fs",
	"boundingBox.fs",
	"simpleColorShader.fs",
	"bullet.fs",
	"image.fs",
	"skybox.fs",
	"texture.fs",
	"texture.fs",
	"menubox.fs"
};
std::string Loader::textures2DName[TEXTURES2D_COUNT] = {
	"jaWon.png",
	"graczWon.png"
};
std::string Loader::textures2DPath[TEXTURES2D_COUNT] = {
	"res/textures",
	"res/textures"
};

std::vector<Model> Loader::models;
std::vector<Shader> Loader::shaders;
std::vector<unsigned int> Loader::textures2D;
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

bool Loader::unloadShaders()
{
	for (int i = 0; i < shaders.size(); ++i) {
		glDeleteProgram(shaders[i].ID);
	}
	shaders.clear();
	return true;
}

bool Loader::unloadModels()
{
	for (Model& model: models) {
		for_each(model.meshes.begin(), model.meshes.end(), [](Mesh& mesh) {
			mesh.clearBuffers();
		});
		for_each(model.textures_loaded.begin(), model.textures_loaded.end(), [](Texture& tex) {
			glDeleteTextures(1, &tex.id);
		});
	}
	models.clear();
	return true;
}

bool Loader::loadTextures2D()
{
	for (int i = 0; i < Loader::LoadedTextures2D::TEXTURES2D_COUNT; ++i) {
		textures2D.push_back(TextureFromFile(textures2DName[i].c_str(),textures2DPath[i]));
	}
	return true;
}

bool Loader::unloadTextures2D()
{
	for (int i = 0; i < Loader::LoadedTextures2D::TEXTURES2D_COUNT; ++i) {
		glDeleteTextures(TEXTURES2D_COUNT, textures2D.data());
	}
	textures2D.clear();
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

unsigned int Loader::getTexture2D(LoadedTextures2D textureId)
{
	return Loader::textures2D[textureId];
}
