
#include "Terrain.h"
#include<SDL.h>
#include<SDL_image.h>

using namespace std;
static GLuint shaderProgram = 0;
static SDL_Surface *heightData = NULL, *normData = NULL;
void Terrain::loadHeighData()
{
	heightData = IMG_Load("height.obj");
	if (!heightData) {
		std::cout << "no height data!";
	}
	normData = IMG_Load("normals.obj");
	if (!normData) {
		std::cout << "no normal data!";
	}
}

struct float3 {
	float x, y, z;
	float3() {}
	float3(float x, float y, float z) :x(x), y(y), z(z) {}

};

struct uint3 {
	unsigned int x, y, z;
	uint3(unsigned int x, unsigned int y, unsigned int z) :x(x), y(y), z(z) {}
};

static GLuint terrainVbo = 0, terrainVboNorm = 0, terrainEbo = 0;
int terrainCount = 0;


void Terrain::genTerrain(int wRes, int lRes)
{
	const int sz = wRes * lRes;
	std::vector<float3> verts(sz), norms(sz);
	std::vector<uint3> tris;
	const float w = 10.0f, h = 3.0f, l = 10.0f;

	int i = 0;
	for (int z = 0; z < lRes; z++) for (int x = 0; x < wRes; x++) {
		int ix = 0, iy = 0;
		float3 v = float3(x / (float)wRes, 0, z / (float)lRes);

		ix = (int)roundf(x*heightData->w);
		iy = (int)roundf(z*heightData->h);

		float heightValue = (getPixel(heightData, ix, iy) & 0xff) / 255.0f; // normalizowanie
		int normalValue = getPixel(normData, ix, iy);

		//if (i % 1000 == 0) {
		//	std::cout << (heightValue & 0xff) << std::endl;
		//}

		v.x *= w;
		v.z *= l;
		v.x -= w / 2;
		v.z -= l / 2;
		v.y = heightValue * h;

		float nr = (normalValue & 0xff) / 255.0f, nb = ((normalValue >> 16) & 0xFF) / 255.0f, ng = ((normalValue >> 8) & 0xFF) / 255.0f;
		float3 n;
		n.x =-(float)(nb*2.0f - 1.0);
		n.y = (float)(nr*2.0f - 1.0);
		n.z =-(float)(ng*2.0f - 1.0);
		verts[i] = v;
		norms[i] = n; /*float3(0, 1, 0);*/

		if ((i + 1 % wRes) != 0 && z + 1 < lRes) {
			uint3 tri = uint3(i, i + wRes, i + wRes + 1);
			uint3 tri2 = uint3(i, i + wRes + 1, i + 1);

			tris.push_back(tri);
			tris.push_back(tri2);
		}

		i++;
	}
	terrainCount = tris.size();
	glGenBuffers(1, &terrainVbo);
	glGenBuffers(1, &terrainVboNorm);
	glGenBuffers(1, &terrainEbo);

	glBindBuffer(GL_ARRAY_BUFFER, terrainVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float3)*verts.size(), verts.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, terrainVboNorm);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float3)*norms.size(), norms.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, terrainEbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint3)*tris.size(), tris.data(), GL_STATIC_DRAW);
}

int Terrain::getPixel(SDL_Surface * surface, int x, int y)
{
	int bpp = surface->format->BytesPerPixel;
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

	if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
		return p[0] << 16 | p[1] << 8 | p[2];
	else
		return p[0] | p[1] << 8 | p[2] << 16;
}
