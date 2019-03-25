#pragma once
//#include"ResourceManager2D.h"
#include"Sprite.h"
#include <GL/glew.h>

enum GameState {
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN
};
class GameSprite
{
public:
	GameState              State;
	GLboolean              Keys[1024];
	GLuint                 Width, Height;
	GameSprite(GLuint width, GLuint height);
	void Init();
	void Update(GLfloat dt);
	void ProcessInput(GLfloat dt);
	void Render();
	~GameSprite();
};

