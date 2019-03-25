////
////#include <GL/glew.h>
////#include <SDL_opengl.h>
////#include <GL/GL.h>
////#include <glm/glm.hpp>
////#include <glm/gtc/matrix_transform.hpp>
////#include <glm/gtc/type_ptr.hpp>
////#define GLEW_STATIC
////#include <GL/glew.h>
//////#include <GLFW/glfw3.h>
////
////#include"GameSprite.h"
////#include "ResourceManager2D.h"
////
////#include "Shader.h"
////#include "Camera.h"
////#include "Model.h"
////
//// GLFW function declerations
////void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
////
//// The Width of the screen
////const GLuint SCREEN_WIDTH = 800;
//// The height of the screen
////const GLuint SCREEN_HEIGHT = 600;
////
////Game Breakout(SCREEN_WIDTH, SCREEN_HEIGHT);
////
////
//int main(int argc, char *argv[])
//{
//	glfwInit();
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
//
//	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Breakout", nullptr, nullptr);
//	glfwMakeContextCurrent(window);
//
//	glewExperimental = GL_TRUE;
//	glewInit();
//	glGetError(); // Call it once to catch glewInit() bug, all other errors are now from our application.
//
//	glfwSetKeyCallback(window, key_callback);
//
//	// OpenGL configuration
//	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
//	glEnable(GL_CULL_FACE);
//	glEnable(GL_BLEND);
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//
//	// Initialize game
//	Breakout.Init();
//
//	 DeltaTime variables
//	GLfloat deltaTime = 0.0f;
//	GLfloat lastFrame = 0.0f;
//
//	 //Start Game within Menu State
//	Breakout.State = GAME_ACTIVE;
//
//	while (!glfwWindowShouldClose(window))
//	{
//		// Calculate delta time
//		GLfloat currentFrame = glfwGetTime();
//		deltaTime = currentFrame - lastFrame;
//		lastFrame = currentFrame;
//		glfwPollEvents();
//
//		deltaTime = 0.001f;
//		// Manage user input
//		Breakout.ProcessInput(deltaTime);
//
//		// Update Game state
//		Breakout.Update(deltaTime);
//
//	//	 Render
//		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
//		glClear(GL_COLOR_BUFFER_BIT);
//      Breakout.Render();
//
//		glfwSwapBuffers(window);
//	}
//
//	// Delete all resources as loaded using the resource manager
//	ResourceManager::Clear();
//
//	glfwTerminate();
//	return 0;
//}
//
////void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
////{
////	 When a user presses the escape key, we set the WindowShouldClose property to true, closing the application
////	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
////		glfwSetWindowShouldClose(window, GL_TRUE);
////	if (key >= 0 && key < 1024)
////	{
////		if (action == GLFW_PRESS)
////			Breakout.Keys[key] = GL_TRUE;
////		else if (action == GLFW_RELEASE)
////			Breakout.Keys[key] = GL_FALSE;
////	}
////}
////
////
////namespace static_geometry {
////
////	glm::vec2 quad2D[4] =
////	{
////	   glm::vec2(0, 1), // Top-left point
////	   glm::vec2(0, 0), // Bottom-left point
////	   glm::vec2(1, 1), // Top-right point
////	   glm::vec2(1, 0) // Bottom-right point
////	};
////}
////glm::mat4 projection = glm::ortho(0.0f, 800.0f, 600.0f, 0.0f, -1.0f, 1.0f);
////const GLuint SCREEN_WIDTH = 800;
//// The height of the screen
////const GLuint SCREEN_HEIGHT = 600;
////
////GameSprite Breakout(SCREEN_WIDTH, SCREEN_HEIGHT);
////
////int initSDL() {
////	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
////		std::cout << "Nie mozna zainicjowac SDLA: " << SDL_GetError() << std::endl;
////		return -1;
////	}
////	if (IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) < 0) {
////		std::cout << "Nie mozna zainicjowac SDL_image: " << IMG_GetError() << std::endl;
////		return -1;
////	}
////	return 0;
////}
////glm::mat4 _orthoMatrix_orthoMatrix = glm::ortho(0.0f, float(1.0f), 0.0f, float(1.0f));
////int main(int argc, char *argv[])
////{
////
////	if (initSDL() < 0) return -1;
////
////	SDL_Window* window;
////	SDL_GLContext context;
////	inicjalizacje opengla i glewa, utworzenie okna i jego zawartosci
////	{
////		ustawienie wersji opengla
////		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
////		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
////		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
////		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
////		utworzenie okna
////		window = SDL_CreateWindow("Simple Shooter", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
////
////		"zawartosc" okna
////		context = SDL_GL_CreateContext(window);
////
////		inicjalizacja glewa
////		glewExperimental = GL_TRUE;
////		glewInit();
////		glViewport(0, 0, 800, 600);
////		Breakout.Init();
////
////	}
////	glEnable(GL_DEPTH_TEST);
////	wyswietlane jako siatka glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
////
////	/*Shader ourShader("vertex.vs", "fragment.fs");
////	Model ourModel("res/models/headphones_UVW.fbx");
////	Model pistolet("res/models/pistolet/pistolet.obj");*/
////
////	float deltaTime = 0.0;
////	float lastFrame = 0.0;
////	drawHUD();
////	Breakout.State = GAME_ACTIVE;
////	SDL_Event windowEvent;
////	SDL_SetRelativeMouseMode(SDL_TRUE);
////	Camera kamera(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.1f, 1.0f);
////	Camera::Movement nextMove;
////	while (true)
////	{
////		float currentFrame = SDL_GetTicks() / 1000.0f;
////		deltaTime = currentFrame - lastFrame;
////		lastFrame = currentFrame;
////		float cameraSpeed = 1.0f * deltaTime;
////		while (SDL_PollEvent(&windowEvent))
////		{
////			if (windowEvent.type == SDL_QUIT) {
////				break; //jakis hack na wyjscie, eventy musi pobierac w whilu zeby kamera byla plynna...
////			}
////			switch (windowEvent.type) {
////			case SDL_KEYDOWN: {
////
////				switch (windowEvent.key.keysym.sym) {
////				case SDLK_w: {
////					nextMove[Camera::MOVE_FORWARD] = 1;
////					break;
////				}
////				case SDLK_s: {
////					nextMove[Camera::MOVE_BACKWARD] = 1;
////					break;
////				}
////				case SDLK_a: {
////					nextMove[Camera::STRAFE_LEFT] = 1;
////					break;
////				}
////				case SDLK_d: {
////					nextMove[Camera::STRAFE_RIGHT] = 1;;
////					break;
////				}
////				default: break;
////				}
////				break;
////			}
////			case SDL_KEYUP: {
////
////				switch (windowEvent.key.keysym.sym) {
////				case SDLK_w: {
////					nextMove[Camera::MOVE_FORWARD] = 0;
////					break;
////				}
////				case SDLK_s: {
////					nextMove[Camera::MOVE_BACKWARD] = 0;
////					break;
////				}
////				case SDLK_a: {
////					nextMove[Camera::STRAFE_LEFT] = 0;
////					break;
////				}
////				case SDLK_d: {
////					nextMove[Camera::STRAFE_RIGHT] = 0;
////					break;
////				}
////				default: break;
////				}
////				break;
////			}
////			case SDL_MOUSEMOTION: {
////				kamera.turnCamera(windowEvent.motion);
////			}
////			default: break;
////			}
////		}
////		Breakout.ProcessInput(deltaTime);
////		Breakout.Update(deltaTime);
////		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
////		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
////		kamera.moveCamera(nextMove, deltaTime);
////		ourShader.use();
////		glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
////		glm::mat4 view = kamera.getViewMatrix();
////		//glm::mat4 view = glm::mat4(1.0f);
////		ourShader.setMat4("projection", projection);
////		ourShader.setMat4("view", view);
////		glm::mat4 model = glm::mat4(1.0f);
////		model = glm::rotate(model, (float)glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
////		model = glm::scale(model, glm::vec3(0.03f, 0.03f, 0.03f));
////		ourShader.setMat4("model", model);
////		ourModel.Draw(ourShader);
////		// note that we're translating the scene in the reverse direction of where we want to move
////		view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.1f, -0.5f));
////		ourShader.setMat4("view", view);
////		model = glm::mat4(1.0f);
////		std::cout << kamera.cameraPos.x << " " << kamera.cameraPos.y << " " << kamera.cameraPos.z << std::endl;
////		model = glm::rotate(model, (float)glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
////		model = glm::scale(model, glm::vec3(0.03f, 0.03f, 0.03f));
////		ourShader.setMat4("model", model);
////		pistolet.Draw(ourShader);
////		/*drawHealth4(0.3f);
////		glBegin(GL_QUADS);
////		glColor3f(1, 0, 0);
////		glVertex2f(0, 0);
////		glVertex2f(1, 0);
////		glVertex2f(1, 0.2);
////		glVertex2f(0, 0.2);*/
////		glEnd();
////		ResourceManager2D::Clear();
////		SDL_GL_SwapWindow(window);
////	}
////
////	SDL_GL_DeleteContext(context);
////	SDL_DestroyWindow(window);
////	IMG_Quit();
////	SDL_Quit();
////	return 0;
////}