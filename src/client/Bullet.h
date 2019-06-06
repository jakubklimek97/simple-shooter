#pragma once
#include <SDL.h>
#include <GL/glew.h>
#include <SDL_opengl.h>

#include <glm/glm.hpp>
#include "Shader.h"
class Bullet
{
public:
	Bullet(glm::vec3 pos1, glm::vec3 vector);
	Bullet(float verticesArray[6]);
	~Bullet();
	static void setShader(Shader* shader);
	void draw(const glm::mat4 &projectionMatrix, const  glm::mat4& viewMatrix, const  glm::vec3& color);
	float vertices[6];

private:
	unsigned int vao;
	unsigned int vbo;
	static Shader* shaderRef;
	bool needInitialising = false;
};
