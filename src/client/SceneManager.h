#pragma once
#include "Scene.h"
#include "SceneMultiplayerGame.h"
#include "SceneGame.h"
#include "SceneMulti.h"
#include"SceneMenu.h"
#include"SceneExit.h"
#include"SceneLoad.h"
//do³¹czamy sceny
class SceneManager
{
public:
	enum Scenes {
		SCENE_DEFAULT,
		SCENE_SANDBOX,
		SCENE_MULTIPLAYER,
		SCENE_MENU,
		SCENE_MULTI,
		SCENE_LOAD,
		SCENE_EXIT,
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
	SceneMulti *SceneMultiPtr = nullptr;
	SceneMenu *SceneMenuPtr = nullptr;
	SceneExit *SceneExitPtr = nullptr;
	SceneLoad *SceneLoadPtr = nullptr;
	Scene** scenePointers;
	Scene* currScenePtr = nullptr;
	

};

