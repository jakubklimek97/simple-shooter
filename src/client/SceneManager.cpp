#include "SceneManager.h"



SceneManager::SceneManager()
{
	scenePointers = new Scene*[SCENE_QUIT];
}


SceneManager::~SceneManager()
{
	if (SceneMultiPtr)
		delete SceneMultiPtr;
	if (SceneMenuPtr)
		delete SceneMenuPtr;
	if (SceneLoadPtr)
		delete SceneLoadPtr;
	if (SceneExitPtr)
		delete SceneExitPtr;
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
	case SCENE_MULTI: {
		if (!SceneMultiPtr)
			//utworzenie sceny, jezeli jeszcze nie istnieje
			SceneMultiPtr = new SceneMulti(glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.01f, 10000.0f),
				new Camera(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.1f, 8.0f)
			);
		newScenePtr = SceneMultiPtr;
		break;
	}
	case SCENE_QUIT: {
		newScenePtr = nullptr;
		break;
	}
	case SCENE_MENU: {
		if(!SceneMenuPtr)
			
			SceneMenuPtr = new SceneMenu(glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.01f, 100.0f),
				
				new Camera(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.1f, 8.0f)
			);
		newScenePtr = SceneMenuPtr;
		break;
	}
	case SCENE_LOAD: {
		if (!SceneLoadPtr)

			SceneLoadPtr = new SceneLoad(glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.01f, 100.0f),

				new Camera(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.1f, 8.0f)
			);
		newScenePtr = SceneLoadPtr;
		break;

	}
	case SCENE_EXIT: {
		if (!SceneExitPtr)

			SceneExitPtr = new SceneExit(glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.01f, 100.0f),

				new Camera(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.1f, 8.0f)
			);
		newScenePtr = SceneExitPtr;
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
	case SCENE_MULTI: {
		if (!SceneMultiPtr)
			//utworzenie sceny, jezeli jeszcze nie istnieje
			SceneMultiPtr = new SceneMulti(glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.01f, 100.0f),
				new Camera(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.1f, 8.0f)
			);
		return SceneMultiPtr;
		break;
	}
	case SCENE_MENU: {
		if (!SceneMenuPtr) {
			//utworzenie sceny, jezeli jeszcze nie istnieje
			SceneMenuPtr = new SceneMenu(glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.01f, 100.0f),
				new Camera(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.1f, 8.0f)
			);
			return SceneMenuPtr;
			break;

		}

	}

	case SCENE_LOAD: {
		if (!SceneLoadPtr) {
			//utworzenie sceny, jezeli jeszcze nie istnieje
			SceneLoadPtr = new SceneLoad(glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.01f, 100.0f),
				new Camera(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.1f, 8.0f)
			);
			return SceneLoadPtr;
			break;

		}


	}

	case SCENE_EXIT: {
		if (!SceneExitPtr) {
			//utworzenie sceny, jezeli jeszcze nie istnieje
			SceneExitPtr = new SceneExit(glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.01f, 100.0f),
				new Camera(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.1f, 8.0f)
			);
			return SceneExitPtr;
			break;

		}


	}


	}
}