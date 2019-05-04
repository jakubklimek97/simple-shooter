#pragma once
#include<array>
#include<iterator>
#include "TextureClass.h"
#include "vertexBufferObject.h"
#include<iostream>
class CSkybox
{
public:
	CSkybox();
	void LoadSkybox(string a_sFront, string a_sBack, string a_sLeft, string a_sRight, string a_sTop, string a_sBottom);
	CSkybox(string a_sFront, string a_sBack, string a_sLeft, string a_sRight, string a_sTop, string a_sBottom);
	void LoadSkyBoxVector(vector<string> vec);
	void DeleteSkybox();
	void LoadCubeMap(vector<string> faces);
	void BindBuffer();
private:
	bool Skyloaded;
	bool LoadCube;

	unsigned int uiVAO, skyboxVAO, skyboxVBO;
	string sFront, sBack, sLeft, sRight, sTop, sBottom;
	vector<string> faces;
	array<float, 109> SkyBoxVertices;
	
};