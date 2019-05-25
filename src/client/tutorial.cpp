#include <iostream>
#include<sstream>
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
#include "Scene.h"
#include "LightObject.h"
#include "BoundingBox.h"
#include"HeightMap.h"
#include"vertexBufferObject.h"
#include"DirLight.h"
#include"TextureClass.h"
#include"Flashlight.h"
#include"Skybox.h"
#include"Geometry.h"
#include"HUD.h"
#include"Fog.h"
#include"TextManager.h"
#include"SpriteRenderer.h"
#include"ResourceManager.h"

UINT uiVAOs[1]; // Only one VAO now //TEST


static int counter = 1;
vector<glm::vec3>Pos; //Fog


#define FOG_EQUATION_EXP		1
#define FOG_EQUATION_EXP2		2

namespace FogParameters
{
	float fDensity = 0.05f;
	float fStart = 10.0f;
	float fEnd = 75.0f;
	glm::vec4 vFogColor = glm::vec4(0.7f, 0.7f, 0.7f, 1.0f);
	int iFogEquation = FOG_EQUATION_EXP; // 0 = linear, 1 = exp, 2 = exp2
};

unsigned int quadVAO = 0;
unsigned int quadVBO;
float heightScale = 0.3f;
static int zycie = 500000;
static float i = 0.0f;




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
	pWindow = SDL_CreateWindow("Simple Shooter", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_OPENGL);

//	"zawartosc" okna
	context = SDL_GL_CreateContext(pWindow);

//	inicjalizacja glewa
	glewExperimental = GL_TRUE;
	glewInit();
	glViewport(0, 0, 1280, 720);
	glEnable(GL_DEPTH_TEST);
}



int main(int argc, char *argv[])
{
	TextManager* Text;
	SpriteRenderer *Renderer;

	HUD life1, life2, life3;
	HeightMapBuffer vboSceneObjects;
	CSkybox MainSkybox;
	HeightMap hmWorld;

	CDirectionalLight dlSun;
	Fog FOG;

	if (initSDL() < 0) return -1;
	SDL_Window* window;
	SDL_GLContext context;
	initOpenGL(window, context);
//	wyswietlane jako siatka glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

 	Shader lightShader("LightShader.vs", "LightShader.fs");
	Shader boundingBoxShader("boundingBox.vs", "boundingBox.fs");
	Shader simpleShader("simpleColorShader.vs", "simpleColorShader.fs");
	Model kostka("res/models/kostka/kos.obj");
	Model pistolet("res/models/pistolet/pistolet.obj");
	Shader skyboxShader("skybox.vs", "skybox.fs");
	Shader Shader2d("texture.vs", "texture.fs");
	Shader Shader2d2("texture.vs", "texture.fs");
	float deltaTime = 0.0;
	float lastFrame = 0.0;
	SDL_Event windowEvent;
	SDL_SetRelativeMouseMode(SDL_TRUE);

	glClearColor(0.0, 0.0, 0.0, 0.0);


	
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	vboSceneObjects.CreateVBO();
	glGenVertexArrays(1, uiVAOs); // Create one VAO
	glBindVertexArray(uiVAOs[0]);

	vboSceneObjects.BindVBO();
	// Vertex positions


	for(int i =0;i< 36;++i)
	{
		vboSceneObjects.AddData(&vCubeVertices[i], sizeof(glm::vec3));
		vboSceneObjects.AddData(&vCubeTexCoords[i % 6], sizeof(glm::vec2));
		vboSceneObjects.AddData(&vCubeNormals[i / 6], sizeof(glm::vec3));
	}


	

	vboSceneObjects.UploadDataToGPU(GL_STATIC_DRAW);
	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3) + sizeof(glm::vec2), 0);
	// Texture coordinates
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3) + sizeof(glm::vec2), (void*)sizeof(glm::vec3));
	// Normal vectors
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3) + sizeof(glm::vec2), (void*)(sizeof(glm::vec3) + sizeof(glm::vec2)));

