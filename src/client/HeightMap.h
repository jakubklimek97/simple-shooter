#pragma once
#define NUMTERRAINSHADERS 3
#include"vertexBufferObject.h"
#include"shaders.h"
#include <string>
#include <SDL.h>
#include <SDL_image.h>

#include <GL/glew.h>
#include <SDL_opengl.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include"Camera.h"



class CMultiLayeredHeightmap
{
public:
	static bool LoadTerrainShaderProgram();
	static void ReleaseTerrainShaderProgram();
	glm::mat4 GetScaleMatrix();
	bool LoadHeightMapFromImage(const char *path, const std::string &directory);
	void ReleaseHeightmap();
	static bool CheckCollision(glm::vec3 cameraPos);
	void RenderHeightmap();
	void RenderHeightmapForNormals();
	void SetRenderSize(float fQuadSize, float fHeight);
	void SetRenderSize(float fRenderX, float fHeight, float fRenderZ);
	vector< vector< glm::vec3> > GetVertexData();
	void SetVertexData();
	vector< vector< glm::vec2> >  GetCoordsData();
	void SetCoordsData();
	int GetNumHeightmapRows();
	int GetNumHeightmapCols();
	int counter = 0;
	static CShaderProgram* GetShaderProgram();

	CMultiLayeredHeightmap();


private:
	unsigned int uiVAO;

	bool bLoaded;
	bool bShaderProgramLoaded;
	int iRows;
	int iCols;
	glm::vec3 vRenderScale;
	 vector< vector< glm::vec3> > vVertexData;
	 vector< vector< glm::vec2> > vCoordsData;
	CVertexBufferObject vboHeightmapData;
	CVertexBufferObject vboHeightmapIndices;
	CMultiLayeredHeightmap *Heightmap;
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	static CShaderProgram spTerrain;
	static CShader shTerrainShaders[NUMTERRAINSHADERS];
};

