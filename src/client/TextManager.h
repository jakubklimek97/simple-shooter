#pragma once
#include<GL/glew.h>
#include"glm/glm.hpp"
#include<map>
#include"Shader.h"
using namespace std;
struct Character {
	GLuint TextureID;
	glm::ivec2 Size;
	glm::ivec2 Bearing;
	long int Advance;
};


class TextManager
{
public:
	TextManager(unsigned int width, unsigned int height);
	void RenderText(string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color = glm::vec3(1.0f));
	void Load(string font, unsigned int fontSize);
	std::map<char, Character> Characters;
	Shader TextShader;
private:
	unsigned int VAO, VBO;

};

