#include"HeightMap.h"


CShaderProgram CMultiLayeredHeightmap::spTerrain;
CShader CMultiLayeredHeightmap::shTerrainShaders[NUMTERRAINSHADERS];



CMultiLayeredHeightmap::CMultiLayeredHeightmap()
{
	vRenderScale = glm::vec3(1.0f, 1.0f, 1.0f);
	
}


bool CMultiLayeredHeightmap::LoadHeightMapFromImage(const char *path, const std::string &directory)
{
	if (bLoaded)
	{
		bLoaded = false;
		ReleaseHeightmap();
	}
	std::string filename = std::string(path);

	filename = directory + '/' + filename;


	int width = 1, height = 1, nrComponents = 1;

	SDL_Surface* ptr = IMG_Load(filename.c_str());
	if (!ptr) {
	//	std::cout << "ERROR:LOADING MAP";
		return 0;

	}
	width = ptr->w;
	height = ptr->h;

	nrComponents = ptr->format->BytesPerPixel;
	unsigned int check = ptr->format->BitsPerPixel;
	
	unsigned char *bDataPointer = static_cast<unsigned char*>(ptr->pixels); // static_cast
	iRows = width;
	iCols = height;


	GLenum format;
	if (nrComponents == 1)
		format = GL_RED;
	else if (nrComponents == 3)
		format = GL_RGB;
	else if (nrComponents == 4)
		format = GL_RGBA;

	// How much to increase data pointer to get to next pixel data
	unsigned int ptr_inc = nrComponents;
	// Length of one row in data
	unsigned int row_step = ptr_inc * iCols;

	vboHeightmapData.CreateVBO();

	SetVertexData();
	SetCoordsData();
	float fTextureU = float(iCols)*0.1f;
	float fTextureV = float(iRows)*0.1f;

	 for(int i = 0;i<iRows;++i)
	 {
		for(int j = 0; j<iCols;++j)
		{
			float fScaleC = float(j) / float(iCols - 1);
			float fScaleR = float(i) / float(iRows - 1);
			float fVertexHeight = float(*(bDataPointer + row_step * i + j * ptr_inc)) / 255.0f;
			vVertexData[i][j] = glm::vec3(-0.5f + fScaleC, fVertexHeight, -0.5f + fScaleR);
			vCoordsData[i][j] = glm::vec2(fTextureU*fScaleC, fTextureV*fScaleR);
		}
	}

	// Normals are here - the heightmap contains ( (iRows-1)*(iCols-1) quads, each one containing 2 triangles, therefore array of we have 3D array)
	vector< vector<glm::vec3> > vNormals[2];

	    for(int i = 0;i<2;++i)
		vNormals[i] = vector< vector<glm::vec3> >(iRows - 1, vector<glm::vec3>(iCols - 1));

	 for(int i = 0;i<iRows-1;++i)
	{
	 for(int j = 0;j<iCols-1;++j)
		{
			glm::vec3 vTriangle0[] =
			{
				vVertexData[i][j],
				vVertexData[i + 1][j],
				vVertexData[i + 1][j + 1]
			};
			glm::vec3 vTriangle1[] =
			{
				vVertexData[i + 1][j + 1],
				vVertexData[i][j + 1],
				vVertexData[i][j]
			};

			glm::vec3 vTriangleNorm0 = glm::cross(vTriangle0[0] - vTriangle0[1], vTriangle0[1] - vTriangle0[2]);
			glm::vec3 vTriangleNorm1 = glm::cross(vTriangle1[0] - vTriangle1[1], vTriangle1[1] - vTriangle1[2]);

			vNormals[0][i][j] = glm::normalize(vTriangleNorm0);
			vNormals[1][i][j] = glm::normalize(vTriangleNorm1);
		}
	}

vector< vector<glm::vec3> > vFinalNormals = vector< vector<glm::vec3> >(iRows, vector<glm::vec3>(iCols));

	for(int i = 0;i<iRows;++i)
	  for(int j = 0 ;j<iCols;++j)
	{
		// Now we wanna calculate final normal for [i][j] vertex. We will have a look at all triangles this vertex is part of, and then we will make average vector
		// of all adjacent triangles' normals

		glm::vec3 vFinalNormal = glm::vec3(0.0f, 0.0f, 0.0f);

		// Look for upper-left triangles
		if (j != 0 && i != 0)
			 for(int k =0; k<2;++k)
			vFinalNormal += vNormals[k][i - 1][j - 1];
		// Look for upper-right triangles
		if (i != 0 && j != iCols - 1)vFinalNormal += vNormals[0][i - 1][j];
		// Look for bottom-right triangles
		if (i != iRows - 1 && j != iCols - 1)
			for(int k =0;k<2;++k)
				vFinalNormal += vNormals[k][i][j];
		// Look for bottom-left triangles
		if (i != iRows - 1 && j != 0)
			vFinalNormal += vNormals[1][i][j - 1];
		vFinalNormal = glm::normalize(vFinalNormal);

		vFinalNormals[i][j] = vFinalNormal; // Store final normal of j-th vertex in i-th row
	}

	// First, create a VBO with only vertex data
	vboHeightmapData.CreateVBO(iRows*iCols*(2 * sizeof(glm::vec3) + sizeof(glm::vec2))); // Preallocate memory
	for(int i = 0;i<iRows;++i)
	{
		for(int j =0;j<iCols;++j)
		{
			vboHeightmapData.AddData(&vVertexData[i][j], sizeof(glm::vec3)); // Add vertex
			vboHeightmapData.AddData(&vCoordsData[i][j], sizeof(glm::vec2)); // Add tex. coord
			vboHeightmapData.AddData(&vFinalNormals[i][j], sizeof(glm::vec3)); // Add normal
		}
	}
	// Now create a VBO with heightmap indices
	vboHeightmapIndices.CreateVBO();
	int iPrimitiveRestartIndex = iRows * iCols;
    for(int i = 0; i<iRows-1;++i)
	{
		for(int j =0;j<iCols;j++)
			for(int z = 0;z<2;++z)
		{
			int iRow = i + (1 - z);
			int iIndex = iRow * iCols + j;
			vboHeightmapIndices.AddData(&iIndex, sizeof(int));
		
		}
		// Restart triangle strips
		vboHeightmapIndices.AddData(&iPrimitiveRestartIndex, sizeof(int));
	}

	glGenVertexArrays(1, &uiVAO);
	glBindVertexArray(uiVAO);
	// Attach vertex data to this VAO
	vboHeightmapData.BindVBO();
	vboHeightmapData.UploadDataToGPU(GL_STATIC_DRAW);
	
	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3) + sizeof(glm::vec2), 0);
	// Texture coordinates
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3) + sizeof(glm::vec2), (void*)sizeof(glm::vec3));

	// Normal vectors
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3) + sizeof(glm::vec2), (void*)(sizeof(glm::vec3) + sizeof(glm::vec2)));

	// And now attach index data to this VAO
	// Here don't forget to bind another type of VBO - the element array buffer, or simplier indices to vertices
	vboHeightmapIndices.BindVBO(GL_ELEMENT_ARRAY_BUFFER);
	vboHeightmapIndices.UploadDataToGPU(GL_STATIC_DRAW);
	bLoaded = true; // If get here, we succeeded with generating heightmap
	return true;
}


