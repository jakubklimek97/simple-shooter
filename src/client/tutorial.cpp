#include <iostream>
//#include <SDL.h>
//#include <SDL_image.h>
//
//#include <GL/glew.h>
//#include <SDL_opengl.h>
//
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "Entity.h"
#include "Scene.h"
#include "LightObject.h"
#include "BoundingBox.h"
#include"CommonHeader.h"
#include"HeightMap.h"
#include"texture.h"
#include"DirLight.h"
#include"shaders.h"



CVertexBufferObject vboSceneObjects;
UINT uiVAOSceneObjects;



CDirectionalLight dlSun;


CMultiLayeredHeightmap hmWorld;




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
	//GL_CHECK(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3) + sizeof(glm::vec2), (void*)sizeof(glm::vec3)))
	//utworzenie okna
	pWindow = SDL_CreateWindow("Simple Shooter", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	//GL_CHECK(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3) + sizeof(glm::vec2), (void*)sizeof(glm::vec3)))
	//"zawartosc" okna
	context = SDL_GL_CreateContext(pWindow);
	//GL_CHECK(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3) + sizeof(glm::vec2), (void*)sizeof(glm::vec3)))
	//inicjalizacja glewa
	glewExperimental = GL_TRUE;
	glewInit();
	glViewport(0, 0, 800, 600);
	glEnable(GL_DEPTH_TEST);
//	GL_CHECK(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3) + sizeof(glm::vec2), (void*)sizeof(glm::vec3)));
}


