#pragma warning (disable:4996)
#include "shaders.h"
#include <glm/gtc/type_ptr.hpp>

CShader::CShader()
{
	bLoaded = false;
}

CShader shShaders[NUMSHADERS];
ShaderProgram spMain, spFogAndLight, test;


bool PrepareShaderPrograms()
{
	// Load shaders and create shader program
	string sShaderFileNames[] = { "main_shader.vert", "main_shader.frag", "dirLight.frag",
		"spotLight.frag","fog.frag","shader_fog.frag","shader_fog.vert"
	};

   for(int i =0;i<NUMSHADERS;++i)
	{
		string sExt = sShaderFileNames[i].substr((int) sShaderFileNames[i].size() - 4, 4);
		int iShaderType = sExt == "vert" ? GL_VERTEX_SHADER : (sExt == "frag" ? GL_FRAGMENT_SHADER : GL_GEOMETRY_SHADER);
		shShaders[i].LoadShader("res\\shaders\\" + sShaderFileNames[i], iShaderType);
	}

	// Create shader programs
	spMain.CreateProgram();
	spMain.AddShaderToProgram(&shShaders[0]);
	spMain.AddShaderToProgram(&shShaders[1]);
	spMain.AddShaderToProgram(&shShaders[2]);
	spMain.AddShaderToProgram(&shShaders[3]); // najprawdopodobniej nie bedzie korzystane
	spMain.LinkProgram();

	spFogAndLight.CreateProgram();
	spFogAndLight.AddShaderToProgram(&shShaders[4]);
	spFogAndLight.AddShaderToProgram(&shShaders[5]);
	spFogAndLight.AddShaderToProgram(&shShaders[6]);
	spFogAndLight.LinkProgram();





	return true;
}



bool CShader::LoadShader(string sFile, int a_iType)
{
	vector<string> sLines;

	if (!GetLinesFromFile(sFile, false, &sLines))return false;

	const char** sProgram = new const char*[(int) sLines.size()];
	for(int i = 0;i<(int)sLines.size();++i) 
		sProgram[i] = sLines[i].c_str();

	uiShader = glCreateShader(a_iType);

	glShaderSource(uiShader,(int)(sLines.size()), sProgram, NULL);
	glCompileShader(uiShader);

	delete[] sProgram;

	int iCompilationStatus;
	glGetShaderiv(uiShader, GL_COMPILE_STATUS, &iCompilationStatus);

	if (iCompilationStatus == GL_FALSE)
	{
		char sInfoLog[1024];
//		char sFinalMessage[1536];
		int iLogLength;
		glGetShaderInfoLog(uiShader, 1024, &iLogLength, sInfoLog);
		//sprintf(sFinalMessage, "Error! Shader file %s wasn't compiled! The compiler returned:\n\n%s", sFile.c_str(), sInfoLog);
//		MessageBox(NULL, sFinalMessage, "Error", MB_ICONERROR);
		return false;
	}
	iType = a_iType;
	bLoaded = true;

	return true;

}


bool CShader::GetLinesFromFile(string File, bool IncludePart, vector<string>* Result)
{

	FILE* fp = fopen(File.c_str(), "rt");
	if (!fp)return false; // File not loaded

	string Directory;
	int slashIndex = -1;
	for (int i = File.size(); i >= 0;--i)
	{
		if (File[i] == '\\' || File[i] == '/')
		{
			slashIndex = i;
			break;
		}
	}

	Directory = File.substr(0, slashIndex + 1);

	// Get all lines from a file

	char Line[255];

	bool IncludePartCheck = false;

	while (fgets(Line, 255, fp))
	{
		stringstream ss(Line);
		string FirstLine;
		ss >> FirstLine;
		if (FirstLine == "#include")
		{
			string NameFile;
			ss >> NameFile;
			if ((int)(NameFile.size()) > 0 && NameFile[0] == '\"' && NameFile[(int)(NameFile.size())- 1] == '\"')
			{
				NameFile = NameFile.substr(1, int(NameFile.size()) - 2);
				GetLinesFromFile(Directory + NameFile, true, Result);
			}
		}
		else if (FirstLine == "#include_part")
			IncludePartCheck = true;
		else if (FirstLine == "#definition_part")
			IncludePartCheck = false;
		else if (!IncludePart || (IncludePart && IncludePartCheck))
			Result->push_back(Line);
	}
	fclose(fp);

	return true;
}


bool CShader::IsLoaded()
{
	return bLoaded;
}


unsigned int CShader::GetShaderID()
{
	return uiShader;
}



void CShader::DeleteShader()
{
	if (!IsLoaded())return;
	bLoaded = false;
	glDeleteShader(uiShader);
}

ShaderProgram::ShaderProgram()
{
	bLinked = false;
}

void ShaderProgram::CreateProgram()
{
	uiProgram = glCreateProgram();
}



bool ShaderProgram::AddShaderToProgram(CShader* shShader)
{
	if (!shShader->IsLoaded())return false;
	glAttachShader(uiProgram, shShader->GetShaderID());
	return true;
}


