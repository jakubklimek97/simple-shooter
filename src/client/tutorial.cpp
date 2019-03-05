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
#include "Terrain.h"


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
		//Sprawdzanie wersji OpenGl
		std::string versionGL = (const char*)glGetString(GL_VERSION);
		std::cout << "OpenGL: " << versionGL << std::endl;

		float version = atof(versionGL.c_str());
		if (version < 3.1f) {
			std::cout << "Outdated version of OpenGL" << std::endl;
			exit(1);
		}

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

	float rot = 0;
	bool wireframe = false;
	
	GLuint vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	loadHeightData();
	genTerrain();

	float deltaTime = 0.0;
	float lastFrame = 0.0;
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
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, terrainVbo);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ARRAY_BUFFER, terrainVboNorm);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, terrainEbo);

		if (!wireframe)
		{
			glDrawElements(GL_TRIANGLES, terrainCount * 3, GL_UNSIGNED_INT, 0);

		}
		else {
			for (int i = 0; i < terrainCount; i++) {
				glDrawElements(GL_LINE_LOOP, 3, GL_UNSIGNED_INT, (GLvoid*)(sizeof(uint3)*i));
			}
		}
		rot += 0.01f; //skad rot

		SDL_GL_SwapWindow(mainWindow); // skad


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
		SDL_GL_SwapWindow(window);
	}
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &terrainVbo);
	glDeleteBuffers(1, &terrainEbo);
	glDeleteProgram(shaderProgram);
	if (heightData)
		SDL_freeSurface(heightData);
	if (normData)
		SDL_FreeSurface(normData);
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit();
	return 0;
}