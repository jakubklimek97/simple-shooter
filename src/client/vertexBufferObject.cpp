
#include "vertexBufferObject.h"

HeightMapBuffer::HeightMapBuffer()
{
	bDataUploaded = false;
	//uiBuffer = 0;
}



void HeightMapBuffer::CreateVBO(int a_iSize)
{
	glGenBuffers(1, &uiBuffer);
	data.reserve(a_iSize);
	iSize = a_iSize;
	//iCurrentSize = 0;
}



void HeightMapBuffer::DeleteVBO()
{
	glDeleteBuffers(1, &uiBuffer);
	bDataUploaded = false;
	data.clear();
}


void* HeightMapBuffer::MapBufferToMemory(int iUsageHint)
{
	if (!bDataUploaded)return NULL;
	void* ptrRes = glMapBuffer(iBufferType, iUsageHint);
	return ptrRes;
}



void* HeightMapBuffer::MapSubBufferToMemory(int iUsageHint, unsigned int uiOffset, unsigned int uiLength)
{
	if (!bDataUploaded)return NULL;
	void* ptrRes = glMapBufferRange(iBufferType, uiOffset, uiLength, iUsageHint);
	return ptrRes;
}



void HeightMapBuffer::UnmapBuffer()
{
	glUnmapBuffer(iBufferType);
}



void HeightMapBuffer::BindVBO(int a_iBufferType)
{
	iBufferType = a_iBufferType;
	glBindBuffer(iBufferType, uiBuffer);
}



void HeightMapBuffer::UploadDataToGPU(int iDrawingHint)
{
	// try catch na blad
	glBufferData(iBufferType, data.size(), &data[0], iDrawingHint);
	bDataUploaded = true;
	data.clear();
}



void HeightMapBuffer::AddData(void* ptrData, unsigned int uiDataSize)
{
	data.insert(data.end(), (unsigned char*)ptrData, (unsigned char*)ptrData + uiDataSize);
	iCurrentSize += uiDataSize;
}



void* HeightMapBuffer::GetDataPointer()
{
	if (bDataUploaded)return NULL;
	return (void*)data[0];
}



unsigned int HeightMapBuffer::GetBufferID()
{
	return uiBuffer;
}



