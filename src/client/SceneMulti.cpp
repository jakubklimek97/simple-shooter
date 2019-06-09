#include "SceneMulti.h"
#include "Terrain.h"
#include "Loader.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "SceneManager.h"

static int currentSelection = 0;
static glm::vec3 color = glm::vec3(1, 0, 1);
// testowanie menu

SceneMulti::SceneMulti(glm::mat4 projectionMatrix, Camera * camera): Scene(projectionMatrix, camera)
{
}

SceneMulti::~SceneMulti()
{
}
void SceneMulti::InitScene()
{
	lastFrame = SDL_GetTicks() / 1000.0f;


	VBOSceneObject.CreateVBO();
	glGenVertexArrays(1, uVAO); // Create one VAO
	glBindVertexArray(uVAO[0]);

	VBOSceneObject.BindVBO();
	// Vertex positions


	for (int i = 0; i < 36; ++i)
	{
		VBOSceneObject.AddData(&vCubeVertices[i], sizeof(glm::vec3));
		VBOSceneObject.AddData(&vCubeTexCoords[i % 6], sizeof(glm::vec2));
		VBOSceneObject.AddData(&vCubeNormals[i / 6], sizeof(glm::vec3));
	}




	VBOSceneObject.UploadDataToGPU(GL_STATIC_DRAW);
	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3) + sizeof(glm::vec2), 0);
	// Texture coordinates
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3) + sizeof(glm::vec2), (void*)sizeof(glm::vec3));
	// Normal vectors
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3) + sizeof(glm::vec2), (void*)(sizeof(glm::vec3) + sizeof(glm::vec2)));


	SetLight(new LightObject(Loader::getModel(Loader::LoadedModels::CUBE), glm::vec3(0.0f, 10.0f, 0.0f), 0.0f, glm::vec3(0.2f), &Loader::getShader(Loader::LoadedShaders::SIMPLE), glm::vec3(1.0f, 1.0f, 1.0f)));
	player2 = addObject(new Entity(Loader::getModel(Loader::LoadedModels::PLAYER), glm::vec3(20.0f, 0.0f, 7.0f), 0.0f, glm::vec3(1.0f)));
	player2->setShader(Loader::getShader(Loader::LoadedShaders::LIGHT));
	SDL_SetRelativeMouseMode(SDL_TRUE);

	Bullet::setShader(&Loader::getShader(Loader::LoadedShaders::SIMPLE));
	
	SH.push_back(Loader::getShader(Loader::LoadedShaders::SHADER2D));
	SH.push_back(Loader::getShader(Loader::LoadedShaders::SHADER2D2));
	
	
	Sun = CDirectionalLight(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(sqrt(2.0f) / 2, -sqrt(2.0f) / 2, 0), 0.5f);
	
	MainSkybox = CSkybox(Loader::getShader(Loader::LoadedShaders::SKYBOX), "res/img/right.jpg", "res/img/left.jpg", "res/img/top.jpg", "res/img/bottom.jpg", "res/img/front.jpg", "res/img/back.jpg");
	
	life1 = HUD(SH, "heart.png", "sand.jpg", 0.01f, 1);
	life2 = HUD(SH, "heart.png", "sand.jpg", 0.01f, 1);
	life3 = HUD(SH, "heart.png", "sand.jpg", 0.01f, 1);

	PrepareShaderPrograms();
	LoadAllTextures();

	HeightMap::LoadTerrainShaderProgram();
    World.LoadMapFromImage("hm.png", "res/img");


	SoundMgr = Sound::Instance();
	SoundM = SoundManager::Instance();

	SoundM->PlayMusic("Battle_in_the_Stars.ogg", -1);


}

void SceneMulti::UnInitScene()
{
	delete player2;
	Sound::Release();
	SoundMgr = NULL;
	SoundManager::Release();
	SoundM = NULL;
}

