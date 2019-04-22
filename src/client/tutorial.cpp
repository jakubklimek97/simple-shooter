#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <GL/glew.h>
#include <SDL_opengl.h>
#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "Entity.h"
#include "Scene.h"
#include "LightObject.h"
#include "BoundingBox.h"
#include"HeightMap.h"
#include"vertexBufferObject.h"
#include"DirLight.h"
#include"TextureClass.h"

CVertexBufferObject vboSceneObjects;
UINT uiVAOSceneObjects;

CMultiLayeredHeightmap hmWorld;

CDirectionalLight dlSun;

Scene testowa2(glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f), new Camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.1f, 1.0f));


void initScene() {

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	
	spMain.UseProgram();

	PrepareShaderPrograms();

	LoadAllTextures();

	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0);

	dlSun = CDirectionalLight(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(sqrt(2.0f) / 2, -sqrt(2.0f) / 2, 0), 0.5f);

	CMultiLayeredHeightmap::LoadTerrainShaderProgram();
	hmWorld.LoadHeightMapFromImage("consider_this_question.png","res/img");

	

}

void renderScene() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	spMain.UseProgram();

	spMain.SetUniform("matrices.projMatrix", testowa2.GetProjectionMatrix());
	spMain.SetUniform("matrices.viewMatrix", testowa2.GetViewMatrix());

	spMain.SetUniform("gSampler", 0);

	spMain.SetUniform("matrices.modelMatrix", glm::mat4(1.0));
	spMain.SetUniform("matrices.normalMatrix", glm::mat4(1.0));
	spMain.SetUniform("vColor", glm::vec4(1, 1, 1, 1));

	static float fAngleOfDarkness = 45.0f;

	dlSun.vDirection = glm::vec3(-sin(fAngleOfDarkness*3.1415f / 180.0f), -cos(fAngleOfDarkness*3.1415f / 180.0f), 0.0f);
	dlSun.SetUniformData(&spMain, "sunLight");

	//// Now we're going to render terrain

	
	hmWorld.SetRenderSize(300.0f, 35.0f, 300.0f);
	CShaderProgram* spTerrain = CMultiLayeredHeightmap::GetShaderProgram();

  
//	spTerrain->UseProgram();


	spTerrain->SetUniform("matrices.projMatrix", testowa2.GetProjectionMatrix());
	spTerrain->SetUniform("matrices.viewMatrix", testowa2.GetViewMatrix());

	// We bind all 5 textures - 3 of them are textures for layers, 1 texture is a "path" texture, and last one is
	// the places in heightmap where path should be and how intense should it be
	FOR(i, 5)
	{
		char sSamplerName[256];
//		sprintf(sSamplerName, "gSampler[%d]", i);
		tTextures[i].BindTexture(i);
		spTerrain->SetUniform(sSamplerName, i);
	}

	// ... set some uniforms
	spTerrain->SetModelAndNormalMatrix("matrices.modelMatrix", "matrices.normalMatrix", glm::mat4(1.0));
	spTerrain->SetUniform("vColor", glm::vec4(1, 1, 1, 1));

	dlSun.SetUniformData(spTerrain, "sunLight");

	// ... and finally render heightmap
	hmWorld.RenderHeightmap();

	//testowa2.getCamera()->moveCamera(nextMove, deltaTime);
	//barkuje update kamera !!!


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



