#pragma once
#include "Scene.h"
class SceneGame :
	public Scene
{
public:
	SceneGame(glm::mat4 projectionMatrix, Camera* camera);
	~SceneGame();
	virtual void InitScene();
	virtual void handleEvents(SDL_Event& e);
	virtual void render();
private: //zmienne uzywane przez scene (indywidualne)
	float ostatniWystrzal = 0.0f;
	bool quit = false;
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;
	float currentFrame = 0.0f;
};

