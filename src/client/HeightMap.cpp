#include"HeightMap.h"


ShaderProgram HeightMap::spTerrain;
CShader HeightMap::shTerrainShaders[NUMTERRAINSHADERS];



HeightMap::HeightMap()
{
	vRenderScale = glm::vec3(1.0f, 1.0f, 1.0f);

}


bool HeightMap::LoadMapFromImage(const char *path, const std::string &directory)
{
	if (bLoaded)
	{
		bLoaded = false;
		ReleaseHeightmap();
	}
	std::string filename = std::string(path);

	filename = directory + '/' + filename;


	int nrComponents = 1;

	SDL_Surface* ptr = IMG_Load(filename.c_str());
	if (!ptr) {
		//	std::cout << "ERROR:LOADING MAP";
		return 0;

	}
	width = ptr->w;
	height = ptr->h;

	nrComponents = ptr->format->BytesPerPixel;
	unsigned int check = ptr->format->BitsPerPixel;

	unsigned char *PointerData = static_cast<unsigned char*>(ptr->pixels); // static_cast
	Rows = width;
	Columns = height;


	GLenum format;
	if (nrComponents == 1)
		format = GL_RED;
	else if (nrComponents == 3)
		format = GL_RGB;
	else if (nrComponents == 4)
		format = GL_RGBA;

	// How much to increase data pointer to get to next pixel data
	unsigned int pointer_incrementation = nrComponents;
	// Length of one row in data
	unsigned int step_row = pointer_incrementation * Columns;

	vboHeightmapData.CreateVBO();

	SetVertexData();
	SetCoordsData();
	SetHeightsData();
	float TextureCol = static_cast<float>(Columns)*0.1f;
	float TextureRow = static_cast<float>(Rows)*0.1f;

	for (int i = 0; i < Rows; ++i)
	{
		for (int j = 0; j < Columns; ++j)
		{
			float ScaleColumns = static_cast<float>(j) / static_cast<float>(Columns - 1)*500.0f;
			float ScaleRows = static_cast<float>(i) / static_cast<float>(Rows - 1) * 500.0f;
			float HeightValue = (static_cast<float>(*(PointerData + step_row * i + j * pointer_incrementation)) / 255.0f)*50.0f;
			Heihgts[j][i] = HeightValue;
			DataVer[i][j] = glm::vec3(ScaleColumns, HeightValue, ScaleRows);
			DataCoordinate[i][j] = glm::vec2((TextureCol*ScaleColumns) / 100.0f, (TextureRow*ScaleRows) / 100.0f);
		}
	}
	//Calculating normals 1 etap
	vector< vector<glm::vec3> > HeightNormal[2];

	for (int i = 0; i < 2; ++i)
		HeightNormal[i] = vector< vector<glm::vec3> >(Rows - 1, vector<glm::vec3>(Columns - 1));

	for (int i = 0; i < Rows - 1; ++i)
	{
		for (int j = 0; j < Columns - 1; ++j)
		{
			glm::vec3 ZeroTriangle[] =
			{
				DataVer[i][j],
				DataVer[i + 1][j],
				DataVer[i + 1][j + 1]
			};
			glm::vec3 FirstTriangle[] =
			{
				DataVer[i + 1][j + 1],
				DataVer[i][j + 1],
				DataVer[i][j]
			};

			glm::vec3 ZeroNormalTriangle = glm::cross(ZeroTriangle[0] - ZeroTriangle[1], ZeroTriangle[1] - ZeroTriangle[2]);
			glm::vec3 FirstNormalTriangle = glm::cross(FirstTriangle[0] - FirstTriangle[1], FirstTriangle[1] - FirstTriangle[2]);

			HeightNormal[0][i][j] = glm::normalize(ZeroNormalTriangle);
			HeightNormal[1][i][j] = glm::normalize(FirstNormalTriangle);
		}
	}
	//FInal Normals
	vector< vector<glm::vec3> > NomalsFinal = vector< vector<glm::vec3> >(Rows, vector<glm::vec3>(Columns));

	for (int i = 0; i < Rows; ++i)
		for (int j = 0; j < Columns; ++j)
		{


			glm::vec3 BufforFinalNormal = glm::vec3(0.0f, 0.0f, 0.0f);

			// upper-left triangle
			if (j != 0 && i != 0)
				for (int k = 0; k < 2; ++k)
					BufforFinalNormal += HeightNormal[k][i - 1][j - 1];
			// upper-right triangle
			if (i != 0 && j != Columns - 1)BufforFinalNormal += HeightNormal[0][i - 1][j];
			// bottom-right triangle
			if (i != Rows - 1 && j != Columns - 1)
				for (int k = 0; k < 2; ++k)
					BufforFinalNormal += HeightNormal[k][i][j];
			// bottom-left triangle
			if (i != Rows - 1 && j != 0)
				BufforFinalNormal += HeightNormal[1][i][j - 1];
			BufforFinalNormal = glm::normalize(BufforFinalNormal);

			NomalsFinal[i][j] = BufforFinalNormal; // Store final normal of j-th vertex in i-th row
		}


	vboHeightmapData.CreateVBO(Rows*Columns*(2 * sizeof(glm::vec3) + sizeof(glm::vec2))); // Preallocate memory
	for (int i = 0; i < Rows; ++i)
	{
		for (int j = 0; j < Columns; ++j)
		{
			vboHeightmapData.AddData(&DataVer[i][j], sizeof(glm::vec3)); // Add vertex
			vboHeightmapData.AddData(&DataCoordinate[i][j], sizeof(glm::vec2)); // Add tex. coord
			vboHeightmapData.AddData(&NomalsFinal[i][j], sizeof(glm::vec3)); // Add normal
		}
	}
	// Now create a VBO with heightmap indices
	vboHeightmapIndices.CreateVBO();
	int iPrimitiveRestartIndex = Rows * Columns;
	for (int i = 0; i < Rows - 1; ++i)
	{
		for (int j = 0; j < Columns; j++)
			for (int z = 0; z < 2; ++z)
			{
				int iRow = i + (1 - z);
				int iIndex = iRow * Columns + j;
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


bool HeightMap::LoadTerrainShaderProgram()
{
	bool Loaded = true;
	Loaded &= shShaders[0].LoadShader("res\\shaders\\terrain.vert", GL_VERTEX_SHADER);
	Loaded &= shShaders[1].LoadShader("res\\shaders\\terrain.frag", GL_FRAGMENT_SHADER);
	Loaded &= shShaders[2].LoadShader("res\\shaders\\dirLight.frag", GL_FRAGMENT_SHADER);
	//bOK &= shShaders[3].LoadShader("res\\shaders\\fog1.frag", GL_FRAGMENT_SHADER);
	spTerrain.CreateProgram();

	for (int i = 0; i < NUMTERRAINSHADERS; ++i)
		spTerrain.AddShaderToProgram(&shShaders[i]);

	spTerrain.LinkProgram();

	return Loaded;
}


void HeightMap::SetRenderSize(float fRenderX, float fHeight, float fRenderZ)
{
	vRenderScale = glm::vec3(fRenderX, fHeight, fRenderZ);
}

void HeightMap::SetHeightsData()
{
	Heihgts.assign(Rows, vector<float>(Columns));
}

vector< vector< glm::vec3> > HeightMap::GetVertexData()
{
	return DataVer;
}

void HeightMap::SetVertexData()
{
	DataVer.assign(Rows, vector<glm::vec3>(Columns));
}

vector<vector<glm::vec2>> HeightMap::GetCoordsData()
{
	return DataCoordinate;
}

void HeightMap::getHeightOfTerrain(float CameraX, float CameraY, float CameraZ)
{
}

void HeightMap::SetCoordsData()
{
	DataCoordinate.assign(Rows, vector<glm::vec2>(Columns));
}



void HeightMap::SetRenderSize(float fQuadSize, float fHeight)
{
	vRenderScale = glm::vec3(float(Columns)*fQuadSize, fHeight, float(Rows)*fQuadSize);
}


void HeightMap::RenderHeightmap(glm::mat4 projection)
{

	spTerrain.UseProgram();

	spTerrain.SetUniform("fRenderHeight", vRenderScale.y);
	spTerrain.SetUniform("fMaxTextureU", float(Columns)*0.1f);
	spTerrain.SetUniform("fMaxTextureV", float(Rows)*0.1f);

	spTerrain.SetUniform("HeightmapScaleMatrix", GetScaleMatrix());

	glBindVertexArray(uiVAO);
	glEnable(GL_PRIMITIVE_RESTART);
	glPrimitiveRestartIndex(Rows*Columns);
	int iNumIndices = (Rows - 1)*Columns * 2 + Rows - 1;

	glDrawElements(GL_TRIANGLE_STRIP, iNumIndices, GL_UNSIGNED_INT, 0);

}


void HeightMap::ReleaseHeightmap()
{
	if (!bLoaded)
		return; // Heightmap must be loaded
	vboHeightmapData.DeleteVBO();
	vboHeightmapIndices.DeleteVBO();
	glDeleteVertexArrays(1, &uiVAO);
	bLoaded = false;
}

float HeightMap::TerrainCollide(float WorldX, float WorldZ)
{
	float gridsize = SIZES / (static_cast<float>(Heihgts.size() - 1));
	int gridX = (static_cast<int>(floor(WorldX / gridsize)));
	int gridZ = (static_cast<int>(floor(WorldZ / gridsize)));
	if (gridX >= Heihgts.size() - 1 || gridZ >= Heihgts.size() - 1 || gridX < 0 || gridZ < 0) {
		return 0;
	}

	float xCoord = std::fmod(WorldX, gridsize) / gridsize;
	float zCoord = std::fmod(WorldZ, gridsize) / gridsize;
	if (xCoord <= (1 - zCoord)) {
		Collide = BarryCentric(
			glm::vec3(0, Heihgts[gridX][gridZ], 0),
			glm::vec3(1, Heihgts[gridX + 1][gridZ], 0),
			glm::vec3(0, Heihgts[gridX][gridZ + 1], 1),
			glm::vec2(xCoord, zCoord));


	}
	else {
		Collide = BarryCentric(
			glm::vec3(1, Heihgts[gridX + 1][gridZ], 0),
			glm::vec3(1, Heihgts[gridX + 1][gridZ + 1], 1),
			glm::vec3(0, Heihgts[gridX][gridZ + 1], 1),
			glm::vec2(xCoord, zCoord));

	}



	return Collide;
}

float HeightMap::BarryCentric(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec2 pos)
{
	float det = (p2.z - p3.z) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.z - p3.z);
	float l1 = ((p2.z - p3.z) * (pos.x - p3.x) + (p3.x - p2.x) * (pos.y - p3.z)) / det;
	float l2 = ((p3.z - p1.z) * (pos.x - p3.x) + (p1.x - p3.x) * (pos.y - p3.z)) / det;
	float l3 = 1.0f - l1 - l2;
	return l1 * p1.y + l2 * p2.y + l3 * p3.y;
}

ShaderProgram* HeightMap::GetShaderProgram()
{
	return &spTerrain;
}

void HeightMap::ReleaseTerrainShaderProgram()
{
	spTerrain.DeleteProgram();
	for (int i = 0; i < NUMTERRAINSHADERS; ++i)
		shShaders[i].DeleteShader();
}


int HeightMap::GetNumHeightmapRows()
{
	return Rows;
}

int HeightMap::GetNumHeightmapCols()
{
	return Columns;
}

glm::mat4 HeightMap::GetScaleMatrix() {

	return glm::scale(glm::mat4(1.0), glm::vec3(vRenderScale));
}