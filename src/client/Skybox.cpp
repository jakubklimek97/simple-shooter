
#include "skybox.h"
#include"vertexBufferObject.h"
#include<algorithm>
#include<iterator>
#include<list>

#define _USE_MATH_DEFINES

float CSkybox::roatationspeed = 1.0f;

CSkybox::CSkybox()
{
	ReserveVector(6);
	vector<string>::const_iterator it;
	for ( it = faces.begin(); it != faces.end(); ++it)
		faces.push_back(" ");

}

CSkybox::CSkybox(Shader&Program,string a_sFront, string a_sBack, string a_sLeft, string a_sRight, string a_sTop, string a_sBottom) {
	
	vector <string> TextureNames = { a_sFront,a_sBack,a_sLeft,a_sRight,a_sTop,a_sBottom };

	LoadSkyBoxVector(TextureNames);


      SkyBoxVertices = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	  LoadCubeMap(faces);
	  BindBuffer(Program);
	 
}


void CSkybox::LoadSkyBoxVector(vector<string> vec)
{
	copy (vec.begin(),vec.end(), back_inserter(faces));

}
	


//void CSkybox::DeleteSkybox() 
//{
//	for (int i = 0; i < 6; ++i)
//		tTextures[i].DeleteTexture();
//	glDeleteVertexArrays(1, &uiVAO);
//	//vboRenderData.DeleteVBO();
//}

void CSkybox::LoadCubeMap(vector<string> faces)
{

	glGenTextures(1, &skyboxID);
	glBindTexture(GL_TEXTURE_2D, skyboxID);

	int width, height;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		SDL_Surface*ptr = IMG_Load(faces[i].c_str());
		width = ptr->w;
		height = ptr->h;
		if (ptr)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, ptr->pixels);
			SDL_FreeSurface(ptr);
		}
		else
		{
			LoadCube = false;
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	
	LoadCube = true;



}

void CSkybox::BindBuffer(Shader&Program)
{
	

	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(SkyBoxVertices), &SkyBoxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	
	Program.use();
	Program.setInt("skybox", 0);
}

void CSkybox::RenderSkybox(Shader&Program, glm::mat4 ViewMatrix, glm::mat4 ProjectionMatrix)
{
	glDepthFunc(GL_LEQUAL);
	
		// change depth function so depth test passes when values are equal to depth buffer's content
	Program.use();
	
	float rotation =+ 10.0f*roatationspeed * _deltatime;
	glm::mat4 view = ViewMatrix;
	view[3][0] = 0;
	view[3][1] = 0;
	view[3][2] = 0;
	float Radian = static_cast<float>(DegreetoRadians(rotation));
	 glm::mat4 view2 = glm::rotate(ViewMatrix, glm::radians(rotation), glm::vec3( 0,1,0 ));
	 glm::mat4 view3 = glm::mat4(glm::mat3(view2));
	//glm::rotate(DegreetoRadians(rotation), glm::vec3(0, 1, 0), ViewMatrix, ViewMatrix);
	glm::vec3 fogColour = { r = 0.7f,g = 0.7f,b = 0.7f };// remove translation from the view matrix
	Program.setMat4("view", view3);
	Program.setMat4("projection", ProjectionMatrix);
	Program.setVec3("fogColour", fogColour);
	// skybox cube
	glBindVertexArray(skyboxVAO);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxID);
	glActiveTexture(GL_TEXTURE0);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS);
}

void CSkybox::SetDeltatime(float deltatime)
{
	_deltatime = deltatime;
}

double CSkybox::DegreetoRadians(double degree)
{
	double radian = degree * M_PI / 180;
	return radian;
}

vector<string> CSkybox::ReserveVector(int size)
{
	faces.reserve(6);
	return faces;
}