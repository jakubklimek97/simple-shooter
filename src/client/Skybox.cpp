
#include "skybox.h"
#include"vertexBufferObject.h"
#include<algorithm>
#include<iterator>
#include<list>
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
	Program.use();

	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(SkyBoxVertices), &SkyBoxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
}

void CSkybox::RenderSkybox(Shader&Program, glm::mat4 ViewMatrix, glm::mat4 ProjectionMatrix)
{
	glDepthFunc(GL_LEQUAL);

		// change depth function so depth test passes when values are equal to depth buffer's content
	Program.use();
	glm::mat4 view = glm::mat4(glm::mat3(ViewMatrix)); // remove translation from the view matrix
	Program.setMat4("view", view);
	Program.setMat4("projection",ProjectionMatrix);
	// skybox cube
	glBindVertexArray(skyboxVAO);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxID);
	glActiveTexture(GL_TEXTURE0);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS);
}

vector<string> CSkybox::ReserveVector(int size)
{
	faces.reserve(6);
	return faces;
}