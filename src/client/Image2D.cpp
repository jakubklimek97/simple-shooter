#include "Image2D.h"



Image2D::Image2D(float xMin, float xMax, float yMin, float yMax)
{
	vertices[0] = xMax;
	vertices[1] = yMax;
	vertices[2] = 0.0f;
	vertices[3] = 1.0f;
	vertices[4] = 0.0f;
	vertices[5] = xMax;
	vertices[6] = yMin;
	vertices[7] = 0.0f;
	vertices[8] = 1.0f;
	vertices[9] = 1.0f;
	vertices[10] = xMin;
	vertices[11] = yMin;
	vertices[12] = 0.0f;
	vertices[13] = 0.0f;
	vertices[14] = 1.0f;
	vertices[15] = xMin;
	vertices[16] = yMax;
	vertices[17] = 0.0f;
	vertices[18] = 0.0f;
	vertices[19] = 0.0f;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);
}

void Image2D::SetTexture(unsigned int textureId)
{
	this->textureId = textureId;
	textureSet = true;
}

void Image2D::Draw(Shader & shader)
{
	if (!textureSet)
		return;
	shader.use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

Image2D::~Image2D()
{
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &vao);
	glDeleteVertexArrays(1, &vao);
}
