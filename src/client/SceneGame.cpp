#include "SceneGame.h"
#include "Terrain.h"
#include "Loader.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

SceneGame::SceneGame(glm::mat4 projectionMatrix, Camera * camera): Scene(projectionMatrix, camera)
{
}

SceneGame::~SceneGame()
{
}
void SceneGame::InitScene()
{
	lastFrame = SDL_GetTicks() / 1000.0f;
	Terrain* terrain = new Terrain();
	terrain->loadTerrain("res/models/teren_org/teren.obj", "res/models/teren_org/height.png", Loader::getShader(Loader::LoadedShaders::LIGHT));
	addTerrain(terrain);
	SetLight(new LightObject(Loader::getModel(Loader::LoadedModels::CUBE), glm::vec3(0.0f, 10.0f, 0.0f), 0.0f, glm::vec3(0.2f), &Loader::getShader(Loader::LoadedShaders::SIMPLE), glm::vec3(1.0f, 1.0f, 1.0f)));
	player2 = addObject(new Entity(Loader::getModel(Loader::LoadedModels::PLAYER), glm::vec3(45.0f, 5.0f, 45.0f), 0.0f, glm::vec3(1.0f)));
	player2->setShader(Loader::getShader(Loader::LoadedShaders::LIGHT));
	SDL_SetRelativeMouseMode(SDL_TRUE);

	Bullet::setShader(&Loader::getShader(Loader::LoadedShaders::SIMPLE));
	imgPtr = new Image2D(-0.5f, 0.5f, -0.5f, 0.5f);
	//imgPtr->SetTexture(Loader::getTexture2D(Loader::LoadedTextures2D::GRACZ_WON));
}

void SceneGame::UnInitScene()
{
	delete player2;
	delete imgPtr;
}

void SceneGame::handleEvents(SDL_Event & e)
{
	currentFrame = SDL_GetTicks() / 1000.0f;
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
	while (SDL_PollEvent(&e))
	{
		if (e.type == SDL_QUIT) {
			run = false;; //jakis hack na wyjscie, eventy musi pobierac w whilu zeby kamera byla plynna...
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
			if (e.button.button == SDL_BUTTON_LEFT && currentFrame > ostatniWystrzal) {
				ostatniWystrzal = currentFrame + 0.5f;
				Bullet* tmpBullet = new Bullet(getCamera()->cameraPos, getCamera()->cameraFront);
				//sprawdzenie kolizji
				glm::vec3 pos = getCamera()->cameraPos;
				glm::vec3 player2Pos = player2->getPosition();
				glm::vec3 bulletVec = getCamera()->cameraFront;
				bulletVec.y -= 0.1f;
				//2 gracze na tej samej czesci
					if ((pos.z <= 17.5f && player2Pos.z <= 17.5f) || (pos.z >= 32.5f && player2Pos.z >= 32.5f)) {
						float odleglosc = sqrt(powf(pos.x - player2Pos.x, 2) + powf(pos.y - player2Pos.y, 2) + powf(pos.z - player2Pos.z, 2));
						glm::vec3 test = glm::vec3(pos.x + bulletVec.x * odleglosc, pos.y + odleglosc * bulletVec.y, pos.z + odleglosc * bulletVec.z);
						std::cout << "----------------" << std::endl;
						std::cout << pos.x << " " << pos.y << " " << pos.z << std::endl;
						std::cout << player2Pos.x << " " << player2Pos.y << " " << player2Pos.z << std::endl;
						std::cout << test.x << " " << test.y << " " << test.z << std::endl;
						std::cout << bulletVec.x << " " << bulletVec.y << " " << bulletVec.z << std::endl;
						float deltaY = test.y - player2Pos.y;
						if (abs(test.z - player2Pos.z) <= 0.75f && abs(test.x - player2Pos.x) <= 0.75f) {
							if(test.y >= player2Pos.y &&  test.y - player2Pos.y <= 2.0f)
								std::cout << "trafienie" << std::endl;
							
						}
					}
					//gora/dol czesc blizsza 0
					else if((pos.z <= 32.5f && player2Pos.z <= 17.5f) || (pos.z <= 17.5f && player2Pos.z <= 32.5f)) {
						float odleglosc = sqrt(powf(pos.x - player2Pos.x, 2) + powf(pos.y - player2Pos.y, 2) + powf(pos.z - player2Pos.z, 2));
						glm::vec3 test = glm::vec3(pos.x + bulletVec.x * odleglosc, pos.y + odleglosc * bulletVec.y, pos.z + odleglosc * bulletVec.z);
						if (abs(test.z - player2Pos.z) <= 0.75f && abs(test.x - player2Pos.x) <= 0.75f && test.y >= player2Pos.y &&  test.y - player2Pos.y <= 2.0f) {
							float t = (17.5f - pos.z) / bulletVec.z;
							float y = pos.y + bulletVec.y * t;
							float testPos = std::min(3.0f, player2Pos.y);
							if (y >= testPos)
								std::cout << "trafienie" << std::endl;
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
								std::cout << "trafienie" << std::endl;
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
								if(y2 >= 3.0f)
									std::cout << "trafienie" << std::endl;
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
				
			}
		}
		default: break;
		}
	}
	movePlayer(nextMove, deltaTime);
	glm::vec3 pos = getCamera()->cameraPos;
}

void SceneGame::render()
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
	if (ostatniWystrzal != 0.0) {
		if (currentFrame >= ostatniWystrzal) {
			ostatniWystrzal = 0.0;
		}
		else {
			model = glm::rotate(model, -glm::sin((ostatniWystrzal - currentFrame) / 0.5f) / 3.0f, glm::vec3(0.0f, 0.0f, 1.0f));
		}
	}
	bulletLock.lock();
	for (Bullet* bullet : bulletContainer) {
		bullet->draw(GetProjectionMatrix(), getCamera()->getViewMatrix(), glm::vec3(1.0f, 0.0f, 0.0f));
	}
	bulletLock.unlock();
	model = glm::scale(model, glm::vec3(0.03f, 0.03f, 0.03f));
	Shader& lightShader = Loader::getShader(Loader::LoadedShaders::LIGHT);
	lightShader.use();
	lightShader.setMat4("model", model);
	lightShader.setMat4("projection", GetProjectionMatrix());
	lightShader.setMat4("view", GetViewMatrix());
	lightShader.setVec3("lightColor", GetLight()->GetColor());
	lightShader.setVec3("lightPos", GetLight()->GetPosition());
	lightShader.setVec3("viewPos", getCamera()->cameraPos);
	glClear(GL_DEPTH_BUFFER_BIT);
	Loader::getModel(Loader::LoadedModels::GUN).Draw(lightShader);
	glClear(GL_DEPTH_BUFFER_BIT);
	imgPtr->Draw(Loader::getShader(Loader::LoadedShaders::IMAGE));
}

void SceneGame::deleteBullet()
{
	SDL_Delay(500);
	bulletLock.lock();
	delete bulletContainer.front();
	bulletContainer.pop_front();
	bulletLock.unlock();
}
