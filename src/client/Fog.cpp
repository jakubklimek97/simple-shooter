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
	Program->SetUniform(FogName + ".vFogColor", _FogColor);
	Program->SetUniform(FogName + ".fDensity", _FogDensity);
	Program->SetUniform(FogName + ".iEquation", _Equation);
}
