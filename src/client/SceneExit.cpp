#include "SceneExit.h"
#include "Loader.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "SceneManager.h"

SceneExit::SceneExit(glm::mat4 projectionMatrix, Camera * camera) : Scene(projectionMatrix, camera)
{

}
SceneExit::~SceneExit()
{

}

void SceneExit::InitScene()
{
	lastFrame = SDL_GetTicks() / 1000.0f;


	SetLight(new LightObject(Loader::getModel(Loader::LoadedModels::CUBE), glm::vec3(0.0f, 10.0f, 0.0f), 0.0f, glm::vec3(0.2f), &Loader::getShader(Loader::LoadedShaders::SIMPLE), glm::vec3(1.0f, 1.0f, 1.0f)));
	player2 = addObject(new Entity(Loader::getModel(Loader::LoadedModels::PLAYER), glm::vec3(20.0f, 0.0f, 7.0f), 0.0f, glm::vec3(1.0f)));
	player2->setShader(Loader::getShader(Loader::LoadedShaders::LIGHT));
	SDL_SetRelativeMouseMode(SDL_TRUE);



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

}

void SceneExit::UnInitScene()
{
	delete player2;
	delete Text;
	delete Renderer;
}

void SceneExit::handleEvents(SDL_Event & e)
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

				break;
			}
			case SDLK_s: {

				break;
			}
			case SDLK_a: {
				break;
			}
			case SDLK_d: {
				break;
			}
			case SDLK_DOWN:

				break;

			case SDLK_SPACE: {
				break;
			}
			default: break;
			}
			break;
		}
		case SDL_KEYUP: {

			switch (e.key.keysym.sym) {
			case SDLK_w: {

				break;
			}
			case SDLK_s: {

				break;
			}
			case SDLK_a: {
				break;
			}
			case SDLK_d: {
				break;
			}


			case SDLK_UP:
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

void SceneExit::render()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	Text->RenderText("GAME OVER", 575.0f, 125.0f, 1.0, glm::vec3(1, 1, 1));

	Renderer->DrawSprite(ResourceManager::GetTexture("Button3"), glm::vec2(800, 600), glm::vec2(350, 100), 0.0f, glm::vec3(0, 1, 1));


	glDisable(GL_BLEND);

	MainSkybox.SetDeltatime(currentFrame);
	MainSkybox.RenderSkybox(Loader::getShader(Loader::LoadedShaders::MENUBOX), this->GetViewMatrix(), this->GetProjectionMatrix());

	DrawObjects();
	
	glClear(GL_DEPTH_BUFFER_BIT);

}

