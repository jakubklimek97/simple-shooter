#pragma once

#include "shaders.h"
#include"GL/glew.h"
#include"glm/glm.hpp"

enum FogEquation {
	EXP = 1,
	EXP2 = 2
};


class Fog
{
public:
	Fog();
	Fog(glm::vec4 FogColor, float FogDensity, int Equation);
	~Fog() = default;
	 void SetUniformData(ShaderProgram* Program, string FogName);

private:
	glm::vec4 _FogColor;
	float _FogDensity;
	int _Equation;

};

