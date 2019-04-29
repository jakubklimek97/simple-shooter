#pragma once
#include<map>
#include"Shader.h"
class ResourceManager
{
public:
	static std::map<std::string, Shader > Shaders;
	ResourceManager();
	~ResourceManager();
};

