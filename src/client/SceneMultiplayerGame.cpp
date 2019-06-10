#include "SceneMultiplayerGame.h"
#include "Terrain.h"
#include "Loader.h"
#include "Networking.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "SceneManager.h"
#include <algorithm>



SceneMultiplayerGame::SceneMultiplayerGame(glm::mat4 projectionMatrix, Camera * camera) : Scene(projectionMatrix, camera)
{
}

SceneMultiplayerGame::~SceneMultiplayerGame()
{
}

void SceneMultiplayerGame::InitScene()
{
	run = true;
	scoreBoard = false;
	imgPtr = new Image2D(-0.5f, 0.5f, -0.5f, 0.5f);
	health = 3;
	ostatniWystrzal = 0.0f;
	lastFrame = SDL_GetTicks() / 1000.0f;
	Terrain* terrain = new Terrain();
	terrain->loadTerrain("res/models/teren_org/teren.obj", "res/models/teren_org/height.png", Loader::getShader(Loader::LoadedShaders::LIGHT));
	addTerrain(terrain);
	SetLight(new LightObject(Loader::getModel(Loader::LoadedModels::CUBE), glm::vec3(0.0f, 10.0f, 0.0f), 0.0f, glm::vec3(0.2f), &Loader::getShader(Loader::LoadedShaders::SIMPLE), glm::vec3(1.0f, 1.0f, 1.0f)));
	player2 = addObject(new Entity(Loader::getModel(Loader::LoadedModels::PLAYER), glm::vec3(20.0f, 0.0f, 7.0f), 0.0f, glm::vec3(1.0f)));
	player2->setShader(Loader::getShader(Loader::LoadedShaders::LIGHT));
	Networking::setDataSize(sizeof(pos));
	SDL_SetRelativeMouseMode(SDL_TRUE);
	SoundMgr = Sound::Instance();
	SoundM = SoundManager::Instance();
	SoundM->PlayMusic("DeathMatch_Boss_Theme.ogg", -1);
	SoundM->VolumeMusic(15);
	MainSkybox = CSkybox(Loader::getShader(Loader::LoadedShaders::SKYBOX), "res/img/right.jpg", "res/img/left.jpg", "res/img/top.jpg", "res/img/bottom.jpg", "res/img/front.jpg", "res/img/back.jpg");
	SH.push_back(Loader::getShader(Loader::LoadedShaders::SHADER2D));
	SH.push_back(Loader::getShader(Loader::LoadedShaders::SHADER2D2));
	life1 = new HUD(SH, "heart.png", "sand.jpg", 0.01f, 1);
	life2 = new HUD(SH, "heart.png", "sand.jpg", 0.01f, 1);
	life3 = new HUD(SH, "heart.png", "sand.jpg", 0.01f, 1);
	Bullet::setShader(&Loader::getShader(Loader::LoadedShaders::SIMPLE));
	if (isServer) {
		connectionThread = new std::thread([=] {serverConnectionHandlerThread(); });
		getCamera()->cameraPos = glm::vec3(25.0f, 0.85f, 5.0f);
	}
	else {
		connectionThread = new std::thread([=] {connectionHandlerThread(); });
		getCamera()->cameraPos = glm::vec3(25.0f, 0.85f, 45.0f);
	}
	

}

void SceneMultiplayerGame::UnInitScene()
{
	delete life1;
	delete life2;
	delete life3;
	SoundM->HaltSFX(0);
	SoundM->HaltMusic();
	connectionThread->join();
	Networking::stopServer();
	this->removeObject(player2);
	delete connectionThread;
	delete player2;
	delete imgPtr;
	Sound::Release();
	SoundMgr = nullptr;
	SoundManager::Release();
	SoundM = nullptr;
	SH.clear();
}

