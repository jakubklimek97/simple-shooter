#pragma once
#include"GL/glew.h"
#include"SDL_image.h"
#include"Shader.h"
#include<vector>
#include<iterator>
#include<algorithm>
using namespace std;



class HUD
{
public:

	struct Calculate {
		void operator() ( Shader Program) {
			Program.use();
			Program.setInt("texture2", 1);
		}
	}StructShader;

	HUD();
	HUD(vector<reference_wrapper<const Shader>>LifeShader,const string &heart,const string &container,float Rotation, int Lifeamount);
	~HUD();
	void InitBuffer();
	unsigned int LoadTexture(const std::string & path, const std::string & directory, bool gamma);
	void RotationIntensity();
	void InitHUD(vector<reference_wrapper<const Shader>>_LifeShader);
	void Incrementvalue();
	void RenderHUD(vector<reference_wrapper<const Shader>>_LifeShader);
	void SetIndices();
	void SetVertices();
	void SetData();
private:
	int _Lifeamount;
	 static int lifenumber;
	 vector<float> vertices;
	vector <int> indices;
	vector<int>HealthID;
	vector<reference_wrapper<const Shader>>_LifeShader;
	vector<reference_wrapper<const Shader>>::iterator LifeIterator;
	static float _incrementvalue;
	unsigned int texture1, texture2, texture3, texture4;
	unsigned int VBO, VAO, EBO;
	static int life; // time before image disappear
	float _gamma;
	bool loaded;
};


