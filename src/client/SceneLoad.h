#pragma once
#include"Scene.h"
#include"Skybox.h"
#include"TextManager.h"
#include"ResourceManager.h"
#include"SpriteRenderer.h"
#include"SoundManager.h"
#include"Sound.h"
#include <glm/glm.hpp>
#include<list>
#include "SceneMultiplayerGame.h"
class SceneLoad :public Scene
{
public:
	SceneLoad(glm::mat4 projectionMatrix, Camera* camera);
	~SceneLoad();
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
	TextManager *Text2;
	SpriteRenderer*Renderer;
	SpriteRenderer*Background;
	std::vector<char> IP;
	std::vector<char>::iterator it;
	bool SelectedMode;
	SoundManager* SoundM;
	Sound* SoundMgr;
	void serverThread();
	bool serverRun = false;
	bool connected = false;
	SceneMultiplayerGame* multiplayerScene;
};

