#include "Bullet.h"
#include <cassert>

#define Y_MODIFIER -0.1f

Shader* Bullet::shaderRef = nullptr;
Bullet::Bullet(glm::vec3 pos1, glm::vec3 pos2)
{
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	pos2.y += Y_MODIFIER;
	pos2 = pos2 * 50.0f + pos1;
	vertices[0] = pos2.x;
	vertices[1] = pos2.y;
	vertices[2] = pos2.z;
	vertices[3] = pos1.x;
	vertices[4] = pos1.y - 0.05f;
	vertices[5] = pos1.z;
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6, vertices, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

Bullet::Bullet(float verticesArray[6])
{
	memcpy(vertices, verticesArray, sizeof(float) * 6);
	needInitialising = true;
}

Bullet::~Bullet()
{
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
}

void Bullet::setShader(Shader* shader)
{
	Bullet::shaderRef = shader;
}

void Bullet::draw(const glm::mat4 &projectionMatrix, const  glm::mat4& viewMatrix, const  glm::vec3& color)
{
	if (needInitialising) {
		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6, vertices, GL_DYNAMIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		needInitialising = false;
	}
	assert(Bullet::shaderRef != nullptr); //Shader has to be set beforehand
	shaderRef->use();
	shaderRef->setMat4("model", glm::mat4(1.0f));
	shaderRef->setMat4("projection", projectionMatrix);
	shaderRef->setMat4("view", viewMatrix);
	shaderRef->setVec3("color", color);
	glBindVertexArray(vao);
	glDrawArrays(GL_LINES, 0, 2);
}
