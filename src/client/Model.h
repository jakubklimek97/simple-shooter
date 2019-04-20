#pragma once
#include <gl/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"
#include "Shader.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

class Model
{
public:
	unsigned uiTexture; // Texture name
	unsigned uiSampler; // Sampler name
	std::vector<Texture> textures_loaded;
	std::vector<Mesh> meshes;
	std::string directory;
	bool gammaCorrection;
	void BindTExtures(int Textureunit);
	Model(std::string const &path, bool gamma = false);
	~Model();
	void Draw(Shader shader);
	Model() = default;
	int TextureFromFile2(const char *path, const std::string &directory, bool gamma);

private:
	void loadModel(std::string const &path);
	void processNode(aiNode *node, const aiScene *scene);
	Mesh processMesh(aiMesh *mesh, const aiScene *scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
};