//int main(int argc, char *argv[])
//{
//	if (initSDL() < 0) return -1;
//	SDL_Window* window;
//	SDL_GLContext context;
//	initOpenGL(window, context);
////	//wyswietlane jako siatka glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
/////*
// 	Shader lightShader("LightShader.vs", "LightShader.fs");
//	Shader boundingBoxShader("boundingBox.vs", "boundingBox.fs");
//	Shader simpleShader("simpleColorShader.vs", "simpleColorShader.fs");
//	Model kostka("res/models/kostka/kos.obj");
//	Model pistolet("res/models/pistolet/pistolet.obj");
//
//
//	/*spMain.UseProgram();
//	CShaderProgram* spTerrain = CMultiLayeredHeightmap::GetShaderProgram();
//
//	spTerrain->UseProgram();
//*/
//	initScene();
//
//	float deltaTime = 0.0;
//	float lastFrame = 0.0;
//	SDL_Event windowEvent;
//	SDL_SetRelativeMouseMode(SDL_TRUE);
//
//
//	//Scene testowa(glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f), new Camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.1f, 1.0f));
//	//LightObject* light = testowa.SetLight(new LightObject(kostka, glm::vec3(2.5f, 1.0f, 2.0f), 0.0f, glm::vec3(0.2f), &simpleShader, glm::vec3(1.0f, 1.0f, 1.0f)));
//	//Entity* testCube = testowa.addObject(new Entity(kostka, glm::vec3(5.0f, -1.0f, 0.0f), 0.0f, glm::vec3(0.5f, 0.5f, 0.5f)));
//
//	//Entity* testBoundingBox = testowa.addObject(new Entity(kostka, glm::vec3(4.0f, 0.0f, 0.0f), 0.0f, glm::vec3(0.5f, 0.5f, 0.5f)));
//	//testBoundingBox->setShader(lightShader);
//	//testBoundingBox->rotateY(45.0f);
//	//BoundingBox box(*testBoundingBox);
//	//testowa.removeObject(testCube);
//
//	Camera::Movement nextMove;
//	float ostatniWystrzal = 0.0;
//	while (true)
//	{
//		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//		float currentFrame = SDL_GetTicks() / 1000.0f;
//		deltaTime = currentFrame - lastFrame;
//		lastFrame = currentFrame;
//
//		float cameraSpeed = 1.0f * deltaTime;
//		while (SDL_PollEvent(&windowEvent))
//		{
//			if (windowEvent.type == SDL_QUIT) {
//				break; //jakis hack na wyjscie, eventy musi pobierac w whilu zeby kamera byla plynna...
//			}
//			switch (windowEvent.type) {
//			case SDL_KEYDOWN: {
//
//				switch (windowEvent.key.keysym.sym) {
//				case SDLK_w: {
//					nextMove[Camera::MOVE_FORWARD] = 1;
//					break;
//				}
//				case SDLK_s: {
//					nextMove[Camera::MOVE_BACKWARD] = 1;
//					break;
//				}
//				case SDLK_a: {
//					nextMove[Camera::STRAFE_LEFT] = 1;
//					break;
//				}
//				case SDLK_d: {
//					nextMove[Camera::STRAFE_RIGHT] = 1;;
//					break;
//				}
//				default: break;
//				}
//				break;
//			}
//			case SDL_KEYUP: {
//
//				switch (windowEvent.key.keysym.sym) {
//				case SDLK_w: {
//					nextMove[Camera::MOVE_FORWARD] = 0;
//					break;
//				}
//				case SDLK_s: {
//					nextMove[Camera::MOVE_BACKWARD] = 0;
//					break;
//				}
//				case SDLK_a: {
//					nextMove[Camera::STRAFE_LEFT] = 0;
//					break;
//				}
//				case SDLK_d: {
//					nextMove[Camera::STRAFE_RIGHT] = 0;;
//					break;
//				}
//				default: break;
//				}
//				break;
//			}
//			case SDL_MOUSEMOTION: {
//				
//				//testowa.getCamera()->turnCamera(windowEvent.motion);
//			}
//			case SDL_MOUSEBUTTONDOWN: {
//				if (windowEvent.button.button == SDL_BUTTON_LEFT) {
//					ostatniWystrzal = currentFrame + 0.5;
//				}
//			}
//			default: break;
//			}
//		}
//
//		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
//		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//	//	renderScene();
//		
//		/*testowa.getCamera()->moveCamera(nextMove, deltaTime);
//		testCube->rotateZ(glm::radians(1.0f));
//		testCube->rotateY(glm::radians(1.0f));
//		testBoundingBox->rotateY(glm::radians(2.0f));
//		testowa.DrawObjects();
//		box.calculateBoundingBox();
//		box.Draw(testowa.GetProjectionMatrix(), testowa.GetViewMatrix(), boundingBoxShader);
//
//		glm::mat4 model = glm::mat4(1.0f);
//		model = glm::translate(model, testowa.getCamera()->cameraPos + testowa.getCamera()->cameraFront);
//		model = glm::rotate(model, glm::radians(180.0f - testowa.getCamera()->yaw), glm::vec3(0.0f, 1.0f, 0.0f));
//		model = glm::rotate(model, glm::radians(-testowa.getCamera()->pitch), glm::vec3(0.0f, 0.0f, 1.0f));
//		model = glm::translate(model, glm::vec3(0.35f, -0.15f, 0.0f));
//		std::cout << testowa.getCamera()->yaw << std::endl;
//		if (ostatniWystrzal != 0.0) {
//			if (currentFrame >= ostatniWystrzal) {
//				ostatniWystrzal = 0.0;
//			}
//			else {
//				std::cout << glm::cos((ostatniWystrzal - currentFrame) / 2.0f) << std::endl;
//				model = glm::rotate(model, -glm::sin((ostatniWystrzal - currentFrame) / 0.5f) / 3.0f, glm::vec3(0.0f, 0.0f, 1.0f));
//			}
//		}
//		model = glm::scale(model, glm::vec3(0.03f, 0.03f, 0.03f));
//		lightShader.use();
//		lightShader.setMat4("model", model);
//		lightShader.setMat4("projection", testowa.GetProjectionMatrix());
//		lightShader.setMat4("view", testowa.GetViewMatrix());
//		lightShader.setVec3("lightColor", light->GetColor());
//		lightShader.setVec3("lightPos", light->GetPosition());
//		lightShader.setVec3("viewPos", testowa.getCamera()->cameraPos);
//		pistolet.Draw(lightShader);
//*/
//
//		renderScene();
//		
//		SDL_GL_SwapWindow(window);
//	}
//
//	SDL_GL_DeleteContext(context);
//	SDL_DestroyWindow(window);
//	IMG_Quit();
//	SDL_Quit();
//	return 0;
//}