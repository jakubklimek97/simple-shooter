#pragma once
#include "Scene.h"
#include <thread>
#include <mutex>
#include <deque>
#include "Bullet.h"
#include "Image2D.h"
#include "Sound.h"
#include "SoundManager.h"
#include "HUD.h"
#include"Skybox.h"
class SceneMultiplayerGame :
	public Scene
{
public:
	SceneMultiplayerGame(glm::mat4 projectionMatrix, Camera* camera);
	~SceneMultiplayerGame();
	virtual void InitScene();
	virtual void UnInitScene();
	virtual void handleEvents(SDL_Event& e);
	virtual void render();
	void setServer(bool value);
private:
	float ostatniWystrzal = 0.0f;
	bool quit = false;
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;
	float currentFrame = 0.0f;
	Camera::Movement nextMove;
	bool isServer = false;

	Entity * player2;
	void serverConnectionHandlerThread();
	void connectionHandlerThread();
	std::mutex loopLock;
	std::mutex posLock;
	std::mutex playerPosLock;
	std::mutex quitLock;
	float pos[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	float playerPos[4] = { 0.0f };
	std::thread* connectionThread;
	std::thread* tmpThreadObject;
	std::mutex bulletLock;
	std::deque<Bullet*> bulletContainer;
	void deleteBullet();
	bool sendBullet = false;
	bool hit;
	int health;
	Bullet* tmpBulletPtr;
	bool scoreBoard;
	Image2D* imgPtr;
	SoundManager* SoundM;
	Sound* SoundMgr;
	HUD *life1, *life2, *life3;
	std::vector<reference_wrapper<const Shader>>SH;
	CSkybox MainSkybox;

};

