#include "Terrain.h"



//void CMultiLayeredHeightmap::SetRenderSize(float fRenderX, float fHeight, float fRenderZ)
//{
//	vRenderScale = glm::vec3(fRenderX, fHeight, fRenderZ);
//}
//
//void CMultiLayeredHeightmap::SetRenderSize(float fQuadSize, float fHeight)
//{
//	vRenderScale = glm::vec3(float(iCols)*fQuadSize, fHeight, float(iRows)*fQuadSize);
//}


//bool Terrain::LoadTerrainShaderProgram()
//{
//	bool bOK = true;
//	bOK &= shShaders[0].LoadShader("data\\shaders\\terrain.vert", GL_VERTEX_SHADER);
//	bOK &= shShaders[1].LoadShader("data\\shaders\\terrain.frag", GL_FRAGMENT_SHADER);
//	bOK &= shShaders[2].LoadShader("data\\shaders\\dirLight.frag", GL_FRAGMENT_SHADER);
//
//	spTerrain.CreateProgram();
//	FOR(i, NUMTERRAINSHADERS)spTerrain.AddShaderToProgram(&shShaders[i]);
//	spTerrain.LinkProgram();
//
//	return bOK;
//}
//
//void Terrain::ReleaseTerrainShaderProgram()
//{
//	spTerrain.DeleteProgram();
//	FOR(i, NUMTERRAINSHADERS)shShaders[i].DeleteShader();
//}

int Terrain::GetNumHeightmapRows()
{
	return row;
}

int Terrain::GetNumHeightmapCols()
{
	return col;

}

//void Terrain::RenderHeightMap()
//{
//	spTerrain.UseProgram();
//
//	spTerrain.SetUniform("fRenderHeight", RenderScale.y);
//	spTerrain.SetUniform("fMaxTextureU", float(col)*0.1f);
//	spTerrain.SetUniform("fMaxTextureV", float(row)*0.1f);
//
//	spTerrain.SetUniform("HeightmapScaleMatrix", glm::scale(glm::mat4(1.0), glm::vec3(vRenderScale)));
//
//	// Now we're ready to render - we are drawing set of triangle strips using one call, but we g otta enable primitive restart
//	glBindVertexArray(VAO);
//	glEnable(GL_PRIMITIVE_RESTART);
//	glPrimitiveRestartIndex(row*col);
//
//	int iNumIndices = (row - 1)*col * 2 + row - 1;
//	glDrawElements(GL_TRIANGLE_STRIP, iNumIndices, GL_UNSIGNED_INT, 0);
//
//
//}

Terrain::Terrain()
{
}


Terrain::~Terrain()
{
}
