#pragma once
#include"GL/glew.h"
class HUD
{
public:
	HUD();
	~HUD();
	void InitBuffer();


private:
	GLuint VBO, VAO, EBO;
//	float vertices[];
};

