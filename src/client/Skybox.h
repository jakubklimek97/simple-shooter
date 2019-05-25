#pragma once
#include<array>
#include<iterator>
#include "TextureClass.h"
#include "vertexBufferObject.h"
#include"Shader.h"
#include<iostream>
#include<math.h>
class CSkybox
{
public:
	CSkybox();
	CSkybox(Shader&Program,string a_sFront, string a_sBack, string a_sLeft, string a_sRight, string a_sTop, string a_sBottom);
	void LoadSkyBoxVector(vector<string> vec);
//	void DeleteSkybox();
	void LoadCubeMap(vector<string> faces);
	void BindBuffer(Shader&Program);
	void RenderSkybox(Shader&rogram,glm::mat4 ViewMatrix, glm::mat4 ProjectionMatrix);
	void SetDeltatime(float deltatime);
	double DegreetoRadians(double degree);
private:
 vector<string> ReserveVector(int size);
	bool Skyloaded;
	bool LoadCube;
	unsigned int skyboxVAO, skyboxVBO;
	vector<string> faces;
	array<float, 109> SkyBoxVertices;
	unsigned int skyboxID;
	float r, g, b;
    static float roatationspeed;
	float _deltatime;
	double pi = 3.14;
	
};