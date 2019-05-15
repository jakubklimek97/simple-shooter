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

std::mutex loopLock;
std::mutex posLock;
std::mutex playerPosLock;
float pos[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
float playerPos[4] = { 0.0f };
std::thread* connectionThread;

void connectionHandlerThread() {
	bool connection = true;
	Networking::MessageType ctrl;
	float posTemp[4];
	while (connection) {
		Networking::recvControlMsg(&ctrl);
		if (ctrl == Networking::MessageType::POSITION_FOLLOWS) {
			connection = Networking::recvData(posTemp);
			if (connection) {
				posLock.lock();
				pos[0] = posTemp[0];
				pos[1] = posTemp[1];
				pos[2] = posTemp[2];
				pos[3] = posTemp[3];
				posLock.unlock();
			}
			else break;
		}
		else if (ctrl == Networking::MessageType::QUIT) {
			std::cout << "Zakonczenie przez serwer" << std::endl;
			Networking::closeConnection();
			connection = false;
			return; //zakonczenie polaczenia
		}
		Networking::sendControlMsg(Networking::MessageType::POSITION_FOLLOWS);
		playerPosLock.lock();
		posTemp[0] = playerPos[0];
		posTemp[1] = playerPos[1];
		posTemp[2] = playerPos[2];
		posTemp[3] = playerPos[3];
		playerPosLock.unlock();
		Networking::sendData(posTemp);
		std::this_thread::sleep_for(std::chrono::milliseconds(16)); //16ms opoznienia miedzy wiadomosciami
	}
}
void serverConnectionHandlerThread() {
	bool connection = true;
	Networking::MessageType ctrl;
	float posTemp[4];
	while (connection) {
		Networking::sendControlMsg(Networking::MessageType::POSITION_FOLLOWS);
		playerPosLock.lock();
		posTemp[0] = playerPos[0];
		posTemp[1] = playerPos[1];
		posTemp[2] = playerPos[2];
		posTemp[3] = playerPos[3];
		playerPosLock.unlock();
		Networking::sendData(posTemp);
		Networking::recvControlMsg(&ctrl);
		if (ctrl == Networking::MessageType::POSITION_FOLLOWS) {
			connection = Networking::recvData(posTemp);
			if (connection) {
				posLock.lock();
				pos[0] = posTemp[0];
				pos[1] = posTemp[1];
				pos[2] = posTemp[2];
				pos[3] = posTemp[3];
				posLock.unlock();
			}
			else break;
		}
		else if (ctrl == Networking::MessageType::QUIT) {
			std::cout << "Zakonczenie przez serwer" << std::endl;
			Networking::closeConnection();
			connection = false;
			break; //zakonczenie polaczenia
		}	
	}
	connection = false;
	Networking::closeConnection();
}
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

	SceneGame* scenaGra = new SceneGame(glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.01f, 100.0f), new Camera(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.1f, 8.0f));
	scenaGra->InitScene();
	SDL_Event e;
	Networking::setDataSize(sizeof(pos));
	//prosta obsluga serwera(na razie w mainie)
	if (czySerwer) {
		serverThread();
		connectionThread = new std::thread(serverConnectionHandlerThread);
	}
	else {
		bool connection = Networking::connect("localhost", 60100);
		if (connection) {
			connectionThread = new std::thread(connectionHandlerThread);
		}
	}
	/////////////////////////////////////////////////
	while (scenaGra->run)
	{
		scenaGra->handleEvents(e);
		glm::vec3 playerPosV = scenaGra->getCamera()->cameraPos;
		playerPosLock.lock();
		playerPos[0] = playerPosV.x;
		playerPos[1] = playerPosV.y-1.0f;
		playerPos[2] = playerPosV.z;
		playerPos[3] = scenaGra->getCamera()->yaw;
		playerPosLock.unlock();
		posLock.lock();
		glm::vec3 player2pos(pos[0], pos[1], pos[2]);
		scenaGra->player2->setRotationY(-glm::radians(pos[3]));
		posLock.unlock();
		scenaGra->player2->setPosition(player2pos);
		//scenaGra->player2->setRotationY(-glm::radians(scenaGra->getCamera()->yaw));
		scenaGra->render();
		std::cout << pos[3] << std::endl;
		
		SDL_GL_SwapWindow(window); // zostanie w glownej petli aplikacji, nie ma sensu sie powtarzac
	}
	//na razie takie zakonczenie serwera
	Networking::stopServer();
	////////////////////////////
	scenaGra->UnInitScene();
	delete scenaGra;
	Loader::unloadShaders();
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	SDLNet_Quit();
	IMG_Quit();
	SDL_Quit();
	return 0;
}