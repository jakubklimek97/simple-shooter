#pragma once
#include <ctime>
#include <algorithm>
#include <cmath>
#include <vector>
#include <sstream>
#include <queue>


using namespace std;

#include <gl/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class HeightMapBuffer
{
public:
	void CreateVBO(int a_iSize = 0);
	void DeleteVBO();

	void* MapBufferToMemory(int iUsageHint);
	void* MapSubBufferToMemory(int iUsageHint, unsigned uiOffset, unsigned uiLength);
	void UnmapBuffer();

	void BindVBO(int a_iBufferType = GL_ARRAY_BUFFER);
	void UploadDataToGPU(int iUsageHint);

	void AddData(void* ptrData, unsigned uiDataSize);

	void* GetDataPointer();
	unsigned GetBufferID();


	HeightMapBuffer();

private:
	unsigned uiBuffer;
	int iSize;
	int iCurrentSize;
	int iBufferType;
	std::vector<unsigned char> data;

	bool bDataUploaded;
};