//float fRotationAngleCube = 0.0f, fRotationAnglePyramid = 0.0f;
//float fCubeRotationSpeed = 0.0f, fPyramidRotationSpeed = 0.0f;
//const float PIover180 = 3.1415f / 180.0f;
//
//void displayTextureFiltersInfo()
//{
//	char buf[255];
//	string sInfoMinification[] =
//	{
//		"Nearest",
//		"Bilinear"
//	};
//	string sInfoMagnification[] =
//	{
//		"Nearest",
//		"Bilinear",
//		"Nearest on closest mipmap",
//		"Bilinear on closest mipmap",
//		"Trilinear"
//	};
//	sprintf(buf, "Mag. Filter: %s, Min. Filter: %s", sInfoMinification[tSnow.GetMagnificationFilter()].c_str(),
//		sInfoMagnification[tSnow.GetMinificationFilter() - 2].c_str());
//	//SetWindowText(appMain.hWnd, buf);
//}
//
//
//void init() {
//	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
//    vboSceneObjects.CreateVBO();
//	glGenVertexArrays(1, &uiVAO); // Create one VAO
//	glBindVertexArray(uiVAO);
//
//	vboSceneObjects.BindVBO();
//
//	// Add cube to VBO
//
//	FOR(i, 36)
//	{
//		vboSceneObjects.AddData(&vCubeVertices[i], sizeof(glm::vec3));
//		vboSceneObjects.AddData(&vCubeTexCoords[i % 6], sizeof(glm::vec2));
//	}
//
//	// Add pyramid to VBO
//
//	FOR(i, 12)
//	{
//		vboSceneObjects.AddData(&vPyramidVertices[i], sizeof(glm::vec3));
//		vboSceneObjects.AddData(&vPyramidTexCoords[i % 3], sizeof(glm::vec2));
//	}
//
//	// Add ground to VBO
//
//	FOR(i, 6)
//	{
//		vboSceneObjects.AddData(&vGround[i], sizeof(glm::vec3));
//		vCubeTexCoords[i] *= 5.0f;
//		vboSceneObjects.AddData(&vCubeTexCoords[i % 6], sizeof(glm::vec2));
//	}
//
//	vboSceneObjects.UploadDataToGPU(GL_STATIC_DRAW);
//
//	// Vertex positions start on zero index, and distance between two consecutive is sizeof whole
//	// vertex data (position and tex. coord)
//	glEnableVertexAttribArray(0);
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3) + sizeof(glm::vec2), 0);
//	// Texture coordinates start right after positon, thus on (sizeof(glm::vec3)) index,
//	// and distance between two consecutive is sizeof whole vertex data (position and tex. coord)
//	glEnableVertexAttribArray(1);
//	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec3) + sizeof(glm::vec2), (void*)sizeof(glm::vec3));
//
//	// Load shaders and create shader program
//
//	shVertex.LoadShader("res\\shaders\\shader.vs", GL_VERTEX_SHADER);
//	shFragment.LoadShader("res\\shaders\\shader.fs", GL_FRAGMENT_SHADER);
//
//	spMain.CreateProgram();
//	spMain.AddShaderToProgram(&shVertex);
//	spMain.AddShaderToProgram(&shFragment);
//
//	spMain.LinkProgram();
//	spMain.UseProgram();
//
//	glEnable(GL_DEPTH_TEST);
//	glClearDepth(1.0);
//
//	// Finally, load our texture
//
//	tGold.LoadTexture2D("golddiag.jpg","res/img", true);
//	tGold.SetFiltering(TEXTURE_FILTER_MAG_BILINEAR, TEXTURE_FILTER_MIN_BILINEAR_MIPMAP);
//	//sTextureNames[i], "res/img", true
//	tSnow.LoadTexture2D("snow.jpg","res/img",true);
//	tSnow.SetFiltering(TEXTURE_FILTER_MAG_BILINEAR, TEXTURE_FILTER_MIN_BILINEAR_MIPMAP);
//	glEnable(GL_TEXTURE_2D);
//}
//
//void render() {
//	//int iModelViewLoc = glGetUniformLocation(spMain.GetProgramID(), "modelViewMatrix");
//	
//
//
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//	int iModelViewLoc = glGetUniformLocation(spMain.GetProgramID(), "modelViewMatrix");
//	int iProjectionLoc = glGetUniformLocation(spMain.GetProgramID(), "projectionMatrix");
////	glUniformMatrix4fv(iProjectionLoc, 1, GL_FALSE, glm::value_ptr(*oglControl->getProjectionMatrix()));
//
//	glm::mat4 mModelView = glm::lookAt(glm::vec3(0, 12, 27), glm::vec3(0, 0, 0), glm::vec3(0.0f, 1.0f, 0.0f));
//	glm::mat4 mCurrent;
//
//	glBindVertexArray(uiVAO);
//
//	// Texture binding - we set GL_ACTIVE_TEXTURE0, and then we tell fragment shader,
//	// that gSampler variable will fetch data from GL_ACTIVE_TEXTURE0
//
//	int iSamplerLoc = glGetUniformLocation(spMain.GetProgramID(), "gSampler");
//	glUniform1i(iSamplerLoc, 0);
//
//	tGold.BindTexture(0);
//
//	// Rendering of cube
//
//	mCurrent = glm::translate(mModelView, glm::vec3(-8.0f, 0.0f, 0.0f));
//	mCurrent = glm::scale(mCurrent, glm::vec3(10.0f, 10.0f, 10.0f));
//	mCurrent = glm::rotate(mCurrent, fRotationAngleCube, glm::vec3(1.0f, 0.0f, 0.0f));
//	glUniformMatrix4fv(iModelViewLoc, 1, GL_FALSE, glm::value_ptr(mCurrent));
//
//	glDrawArrays(GL_TRIANGLES, 0, 36);
//
//	// Rendering of pyramid
//
//	mCurrent = glm::translate(mModelView, glm::vec3(8.0f, 0.0f, 0.0f));
//	mCurrent = glm::scale(mCurrent, glm::vec3(10.0f, 10.0f, 10.0f));
//	mCurrent = glm::rotate(mCurrent, fRotationAnglePyramid, glm::vec3(0.0f, 1.0f, 0.0f));
//	glUniformMatrix4fv(iModelViewLoc, 1, GL_FALSE, glm::value_ptr(mCurrent));
//
//	glDrawArrays(GL_TRIANGLES, 36, 12);
//
//
//	tSnow.BindTexture();
//	 mModelView = glm::lookAt(glm::vec3(0, 12, 27), glm::vec3(0, 0, 0), glm::vec3(0.0f, 1.0f, 0.0f));
//	glUniformMatrix4fv(iModelViewLoc, 1, GL_FALSE, glm::value_ptr(mModelView));
//	glDrawArrays(GL_TRIANGLES, 48, 6);
//}
void intialize() {
	//GL_CHECK(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3) + sizeof(glm::vec2), (void*)sizeof(glm::vec3)))

//	GL_CHECK(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3) + sizeof(glm::vec2), (void*)sizeof(glm::vec3)))
	PrepareShaderPrograms();

	LoadAllTextures();

	//glEnable(GL_DEPTH_TEST);
	//glClearDepth(1.0);

	dlSun = CDirectionalLight(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(sqrt(2.0f) / 2, -sqrt(2.0f) / 2, 0), 0.5f);
	
	CMultiLayeredHeightmap::LoadTerrainShaderProgram();
	hmWorld.LoadHeightMapFromImage("consider_this_question.bmp","res/img");
	//GL_CHECK(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3) + sizeof(glm::vec2), (void*)sizeof(glm::vec3)))
}

