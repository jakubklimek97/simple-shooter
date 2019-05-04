#include "HUD.h"



HUD::HUD()
{
	float vertices[] = {
		// positions          // colors           // texture coords
	-0.7f, -1.0f, 0.0f,    1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
	-0.7f, -0.7f, 0.0f,    0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
	-1.0f,  -0.7f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
	-1.0f,  -1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
	};
}


HUD::~HUD()
{
}

void HUD::InitBuffer()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}