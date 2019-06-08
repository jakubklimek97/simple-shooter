#pragma once
#include"Scene.h"
#include"Skybox.h"
#include"TextManager.h"
#include"ResourceManager.h"
#include"SpriteRenderer.h"
#include"Sound.h"
#include"SoundManager.h"
#include <glm/glm.hpp>
class SceneMenu:public Scene
{
public:
	SceneMenu(glm::mat4 projectionMatrix, Camera* camera);
	~SceneMenu();
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
	static int currentSelection;
	static bool EnterPressed;
	static float rotation;
	TextManager *Text;
	TextManager *Text2;
	SpriteRenderer*Renderer;
	SoundManager* SoundM;
	Sound* SoundMgr;
};
extern bool IsClient;
