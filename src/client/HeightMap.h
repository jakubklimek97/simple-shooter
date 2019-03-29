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

#define FOR(q,n) for(int q=0;q<n;q++)
#define SFOR(q,s,e) for(int q=s;q<=e;q++)
#define RFOR(q,n) for(int q=n;q>=0;q--)
#define RSFOR(q,s,e) for(int q=s;q>=e;q--)

#define ESZ(elem) (int)elem.size()


class CMultiLayeredHeightmap
{
public:
	static bool LoadTerrainShaderProgram();
	static void ReleaseTerrainShaderProgram();

	bool LoadHeightMapFromImage2(const char *path, const std::string &directory);
	void ReleaseHeightmap();

	void RenderHeightmap();

	void SetRenderSize(float fQuadSize, float fHeight);
	void SetRenderSize(float fRenderX, float fHeight, float fRenderZ);

	int GetNumHeightmapRows();
	int GetNumHeightmapCols();

	static CShaderProgram* GetShaderProgram();

	CMultiLayeredHeightmap();

private:
	UINT uiVAO;

	bool bLoaded;
	bool bShaderProgramLoaded;
	int iRows;
	int iCols;

	glm::vec3 vRenderScale;

	CVertexBufferObject vboHeightmapData;
	CVertexBufferObject vboHeightmapIndices;

	static CShaderProgram spTerrain;
	static CShader shTerrainShaders[NUMTERRAINSHADERS];
};;

