#pragma once
#include <glm/glm.hpp>
#include"GL/glew.h"
class Terrain
{
public:
	static bool LoadTerrainShaderProgram();
	static void ReleaseTerrainShaderProgram();

	void SetRenderSize(float fQuadSize, float fHeight);
	void SetRenderSize(float fRenderX, float fHeight, float fRenderZ);

	//gettery moze protected??
	int GetNumHeightmapRows();
	int GetNumHeightmapCols();

	void RenderHeightMap();
private:
	Terrain();
	~Terrain();
	int row;
	int col;
	glm::vec3 RenderScale;
	unsigned int VAO;
	//static CShaderProgram spTerrain;
	//static CShader shTerrainShaders[NUMTERRAINSHADERS];
};

