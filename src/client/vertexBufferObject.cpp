
#include "vertexBufferObject.h"

CVertexBufferObject::CVertexBufferObject()
{
	bDataUploaded = false;
	uiBuffer = 0;
}



void CVertexBufferObject::CreateVBO(int a_iSize)
{
	glGenBuffers(1, &uiBuffer);
	data.reserve(a_iSize);
	iSize = a_iSize;
	iCurrentSize = 0;
}



void CVertexBufferObject::DeleteVBO()
{
	glDeleteBuffers(1, &uiBuffer);
	bDataUploaded = false;
	data.clear();
}


void* CVertexBufferObject::MapBufferToMemory(int iUsageHint)
{
	if (!bDataUploaded)return NULL;
	void* ptrRes = glMapBuffer(iBufferType, iUsageHint);
	return ptrRes;
}



void* CVertexBufferObject::MapSubBufferToMemory(int iUsageHint, unsigned int uiOffset, unsigned int uiLength)
{
	if (!bDataUploaded)return NULL;
	void* ptrRes = glMapBufferRange(iBufferType, uiOffset, uiLength, iUsageHint);
	return ptrRes;
}



void CVertexBufferObject::UnmapBuffer()
{
	glUnmapBuffer(iBufferType);
}



void CVertexBufferObject::BindVBO(int a_iBufferType)
{
	iBufferType = a_iBufferType;
	glBindBuffer(iBufferType, uiBuffer);
}



void CVertexBufferObject::UploadDataToGPU(int iDrawingHint)
{
	glBufferData(iBufferType, data.size(), &data[0], iDrawingHint);
	bDataUploaded = true;
	data.clear();
}



void CVertexBufferObject::AddData(void* ptrData, unsigned int uiDataSize)
{
	data.insert(data.end(), (unsigned char*)ptrData, (unsigned char*)ptrData + uiDataSize);
	iCurrentSize += uiDataSize;
}



void* CVertexBufferObject::GetDataPointer()
{
	if (bDataUploaded)return NULL;
	return (void*)data[0];
}



unsigned int CVertexBufferObject::GetBufferID()
{
	return uiBuffer;
}



int CVertexBufferObject::GetCurrentSize()
{
	return iCurrentSize;
}