#pragma once
#include <ctime>
#include <windows.h>

#include <algorithm>
#include <cmath>
#include <cstring>
#include <vector>
#include <sstream>
#include <queue>
#include <map>
#include <set>

using namespace std;

#include <gl/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
/********************************

Class:		CVertexBufferObject

Purpose:	Wraps OpenGL vertex buffer
			object.

********************************/

class CVertexBufferObject
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

	int GetCurrentSize();

	CVertexBufferObject();

private:
	unsigned uiBuffer;
	int iSize;
	int iCurrentSize;
	int iBufferType;
	std::vector<BYTE> data;

	bool bDataUploaded;
};