void render() {

	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	spMain.UseProgram();

//	spMain.SetUniform("matrices.projMatrix", GetProjectionMatrix());
//	spMain.SetUniform("matrices.viewMatrix", cCamera.Look());

	spMain.SetUniform("gSampler", 0);

	spMain.SetUniform("matrices.modelMatrix", glm::mat4(1.0));
	spMain.SetUniform("matrices.normalMatrix", glm::mat4(1.0));
	spMain.SetUniform("vColor", glm::vec4(1, 1, 1, 1));

	// This values will set the darkness of whole scene, that's why such name of variable :D
	static float fAngleOfDarkness = 45.0f;

	dlSun.vDirection = glm::vec3(-sin(fAngleOfDarkness*3.1415f / 180.0f), -cos(fAngleOfDarkness*3.1415f / 180.0f), 0.0f);
	dlSun.SetUniformData(&spMain, "sunLight");

	//spMain.SetUniform("matrices.modelMatrix", glm::translate(glm::mat4(1.0), cCamera.vEye));
	//sbMainSkybox.RenderSkybox();

	spMain.SetUniform("matrices.modelMatrix", glm::mat4(1.0));

	// Now we're going to render terrain

	hmWorld.SetRenderSize(300.0f, 35.0f, 300.0f);
	CShaderProgram* spTerrain = CMultiLayeredHeightmap::GetShaderProgram();

	spTerrain->UseProgram();

	

	// We bind all 5 textures - 3 of them are textures for layers, 1 texture is a "path" texture, and last one is
	// the places in heightmap where path should be and how intense should it be
	FOR(i, 5)
	{
		char sSamplerName[256];
		sprintf(sSamplerName, "gSampler[%d]", i);
		tTextures[i].BindTexture(i);
		spTerrain->SetUniform(sSamplerName, i);
	}

	// ... set some uniforms
	spTerrain->SetModelAndNormalMatrix("matrices.modelMatrix", "matrices.normalMatrix", glm::mat4(1.0));
	spTerrain->SetUniform("vColor", glm::vec4(1, 1, 1, 1));

	dlSun.SetUniformData(spTerrain, "sunLight");

	// ... and finally render heightmap
	hmWorld.RenderHeightmap();
//	GL_CHECK(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3) + sizeof(glm::vec2), (void*)sizeof(glm::vec3)))

}


