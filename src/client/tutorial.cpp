#include <iostream>
#include <SDL.h>
#include <SDL_image.h>

#include <GL/glew.h>
#include <SDL_opengl.h>
#include <GL/GL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Camera.h"
#include "Model.h"
//void MyOrtho2D(float* mat, float left, float right, float bottom, float top)
//{
//	// this is basically from
//
//	const float zNear = -1.0f;
//	const float zFar = 1.0f;
//	const float inv_z = 1.0f / (zFar - zNear);
//	const float inv_y = 1.0f / (top - bottom);
//	const float inv_x = 1.0f / (right - left);
//
//	//first column
//	*mat++ = (2.0f*inv_x);
//	*mat++ = (0.0f);
//	*mat++ = (0.0f);
//	*mat++ = (0.0f);
//
//	//second
//	*mat++ = (0.0f);
//	*mat++ = (2.0*inv_y);
//	*mat++ = (0.0f);
//	*mat++ = (0.0f);
//
//	//third
//	*mat++ = (0.0f);
//	*mat++ = (0.0f);
//	*mat++ = (-2.0f*inv_z);
//	*mat++ = (0.0f);
//
//	//fourth
//	*mat++ = (-(right + left)*inv_x);
//	*mat++ = (-(top + bottom)*inv_y);
//	*mat++ = (-(zFar + zNear)*inv_z);
//	*mat++ = (1.0f);
//}
//unsigned int HUDid;
//void HUD() {
//	HUDid = Model::TextureFromFile3("heart.png", "res/img",false);
//	//glEnable(GL_TEXTURE_2D);//dodane
//	glBindTexture(GL_TEXTURE_2D, HUDid);
//	glColor3f(0.0, 0.0, 0.0);
//	glBegin(GL_QUADS);
//	glTexCoord2f(0.0, 1.0); glVertex2f(0.05, 0.05);
//	glTexCoord2f(1.0, 1.0); glVertex2f(0.3, 0.05);
//	glTexCoord2f(1.0, 0.0); glVertex2f(0.3, 0.15);
//	glTexCoord2f(0.0, 0.0); glVertex2f(0.05, 0.15);
//	glEnd();
//}
//
//
//
//void drawHUD() {
//
//	glMatrixMode(GL_PROJECTION);
//	glPushMatrix();
//	glLoadIdentity();
//	//float my_proj_matrix[16];
//	//gluOrtho2D(0.0, 1.0, 1.0, 0.0);
//	//MyOrtho2D(my_proj_matrix, 0.0f, 1.0f, 1.0f, 0.0f);
//	glOrtho(0.0, 1.0, 1.0, 0.0, 1.0, 1.0);
//	glMatrixMode(GL_MODELVIEW);
//	glPushMatrix();
//	glLoadIdentity();
//	HUD();
//	glMatrixMode(GL_PROJECTION);
//	glPopMatrix();
//	glMatrixMode(GL_MODELVIEW);
//	glPopMatrix();
//
//
//}


void drawHealth4(float health) {
	const int numDiv = 15;
	const float sep = 0.04;
	const float barHeight = 1.0 / (float)numDiv;
	glBegin(GL_QUADS);
	glColor3f(1, 0, 0);
	for (float i = 0; i < health; i += (sep + barHeight)) {
		glVertex2f(0, i);
		glVertex2f(1, i);
		glVertex2f(1, i + barHeight);
		glVertex2f(0, i + barHeight);
	}
	glEnd();
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
	return 0;
}


int main(int argc, char *argv[])
{



	if (initSDL() < 0) return -1;

	SDL_Window* window;
	SDL_GLContext context;
	//inicjalizacje opengla i glewa, utworzenie okna i jego zawartosci
	{
		//ustawienie wersji opengla
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
		//utworzenie okna
		window = SDL_CreateWindow("Simple Shooter", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);

		//"zawartosc" okna
		context = SDL_GL_CreateContext(window);

		//inicjalizacja glewa
		glewExperimental = GL_TRUE;
		glewInit();
		glViewport(0, 0, 800, 600);

	}
	glEnable(GL_DEPTH_TEST);
	//wyswietlane jako siatka glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	Shader ourShader("vertex.vs", "fragment.fs");
	Model ourModel("res/models/headphones_UVW.fbx");
	Model pistolet("res/models/pistolet/pistolet.obj");

	float deltaTime = 0.0;
	float lastFrame = 0.0;
	//drawHUD();
	SDL_Event windowEvent;
	SDL_SetRelativeMouseMode(SDL_TRUE);
	Camera kamera(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.1f, 1.0f);
	Camera::Movement nextMove;
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
			default: break;
			}
		}

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		kamera.moveCamera(nextMove, deltaTime);
		ourShader.use();
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
		glm::mat4 view = kamera.getViewMatrix();
		//glm::mat4 view = glm::mat4(1.0f);
		ourShader.setMat4("projection", projection);
		ourShader.setMat4("view", view);
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate(model, (float)glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.03f, 0.03f, 0.03f));
		ourShader.setMat4("model", model);
		ourModel.Draw(ourShader);
		// note that we're translating the scene in the reverse direction of where we want to move
		view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.1f, -0.5f));
		ourShader.setMat4("view", view);
		model = glm::mat4(1.0f);
		std::cout << kamera.cameraPos.x << " " << kamera.cameraPos.y << " " << kamera.cameraPos.z << std::endl;
		model = glm::rotate(model, (float)glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.03f, 0.03f, 0.03f));
		ourShader.setMat4("model", model);
		pistolet.Draw(ourShader);
		/*drawHealth4(0.3f);
		glBegin(GL_QUADS);
		glColor3f(1, 0, 0);
		glVertex2f(0, 0);
		glVertex2f(1, 0);
		glVertex2f(1, 0.2);
		glVertex2f(0, 0.2);*/
		//glEnd();
		SDL_GL_SwapWindow(window);
	}
	
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit();
	return 0;
}