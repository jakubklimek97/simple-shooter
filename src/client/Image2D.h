#pragma once
#include "Shader.h"
#include <GL/glew.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
class Image2D
{
public:
	Image2D(float xMin, float xMax, float yMin, float yMax);
	void SetTexture(unsigned int textureId);
	void Draw(Shader &shader);
	~Image2D();
private:
	unsigned int textureId;
	bool textureSet = false;
	float vertices[20];
	unsigned int vao;
	unsigned int vbo;
	unsigned int ebo;
	unsigned int indices[6] = {
		0, 1, 3, 
		1, 2, 3  
	};
};

