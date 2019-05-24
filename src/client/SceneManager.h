#pragma once
#include "Scene.h"
#include "SceneMultiplayerGame.h"
#include "SceneGame.h"
//do³¹czamy sceny
class SceneManager
{
public:
	enum Scenes {
		SCENE_DEFAULT,
		SCENE_SANDBOX,
		SCENE_MULTIPLAYER,
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
	SceneGame *sceneGamePtr = nullptr;
	SceneMultiplayerGame *sceneMultiplayerGamePtr = nullptr;
	Scene** scenePointers;
	Scene* currScenePtr = nullptr;
	

};

