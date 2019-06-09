#include "Fog.h"



Fog::Fog()
{
	_FogColor = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
	_FogDensity = 0.05f;
	_Equation = FogEquation::EXP;
}

Fog::Fog(glm::vec4 FogColor, float FogDensity, int Equation)
{
	_FogColor = FogColor;
	_FogDensity = FogDensity;
	_Equation = Equation;
}

void Fog::SetUniformData(ShaderProgram * Program, string FogName)
{
	Program->SetUniform(FogName + ".vColor", _FogColor);
	Program->SetUniform(FogName + ".fAmbientIntensity", _AmbientIntensity);
	Program->SetUniform(FogName + ".fDensity", _FogDensity);
	Program->SetUniform(FogName + ".iEquation", _Equation);
	Program->SetUniform(FogName + "gSampler", 0);
}

void Fog::SetUniformData(ShaderProgram * Program, glm::mat4 ModelMatrix, glm::mat4 ProjectionMatrix,glm::vec3 CameraPosition)
{
	Program->SetUniform("matrices.modelViewMatrix", &ModelMatrix);
	Program->SetUniform("matrices.modelViewMatrix", glm::translate(ModelMatrix, CameraPosition));
}
