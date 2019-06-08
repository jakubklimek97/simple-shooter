#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <SDL.h>
#include <SDL_image.h>

#include <GL/glew.h>
#include <SDL_opengl.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "Entity.h"
#include "Scene.h"
#include "LightObject.h"
#include "BoundingBox.h"
#include "Terrain.h"
#include "Loader.h"
#include "SceneMulti.h"
#include "SceneManager.h"




int initSDL() {
	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) < 0) {
		std::cout << "Nie mozna zainicjowac SDLA: " << SDL_GetError() << std::endl;
		return -1;
	}
	if (IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) < 0) {
		std::cout << "Nie mozna zainicjowac SDL_image: " << IMG_GetError() << std::endl;
		return -1;
	}
	return 0;
}
void initOpenGL(SDL_Window* &pWindow, SDL_GLContext &context) {
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	//utworzenie okna
	pWindow = SDL_CreateWindow("Simple Shooter", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_OPENGL);
	
	//"zawartosc" okna
	context = SDL_GL_CreateContext(pWindow);

	//inicjalizacja glewa
	glewExperimental = GL_TRUE;
	glewInit();
	glViewport(0, 0, 1280, 720);
	glEnable(GL_DEPTH_TEST);
}

int main(int argc, char *argv[])
{
	if (initSDL() < 0) return -1;
	SDL_Window* window;
	SDL_GLContext context;
	initOpenGL(window, context);
	//wyswietlane jako siatka glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	Loader::loadModels();
	Loader::loadShaders();
	Loader::loadTextures2D();


	SceneMulti* multiplayerScene = (SceneMulti*)SceneManager::GetInstance().getScene(SceneManager::Scenes::SCENE_MULTI);	

	SceneMulti* menuScene = (SceneMulti*)SceneManager::GetInstance().getScene(SceneManager::Scenes::SCENE_MENU);

	SceneManager::GetInstance().SelectScene(SceneManager::Scenes::SCENE_MENU);
	SDL_Event e;
	while (SceneManager::GetInstance().currentScene != SceneManager::SCENE_QUIT)
	{
		SceneManager::GetInstance().getScene()->handleEvents(e);

		if (SceneManager::GetInstance().getScene())
			SceneManager::GetInstance().getScene()->render();

		SDL_GL_SwapWindow(window); 
	}
	Loader::unloadShaders();
	Loader::unloadModels();
	Loader::unloadTextures2D();
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit();
	return 0;
}