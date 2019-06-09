#include "SceneManager.h"



SceneManager::SceneManager()
{
	scenePointers = new Scene*[SCENE_QUIT];
}


SceneManager::~SceneManager()
{
	if (sceneGamePtr)
		delete sceneGamePtr;
	if (sceneMultiplayerGamePtr)
		delete sceneMultiplayerGamePtr;
	//nalezy usunac kazda ze scen (Moze mozna sprytniej?)
}

SceneManager& SceneManager::GetInstance()
{
	static SceneManager sceneManagerObj;
	return sceneManagerObj;
}

void SceneManager::SelectScene(Scenes newScene)
{
	Scene* newScenePtr;
	// kazda scena w swoim case'ie 
	switch (newScene) {
	case SCENE_MULTIPLAYER: {
		if (!sceneMultiplayerGamePtr)
			//utworzenie sceny, jezeli jeszcze nie istnieje
			sceneMultiplayerGamePtr = new SceneMultiplayerGame(glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.01f, 100.0f), 
				new Camera(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.1f, 8.0f)
			);
		newScenePtr = sceneMultiplayerGamePtr;
		break;
	}
	case SCENE_QUIT: {
		newScenePtr = nullptr;
		break;
	}
	case SCENE_SANDBOX: {
		if (!sceneGamePtr)
			//utworzenie sceny, jezeli jeszcze nie istnieje
			sceneGamePtr = new SceneGame(glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.01f, 100.0f), 
				new Camera(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.1f, 8.0f)
			);
			newScenePtr = sceneGamePtr;
			break;
	}
	}
	if (currScenePtr)
		currScenePtr->UnInitScene();
	currScenePtr = newScenePtr;
	if (newScenePtr)
		newScenePtr->InitScene();
	currentScene = newScene;
}

Scene * SceneManager::getScene()
{
	return currScenePtr;
}

Scene * SceneManager::getScene(Scenes scene)
{
	switch (scene) {
	case SCENE_MULTIPLAYER: {
		if (!sceneMultiplayerGamePtr)
			//utworzenie sceny, jezeli jeszcze nie istnieje
			sceneMultiplayerGamePtr = new SceneMultiplayerGame(glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.01f, 100.0f),
				new Camera(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.1f, 8.0f)
			);
		return sceneMultiplayerGamePtr;
		break;
	}
	}
}
