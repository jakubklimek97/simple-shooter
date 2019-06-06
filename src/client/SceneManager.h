#pragma once
#include "Scene.h"
#include "SceneMulti.h"
//do��czamy sceny
class SceneManager
{
public:
	enum Scenes {
		SCENE_DEFAULT,
		SCENE_SANDBOX,
		SCENE_MULTIPLAYER,
		SCENE_MULTI,
		SCENE_QUIT
	};
	static SceneManager& GetInstance();
	void SelectScene(enum Scenes newScene);
	Scene* getScene();
	Scene* getScene(Scenes scene);
	enum Scenes currentScene = SCENE_DEFAULT;
private:
	SceneManager();
	~SceneManager();
	SceneMulti *SceneMultiPtr = nullptr;

	Scene** scenePointers;
	Scene* currScenePtr = nullptr;
	

};
