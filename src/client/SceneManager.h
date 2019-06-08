#pragma once
#include "Scene.h"
#include "SceneMulti.h"
#include"SceneMenu.h"
#include"SceneExit.h"
#include"SceneLoad.h"
//do��czamy sceny
class SceneManager
{
public:
	enum Scenes {
		SCENE_DEFAULT,
		SCENE_SANDBOX,
		SCENE_MENU,
		SCENE_MULTIPLAYER,
		SCENE_MULTI,
		SCENE_LOAD,
		SCENE_QUIT,
		SCENE_EXIT
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
	SceneMenu *SceneMenuPtr = nullptr;
	SceneExit *SceneExitPtr = nullptr;
	SceneLoad *SceneLoadPtr = nullptr;
	Scene** scenePointers;
	Scene* currScenePtr = nullptr;
	

};