int main(int argc, char *argv[])
{
	//GL_CHECK(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3) + sizeof(glm::vec2), (void*)sizeof(glm::vec3)))
	if (initSDL() < 0) return -1;
	SDL_Window* window;
//	GL_CHECK(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3) + sizeof(glm::vec2), (void*)sizeof(glm::vec3)))
	SDL_GLContext context;
	initOpenGL(window, context);
//	GL_CHECK(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3) + sizeof(glm::vec2), (void*)sizeof(glm::vec3)))
//	wyswietlane jako siatka 
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//	GL_CHECK(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3) + sizeof(glm::vec2), (void*)sizeof(glm::vec3)));
	//GL_CHECK(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3) + sizeof(glm::vec2), (void*)sizeof(glm::vec3)))

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	
//	GL_CHECK(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3) + sizeof(glm::vec2), (void*)sizeof(glm::vec3)))
	
	//GL_CHECK(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3) + sizeof(glm::vec2), (void*)sizeof(glm::vec3)))
	PrepareShaderPrograms();
	//GL_CHECK(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3) + sizeof(glm::vec2), (void*)sizeof(glm::vec3)));
	Shader ourShader("vertex.vs", "fragment.fs");
	Shader lightShader("LightShader.vs", "LightShader.fs");
	//GL_CHECK(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3) + sizeof(glm::vec2), (void*)sizeof(glm::vec3)));
	Shader boundingBoxShader("boundingBox.vs", "boundingBox.fs");
	Shader simpleShader("simpleColorShader.vs", "simpleColorShader.fs");
	Model kostka("res/models/kostka/kos.obj");
	Model pistolet("res/models/pistolet/pistolet.obj");

	LoadAllTextures();

	//GL_CHECK(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3) + sizeof(glm::vec2), (void*)sizeof(glm::vec3)))

	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0);

	dlSun = CDirectionalLight(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(sqrt(2.0f) / 2, -sqrt(2.0f) / 2, 0), 0.5f);

	CMultiLayeredHeightmap::LoadTerrainShaderProgram();
	hmWorld.LoadHeightMapFromImage("consider_this_question.bmp", "res/img");

	//GL_CHECK(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3) + sizeof(glm::vec2), (void*)sizeof(glm::vec3)))
	//GL_CHECK(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3) + sizeof(glm::vec2), (void*)sizeof(glm::vec3)))
	
	float deltaTime = 0.0;
	float lastFrame = 0.0;
	SDL_Event windowEvent;
	SDL_SetRelativeMouseMode(SDL_TRUE);


	Scene testowa(glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f), new Camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.1f, 1.0f));
    LightObject* light = testowa.SetLight(new LightObject(kostka, glm::vec3(2.5f, 1.0f, 2.0f), 0.0f, glm::vec3(0.2f), &simpleShader, glm::vec3(1.0f, 1.0f, 1.0f)));
	Entity* testCube = testowa.addObject(new Entity(kostka, glm::vec3(5.0f, -1.0f, 0.0f), 0.0f, glm::vec3(0.5f, 0.5f, 0.5f)));

	Entity* testBoundingBox = testowa.addObject(new Entity(kostka, glm::vec3(4.0f, 0.0f, 0.0f), 0.0f, glm::vec3(0.5f, 0.5f, 0.5f)));
	testBoundingBox->setShader(lightShader);
	testBoundingBox->rotateY(45.0f);
	BoundingBox box(*testBoundingBox);
	testowa.removeObject(testCube);

	//GL_CHECK(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3) + sizeof(glm::vec2), (void*)sizeof(glm::vec3)))


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
					nextMove[Camera::STRAFE_RIGHT] = 0;;
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
					ostatniWystrzal = currentFrame + 0.5;
				}
			}
			default: break;
			}
		}
		
		glClearColor(0.7f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		

		render();
	//	GL_CHECK(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3) + sizeof(glm::vec2), (void*)sizeof(glm::vec3)))

		testowa.getCamera()->moveCamera(nextMove, deltaTime);
		testowa.DrawObjects();
		//testCube->rotateZ(glm::radians(0.5f));
		//testCube->rotateY(glm::radians(0.2f));
		//testBoundingBox->rotateY(glm::radians(2.0f));
		//testowa.DrawObjects();
		//box.calculateBoundingBox();
	//	box.Draw(testowa.GetProjectionMatrix(), testowa.GetViewMatrix(), boundingBoxShader);

		//GL_CHECK(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3) + sizeof(glm::vec2), (void*)sizeof(glm::vec3)))
	

		

		

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, testowa.getCamera()->cameraPos + testowa.getCamera()->cameraFront);
		model = glm::rotate(model,glm::radians(180.0f-testowa.getCamera()->yaw), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model,glm::radians(-testowa.getCamera()->pitch), glm::vec3(0.0f, 0.0f, 1.0f));
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
		lightShader.setVec3("lightcolor", light->GetColor());
		lightShader.setVec3("lightpos", light->GetPosition());
		lightShader.setVec3("viewpos", testowa.getCamera()->cameraPos);
		pistolet.Draw(lightShader);

//	GL_CHECK(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3) + sizeof(glm::vec2), (void*)sizeof(glm::vec3)))
		


		SDL_GL_SwapWindow(window);


	}

	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit();
	return 0;
}