#include "Model.h"
#include <SDL.h>
#include <SDL_image.h>

unsigned int TextureFromFile(const char *path, const std::string &directory, bool gamma = false);
Model::~Model()
{
}
void Model::Draw(Shader shader)
{
	for (unsigned int i = 0; i < meshes.size(); i++)
		meshes[i].Draw(shader);
}
int Model::TextureFromFile2(const char * path, const std::string & directory, bool gamma)
{

	std::string filename = std::string(path);
	filename = directory + '/' + filename;
	unsigned int textureID;
	glGenTextures(1, &textureID);
	int width = 1, height = 1, nrComponents = 1;
	SDL_Surface* ptr = IMG_Load(filename.c_str());
	if (!ptr) {
		std::cout << "ERROR::TEXTURE_LOADER:: " << IMG_GetError() << std::endl;
	}
	width = ptr->w;
	height = ptr->h;
	nrComponents = ptr->format->BytesPerPixel;
	if (ptr)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, ptr->pixels);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
	}
	SDL_FreeSurface(ptr);
	return textureID;
}
void Model::BindTExtures(int Textureunit)
{
	glActiveTexture(GL_TEXTURE0 + Textureunit);
	glBindTexture(GL_TEXTURE_2D, uiTexture);
	glBindSampler(Textureunit, uiSampler);
}
Model::Model(std::string const &path, bool gamma) : gammaCorrection(gamma)
{
	loadModel(path);
}
void Model::loadModel(const std::string &path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
	{
		std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
		return;
	}
	directory = path.substr(0, path.find_last_of('/'));

	processNode(scene->mRootNode, scene);
}
void Model::processNode(aiNode * node, const aiScene * scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}

}

Mesh Model::processMesh(aiMesh * mesh, const aiScene * scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		glm::vec3 vector;
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Position = vector;
		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.Normal = vector;
		if (mesh->mTextureCoords[0])
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
		}
		else
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);
		vector.x = mesh->mTangents[i].x;
		vector.y = mesh->mTangents[i].y;
		vector.z = mesh->mTangents[i].z;
		vertex.Tangent = vector;
		vector.x = mesh->mBitangents[i].x; 
		vector.y = mesh->mBitangents[i].y;
		vector.z = mesh->mBitangents[i].z;
		vertex.Bitangent = vector;
		vertices.push_back(vertex);
	}
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	float shininess;
	material->Get(AI_MATKEY_SHININESS, shininess);
	std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
	textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
	std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
	textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

	MaterialProperties properties;
	material->Get(AI_MATKEY_SHININESS, properties.shininess);
	properties.shininess = 250.0f;
	if (diffuseMaps.empty()) {
		aiColor3D diffuse;
		material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
		properties.diffuse.r = diffuse.r;
		properties.diffuse.g = diffuse.g;
		properties.diffuse.b = diffuse.b;
	}
	if (specularMaps.empty()) {
		aiColor3D specular;
		material->Get(AI_MATKEY_COLOR_SPECULAR, specular);
		properties.specular.r = specular.r;
		properties.specular.g = specular.g;
		properties.specular.b = specular.b;
		properties.specularTexture = 0.0f;
	}
	return Mesh(vertices, indices, textures, properties);
}
std::vector<Texture> Model::loadMaterialTextures(aiMaterial * mat, aiTextureType type, std::string typeName)
{
	std::vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		bool skip = false;
		for (unsigned int j = 0; j < textures_loaded.size(); j++)
		{
			if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
			{
				textures.push_back(textures_loaded[j]);
				skip = true;
				break;
			}
		}
		if (!skip)
		{   
			Texture texture;
			texture.id = TextureFromFile(str.C_Str(), this->directory);
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);
			textures_loaded.push_back(texture);
		}
	}
	return textures;
}
unsigned int TextureFromFile(const char *path, const std::string &directory, bool gamma)
{
	std::string filename = std::string(path);
	filename = directory + '/' + filename;
	unsigned int textureID;
	glGenTextures(1, &textureID);
	int width = 1, height = 1, nrComponents = 1;
	SDL_Surface* ptr = IMG_Load(filename.c_str());
	if (!ptr) {
		std::cout << "ERROR::TEXTURE_LOADER:: " << IMG_GetError() << std::endl;
	}
	width = ptr->w;
	height = ptr->h;
	nrComponents = ptr->format->BytesPerPixel;
	if (ptr)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, ptr->pixels);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
	}
	SDL_FreeSurface(ptr);
	return textureID;
}