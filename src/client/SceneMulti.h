#pragma once
#include "Scene.h"
#include "Bullet.h"
#include "HUD.h"
#include"Skybox.h"
#include"vertexBufferObject.h"
#include"HeightMap.h"
#include"TextureClass.h"
#include"DirLight.h"
#include"Geometry.h"
#include <thread>
#include <mutex>
#include <deque>
class SceneMulti :
	public Scene
{
public:
	SceneMulti(glm::mat4 projectionMatrix, Camera* camera);
	~SceneMulti();
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
	CSkybox MainSkybox;
	std::vector<reference_wrapper<const Shader>>SH;
	HUD life1, life2, life3;
	HeightMap World;
	HeightMapBuffer VBOSceneObject;
	CDirectionalLight Sun;
	unsigned int uVAO[1];
};
