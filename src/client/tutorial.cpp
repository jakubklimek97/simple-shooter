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
#include <cstdlib>





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

unsigned int quadVAO = 0;
unsigned int quadVBO;
float heightScale = 0.3f;
static int zycie = 500000;
static float i = 0.0f;

float random() {
	i += 0.06f;
	if (i == 1.1f) {
		i = 0.0f;
		return i;
	}
	zycie -= 1;
	if (zycie < -2300)
		zycie = -1;
	return i;
}
void renderQuad()
{
	if (quadVAO == 0)
	{
		// positions
		glm::vec3 pos1(-1.0f, 1.0f, 0.0f);
		glm::vec3 pos2(-1.0f, -1.0f, 0.0f);
		glm::vec3 pos3(1.0f, -1.0f, 0.0f);
		glm::vec3 pos4(1.0f, 1.0f, 0.0f);
		// texture coordinates
		glm::vec2 uv1(0.0f, 1.0f);
		glm::vec2 uv2(0.0f, 0.0f);
		glm::vec2 uv3(1.0f, 0.0f);
		glm::vec2 uv4(1.0f, 1.0f);
		// normal vector
		glm::vec3 nm(0.0f, 0.0f, 1.0f);

		// calculate tangent/bitangent vectors of both triangles
		glm::vec3 tangent1, bitangent1;
		glm::vec3 tangent2, bitangent2;
		// triangle 1
		// ----------
		glm::vec3 edge1 = pos2 - pos1;
		glm::vec3 edge2 = pos3 - pos1;
		glm::vec2 deltaUV1 = uv2 - uv1;
		glm::vec2 deltaUV2 = uv3 - uv1;

		float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

		tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
		tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
		tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
		tangent1 = glm::normalize(tangent1);

		bitangent1.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
		bitangent1.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
		bitangent1.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
		bitangent1 = glm::normalize(bitangent1);

		// triangle 2
		// ----------
		edge1 = pos3 - pos1;
		edge2 = pos4 - pos1;
		deltaUV1 = uv3 - uv1;
		deltaUV2 = uv4 - uv1;

		f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

		tangent2.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
		tangent2.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
		tangent2.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
		tangent2 = glm::normalize(tangent2);


		bitangent2.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
		bitangent2.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
		bitangent2.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
		bitangent2 = glm::normalize(bitangent2);


		float quadVertices[] = {
			// positions            // normal         // texcoords  // tangent                          // bitangent
			pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
			pos2.x, pos2.y, pos2.z, nm.x, nm.y, nm.z, uv2.x, uv2.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
			pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,

			pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
			pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
			pos4.x, pos4.y, pos4.z, nm.x, nm.y, nm.z, uv4.x, uv4.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z
		};
		// configure plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(8 * sizeof(float)));
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(11 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}



int main(int argc, char *argv[])
{

	float vertices[] = {
		// positions          // colors           // texture coords
		-0.7f, -1.0f, 0.0f,    1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
		-0.7f, -0.7f, 0.0f,    0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
		-1.0f,  -0.7f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
		-1.0f,  -1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
	};
	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};
	float vertices2[] = {
		// positions          // colors           // texture coords
		-0.4f, -1.0f, 0.0f,    1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
		-0.4f, -0.7f, 0.0f,    0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
		-0.7f, -0.7f, 0.0f,     0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
		-0.7f, -1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
	};
	unsigned int indices2[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	if (initSDL() < 0) return -1;
	SDL_Window* window;
	SDL_GLContext context;
	initOpenGL(window, context);
	//wyswietlane jako siatka glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	Shader ourShader("vertex.vs", "fragment.fs");
	Model ourModel("res/models/headphones_UVW.fbx");
	Model pistolet("res/models/pistolet/pistolet.obj");
	//Model kostka("res/models/kostka/kos.obj");
	Shader Shader2d("texture.vs", "texture.fs");
	Shader Shader2d2("texture.vs", "texture.fs");
	Shader ShaderH("parallel_mapping.vs", "parallel_mapping.fs");
	//1 zycie
	
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//2 zycie
	unsigned int VBO2, VAO2, EBO2;
	glGenVertexArrays(1, &VAO2);
	glGenBuffers(1, &VBO2);
	glGenBuffers(1, &EBO2);

	glBindVertexArray(VAO2);

	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices2), indices2, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//1 zycie
	unsigned texture1 = Model::TextureFromFile3("heart.png", "res/img", false);
    unsigned texture2 = Model::TextureFromFile3("container.jpg", "res/img", false);
	//2  zycie
	unsigned texture3 = Model::TextureFromFile3("heart.png", "res/img", false);
	unsigned texture4 = Model::TextureFromFile3("container.jpg", "res/img", false);
	
	
	

	Shader2d.use(); // don't forget to activate/use the shader before setting uniforms!
   // either set it manually like so:

	GLfloat gamma;
	gamma = (sin(i) / 2) + 0.5;

	glUniform1f(glGetUniformLocation(Shader2d.ID, "gamma"), gamma);
	Shader2d.setInt("texture2", 1);

	Shader2d2.use();
	//2 zycie
	glUniform1f(glGetUniformLocation(Shader2d2.ID, "gamma"), gamma);
	Shader2d2.setInt("texture2", 1);
	//glUniform1i(glGetUniformLocation(Shader2d.ID, "texture2"), 0);
	// or set it via the texture class
	
	

	unsigned int diffuseMap = Model::TextureFromFile3("bricks2.jpg", "res/img", false);
	unsigned int normalMap = Model::TextureFromFile3("bricks2_normal.jpg", "res/img", false);
	unsigned int heightMap = Model::TextureFromFile3("bricks2_disp.jpg", "res/img", false);

	ShaderH.use();
	ShaderH.setInt("diffuseMap", 0);
	ShaderH.setInt("normalMap", 1);
	ShaderH.setInt("depthMap", 2);

	glm::vec3 lightPos(0.5f, 1.0f, 0.3f);

	float deltaTime = 0.0;
	float lastFrame = 0.0;
	SDL_Event windowEvent;
	SDL_SetRelativeMouseMode(SDL_TRUE);
	Camera kamera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.1f, 1.0f);
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
		if(zycie<0){
		
			kamera.moveCamera(nextMove, deltaTime);

			ourShader.use();
			glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
			glm::mat4 view = kamera.getViewMatrix();
			ourShader.setMat4("projection", projection);
			ourShader.setMat4("view", view);
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::rotate(model, (float)glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(0.03f, 0.03f, 0.03f));
			ourShader.setMat4("model", model);
			//ourModel.Draw(ourShader);


			model = glm::mat4(1.0f);
			model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
			ourShader.setMat4("model", model);
			//kostka.Draw(ourShader);
			model = glm::mat4(1.0f);
			model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
			model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));

			ourShader.setMat4("model", model);
			//kostka.Draw(ourShader);

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


			ShaderH.use();
			ShaderH.setMat4("projection", projection);
			ShaderH.setMat4("view", view);
			// render parallax-mapped quad
			model = glm::mat4(1.0f);
			model = glm::rotate(model, (float)glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // rotate the quad to show parallax mapping from multiple directions
			ShaderH.setMat4("model", model);
			ShaderH.setVec3("viewPos", glm::vec3(0.0f, 0.0f, 3.0f));
			ShaderH.setVec3("lightPos", lightPos);
			ShaderH.setFloat("heightScale", heightScale); // adjust with Q and E keys
			std::cout << heightScale << std::endl;
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, diffuseMap);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, normalMap);
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, heightMap);
			renderQuad();

			SDL_GL_SwapWindow(window);


		}

		else {
			kamera.moveCamera(nextMove, deltaTime);

			ourShader.use();
			glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
			glm::mat4 view = kamera.getViewMatrix();
			ourShader.setMat4("projection", projection);
			ourShader.setMat4("view", view);
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::rotate(model, (float)glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(0.03f, 0.03f, 0.03f));
			ourShader.setMat4("model", model);
			//ourModel.Draw(ourShader);


			model = glm::mat4(1.0f);
			model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
			ourShader.setMat4("model", model);
//			kostka.Draw(ourShader);
			model = glm::mat4(1.0f);
			model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
			model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));

			ourShader.setMat4("model", model);
		//	kostka.Draw(ourShader);

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


			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture1);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, texture2);
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, texture3);
			glActiveTexture(GL_TEXTURE3);
			glBindTexture(GL_TEXTURE_2D, texture4);

			// render container
			/*time(&current_time);
			 timeValue = current_time;
			 gamma = (sin(timeValue) / 2) + 0.5;
			glUniform1f(glGetUniformLocation(Shader2d.ID, "gamma"), gamma);*/

			Shader2d.use();

			gamma = (sin(random()) / 2) + 0.5;
			glUniform1f(glGetUniformLocation(Shader2d.ID, "gamma"), gamma);
			Shader2d.setInt("texture2", 1);
			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			//tutaj
			
			Shader2d2.use();

			gamma = (sin(random()) / 2) + 0.5;
			glUniform1f(glGetUniformLocation(Shader2d2.ID, "gamma"), gamma);
			Shader2d2.setInt("texture2", 1);
			glBindVertexArray(VAO2);
			
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


			ShaderH.use();
			ShaderH.setMat4("projection", projection);
			ShaderH.setMat4("view", view);
			// render parallax-mapped quad
			 model = glm::mat4(1.0f);
			model = glm::rotate(model, (float)glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f)); // rotate the quad to show parallax mapping from multiple directions
			ShaderH.setMat4("model", model);
			ShaderH.setVec3("viewPos", glm::vec3(0.0f, 0.0f, 3.0f)); //moze ??
			ShaderH.setVec3("lightPos", lightPos);
			ShaderH.setFloat("heightScale", heightScale); // adjust with Q and E keys
			std::cout << heightScale << std::endl;
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, diffuseMap);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, normalMap);
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, heightMap);
			renderQuad();

			model = glm::mat4(1.0f);
			model = glm::translate(model, lightPos);
			model = glm::scale(model, glm::vec3(0.1f));
			ShaderH.setMat4("model", model);
			renderQuad();

			SDL_GL_SwapWindow(window);
		}
	}

	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit();
	return 0;
}