std::vector<reference_wrapper<const Shader>>SH = { Shader2d, Shader2d2 };
	PrepareShaderPrograms();

	
	life1 = HUD(SH, "heart.png", "sand.jpg", 0.01f, 1);
	life2 = HUD(SH, "heart.png", "sand.jpg", 0.01f, 1);
	life3 = HUD(SH, "heart.png", "sand.jpg", 0.01f, 1);

	MainSkybox = CSkybox(skyboxShader,"res/img/right.jpg", "res/img/left.jpg", "res/img/top.jpg", "res/img/bottom.jpg", "res/img/front.jpg", "res/img/back.jpg");
    LoadAllTextures();


	ResourceManager::LoadShader("res/shaders/sprite.vs", "res/shaders/sprite.fs", "sprite");
	ResourceManager::LoadTexture("res/img/black_img.png", GL_FALSE, "background");
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(1280), static_cast<GLfloat>(720), 0.0f, -1.0f, 1.0f);
	ResourceManager::GetShader("sprite").Use().SetInteger("sprite", 0);
	ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);

	Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));








    Text = new TextManager(1280, 720);
	Text->Load("abel.ttf",32);
	  

	HeightMap::LoadTerrainShaderProgram();
	hmWorld.LoadMapFromImage("hm.png", "res/img");


	dlSun = CDirectionalLight(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(sqrt(2.0f) / 2, -sqrt(2.0f) / 2, 0), 0.5f);
	//DZIALA

