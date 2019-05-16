#include "SceneMultiplayerGame.h"
#include "Terrain.h"
#include "Loader.h"
#include "Networking.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"



SceneMultiplayerGame::SceneMultiplayerGame(glm::mat4 projectionMatrix, Camera * camera) : Scene(projectionMatrix, camera)
{
}

SceneMultiplayerGame::~SceneMultiplayerGame()
{
}

void SceneMultiplayerGame::InitScene()
{
	lastFrame = SDL_GetTicks() / 1000.0f;
	Terrain* terrain = new Terrain();
	terrain->loadTerrain("res/models/teren_org/teren.obj", "res/models/teren_org/height.png", Loader::getShader(Loader::LoadedShaders::LIGHT));
	addTerrain(terrain);
	SetLight(new LightObject(Loader::getModel(Loader::LoadedModels::CUBE), glm::vec3(0.0f, 10.0f, 0.0f), 0.0f, glm::vec3(0.2f), &Loader::getShader(Loader::LoadedShaders::SIMPLE), glm::vec3(1.0f, 1.0f, 1.0f)));
	player2 = addObject(new Entity(Loader::getModel(Loader::LoadedModels::PLAYER), glm::vec3(20.0f, 0.0f, 7.0f), 0.0f, glm::vec3(1.0f)));
	player2->setShader(Loader::getShader(Loader::LoadedShaders::LIGHT));
	Networking::setDataSize(sizeof(pos));
	SDL_SetRelativeMouseMode(SDL_TRUE);
	if (isServer) {
		connectionThread = new std::thread([=] {serverConnectionHandlerThread(); });
	}
	else {
		connectionThread = new std::thread([=] {connectionHandlerThread(); });
	}

}

void SceneMultiplayerGame::UnInitScene()
{
	connectionThread->join();
	delete connectionThread;
	delete player2;
}

void SceneMultiplayerGame::handleEvents(SDL_Event & e)
{
	currentFrame = SDL_GetTicks() / 1000.0f;
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
	while (SDL_PollEvent(&e))
	{
		if (e.type == SDL_QUIT) {
			quitLock.lock();
			run = false; //jakis hack na wyjscie, eventy musi pobierac w whilu zeby kamera byla plynna...
			quitLock.unlock();
		}
		switch (e.type) {
		case SDL_KEYDOWN: {

			switch (e.key.keysym.sym) {
			case SDLK_w: {
				nextMove[Camera::MOVE_FORWARD] = 1;
				break;
			}
			case SDLK_s: {
				nextMove[Camera::MOVE_BACKWARD] = 1;
				break;
			}
			case SDLK_a: {
				nextMove[Camera::STRAFE_LEFT] = 1;
				break;
			}
			case SDLK_d: {
				nextMove[Camera::STRAFE_RIGHT] = 1;;
				break;
			}
			case SDLK_SPACE: {
				playerJumpStart();
				break;
			}
			default: break;
			}
			break;
		}
		case SDL_KEYUP: {

			switch (e.key.keysym.sym) {
			case SDLK_w: {
				nextMove[Camera::MOVE_FORWARD] = 0;
				break;
			}
			case SDLK_s: {
				nextMove[Camera::MOVE_BACKWARD] = 0;
				break;
			}
			case SDLK_a: {
				nextMove[Camera::STRAFE_LEFT] = 0;
				break;
			}
			case SDLK_d: {
				nextMove[Camera::STRAFE_RIGHT] = 0;;
				break;
			}
			default: break;
			}
			break;
		}
		case SDL_MOUSEMOTION: {
			getCamera()->turnCamera(e.motion);
		}
		case SDL_MOUSEBUTTONDOWN: {
			if (e.button.button == SDL_BUTTON_LEFT) {
				ostatniWystrzal = currentFrame + 0.5f;
			}
		}
		default: break;
		}
	}
	movePlayer(nextMove, deltaTime);

	glm::vec3 playerPosV = getCamera()->cameraPos;
	playerPosLock.lock();
	playerPos[0] = playerPosV.x;
	playerPos[1] = playerPosV.y - 1.0f;
	playerPos[2] = playerPosV.z;
	playerPos[3] = getCamera()->yaw;
	playerPosLock.unlock();
	posLock.lock();
	glm::vec3 player2pos(pos[0], pos[1], pos[2]);
	player2->setRotationY(-glm::radians(pos[3]));
	posLock.unlock();
	player2->setPosition(player2pos);
}