bool ShaderProgram::LinkProgram()
{
	glLinkProgram(uiProgram);
	int iLinkStatus;
	glGetProgramiv(uiProgram, GL_LINK_STATUS, &iLinkStatus);
	bLinked = iLinkStatus == GL_TRUE;
	return bLinked;
}

void ShaderProgram::DeleteProgram()
{
	if (!bLinked)return;
	bLinked = false;
	glDeleteProgram(uiProgram);
}


void ShaderProgram::UseProgram()
{
	if (bLinked)glUseProgram(uiProgram);
}

unsigned ShaderProgram::GetProgramID()
{
	return uiProgram;
}



// Setting floats

void ShaderProgram::SetUniform(string sName, float* fValues, int iCount)
{
	int iLoc = glGetUniformLocation(uiProgram, sName.c_str());
	glUniform1fv(iLoc, iCount, fValues);
}

void ShaderProgram::SetUniform(string sName, const float fValue)
{
	int iLoc = glGetUniformLocation(uiProgram, sName.c_str());
	glUniform1fv(iLoc, 1, &fValue);
}

// Setting vectors

void ShaderProgram::SetUniform(string sName, glm::vec2* vVectors, int iCount)
{
	int iLoc = glGetUniformLocation(uiProgram, sName.c_str());
	glUniform2fv(iLoc, iCount, (GLfloat*)vVectors);
}

void ShaderProgram::SetUniform(string sName, const glm::vec2 vVector)
{
	int iLoc = glGetUniformLocation(uiProgram, sName.c_str());
	glUniform2fv(iLoc, 1, (GLfloat*)&vVector);
}

void ShaderProgram::SetUniform(string sName, glm::vec3* vVectors, int iCount)
{
	int iLoc = glGetUniformLocation(uiProgram, sName.c_str());
	glUniform3fv(iLoc, iCount, (GLfloat*)vVectors);
}

void ShaderProgram::SetUniform(string sName, const glm::vec3 vVector)
{
	int iLoc = glGetUniformLocation(uiProgram, sName.c_str());
	glUniform3fv(iLoc, 1, (GLfloat*)&vVector);
}

void ShaderProgram::SetUniform(string sName, glm::vec4* vVectors, int iCount)
{
	int iLoc = glGetUniformLocation(uiProgram, sName.c_str());
	glUniform4fv(iLoc, iCount, (GLfloat*)vVectors);
}

void ShaderProgram::SetUniform(string sName, const glm::vec4 vVector)
{
	int iLoc = glGetUniformLocation(uiProgram, sName.c_str());
	glUniform4fv(iLoc, 1, (GLfloat*)&vVector);
}

// Setting 3x3 matrices

void ShaderProgram::SetUniform(string sName, glm::mat3* mMatrices, int iCount)
{
	int iLoc = glGetUniformLocation(uiProgram, sName.c_str());
	glUniformMatrix3fv(iLoc, iCount, false, (GLfloat*)mMatrices);
}

void ShaderProgram::SetUniform(string sName, const glm::mat3 mMatrix)
{
	int iLoc = glGetUniformLocation(uiProgram, sName.c_str());
	glUniformMatrix3fv(iLoc, 1, false, (GLfloat*)&mMatrix);
}

// Setting 4x4 matrices

void ShaderProgram::SetUniform(string sName, glm::mat4* mMatrices, int iCount)
{
	int iLoc = glGetUniformLocation(uiProgram, sName.c_str());
	glUniformMatrix4fv(iLoc, iCount, false, (GLfloat*)mMatrices);
}

void ShaderProgram::SetUniform(string sName, const glm::mat4 mMatrix)
{
	int iLoc = glGetUniformLocation(uiProgram, sName.c_str());
	glUniformMatrix4fv(iLoc, 1, false, (GLfloat*)&mMatrix);
}

// Setting integers

void ShaderProgram::SetUniform(string sName, int* iValues, int iCount)
{
	int iLoc = glGetUniformLocation(uiProgram, sName.c_str());
	glUniform1iv(iLoc, iCount, iValues);
}

void ShaderProgram::SetUniform(string sName, const int iValue)
{
	int iLoc = glGetUniformLocation(uiProgram, sName.c_str());
	glUniform1i(iLoc, iValue);
}

void ShaderProgram::SetModelAndNormalMatrix(string sModelMatrixName, string sNormalMatrixName, glm::mat4 mModelMatrix)
{
	SetUniform(sModelMatrixName, mModelMatrix);
	SetUniform(sNormalMatrixName, glm::transpose(glm::inverse(mModelMatrix)));
}

void ShaderProgram::SetModelAndNormalMatrix(string sModelMatrixName, string sNormalMatrixName, glm::mat4* mModelMatrix)
{
	SetUniform(sModelMatrixName, mModelMatrix);
	SetUniform(sNormalMatrixName, glm::transpose(glm::inverse(*mModelMatrix)));
}