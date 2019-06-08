#include "SceneMenu.h"
#include "Loader.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "SceneManager.h"
int SceneMenu::currentSelection = 0;
bool SceneMenu::EnterPressed = false;
bool IsClient = false;
SceneMenu::SceneMenu(glm::mat4 projectionMatrix, Camera * camera): Scene(projectionMatrix,camera)
{


}
SceneMenu::~SceneMenu() 
{
	

}

void SceneMenu::InitScene()
{
	lastFrame = SDL_GetTicks() / 1000.0f;

	
	SetLight(new LightObject(Loader::getModel(Loader::LoadedModels::CUBE), glm::vec3(0.0f, 10.0f, 0.0f), 0.0f, glm::vec3(0.2f), &Loader::getShader(Loader::LoadedShaders::SIMPLE), glm::vec3(1.0f, 1.0f, 1.0f)));
	player2 = addObject(new Entity(Loader::getModel(Loader::LoadedModels::PLAYER), glm::vec3(20.0f, 0.0f, 7.0f), 0.0f, glm::vec3(1.0f)));
	player2->setShader(Loader::getShader(Loader::LoadedShaders::LIGHT));
	SDL_SetRelativeMouseMode(SDL_TRUE);

	

	MainSkybox = CSkybox(Loader::getShader(Loader::LoadedShaders::MENUBOX), "res/img/cwd_rt.jpg", 
		"res/img/cwd_lf.jpg", "res/img/cwd_up.jpg", "res/img/cwd_dn.jpg", "res/img/cwd_ft.jpg", "res/img/cwd_bk.jpg");

	

	ResourceManager::LoadShader("res/shaders/sprite.vs", "res/shaders/sprite.fs", "sprite");
	ResourceManager::LoadTexture("res/img/heart.png", GL_FALSE, "background");
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

void SceneMenu::UnInitScene()
{
	delete player2;
	delete Text;
	delete Renderer;
	Sound::Release();
    SoundMgr = NULL;
	SoundManager::Release();
	SoundM = NULL;
	
}

void SceneMenu::handleEvents(SDL_Event & e)
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
			case SDLK_w: {
				
				SoundM->PlaySFX("Button.wav", 0, 1);


				if (!EnterPressed)
					currentSelection = 1;
				else
					currentSelection = 3;
				break;
			}
			case SDLK_s: {
				SoundM->PlaySFX("Button.wav", 0, 1);

				if (!EnterPressed)
					currentSelection = 0;
				else
					currentSelection = 2;
				break;
			}
			case SDLK_a: {
				break;
			}
			case SDLK_d: {
				break;
			}
			case SDLK_DOWN:
				SoundM->PlaySFX("Button.wav", 0, 1);


				if (!EnterPressed)
					currentSelection = 1;
				else
					currentSelection = 3;
				break;

			case SDLK_SPACE: {
				break;
			}
			case SDLK_KP_ENTER: {
				SoundM->PlaySFX("Button.wav", 0, 1);

				if (!currentSelection) {
					EnterPressed = true;
					currentSelection = 2;
					break;
					

				}
				else if ( EnterPressed == true && currentSelection == 2) {
					IsClient = true;
					SoundM->HaltMusic();
					SoundM->HaltSFX(1);
					SceneManager::GetInstance().SelectScene(SceneManager::Scenes::SCENE_LOAD);
					SceneManager::GetInstance().getScene()->render();

					break;
				}
				else if (EnterPressed == true && currentSelection == 3) {
					IsClient = false;
					SoundM->HaltMusic();
					SoundM->HaltSFX(2);

					SceneManager::GetInstance().SelectScene(SceneManager::Scenes::SCENE_LOAD);
					SceneManager::GetInstance().getScene()->render();
					break;


				}
				else {
					SoundM->HaltMusic();
					SoundM->HaltSFX(1);
					SoundM->HaltSFX(2);
					SceneManager::GetInstance().SelectScene(SceneManager::Scenes::SCENE_QUIT);
					SceneManager::GetInstance().getScene()->render();
					break;

				}
				
				
			}
			default: break;
			}
			break;
		}
		case SDL_KEYUP: {


			switch (e.key.keysym.sym) {
			case SDLK_w: {
				SoundM->PlaySFX("Button.wav", 0, 1);

				if (!EnterPressed)
					currentSelection = 1;
				else
					currentSelection = 3;
				break;
			}
			case SDLK_s: {
				SoundM->PlaySFX("Button.wav", 0, 1);

				if (!EnterPressed)
					currentSelection = 0;
				else
					currentSelection = 2;
				break;
			}
			case SDLK_a: {
				SoundM->VolumeMusic(23);
				break;
			}
			case SDLK_d: {
				SoundM->VolumeSFX(1, 128);
				break;
			}
			
			case SDLK_UP:

			SoundM->PlaySFX("Button.wav", 0, 1);


				if (!EnterPressed)
					currentSelection = 0;
				else
					currentSelection = 2;
				break;

			default: break;
			}
			break;
		}
		case SDL_MOUSEMOTION: {
		}
		
		
		default: break;
		}
	}
	movePlayer(nextMove, deltaTime);
}

void SceneMenu::render()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	if (currentSelection == 0 && EnterPressed == false) {
		Text->RenderText("NEW GAME", 575.0f, 125.0f, 1.0, glm::vec3(1, 0, 1));

	}
	else if (currentSelection == 1 && EnterPressed == false) {
		Text->RenderText("EXIT", 615.0f, 290.0f, 1.0f, glm::vec3(1, 0, 1));
	
	}
	else if (currentSelection == 2 && EnterPressed == true) {

		Text->RenderText("CLIENT", 600.0f, 125.0f, 1.0, glm::vec3(1, 0, 1));
	
	}
	else {
	Text->RenderText("SERVER", 590.0f, 290.0f, 1.0f, glm::vec3(1, 0, 1));

	}
	

	Renderer->DrawSprite(ResourceManager::GetTexture("Button1"), glm::vec2(470, 90), glm::vec2(350, 100), 0.0f, glm::vec3(1,1,1));
	
	Renderer->DrawSprite(ResourceManager::GetTexture("Button2"), glm::vec2(470, 250), glm::vec2(350, 100), 0.0f, glm::vec3(1, 1, 1));





	glDisable(GL_BLEND);

	MainSkybox.SetDeltatime(currentFrame);
	MainSkybox.RenderSkybox(Loader::getShader(Loader::LoadedShaders::MENUBOX), this->GetViewMatrix(), this->GetProjectionMatrix());

	DrawObjects();
	
	glClear(GL_DEPTH_BUFFER_BIT);
	
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	Text2->RenderText("SIMPLE SHOOTER", 465.0f, 40.0f, 1.0, glm::vec3(1, 0, 1));

	glDisable(GL_BLEND);


}

