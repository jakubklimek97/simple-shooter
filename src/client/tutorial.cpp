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

UINT uiVAOs[1]; // Only one VAO now //TEST


HUD life1, life2, life3;
CVertexBufferObject vboSceneObjects;
CSkybox MainSkybox;
UINT uiVAOSceneObjects;
CMultiLayeredHeightmap hmWorld;

CDirectionalLight dlSun;

Scene testowa(glm::perspective(glm::radians(55.0f), 1280.0f / 720.0f, 0.1f, 100.0f), new Camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.1f, 1.0f));

//#define FOG_EQUATION_EXP		1
//#define FOG_EQUATION_EXP2		2
//
//namespace FogParameters
//{
//	float fDensity = 0.10f;
//	float fStart = 10.0f;
//	float fEnd = 75.0f;
//	glm::vec4 vFogColor = glm::vec4(0.7f, 0.7f, 0.7f, 1.0f);
//	int iFogEquation = FOG_EQUATION_EXP; // 0 = linear, 1 = exp, 2 = exp2
//};

unsigned int quadVAO = 0;
unsigned int quadVBO;
float heightScale = 0.3f;
static int zycie = 500000;
static float i = 0.0f;

float random() {
	i += 0.01f;
	if (i == 1.1f) {
		i = 0.0f;
		return i;
	}
	zycie -= 1;
	if (zycie < -2300)
		zycie = -1;
	return i;
}

vector<float> vertices = {
	// positions          // colors           // texture coords
	-0.7f, -1.0f, 0.0f,    1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
	-0.7f, -0.7f, 0.0f,    0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
	-1.0f,  -0.7f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
	-1.0f,  -1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
};
vector<int> indices = {
	0, 1, 3, // first triangle
	1, 2, 3  // second triangle
};
vector<float> vertices2 = {
	// positions          // colors           // texture coords
	-0.4f, -1.0f, 0.0f,    1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
	-0.4f, -0.7f, 0.0f,    0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
	-0.7f, -0.7f, 0.0f,     0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
	-0.7f, -1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
};
vector<int>indices2 = {
	0, 1, 3, // first triangle
	1, 2, 3  // second triangle
};




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

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0);
	//glPolygonMode

	//TEST
	//unsigned int VBO, VAO, EBO;
	//glGenVertexArrays(1, &VAO);
	//glGenBuffers(1, &VBO);
	//glGenBuffers(1, &EBO);

	//glBindVertexArray(VAO);

	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, vertices.size()*3*sizeof(float), &vertices.front(), GL_STATIC_DRAW);
	////glBufferData()

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*3*sizeof(int), &indices.front(), GL_STATIC_DRAW);

	//// position attribute
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray(0);
	//// color attribute
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);
	//// texture coord attribute
	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	//glEnableVertexAttribArray(2);

	////2 zycie
	//unsigned int VBO2, VAO2, EBO2;
	//glGenVertexArrays(1, &VAO2);
	//glGenBuffers(1, &VBO2);
	//glGenBuffers(1, &EBO2);

	//glBindVertexArray(VAO2);

	//glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	//glBufferData(GL_ARRAY_BUFFER, vertices2.size() * 3 * sizeof(float), &vertices2.front(), GL_STATIC_DRAW);
	////glBufferData()

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices2.size()*3*sizeof(int), &indices2.front(), GL_STATIC_DRAW);


	//// position attribute
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray(0);
	//// color attribute
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);
	//// texture coord attribute
	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	//glEnableVertexAttribArray(2);

	////1 zycie
	//unsigned texture1 = Model::TextureFromFile2("heart.png", "res/img", false);
	//unsigned texture2 = Model::TextureFromFile2("container.jpg", "res/img", false);
	////2  zycie
	//unsigned texture3 = Model::TextureFromFile2("heart.png", "res/img", false);
	//unsigned texture4 = Model::TextureFromFile2("container.jpg", "res/img", false);

	//Shader2d.use(); // don't forget to activate/use the shader before setting uniforms!
 //  // either set it manually like so:

	//GLfloat gamma;
	//gamma = (sin(i) / 2) + 0.5f;

	//glUniform1f(glGetUniformLocation(Shader2d.ID, "gamma"), gamma);
	//Shader2d.setInt("texture2", 1);

	//Shader2d2.use();
	////2 zycie
	//glUniform1f(glGetUniformLocation(Shader2d2.ID, "gamma"), gamma);
	//Shader2d2.setInt("texture2", 1);

	//vboSceneObjects.CreateVBO();
	//glGenVertexArrays(1, uiVAOs); // Create one VAO
	//glBindVertexArray(uiVAOs[0]);

	//vboSceneObjects.BindVBO();
	//// Vertex positions


	//for(int i =0;i< 36;++i)
	//{
	//	vboSceneObjects.AddData(&vCubeVertices[i], sizeof(glm::vec3));
	//	vboSceneObjects.AddData(&vCubeTexCoords[i % 6], sizeof(glm::vec2));
	//	vboSceneObjects.AddData(&vCubeNormals[i / 6], sizeof(glm::vec3));
	//}


	//// Add ground to VBO

	//for(int i =0;i<6;++i)
	//{
	//	vboSceneObjects.AddData(&vGround[i], sizeof(glm::vec3));
	//    vCubeTexCoords[i] *= 100.0f;
	//	vboSceneObjects.AddData(&vCubeTexCoords[i % 6], sizeof(glm::vec2));
	//	glm::vec3 vGroundNormal(0.0f, 1.0f, 0.0f);
	//	vboSceneObjects.AddData(&vGroundNormal, sizeof(glm::vec3));
	//}

	//vboSceneObjects.UploadDataToGPU(GL_STATIC_DRAW);
	//// Vertex positions
	//glEnableVertexAttribArray(0);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3) + sizeof(glm::vec2), 0);
	//// Texture coordinates
	//glEnableVertexAttribArray(1);
	//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3) + sizeof(glm::vec2), (void*)sizeof(glm::vec3));
	//// Normal vectors
	//glEnableVertexAttribArray(2);
	//glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3) + sizeof(glm::vec2), (void*)(sizeof(glm::vec3) + sizeof(glm::vec2)));