bool CMultiLayeredHeightmap::LoadTerrainShaderProgram()
{
	bool bOK = true;
	bOK &= shShaders[0].LoadShader("res\\shaders\\terrain.vert", GL_VERTEX_SHADER);
	bOK &= shShaders[1].LoadShader("res\\shaders\\terrain.frag", GL_FRAGMENT_SHADER);
	bOK &= shShaders[2].LoadShader("res\\shaders\\dirLight.frag", GL_FRAGMENT_SHADER);

	spTerrain.CreateProgram();

	   for (int i = 0; i < NUMTERRAINSHADERS; ++i)
		spTerrain.AddShaderToProgram(&shShaders[i]);

	spTerrain.LinkProgram();

	return bOK;
}


void CMultiLayeredHeightmap::SetRenderSize(float fRenderX, float fHeight, float fRenderZ)
{
	vRenderScale = glm::vec3(fRenderX, fHeight, fRenderZ);
}

vector< vector< glm::vec3> > CMultiLayeredHeightmap::GetVertexData()
{
	return vVertexData;
}

void CMultiLayeredHeightmap::SetVertexData()
{
  vVertexData.assign(iRows, vector<glm::vec3>(iCols));
}

vector<vector<glm::vec2>> CMultiLayeredHeightmap::GetCoordsData()
{
	return vCoordsData;
}

