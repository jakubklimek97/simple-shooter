#include "Terrain.h"
#include <SDL_image.h>

Terrain::~Terrain() {
	if (heightMap)
		SDL_FreeSurface(heightMap);
}

void Terrain::loadTerrain(std::string modelPath, std::string heightMapPath, Shader& shader)
{
	model = new Model(modelPath);
	heightMap = IMG_Load("res/models/teren/height.png");
	if (!heightMap) {
		std::cout << "ERROR::TEXTURE_LOADER:: " << IMG_GetError() << std::endl;
	}
	entity = new Entity(*model, glm::vec3(0.0f), 0.0f, glm::vec3(1.0f));
	entity->setShader(&shader);

}
#define map_x 10
#define map_y 2
#define map_z 10 //for this game there will be only one map, therefore we can hardcode map dimensions
float Terrain::getHeight(glm::vec3 pos)
{
	if (!heightMap || pos.x < 0.0f || pos.z < 0.0f) return 0.0f;
	int rowLength = heightMap->pitch;
	int column = static_cast<int>((pos.x / (map_x)) * rowLength);
	int row = static_cast<int>((pos.z / (map_z)) * rowLength);
	Uint8 pixel = *((Uint8*)heightMap->pixels + row * rowLength + column * heightMap->format->BytesPerPixel);
	Uint8 r, g, b;
	SDL_GetRGB(pixel, heightMap->format, &r, &g, &b);
	return  ( r/ 255.0f) * map_y;
}

void Terrain::Draw(glm::mat4& projectionMatrix, glm::mat4& viewMatrix, const LightObject& lightObject, const Camera& camera)
{
	entity->Draw(projectionMatrix, viewMatrix, lightObject, camera);
}