std::vector<reference_wrapper<const Shader>>SH = { Shader2d, Shader2d2 };
	PrepareShaderPrograms();

	tTextures[0].LoadTexture2D("sand.jpg", "res/img", true);
	tTextures[0].SetFiltering(TEXTURE_FILTER_MAG_BILINEAR, TEXTURE_FILTER_MIN_BILINEAR_MIPMAP);
	tTextures[1].LoadTexture2D("snow.jpg", "res/img", true);
	tTextures[1].SetFiltering(TEXTURE_FILTER_MAG_BILINEAR, TEXTURE_FILTER_MIN_BILINEAR_MIPMAP);
	
	
	life1 = HUD(SH, "heart.png", "container.jpg", 0.01f, 1);
	life2 = HUD(SH, "heart.png", "container.jpg", 0.01f, 1);
	life3 = HUD(SH, "heart.png", "container.jpg", 0.01f, 1);

	MainSkybox = CSkybox(skyboxShader,"res/img/right.jpg", "res/img/left.jpg", "res/img/top.jpg", "res/img/bottom.jpg", "res/img/front.jpg", "res/img/back.jpg");

    LoadAllTextures();



	CMultiLayeredHeightmap::LoadTerrainShaderProgram();
	hmWorld.LoadHeightMapFromImage("hm.png", "res/img");


	dlSun = CDirectionalLight(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(sqrt(2.0f) / 2, -sqrt(2.0f) / 2, 0), 0.5f);
