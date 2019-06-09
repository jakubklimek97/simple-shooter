#pragma once
#include"GL/glew.h"
enum GameModes {
	MODE_ACTIVE,
	MODE_MENU,
	MODE_WIN,
	MODE_LOSE
};


class Game
{
public:
	// Game States
	GameModes State;
	GLuint Height, Width;
	
	void Initalize();
	void Render();

private:
	Game(GLuint Height, GLuint Width);
	~Game();
};