void SceneMultiplayerGame::render()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	DrawObjects();
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, getCamera()->cameraPos + getCamera()->cameraFront);
	model = glm::rotate(model, glm::radians(180.0f - getCamera()->yaw), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(-getCamera()->pitch), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3(0.35f, -0.15f, 0.0f));
	//std::cout << terrain.getHeight(testowa.getCamera()->cameraPos) << std::endl;
	if (ostatniWystrzal != 0.0) {
		if (currentFrame >= ostatniWystrzal) {
			ostatniWystrzal = 0.0;
		}
		else {
			//std::cout << glm::cos((ostatniWystrzal - currentFrame) / 2.0f) << std::endl;
			model = glm::rotate(model, -glm::sin((ostatniWystrzal - currentFrame) / 0.5f) / 3.0f, glm::vec3(0.0f, 0.0f, 1.0f));
		}
	}
	model = glm::scale(model, glm::vec3(0.03f, 0.03f, 0.03f));
	Shader& lightShader = Loader::getShader(Loader::LoadedShaders::LIGHT);
	lightShader.use();
	lightShader.use();
	lightShader.setMat4("model", model);
	lightShader.setMat4("projection", GetProjectionMatrix());
	lightShader.setMat4("view", GetViewMatrix());
	lightShader.setVec3("lightColor", GetLight()->GetColor());
	lightShader.setVec3("lightPos", GetLight()->GetPosition());
	lightShader.setVec3("viewPos", getCamera()->cameraPos);
	glClear(GL_DEPTH_BUFFER_BIT);
	Loader::getModel(Loader::LoadedModels::GUN).Draw(lightShader);
}

void SceneMultiplayerGame::setServer(bool value)
{
	isServer = value;
}
void SceneMultiplayerGame::serverConnectionHandlerThread() {
	bool connection = true;
	Networking::MessageType ctrl;
	float posTemp[4];
	while (connection && run) {
		connection = Networking::sendControlMsg(Networking::MessageType::POSITION_FOLLOWS);
		playerPosLock.lock();
		posTemp[0] = playerPos[0];
		posTemp[1] = playerPos[1];
		posTemp[2] = playerPos[2];
		posTemp[3] = playerPos[3];
		playerPosLock.unlock();
		connection = Networking::sendData(posTemp);
		connection = Networking::recvControlMsg(&ctrl);
		if (connection && ctrl == Networking::MessageType::POSITION_FOLLOWS) {
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
		else if (!connection || ctrl == Networking::MessageType::QUIT) {
			break; //zakonczenie polaczenia
		}
	}
	connection = false;
	Networking::closeConnection();
	quitLock.lock();
	run = false;
	quitLock.unlock();
}

void SceneMultiplayerGame::connectionHandlerThread() {
	bool connection = true;
	Networking::MessageType ctrl;
	float posTemp[4];
	while (connection && run) {
		connection = Networking::recvControlMsg(&ctrl);
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
		connection = Networking::sendControlMsg(Networking::MessageType::POSITION_FOLLOWS);
		playerPosLock.lock();
		posTemp[0] = playerPos[0];
		posTemp[1] = playerPos[1];
		posTemp[2] = playerPos[2];
		posTemp[3] = playerPos[3];
		playerPosLock.unlock();
		connection = Networking::sendData(posTemp);
		std::this_thread::sleep_for(std::chrono::milliseconds(16)); //16ms opoznienia miedzy wiadomosciami
	}
	Networking::closeConnection();
	quitLock.lock();
	run = false;
	quitLock.unlock();
}