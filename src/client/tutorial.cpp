#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <SDL.h>
#include <SDL_image.h>
#include "SDL_net.h"

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
#include "Networking.h"
#include "utils.h"
#include "Loader.h"
#include "SceneGame.h"
#include "SceneMultiplayerGame.h"



void serverThread() {
	Networking::startServer(60100);
	bool connection;
	do {
		SDL_Delay(1000);
		connection = Networking::acceptConnection();
		std::cout << "Dalej nic..." << std::endl;
	} while (!connection);
}

int initSDL() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cout << "Nie mozna zainicjowac SDLA: " << SDL_GetError() << std::endl;
		return -1;
	}
	if (IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) < 0) {
		std::cout << "Nie mozna zainicjowac SDL_image: " << IMG_GetError() << std::endl;
		return -1;
	}
	if (SDLNet_Init() < 0) {
		std::cout << "Nie mozna zainicjowac SDL_net: " << SDLNet_GetError() << std::endl;
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
	pWindow = SDL_CreateWindow("Simple Shooter", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);

	//"zawartosc" okna
	context = SDL_GL_CreateContext(pWindow);

	//inicjalizacja glewa
	glewExperimental = GL_TRUE;
	glewInit();
	glViewport(0, 0, 800, 600);
	glEnable(GL_DEPTH_TEST);
}

int main(int argc, char *argv[])
{
	int czySerwer; //1- serwer, 0 - klient
	std::cin >> czySerwer;
	if (initSDL() < 0) return -1;
	int serwer = parseParams(argc, argv);
	SDL_Window* window;
	SDL_GLContext context;
	initOpenGL(window, context);
	//wyswietlane jako siatka glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	Loader::loadModels();
	Loader::loadShaders();
	Loader::loadTextures2D();

	/*Entity* testBoundingBox = testowa.addObject(new Entity(kostka, glm::vec3(4.0f, 0.0f, 0.0f), 0.0f, glm::vec3(0.5f, 0.5f, 0.5f)));
	testBoundingBox->setShader(lightShader);
	testBoundingBox->rotateY(45.0f);

	BoundingBox box(*testBoundingBox);
	testowa.removeObject(testCube);
	testowa.removeObject(testBoundingBox);*/
	/*testCube->rotateZ(glm::radians(1.0f));
		testCube->rotateY(glm::radians(1.0f));
		testBoundingBox->rotateY(glm::radians(2.0f));*/
		/* na razie cofnijmy sie do terenu
		box.calculateBoundingBox();
		box.Draw(testowa.GetProjectionMatrix(), testowa.GetViewMatrix(), boundingBoxShader);
		*/
	SceneMultiplayerGame* multiplayerScene = new SceneMultiplayerGame(glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.01f, 100.0f), new Camera(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.1f, 8.0f));
	SceneGame* gameScene = new SceneGame(glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.01f, 100.0f), new Camera(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.1f, 8.0f));
	if (czySerwer) {
		multiplayerScene->setServer(true);
		serverThread();

	}
	else {
		bool connection = Networking::connect("127.0.0.1", 60100);
		multiplayerScene->setServer(false);
		if (!connection)
			return 0; //brzydkie wyjscie, powinno byc w scenie z nawiazywaniem polaczenia
	}
	Scene *ptr = multiplayerScene;
	ptr->InitScene();
	
	SDL_Event e;
	while (ptr->run)
	{
		ptr->handleEvents(e);

		ptr->render();
		
		SDL_GL_SwapWindow(window); // zostanie w glownej petli aplikacji, nie ma sensu sie powtarzac
	}
	//na razie takie zakonczenie serwera
	Networking::stopServer();
	ptr->UnInitScene();
	delete gameScene;
	delete multiplayerScene;
	Loader::unloadShaders();
	Loader::unloadModels();
	Loader::unloadTextures2D();
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	SDLNet_Quit();
	IMG_Quit();
	SDL_Quit();
	return 0;
}