void SceneMultiplayerGame::handleEvents(SDL_Event & e)
{
	currentFrame = SDL_GetTicks() / 1000.0f;
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
	if (!run)
		SceneManager::GetInstance().SelectScene(SceneManager::Scenes::SCENE_MENU);
	if (scoreBoard) {
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT) {
				quitLock.lock();
				run = false;
				quitLock.unlock();
				SceneManager::GetInstance().SelectScene(SceneManager::Scenes::SCENE_QUIT);
			}
			switch (e.type) {
			case SDL_KEYDOWN: {

				switch (e.key.keysym.sym) {
				case SDLK_RETURN: {
					quitLock.lock();
					run = false;
					quitLock.unlock();
					SceneManager::GetInstance().SelectScene(SceneManager::Scenes::SCENE_MENU);
					break;
				}
				default: break;
				}
				break;
			}
			default: break;
			}
		}
	}
	else {
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT) {
				quitLock.lock();
				run = false;
				quitLock.unlock();
				SceneManager::GetInstance().SelectScene(SceneManager::Scenes::SCENE_QUIT);
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
					SoundM->PlaySFX("Jump.ogg");
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
				if (e.button.button == SDL_BUTTON_LEFT && currentFrame > ostatniWystrzal) {
					ostatniWystrzal = currentFrame + 0.5f;
					hit = false;
					Bullet* tmpBullet = new Bullet(getCamera()->cameraPos, getCamera()->cameraFront);
					//sprawdzenie kolizji
					glm::vec3 pos = getCamera()->cameraPos;
					glm::vec3 player2Pos = player2->getPosition();
					glm::vec3 bulletVec = getCamera()->cameraFront;
					bulletVec.y -= 0.1f;
					//2 gracze na tej samej czesci
					if ((pos.z <= 17.5f && player2Pos.z <= 17.5f) || (pos.z >= 32.5f && player2Pos.z >= 32.5f) || (pos.z >= 17.5f && pos.z <= 32.5f && player2Pos.z >= 17.5f && player2Pos.z <= 32.5f)) {
						float odleglosc = sqrt(powf(pos.x - player2Pos.x, 2) + powf(pos.y - player2Pos.y, 2) + powf(pos.z - player2Pos.z, 2));
						glm::vec3 test = glm::vec3(pos.x + bulletVec.x * odleglosc, pos.y + odleglosc * bulletVec.y, pos.z + odleglosc * bulletVec.z);
						float deltaY = test.y - player2Pos.y;
						if (abs(test.z - player2Pos.z) <= 0.75f && abs(test.x - player2Pos.x) <= 0.75f) {
							if (test.y >= player2Pos.y &&  test.y - player2Pos.y <= 2.0f)
								hit = true;

						}
					}
					//gora/dol czesc blizsza 0
					else if ((pos.z <= 32.5f && player2Pos.z <= 17.5f) || (pos.z <= 17.5f && player2Pos.z <= 32.5f)) {
						float odleglosc = sqrt(powf(pos.x - player2Pos.x, 2) + powf(pos.y - player2Pos.y, 2) + powf(pos.z - player2Pos.z, 2));
						glm::vec3 test = glm::vec3(pos.x + bulletVec.x * odleglosc, pos.y + odleglosc * bulletVec.y, pos.z + odleglosc * bulletVec.z);
						if (abs(test.z - player2Pos.z) <= 0.75f && abs(test.x - player2Pos.x) <= 0.75f && test.y >= player2Pos.y &&  test.y - player2Pos.y <= 2.0f) {
							float t = (17.5f - pos.z) / bulletVec.z;
							float y = pos.y + bulletVec.y * t;
						
							float testPos = std::min(3.0f, player2Pos.y);
							if (y >= testPos)
								hit = true;
							else {
								tmpBullet->vertices[0] = pos.x + bulletVec.x * t;
								tmpBullet->vertices[1] = y;
								tmpBullet->vertices[2] = 17.5f;
							}
						}
					}
					//czesc dalsza
					else if ((pos.z >= 32.5f && player2Pos.z >= 17.5f&& player2Pos.z <= 32.5f) || (pos.z <= 32.5f && pos.z >= 17.5f && player2Pos.z >= 32.5f)) {
						float odleglosc = sqrt(powf(pos.x - player2Pos.x, 2) + powf(pos.y - player2Pos.y, 2) + powf(pos.z - player2Pos.z, 2));
						glm::vec3 test = glm::vec3(pos.x + bulletVec.x * odleglosc, pos.y + odleglosc * bulletVec.y, pos.z + odleglosc * bulletVec.z);
						if (abs(test.z - player2Pos.z) <= 0.75f && abs(test.x - player2Pos.x) <= 0.75f && test.y >= player2Pos.y &&  test.y - player2Pos.y <= 2.0f) {
							float t = (32.5f - pos.z) / bulletVec.z;
							float y = pos.y + bulletVec.y * t;
							float testPos = std::min(3.0f, player2Pos.y);
							if (y >= testPos)
								hit = true;
							else {
								tmpBullet->vertices[0] = pos.x + bulletVec.x * t;
								tmpBullet->vertices[1] = y;
								tmpBullet->vertices[2] = 32.5f;
							}
						}
					}
					//strzelanie z 1 na 3, we wszystkich miejscach wektor.y > 3
					else if ((pos.z <= 17.5f && player2Pos.z >= 32.f) || (player2Pos.z <= 17.5f && pos.z >= 32.f)) {
						float odleglosc = sqrt(powf(pos.x - player2Pos.x, 2) + powf(pos.y - player2Pos.y, 2) + powf(pos.z - player2Pos.z, 2));
						glm::vec3 test = glm::vec3(pos.x + bulletVec.x * odleglosc, pos.y + odleglosc * bulletVec.y, pos.z + odleglosc * bulletVec.z);
						if (abs(test.z - player2Pos.z) <= 0.75f && abs(test.x - player2Pos.x) <= 0.75f && test.y >= player2Pos.y &&  test.y - player2Pos.y <= 2.0f) {
							float t1 = (32.5f - pos.z) / bulletVec.z;
							float y1 = pos.y + bulletVec.y * t1;
							float t2 = (17.5f - pos.z) / bulletVec.z;
							float y2 = pos.y + bulletVec.y * t2;
							if (y1 >= 3.0f)
								if (y2 >= 3.0f)
									hit = true;
								else {
									tmpBullet->vertices[0] = pos.x + bulletVec.x * t2;
									tmpBullet->vertices[1] = y2;
									tmpBullet->vertices[2] = 17.5f;
								}
							else {
								if (y2 >= 3.0f) {
									tmpBullet->vertices[0] = pos.x + bulletVec.x * t1;
									tmpBullet->vertices[1] = y1;
									tmpBullet->vertices[2] = 32.5f;
								}
								else {
									tmpBullet->vertices[0] = pos.x + bulletVec.x * t2;
									tmpBullet->vertices[1] = y2;
									tmpBullet->vertices[2] = 17.5f;
								}
							}
						}
					}

					bulletContainer.push_back(tmpBullet);
					tmpThreadObject = new std::thread([=] {deleteBullet(); });
					tmpThreadObject->detach();
					sendBullet = true;
					SoundM->PlaySFX("laser.wav");
				}
			}
			default: break;
			}
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
	//rysowanie skyboxa
	MainSkybox.SetDeltatime(currentFrame);
	MainSkybox.RenderSkybox(Loader::getShader(Loader::LoadedShaders::SKYBOX), this->GetViewMatrix(), this->GetProjectionMatrix());


	//rysowanie pociskow
	bulletLock.lock();
	for (Bullet* bullet : bulletContainer) {
		bullet->draw(GetProjectionMatrix(), getCamera()->getViewMatrix(), glm::vec3(1.0f, 0.0f, 0.0f));
	}
	bulletLock.unlock();
	//rysowanie broni
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, getCamera()->cameraPos + getCamera()->cameraFront);
	model = glm::rotate(model, glm::radians(180.0f - getCamera()->yaw), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(-getCamera()->pitch), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3(0.35f, -0.15f, 0.0f));
	if (ostatniWystrzal != 0.0) {
		if (currentFrame >= ostatniWystrzal) {
			ostatniWystrzal = 0.0;
		}
		else {
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
	if (health >= 1) {
		life1->RenderHUD(SH);
		life1->Incrementvalue();
		life1->RotationIntensity();
		if (health >= 2) {
			life2->RenderHUD(SH);
			if(health == 3)
				life3->RenderHUD(SH);
		}
	}
	if (scoreBoard) {
		nextMove.reset();
		glClear(GL_DEPTH_BUFFER_BIT);
		imgPtr->Draw(Loader::getShader(Loader::LoadedShaders::IMAGE));
	}




}

void SceneMultiplayerGame::setServer(bool value)
{
	isServer = value;
}
void SceneMultiplayerGame::serverConnectionHandlerThread() {
	bool connection = true;
	Networking::MessageType ctrl;
	float posTemp[4];
	float bulletTmp[6];
	std::thread* tmpPtr;
	if (connection) {
		connection = Networking::sendControlMsg(Networking::MessageType::POSITION_FOLLOWS);
		playerPosLock.lock();
		posTemp[0] = playerPos[0];
		posTemp[1] = playerPos[1];
		posTemp[2] = playerPos[2];
		posTemp[3] = playerPos[3];
		playerPosLock.unlock();
		connection = Networking::sendData(posTemp);
	}
	while (connection && run) {
		bulletLock.lock();
		if (sendBullet) {
			sendBullet = false;
			if (hit)
				connection = Networking::sendControlMsg(Networking::MessageType::HIT);
			else
				connection = Networking::sendControlMsg(Networking::MessageType::SHOOT);
			memcpy(bulletTmp, bulletContainer.back()->vertices, sizeof(float) * 6);
			posTemp[0] = bulletTmp[0];
			posTemp[1] = bulletTmp[1];
			posTemp[2] = bulletTmp[2];
			connection = Networking::sendData(posTemp);
			posTemp[0] = bulletTmp[3];
			posTemp[1] = bulletTmp[4];
			posTemp[2] = bulletTmp[5];
			connection = Networking::sendData(posTemp);
		}
		bulletLock.unlock();
		if (health <= 0) {
			connection = Networking::sendControlMsg(Networking::MessageType::NO_HEALTH);
			connection = Networking::sendControlMsg(Networking::MessageType::NO_HEALTH);
			connection = Networking::sendControlMsg(Networking::MessageType::NO_HEALTH);
			scoreBoard = true;
			imgPtr->SetTexture(Loader::getTexture2D(Loader::LoadedTextures2D::PRZECIWNIK_WON));
			//Networking::closeConnection();
			return;
		}
		connection = Networking::recvControlMsg(&ctrl);
		if (connection && (ctrl == Networking::MessageType::SHOOT || ctrl == Networking::MessageType::HIT)) {
			if (ctrl == Networking::MessageType::HIT){
					SoundM->PlaySFX("ScreamA.wav");
					health--;
			}
			else {
				SoundM->PlaySFX("laser.wav");
			}
			connection = Networking::recvData(posTemp);
			bulletTmp[0] = posTemp[0];
			bulletTmp[1] = posTemp[1];
			bulletTmp[2] = posTemp[2];
			connection = Networking::recvData(posTemp);
			if (!connection)
				break;
			bulletTmp[3] = posTemp[0];
			bulletTmp[4] = posTemp[1];
			bulletTmp[5] = posTemp[2];
			bulletLock.lock();
			bulletContainer.push_back(new Bullet(bulletTmp));
			bulletLock.unlock();
			tmpPtr = new std::thread([=] {deleteBullet(); });
			tmpPtr->detach();
		}
		else if (connection && ctrl == Networking::MessageType::POSITION_FOLLOWS) {
			connection = Networking::recvData(posTemp);
			if (connection) {
				posLock.lock();
				pos[0] = posTemp[0];
				pos[1] = posTemp[1];
				pos[2] = posTemp[2];
				pos[3] = posTemp[3];
				posLock.unlock();
				connection = Networking::sendControlMsg(Networking::MessageType::POSITION_FOLLOWS);
				playerPosLock.lock();
				posTemp[0] = playerPos[0];
				posTemp[1] = playerPos[1];
				posTemp[2] = playerPos[2];
				posTemp[3] = playerPos[3];
				playerPosLock.unlock();
				connection = Networking::sendData(posTemp);
			}
			else break;
		}
		else if (connection && ctrl == Networking::MessageType::NO_HEALTH) {
			scoreBoard = true;
			imgPtr->SetTexture(Loader::getTexture2D(Loader::LoadedTextures2D::JA_WON));
			Networking::closeConnection();
			return;
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
	float bulletTmp[6];
	std::thread* tmpPtr;
	while (connection && run) {
		bulletLock.lock();
		if (sendBullet) {
			sendBullet = false;
			if (hit)
				connection = Networking::sendControlMsg(Networking::MessageType::HIT);
			else
				connection = Networking::sendControlMsg(Networking::MessageType::SHOOT);
			memcpy(bulletTmp, bulletContainer.back()->vertices, sizeof(float) * 6);
			posTemp[0] = bulletTmp[0];
			posTemp[1] = bulletTmp[1];
			posTemp[2] = bulletTmp[2];
			connection = Networking::sendData(posTemp);
			posTemp[0] = bulletTmp[3];
			posTemp[1] = bulletTmp[4];
			posTemp[2] = bulletTmp[5];
			connection = Networking::sendData(posTemp);
		}
		bulletLock.unlock();
		if (health <= 0) {
			connection = Networking::sendControlMsg(Networking::MessageType::NO_HEALTH);
			connection = Networking::sendControlMsg(Networking::MessageType::NO_HEALTH);
			scoreBoard = true;
			imgPtr->SetTexture(Loader::getTexture2D(Loader::LoadedTextures2D::PRZECIWNIK_WON));
			Networking::closeConnection();
			return;
		}
		connection = Networking::recvControlMsg(&ctrl);
		if (connection && (ctrl == Networking::MessageType::SHOOT || ctrl == Networking::MessageType::HIT)) {
			if (ctrl == Networking::MessageType::HIT)
			{
				SoundM->PlaySFX("ScreamA.wav");
				health--;
			}
			else {
				SoundM->PlaySFX("laser.wav");
			}
				
			connection = Networking::recvData(posTemp);
			bulletTmp[0] = posTemp[0];
			bulletTmp[1] = posTemp[1];
			bulletTmp[2] = posTemp[2];
			connection = Networking::recvData(posTemp);
			if (!connection)
				break;
			bulletTmp[3] = posTemp[0];
			bulletTmp[4] = posTemp[1];
			bulletTmp[5] = posTemp[2];
			bulletLock.lock();
			bulletContainer.push_back(new Bullet(bulletTmp));
			bulletLock.unlock();
			tmpPtr = new std::thread([=] {deleteBullet(); });
			tmpPtr->detach();
		}
		else if (connection && ctrl == Networking::MessageType::POSITION_FOLLOWS) {
			connection = Networking::recvData(posTemp);
			if (connection) {
				posLock.lock();
				pos[0] = posTemp[0];
				pos[1] = posTemp[1];
				pos[2] = posTemp[2];
				pos[3] = posTemp[3];
				posLock.unlock();
				connection = Networking::sendControlMsg(Networking::MessageType::POSITION_FOLLOWS);
				playerPosLock.lock();
				posTemp[0] = playerPos[0];
				posTemp[1] = playerPos[1];
				posTemp[2] = playerPos[2];
				posTemp[3] = playerPos[3];
				playerPosLock.unlock();
				connection = Networking::sendData(posTemp);
			}
			else break;
		}
		else if (connection && ctrl == Networking::MessageType::NO_HEALTH) {
			scoreBoard = true;
			imgPtr->SetTexture(Loader::getTexture2D(Loader::LoadedTextures2D::JA_WON));
			Networking::closeConnection();
			return;
		}
		else if (ctrl == Networking::MessageType::QUIT) {
			std::cout << "Zakonczenie przez serwer" << std::endl;
			Networking::closeConnection();
			connection = false;
			return; //zakonczenie polaczenia
		}
		
		std::this_thread::sleep_for(std::chrono::milliseconds(16)); //16ms opoznienia miedzy wiadomosciami
	}
	Networking::closeConnection();
	quitLock.lock();
	run = false;
	quitLock.unlock();
}

void SceneMultiplayerGame::deleteBullet()
{
	SDL_Delay(500);
	bulletLock.lock();
	if(bulletContainer.size())
	{ 
		delete bulletContainer.front();
		bulletContainer.pop_front();
	}
	bulletLock.unlock();
}
