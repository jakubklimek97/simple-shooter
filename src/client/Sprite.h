#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include"SpriteShader.h"
#include "Texture2D.h"
#include <GL/glew.h>
#include <GL/GL.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
class Sprite
{
public:
	Sprite(SpriteShader &shader);
	~Sprite();
	void DrawSprite(Texture2D &texture, glm::vec2 position,
		glm::vec2 size = glm::vec2(10, 10), GLfloat rotate = 0.0f,
		glm::vec3 color = glm::vec3(1.0f));
private:
	SpriteShader shader;
	GLuint quadVAO;
	void initRenderData();
};

