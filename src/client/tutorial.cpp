#include <iostream>
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

int initSDL() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
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
	if (initSDL() < 0) return -1;
	SDL_Window* window;
	SDL_GLContext context;
	initOpenGL(window, context);
	//wyswietlane jako siatka glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	Shader ourShader("vertex.vs", "fragment.fs");
	Model ourModel("res/models/headphones_UVW.fbx");
	Model pistolet("res/models/pistolet/pistolet.obj");
	Model kostka("res/models/kostka/kos.obj");

	float deltaTime = 0.0;
	float lastFrame = 0.0;
	SDL_Event windowEvent;
	SDL_SetRelativeMouseMode(SDL_TRUE);
	Camera kamera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.1f, 1.0f);

	Entity cube(kostka, glm::vec3(0.0f, -1.0f, 0.0f), 0.0f, glm::vec3(0.5f, 0.5f, 0.5f));
	//Entity cube2(kostka, glm::vec3(1.5f, -1.0f, 0.0f), 0.0f, glm::vec3(0.5f, 0.5f, 0.5f));
	//Entity cube3(pistolet, glm::vec3(-1.5f, -1.0f, 0.0f), 0.0f, glm::vec3(0.1f, 0.1f, 0.1f));
	cube.setShader(ourShader);
	//cube2.setShader(ourShader);
	//cube3.setShader(ourShader);
	Camera::Movement nextMove;
	
	float ostatniWystrzal = 0.0;
	while (true)
	{
		float currentFrame = SDL_GetTicks() / 1000.0f;
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		float cameraSpeed = 1.0f * deltaTime;
		while (SDL_PollEvent(&windowEvent))
		{
			if (windowEvent.type == SDL_QUIT) {
				break; //jakis hack na wyjscie, eventy musi pobierac w whilu zeby kamera byla plynna...
			}
			switch (windowEvent.type) {
			case SDL_KEYDOWN: {

				switch (windowEvent.key.keysym.sym) {
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
				default: break;
				}
				break;
			}
			case SDL_KEYUP: {

				switch (windowEvent.key.keysym.sym) {
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
				kamera.turnCamera(windowEvent.motion);
			}
			case SDL_MOUSEBUTTONDOWN: {
				if (windowEvent.button.button == SDL_BUTTON_LEFT) {
					ostatniWystrzal = currentFrame + 0.5;
				}
			}
			default: break;
			}
		}

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		kamera.moveCamera(nextMove, deltaTime);

		glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
		glm::mat4 view = kamera.getViewMatrix();
		glm::mat4 model;

		//float rotation = glm::sin(currentFrame) / 10;
		//cube.rotate(rotation);
		cube.Draw(projection, view);
		//cube2.rotate(rotation);
		//cube2.Draw(projection, view);
		//cube3.rotate(rotation);
		//cube3.Draw(projection, view);


		//rysowanie broni
		ourShader.setMat4("view", glm::mat4(1.0f));
		model = glm::mat4(1.0f);
		//std::cout << kamera.cameraPos.x << " " << kamera.cameraPos.y << " " << kamera.cameraPos.z << std::endl;
		model = glm::translate(model, glm::vec3(0.0f, -0.1f, -0.5f));
		model = glm::rotate(model, (float)glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		if (ostatniWystrzal != 0.0) {
			if (currentFrame >= ostatniWystrzal) {
				ostatniWystrzal = 0.0;
			}
			else {
				std::cout << glm::cos((ostatniWystrzal - currentFrame) / 2.0f) << std::endl;
				model = glm::rotate(model, -glm::sin((ostatniWystrzal - currentFrame) / 0.5f) / 3.0f, glm::vec3(0.0f, 0.0f, 1.0f));
			}
		}
		model = glm::scale(model, glm::vec3(0.03f, 0.03f, 0.03f));
		ourShader.setMat4("model", model);
		pistolet.Draw(ourShader);
		//rysowanie broni - koniec
			SDL_GL_SwapWindow(window);
	}

	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit();
	return 0;
}