#include "SceneLoad.h"
#include "Loader.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "SceneManager.h"

SceneLoad::SceneLoad(glm::mat4 projectionMatrix, Camera * camera) : Scene(projectionMatrix, camera)
{

}
SceneLoad::~SceneLoad()
{

}

void SceneLoad::InitScene()
{
	lastFrame = SDL_GetTicks() / 1000.0f;


	SetLight(new LightObject(Loader::getModel(Loader::LoadedModels::CUBE), glm::vec3(0.0f, 10.0f, 0.0f), 0.0f, glm::vec3(0.2f), &Loader::getShader(Loader::LoadedShaders::SIMPLE), glm::vec3(1.0f, 1.0f, 1.0f)));
	player2 = addObject(new Entity(Loader::getModel(Loader::LoadedModels::PLAYER), glm::vec3(20.0f, 0.0f, 7.0f), 0.0f, glm::vec3(1.0f)));
	player2->setShader(Loader::getShader(Loader::LoadedShaders::LIGHT));
	SDL_SetRelativeMouseMode(SDL_TRUE);

	IP.reserve(255);
	

	MainSkybox = CSkybox(Loader::getShader(Loader::LoadedShaders::MENUBOX), "res/img/cwd_rt.jpg",
		"res/img/cwd_lf.jpg", "res/img/cwd_up.jpg", "res/img/cwd_dn.jpg", "res/img/cwd_ft.jpg", "res/img/cwd_bk.jpg");



	ResourceManager::LoadShader("res/shaders/sprite.vs", "res/shaders/sprite.fs", "sprite");
	ResourceManager::LoadTexture("res/img/black_img.png", GL_FALSE, "background");
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(1280), static_cast<GLfloat>(720), 0.0f, -1.0f, 1.0f);
	ResourceManager::GetShader("sprite").Use().SetInteger("sprite", 0);
	ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);

	Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));

	Text = new TextManager(1280, 720);
	Text->Load("abel.ttf", 32);


	Text2 = new TextManager(1280, 720);
	Text2->Load("virgo.ttf", 44);


	SoundMgr = Sound::Instance();
	SoundM = SoundManager::Instance();

	SoundM->PlayMusic("SkyFire_Title_Screen.ogg", -1);
	SoundM->VolumeMusic(15);

}

void SceneLoad::UnInitScene()
{
	delete player2;
	delete Text;
	delete Renderer;
	Sound::Release();
	SoundMgr = NULL;
	SoundManager::Release();
	SoundM = NULL;
}