void CMultiLayeredHeightmap::SetCoordsData()
{
	vCoordsData.assign(iRows, vector<glm::vec2>(iCols));
}



void CMultiLayeredHeightmap::SetRenderSize(float fQuadSize, float fHeight)
{
	vRenderScale = glm::vec3(float(iCols)*fQuadSize, fHeight, float(iRows)*fQuadSize);
}


void CMultiLayeredHeightmap::RenderHeightmap()
{
	
	spTerrain.UseProgram();

	spTerrain.SetUniform("fRenderHeight", vRenderScale.y);
	spTerrain.SetUniform("fMaxTextureU", float(iCols)*0.1f);
	spTerrain.SetUniform("fMaxTextureV", float(iRows)*0.1f);

	spTerrain.SetUniform("HeightmapScaleMatrix",GetScaleMatrix());

	// Now we're ready to render - we are drawing set of triangle strips using one call, but we g otta enable primitive restart
	glBindVertexArray(uiVAO);
	glEnable(GL_PRIMITIVE_RESTART);
	glPrimitiveRestartIndex(iRows*iCols);
	int iNumIndices = (iRows - 1)*iCols * 2 + iRows - 1;
	glDrawElements(GL_TRIANGLE_STRIP, iNumIndices, GL_UNSIGNED_INT, 0);

}


void CMultiLayeredHeightmap::ReleaseHeightmap()
{
	if (!bLoaded)
		return; // Heightmap must be loaded
	vboHeightmapData.DeleteVBO();
	vboHeightmapIndices.DeleteVBO();
	glDeleteVertexArrays(1, &uiVAO);
	bLoaded = false;
}

bool CMultiLayeredHeightmap::CheckCollision(glm::vec3 cameraPos)
{

	//for(int i =0;i<vVertexData.size();++i)
	//	for (int j = 0; j < vVertexData.size(); ++j)
	//	{
	//		auto tmp = vVertexData[i][j];
	//		if (tmp.x == cameraPos.x && tmp.y == cameraPos.y && tmp.z == cameraPos.z)
	//		{
	//			cameraPos.x = cameraPos.x + 2.0f;
	//			return true;
	//		}

	//	}
	return false;
}

CShaderProgram* CMultiLayeredHeightmap::GetShaderProgram()
{
	return &spTerrain;
}

void CMultiLayeredHeightmap::ReleaseTerrainShaderProgram()
{
	spTerrain.DeleteProgram();
	 for(int i =0;i<NUMTERRAINSHADERS;++i)
		shShaders[i].DeleteShader();
}


int CMultiLayeredHeightmap::GetNumHeightmapRows()
{
	return iRows;
}

int CMultiLayeredHeightmap::GetNumHeightmapCols()
{
	return iCols;
}

void CMultiLayeredHeightmap::RenderHeightmapForNormals()

{
	
		// Drawing set of triangle strips using one call, but we g otta enable primitive restart
		
		glBindVertexArray(uiVAO);
	
		glEnable(GL_PRIMITIVE_RESTART);
	
		glPrimitiveRestartIndex(iRows*iCols);

	
		int iNumIndices = (iRows - 1)*iCols * 2 + iRows - 1;
	
		glDrawElements(GL_POINTS, iNumIndices, GL_UNSIGNED_INT, 0);

}

glm::mat4 CMultiLayeredHeightmap::GetScaleMatrix() {

	return glm::scale(glm::mat4(1.0), glm::vec3(vRenderScale));
}