//	FOG = Fog(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f), 0.10f, 1);
	Scene testowa(glm::perspective(glm::radians(55.0f), 1280.0f / 720.0f, 0.1f, 50000.0f), new Camera(glm::vec3(200.0f, 30.0f, 200.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.1f, 1.0f));

	LightObject* light = testowa.SetLight(new LightObject(kostka, glm::vec3(2.5f, 1.0f, 2.0f), 0.0f, glm::vec3(0.2f), &simpleShader, glm::vec3(1.0f, 1.0f, 1.0f)));
	Entity* testCube = testowa.addObject(new Entity(kostka, glm::vec3(5.0f, -1.0f, 0.0f), 0.0f, glm::vec3(0.5f, 0.5f, 0.5f)));
	Entity* testBoundingBox = testowa.addObject(new Entity(kostka, glm::vec3(4.0f, 0.0f, 0.0f), 0.0f, glm::vec3(0.5f, 0.5f, 0.5f)));
	testBoundingBox->setShader(lightShader);
	testBoundingBox->rotateY(45.0f);
	BoundingBox box(*testBoundingBox);
	testowa.removeObject(testCube);
	//DZIALA
	Camera::Movement nextMove;
	float ostatniWystrzal = 0.0;
	while (true)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
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
					nextMove[Camera::STRAFE_RIGHT] = 0;
					break;
				}
				default: break;
				}
				break;
			}
			case SDL_MOUSEMOTION: {
				
				testowa.getCamera()->turnCamera(windowEvent.motion);
			}
			case SDL_MOUSEBUTTONDOWN: {
				if (windowEvent.button.button == SDL_BUTTON_LEFT) {
					ostatniWystrzal = currentFrame + 0.5f;
				}
			}
			default: break;
			}
		}

	//	glClearColor(1.0f, 0.0f, 0.6f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);

		testowa.getCamera()->moveCamera(nextMove, deltaTime);

		Text->RenderText("Player1", 5.0f, 5.0f, 1.0f);
		Text->RenderText("Player2", 250.0f,800.0f/2,1.0f);
		Text->RenderText("Player3", 320.0f,1000.0f/2-20.0f,1.0f,glm::vec3(0.0,1.0f,0.0f));
		Renderer->DrawSprite(ResourceManager::GetTexture("background"), glm::vec2(1, 0), glm::vec2(1200, 800), 1.0f);



		MainSkybox.SetDeltatime(currentFrame);
		MainSkybox.RenderSkybox(skyboxShader, testowa.GetViewMatrix(), testowa.GetProjectionMatrix());


		cout << "POZYCJA GRACZA X (WZDLUZ):" << testowa.getCamera()->cameraPos.x;
		cout << "POZYCJA GRACZA Z (WSZERZ):" << testowa.getCamera()->cameraPos.z;
		cout << "POZYCJA GRACZA Y (WYSOKOSC):" << testowa.getCamera()->cameraPos.y;
		testCube->rotateZ(glm::radians(1.0f));
		testCube->rotateY(glm::radians(1.0f));
		testBoundingBox->rotateY(glm::radians(2.0f));
		testowa.DrawObjects();
		box.calculateBoundingBox();
		box.Draw(testowa.GetProjectionMatrix(), testowa.GetViewMatrix(), boundingBoxShader);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, testowa.getCamera()->cameraPos + testowa.getCamera()->cameraFront);
		model = glm::rotate(model, glm::radians(180.0f - testowa.getCamera()->yaw), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-testowa.getCamera()->pitch), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(0.35f, -0.15f, 0.0f));
		std::cout << testowa.getCamera()->yaw << std::endl;
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
		lightShader.use();
		lightShader.setMat4("model", model);
		lightShader.setMat4("projection", testowa.GetProjectionMatrix());
		lightShader.setMat4("view", testowa.GetViewMatrix());
		lightShader.setVec3("lightColor", light->GetColor());
		lightShader.setVec3("lightPos", light->GetPosition());
		lightShader.setVec3("viewPos", testowa.getCamera()->cameraPos);
		pistolet.Draw(lightShader);

		spMain.UseProgram();
	
		
		
		

		spMain.SetUniform("matrices.projMatrix", testowa.GetProjectionMatrix());
		spMain.SetUniform("matrices.viewMatrix", testowa.GetViewMatrix());

		spMain.SetUniform("gSampler", 0);

		spMain.SetUniform("matrices.modelMatrix", glm::mat4(1.0));
		spMain.SetUniform("matrices.normalMatrix", glm::mat4(1.0));
		spMain.SetUniform("vColor", glm::vec4(1,1, 1, 1));

		static float fAngleOfDarkness = 100.0f;

		dlSun.vDirection = glm::vec3(-sin(fAngleOfDarkness*3.1415f / 180.0f), -cos(fAngleOfDarkness*3.1415f / 180.0f), 0.0f);
		dlSun.SetUniformData(&spMain, "sunLight");


		hmWorld.SetRenderSize(1.0f, 1.0f, 1.0f);
		ShaderProgram* spTerrain = HeightMap::GetShaderProgram();
        spTerrain->UseProgram();


		spTerrain->SetUniform("matrices.projMatrix", testowa.GetProjectionMatrix());
		spTerrain->SetUniform("matrices.viewMatrix", testowa.GetViewMatrix());
		for(int i =0;i<NUMTEXTURES;++i)
		{
			stringstream Sampler;
			Sampler << "gSampler[" << i << "]";
			tTextures[i].BindTexture(i);
			spTerrain->SetUniform(Sampler.str(), i);
		}

		// ... set some uniforms
		spTerrain->SetModelAndNormalMatrix("matrices.modelMatrix", "matrices.normalMatrix", glm::mat4(1.0));
		spTerrain->SetUniform("vColor", glm::vec4(1,1 , 1, 1));

		dlSun.SetUniformData(spTerrain, "sunLight");
	
		////	 ... and finally render heightmap
		hmWorld.RenderHeightmap(testowa.GetProjectionMatrix());
		
		
		float terrainheight = hmWorld.CheckCollision(testowa.getCamera()->cameraPos.x, testowa.getCamera()->cameraPos.z);
		float camera = testowa.getCamera()->cameraPos.y;
		if (camera <= terrainheight||camera <= terrainheight+0.31f||camera <= terrainheight-0.33f) { //200 ->Terrain height
			testowa.getCamera()->cameraPos.y = terrainheight+1.95f;
		}
		else if(terrainheight - testowa.getCamera()->cameraPos.y<0.3f || terrainheight - testowa.getCamera()->cameraPos.y<0.3f|| terrainheight - testowa.getCamera()->cameraPos.y>0.3f|| terrainheight - testowa.getCamera()->cameraPos.y>0.2f)
		 testowa.getCamera()->cameraPos.y = terrainheight + 1.65f;
	 float heightvalue = 0.6f;
		if(camera <= heightvalue)
			testowa.getCamera()->cameraPos.y = testowa.getCamera()->cameraPos.y + 0.25f;

		
		spFogAndLight.UseProgram();


		spFogAndLight.SetUniform("sunLight.vColor", glm::vec3(1.0f, 1.0f, 1.0f));

	
			
		spFogAndLight.SetUniform("sunLight.fAmbientIntensity", 1.0f); // Full light for skybox
		spFogAndLight.SetUniform("sunLight.vDirection", glm::vec3(0, -1, 0));

		spFogAndLight.SetUniform("matrices.projectionMatrix", testowa.GetProjectionMatrix());
		spFogAndLight.SetUniform("gSampler", 0);

		glm::mat4 mModelView = testowa.GetViewMatrix();
		glm::mat4 mModelToCamera;

		spFogAndLight.SetUniform("fogParams.iEquation", FogParameters::iFogEquation);
		spFogAndLight.SetUniform("fogParams.vFogColor", FogParameters::vFogColor);

		spFogAndLight.SetUniform("fogParams.fDensity", FogParameters::fDensity);
		spFogAndLight.SetUniform("matrices.modelViewMatrix", glm::translate(mModelView, testowa.getCamera()->cameraPos));

		glBindVertexArray(uiVAOs[0]);
		spFogAndLight.SetUniform("sunLight.fAmbientIntensity", 0.55f);
		spFogAndLight.SetUniform("matrices.modelViewMatrix", &mModelView);

		//tTextures[1].BindTexture(0);
		float dx[] = { 4.0f, 0.0f, -4.0f, 0.0f };
		float dz[] = { 0.0f, -4.0f, 0.0f, 4.0f };
		int iCurDir = 0;
		glm::vec3 vBoxPos(100.0f, 38.48f, 400.0f);
		const int iSpiralLevels = 4;
		static int counter = 0;
		for (int i = 1; i <= iSpiralLevels; ++i)
		{
			for (int j = 0; j < 6; ++j)
			{
				mModelToCamera = glm::translate(glm::mat4(1.0), vBoxPos);
				mModelToCamera = glm::scale(mModelToCamera, glm::vec3(60.0f, 80.0f, 60.0f));
				// We need to transform normals properly, it's done by transpose of inverse matrix of rotations and scales
				spFogAndLight.SetUniform("matrices.normalMatrix", glm::transpose(glm::inverse(mModelToCamera)));
				spFogAndLight.SetUniform("matrices.modelViewMatrix", mModelView*mModelToCamera);
				glDrawArrays(GL_TRIANGLES, 0, 24);
				Pos.push_back(vBoxPos);
				vBoxPos += glm::vec3(15.01f*dx[iCurDir], 0.0f, 15.01f*dz[iCurDir]);
				
				counter++;
			}
			if (counter >= 6) {
				iCurDir = 1+iCurDir;
				if (iCurDir == 3)
					continue;

				counter = 0;
			}
			else
				continue;
		}
		//KOLIZJA MGLY
	//	tTextures[1].BindTexture();
	
		//spFogAndLight.SetUniform("vColor", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		float posx = testowa.getCamera()->cameraPos.x;
		float posz = testowa.getCamera()->cameraPos.z;
		if (posx > 450.0f || posx < 100.0f) {


			cout << "KOLIZJA";
			testowa.getCamera()->cameraPos.x = 200;
		}
		else if (posz > 400.0f || posz < 65.0f) {
			cout << "KOLZIJA";
			testowa.getCamera()->cameraPos.z = 200;

		}


		life1.Incrementvalue();
		life1.RotationIntensity();
		life1.RenderHUD(SH);
		life2.RenderHUD(SH);
		life3.RenderHUD(SH);


		SDL_GL_SwapWindow(window);
	}

	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit();
	return 0;
}