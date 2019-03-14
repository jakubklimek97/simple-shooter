#include "GameSprite.h"


Sprite *Renderer;

GameSprite::GameSprite(GLuint width, GLuint height) : State(GAME_ACTIVE), Keys(), Width(width), Height(height)
{
}

void GameSprite::Init()
{
	// Load shaders
	ResourceManager2D::LoadShader("shaders/sprite.vs", "shaders/sprite.frag", nullptr, "sprite");
	// Configure shaders
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->Width), static_cast<GLfloat>(this->Height), 0.0f, -1.0f, 1.0f);
	ResourceManager2D::GetShader("sprite").Use().SetInteger("image", 0);
	ResourceManager2D::GetShader("sprite").SetMatrix4("projection", projection);
	// Load textures
	ResourceManager2D::LoadTexture("textures/awesomeface.png", GL_TRUE, "face");
	// Set render-specific controls
	Renderer = new Sprite(ResourceManager2D::GetShader("sprite"));
}

void GameSprite::Update(GLfloat dt)
{
}

void GameSprite::ProcessInput(GLfloat dt)
{
}

void GameSprite::Render()
{
	Renderer->DrawSprite(ResourceManager2D::GetTexture("face"), glm::vec2(200, 200), glm::vec2(300, 400), 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
}

GameSprite::~GameSprite()
{
	delete Renderer;
}