//	slFlashLight = CSpotLight(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 1, 15.0f, 0.017f);
	//DZIALA



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

		glClearColor(1.0f, 0.0f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		testowa.getCamera()->moveCamera(nextMove, deltaTime);

		glEnable(GL_TEXTURE_2D);
		spFogAndLight.UseProgram();

		

		//spFogAndLight.SetUniform("sunLight.vColor", glm::vec3(1.0f, 1.0f, 1.0f));
		//spFogAndLight.SetUniform("sunLight.fAmbientIntensity", 1.0f); // Full light for skybox
		//spFogAndLight.SetUniform("sunLight.vDirection", glm::vec3(0, -1, 0));

		//spFogAndLight.SetUniform("matrices.projectionMatrix", testowa.GetProjectionMatrix());
		//spFogAndLight.SetUniform("gSampler", 0);

		//glm::mat4 mModelView = testowa.GetViewMatrix();
		//glm::mat4 mModelToCamera;

		//spFogAndLight.SetUniform("fogParams.iEquation", FogParameters::iFogEquation);
		//spFogAndLight.SetUniform("fogParams.vFogColor", FogParameters::vFogColor);

		//spFogAndLight.SetUniform("fogParams.fDensity", FogParameters::fDensity);
		//spFogAndLight.SetUniform("matrices.modelViewMatrix", glm::translate(mModelView, testowa.getCamera()->cameraFront));

		//glBindVertexArray(uiVAOs[0]);
		//spFogAndLight.SetUniform("sunLight.fAmbientIntensity", 0.55f);
		//spFogAndLight.SetUniform("matrices.modelViewMatrix", &mModelView);
		//	mModelToCamera = glm::translate(glm::mat4(1.0), glm::vec3(1.0f,1.0f,1.0f));
		//	mModelToCamera = glm::scale(mModelToCamera, glm::vec3(15.0f, 15.0f, 15.0f));
		//	spFogAndLight.SetUniform("matrices.normalMatrix", glm::transpose(glm::inverse(mModelToCamera)));
		//	spFogAndLight.SetUniform("matrices.modelViewMatrix", mModelView*mModelToCamera);

		//tTextures[0].BindTexture();
		//glDrawArrays(GL_TRIANGLES, 36, 6);


		//tTextures[1].BindTexture();

		//float dx[] = { 1.0f, 0.0f, -1.0f, 0.0f };
		//float dz[] = { 0.0f, -1.0f, 0.0f, 1.0f };
		//int iCurDir = 0;
		//glm::vec3 vBoxPos(0.0f, 7.48f, 0.0f);
		//const int iSpiralLevels = 12;
		//for (int i = 1; i <= iSpiralLevels; ++i)
		//{
		//	for (int j = 0; j < i; ++j)
		//	{
		//		mModelToCamera = glm::translate(glm::mat4(1.0), vBoxPos);
		//		mModelToCamera = glm::scale(mModelToCamera, glm::vec3(15.0f, 15.0f, 15.0f));
		//		// We need to transform normals properly, it's done by transpose of inverse matrix of rotations and scales
		//		spFogAndLight.SetUniform("matrices.normalMatrix", glm::transpose(glm::inverse(mModelToCamera)));
		//		spFogAndLight.SetUniform("matrices.modelViewMatrix", mModelView*mModelToCamera);
		//		glDrawArrays(GL_TRIANGLES, 0, 36);

		//		vBoxPos += glm::vec3(15.02f*dx[iCurDir], 0.0f, 15.02f*dz[iCurDir]);
		//	}
		//	iCurDir = (iCurDir + 1) % 4;
		//}




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

		static float fAngleOfDarkness = 20.0f;

		dlSun.vDirection = glm::vec3(-sin(fAngleOfDarkness*3.1415f / 180.0f), -cos(fAngleOfDarkness*3.1415f / 180.0f), 0.0f);
		dlSun.SetUniformData(&spMain, "sunLight");


		hmWorld.SetRenderSize(10.0f, 10.0f, 10.0f);
		CShaderProgram* spTerrain = CMultiLayeredHeightmap::GetShaderProgram();
        spTerrain->UseProgram();


		spTerrain->SetUniform("matrices.projMatrix", testowa.GetProjectionMatrix());
		spTerrain->SetUniform("matrices.viewMatrix", testowa.GetViewMatrix());
		for(int i =0;i<3;++i)
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
		hmWorld.RenderHeightmap();

		life1.Incrementvalue();
		life1.RotationIntensity();
		life1.RenderHUD(SH);
		life2.RenderHUD(SH);
		life3.RenderHUD(SH);
		
			
		MainSkybox.RenderSkybox(skyboxShader, testowa.GetViewMatrix(), testowa.GetProjectionMatrix());
		SDL_GL_SwapWindow(window);
	}

	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit();
	return 0;
}