void SceneMulti::handleEvents(SDL_Event & e)
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
				nextMove[Camera::MOVE_FORWARD] = 1;
				SoundM->PlaySFX("CartoonWalking.ogg");

				break;
			}
			case SDLK_s: {
				nextMove[Camera::MOVE_BACKWARD] = 1;
				SoundM->PlaySFX("CartoonWalking.ogg");
				break;
			}
			case SDLK_a: {
				nextMove[Camera::STRAFE_LEFT] = 1;
				SoundM->PlaySFX("CartoonWalking.ogg");
				break;
			}
			case SDLK_d: {
				nextMove[Camera::STRAFE_RIGHT] = 1;;
				SoundM->PlaySFX("CartoonWalking.ogg");
				break;
			}
			case SDLK_DOWN:
				nextMove[Camera::MOVE_BACKWARD] = 0;
				break;

			case SDLK_SPACE: {
				playerJumpStart();
				SoundM->PlaySFX("Jump.ogg",0,3);
				break;
			}
			default: break;
			}
			break;
		}
		case SDL_KEYUP: {

			switch (e.key.keysym.sym) {
			case SDLK_w: {
				nextMove[Camera::MOVE_FORWARD] = 0;
				SoundM->PlaySFX("CartoonWalking.ogg");
				break;
			}
			case SDLK_s: {
				nextMove[Camera::MOVE_BACKWARD] = 0;
				SoundM->PlaySFX("CartoonWalking.ogg");
				break;
			}
			case SDLK_a: {
				nextMove[Camera::STRAFE_LEFT] = 0;
				SoundM->PlaySFX("CartoonWalking.ogg");
				break;
			}
			case SDLK_d: {
				nextMove[Camera::STRAFE_RIGHT] = 0;
				SoundM->PlaySFX("CartoonWalking.ogg");
				break;
			}
			
			case SDLK_UP:
				break;

			default: break;
			}
			break;
		}
		case SDL_MOUSEMOTION: {
			getCamera()->turnCamera(e.motion);
		}
		case SDL_MOUSEBUTTONDOWN: {
			
			if (e.button.button == SDL_BUTTON_LEFT && currentFrame > ostatniWystrzal) {
				ostatniWystrzal = currentFrame + 0.5f;
				bulletContainer.push_back(new Bullet(getCamera()->cameraPos, getCamera()->cameraFront));
				tmpThreadObject = new std::thread([=] {deleteBullet(); });
				tmpThreadObject->detach();
				SoundM->PlaySFX("laser.wav", 0, 1);

			}
		}
		default: break;
		}
	}
	movePlayer(nextMove, deltaTime);
}

