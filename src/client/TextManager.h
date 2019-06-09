#pragma once
#include<GL/glew.h>
#include"glm/glm.hpp"
#include<map>
#include"Shader.h"
#include<string>
#include<iostream>
using namespace std;
struct Character {
	GLuint TextureID;
	glm::ivec2 Size;
	glm::ivec2 Bearing;
	int  Advance;
};


class TextManager
{
public:
	TextManager(GLuint width, GLuint height);
	void RenderText(string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color = glm::vec3(1.0f));
	void Load(string font, unsigned int fontSize);
	std::map<GLchar, Character> Characters;
	Shader TextShader;
private:
	unsigned int VAO, VBO;

};

