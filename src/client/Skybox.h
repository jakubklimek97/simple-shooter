#pragma once

#include "TextureClass.h"
#include "vertexBufferObject.h"


/********************************

Class:		CSkybox

Purpose:	Class for using skybox.

********************************/

class CSkybox
{
public:
	void LoadSkybox(string a_sFront, string a_sBack, string a_sLeft, string a_sRight, string a_sTop, string a_sBottom);
	void RenderSkybox();

	void DeleteSkybox();
private:
	UINT uiVAO;
	CVertexBufferObject vboRenderData;
	CTexture tTextures[6];
	string sFront, sBack, sLeft, sRight, sTop, sBottom;
};