#pragma once
#include <GL/glew.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;
};

struct Texture {
	unsigned int id;
	std::string type;
	std::string path;
};
struct MaterialProperties {
	glm::vec3 ambient = glm::vec3(0.0f);
	glm::vec3 diffuse = glm::vec3(0.0f);
	glm::vec3 specular = glm::vec3(0.0f);
	float shininess;
	float specularTexture = 1.0f; 
};
class Mesh {
public:
	
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures, MaterialProperties materialProperties);
	~Mesh();
	void clearBuffers();
	void Draw(Shader shader);
private:
	MaterialProperties materialProperties;
	unsigned int VAO, VBO, EBO;
	void setupMesh();
};