void SceneLoad::handleEvents(SDL_Event & e)
{
	currentFrame = SDL_GetTicks() / 1000.0f;
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
	while (SDL_PollEvent(&e))
	{
		if (e.type == SDL_QUIT) {
			SceneManager::GetInstance().SelectScene(SceneManager::Scenes::SCENE_QUIT);
			run = false;; //jakis hack na wyjscie, eventy musi pobierac w whilu zeby kamera byla plynna...
		}
		switch (e.type) {
		case SDL_KEYDOWN: {

			switch (e.key.keysym.sym) {
			case SDLK_0: {
				IP.push_back('0');
				SoundM->PlaySFX("Type.ogg", 0);

				
				break;
			}
			case SDLK_1: {
				IP.push_back('1');
				SoundM->PlaySFX("Type.ogg", 0);


				break;
			}
			case SDLK_2: {
				IP.push_back('2');
				SoundM->PlaySFX("Type.ogg", 0);


				break;
			}
			case SDLK_3: {
				IP.push_back('3');
				SoundM->PlaySFX("Type.ogg", 0);


				break;
			}
			case SDLK_4: {
				IP.push_back('4');
				SoundM->PlaySFX("Type.ogg", 0);


				break;
			}
			case SDLK_5: {
				IP.push_back('5');
				SoundM->PlaySFX("Type.ogg", 0);


				break;
			}
			case SDLK_6: {
				IP.push_back('6');
				SoundM->PlaySFX("Type.ogg", 0);


				break;
			}
			case SDLK_7: {
				IP.push_back('7');
				SoundM->PlaySFX("Type.ogg", 0);


				break;
			}
			case SDLK_8: {
				IP.push_back('8');
				SoundM->PlaySFX("Type.ogg", 0);


				break;
			}
			case SDLK_9: {
				IP.push_back('9');
				SoundM->PlaySFX("Type.ogg", 0);


				break;
			}
			case SDLK_PERIOD: {
				IP.push_back('.');
				SoundM->PlaySFX("Type.ogg", 0);


				break;
			}
			case SDLK_s: {
				IP.push_back('s');
				SoundM->PlaySFX("Type.ogg", 0);

				break;
			}

			case SDLK_KP_ENTER: {
		
				SoundM->PlaySFX("Type.ogg", 0);
				SoundM->HaltSFX(0);
				SoundM->HaltMusic();
				// przepisz IP gdzies do serwera???
				SceneManager::GetInstance().SelectScene(SceneManager::Scenes::SCENE_MULTI);
				SceneManager::GetInstance().getScene()->render();

				break;
			}
			case SDLK_BACKSPACE:
			{
				SoundM->HaltSFX(0);
				SoundM->HaltMusic();
				SceneManager::GetInstance().SelectScene(SceneManager::Scenes::SCENE_MENU);
				SceneManager::GetInstance().getScene()->render();
				break;
			}
			case SDLK_a: {
				break;
			}
			case SDLK_d: {
				break;
			}
			case SDLK_DOWN: {

				break;
			}
			case SDLK_SPACE: {
				SelectedMode = false;
				break;
			}
			default: break;
			}
			break;
		}
		case SDL_KEYUP: {

			switch (e.key.keysym.sym) {
			case SDLK_w: {
				//	nextMove[Camera::MOVE_FORWARD] = 0;
				
				break;
			}
			case SDLK_s: {
				//	nextMove[Camera::MOVE_BACKWARD] = 0;
				
				break;
			}
			case SDLK_a: {
				IP.push_back('a');
				SoundM->PlaySFX("Type.ogg", 0);
				break;
			}
			case SDLK_d: {
				IP.push_back('d');
				SoundM->PlaySFX("Type.ogg", 0);
				break;
			}
		

			case SDLK_UP:
				//	color = glm::vec3(1, 1, 1);
				break;

			default: break;
			}
			break;
		}
		case SDL_MOUSEMOTION: {
			//getCamera()->turnCamera(e.motion);
		}


		default: break;
		}
	}
	movePlayer(nextMove, deltaTime);
}

void SceneLoad::render()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	

	MainSkybox.SetDeltatime(currentFrame);
	MainSkybox.RenderSkybox(Loader::getShader(Loader::LoadedShaders::MENUBOX), this->GetViewMatrix(), this->GetProjectionMatrix());

	DrawObjects();
	
	glClear(GL_DEPTH_BUFFER_BIT);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	IP.shrink_to_fit();
	if (IP.size() > 30) // za duzo znakow w okienku
		IP.clear();
	std::string str(IP.begin(), IP.end());


	if (IsClient) {
		

		Text->RenderText("IP Address:", 60.0f, 125.0f, 1.0, glm::vec3(1, 1, 1));
		
		Text->RenderText(str, 200.0f, 125.0f, 1.0, glm::vec3(1, 1, 1));
		
	}
	else if (!IsClient) {
	//	Text->RenderText("SERVER", 600.0f, 125.0f, 1.0, glm::vec3(1, 0, 1));
		Text->RenderText("Is Client:", 500.0f, 125.0f, 1.0, glm::vec3(1, 0, 1));
		if(SelectedMode) // wybrany tryb
		Text->RenderText("Yes", 680.0f, 125.0f, 1.0, glm::vec3(1, 0, 1));
		else
		Text->RenderText("No", 680.0f, 125.0f, 1.0, glm::vec3(1, 0, 1));
	}


	glDisable(GL_BLEND);


	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	Text2->RenderText("SIMPLE SHOOTER", 465.0f, 40.0f, 1.0, glm::vec3(1, 0, 1));

	glDisable(GL_BLEND);

}

