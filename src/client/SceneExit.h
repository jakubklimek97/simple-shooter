#pragma once
#include"Scene.h"
#include"Skybox.h"
#include"TextManager.h"
#include"ResourceManager.h"
#include"SpriteRenderer.h"
#include <glm/glm.hpp>
class SceneExit :public Scene
{
public:
	SceneExit(glm::mat4 projectionMatrix, Camera* camera);
	~SceneExit();
	virtual void InitScene();
	virtual void UnInitScene();
	virtual void handleEvents(SDL_Event& e);
	virtual void render();
	Entity * player2;
private:
	float ostatniWystrzal = 0.0f;
	bool quit = false;
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;
	float currentFrame = 0.0f;
	Camera::Movement nextMove;
	CSkybox MainSkybox;
	TextManager *Text;
	SpriteRenderer*Renderer;
	SpriteRenderer*Background;
};