void SceneMulti::render()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



	MainSkybox.SetDeltatime(currentFrame);
	MainSkybox.RenderSkybox(Loader::getShader(Loader::LoadedShaders::SKYBOX), this->GetViewMatrix(), this->GetProjectionMatrix());

	DrawObjects();
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, getCamera()->cameraPos + getCamera()->cameraFront);
	model = glm::rotate(model, glm::radians(180.0f - getCamera()->yaw), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(-getCamera()->pitch), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3(0.35f, -0.15f, 0.0f));
	if (ostatniWystrzal != 0.0) {
		if (currentFrame >= ostatniWystrzal) {
			ostatniWystrzal = 0.0;
		}
		else {
			model = glm::rotate(model, -glm::sin((ostatniWystrzal - currentFrame) / 0.5f) / 3.0f, glm::vec3(0.0f, 0.0f, 1.0f));
		}
	}
	bulletLock.lock();
	for (Bullet* bullet : bulletContainer) {
		bullet->draw(GetProjectionMatrix(), getCamera()->getViewMatrix(), glm::vec3(1.0f, 0.0f, 0.0f));
	}
	bulletLock.unlock();
	model = glm::scale(model, glm::vec3(0.03f, 0.03f, 0.03f));
	Shader& lightShader = Loader::getShader(Loader::LoadedShaders::LIGHT);
	lightShader.use();
	lightShader.setMat4("model", model);
	lightShader.setMat4("projection", GetProjectionMatrix());
	lightShader.setMat4("view", GetViewMatrix());
	lightShader.setVec3("lightColor", GetLight()->GetColor());
	lightShader.setVec3("lightPos", GetLight()->GetPosition());
	lightShader.setVec3("viewPos", getCamera()->cameraPos);
	glClear(GL_DEPTH_BUFFER_BIT);
	Loader::getModel(Loader::LoadedModels::GUN).Draw(lightShader);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);


	        spMain.UseProgram();
	        spMain.SetUniform("matrices.projMatrix", GetProjectionMatrix());
			spMain.SetUniform("matrices.viewMatrix", GetViewMatrix());
	
			spMain.SetUniform("gSampler", 0);
	
			spMain.SetUniform("matrices.modelMatrix", glm::mat4(1.0));
			spMain.SetUniform("matrices.normalMatrix", glm::mat4(1.0));
			spMain.SetUniform("vColor", glm::vec4(1,1, 1, 1));
			
			static float fAngleOfDarkness = 10.0f;
	
			Sun.vDirection = glm::vec3(-sin(fAngleOfDarkness*3.1415f / 180.0f), -cos(fAngleOfDarkness*3.1415f / 180.0f), 0.0f);
			Sun.SetUniformData(&spMain, "sunLight");
	
	
			World.SetRenderSize(1.0f, 1.0f, 1.0f);
			ShaderProgram* spTerrain = HeightMap::GetShaderProgram();
	        spTerrain->UseProgram();
	
	
			spTerrain->SetUniform("matrices.projMatrix", GetProjectionMatrix());
			spTerrain->SetUniform("matrices.viewMatrix", GetViewMatrix());
			for(int i =0;i<NUMTEXTURES;++i)
			{
				stringstream Sampler;
				Sampler << "gSampler[" << i << "]";
				tTextures[i].BindTexture(i);
				spTerrain->SetUniform(Sampler.str(), i);
			}
	
			
			spTerrain->SetModelAndNormalMatrix("matrices.modelMatrix", "matrices.normalMatrix", glm::mat4(1.0));
			spTerrain->SetUniform("vColor", glm::vec4(1,1 , 1, 1));
	
			Sun.SetUniformData(spTerrain, "sunLight");
		
		
			World.RenderHeightmap(GetProjectionMatrix());
			
			
			float terrainheight = World.TerrainCollide(getCamera()->cameraPos.x,getCamera()->cameraPos.z);
			float camera = getCamera()->cameraPos.y;
			if (camera <= terrainheight||camera <= terrainheight+0.31f||camera <= terrainheight-0.33f) { //200 ->Terrain height
			   getCamera()->cameraPos.y = terrainheight+1.95f;
			}
			else if(terrainheight - getCamera()->cameraPos.y<0.3f || terrainheight - getCamera()->cameraPos.y<0.3f|| terrainheight - getCamera()->cameraPos.y>0.3f|| terrainheight - getCamera()->cameraPos.y>0.2f)
			 getCamera()->cameraPos.y = terrainheight + 1.65f;
		 float heightvalue = 0.6f;
			if(camera <= heightvalue)
				getCamera()->cameraPos.y = getCamera()->cameraPos.y + 0.25f;
	


					spFogAndLight.UseProgram();


		spFogAndLight.SetUniform("sunLight.vColor", glm::vec3(1.0f, 1.0f, 1.0f));

	
			
		spFogAndLight.SetUniform("sunLight.fAmbientIntensity", 1.0f); // Full light for skybox
		spFogAndLight.SetUniform("sunLight.vDirection", glm::vec3(0, -1, 0));

		spFogAndLight.SetUniform("matrices.projectionMatrix", GetProjectionMatrix());
		spFogAndLight.SetUniform("gSampler", 0);

		glm::mat4 mModelView = GetViewMatrix();
		glm::mat4 mModelToCamera;

		spFogAndLight.SetUniform("fogParams.iEquation", 1);
		spFogAndLight.SetUniform("fogParams.vFogColor", glm::vec4(0.7f, 0.7f, 0.7f, 1.0f));

		spFogAndLight.SetUniform("fogParams.fDensity", 0.05f);
		spFogAndLight.SetUniform("matrices.modelViewMatrix", glm::translate(mModelView, getCamera()->cameraPos));

		glBindVertexArray(uVAO[0]);
		spFogAndLight.SetUniform("sunLight.fAmbientIntensity", 0.55f);
		spFogAndLight.SetUniform("matrices.modelViewMatrix", &mModelView);

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
	
		float posx = getCamera()->cameraPos.x;
		float posz = getCamera()->cameraPos.z;
		if (posx > 450.0f || posx < 100.0f) {


			//cout << "KOLIZJA";
			getCamera()->cameraPos.x = 200;
		}
		else if (posz > 400.0f || posz < 65.0f) {
		//	cout << "KOLZIJA";
		   getCamera()->cameraPos.z = 200;

		}

		life1.Incrementvalue();
		life1.RotationIntensity();
		life1.RenderHUD(SH);
		life2.RenderHUD(SH);
		life3.RenderHUD(SH);

		

}

void SceneMulti::deleteBullet()
{
	SDL_Delay(500);
	bulletLock.lock();
	delete bulletContainer.front();
	bulletContainer.pop_front();
	bulletLock.unlock();
}