#pragma once
#include "Scene.h"
#include "Bullet.h"
#include <thread>
#include <mutex>
#include <deque>
#include "Image2D.h"
class SceneGame :
	public Scene
{
public:
	SceneGame(glm::mat4 projectionMatrix, Camera* camera);
	~SceneGame();
	virtual void InitScene();
	virtual void UnInitScene();
	virtual void handleEvents(SDL_Event& e);
	virtual void render();
	Entity * player2; //na razie, pozniej trafi do private
private: //zmienne uzywane przez scene (indywidualne)
	float ostatniWystrzal = 0.0f;
	bool quit = false;
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;
	float currentFrame = 0.0f;
	Camera::Movement nextMove;
	std::thread* tmpThreadObject;
	std::mutex bulletLock;
	std::deque<Bullet*> bulletContainer;
	void deleteBullet();
	Image2D* imgPtr;
};

