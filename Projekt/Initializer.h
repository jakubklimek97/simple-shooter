#pragma once
#include<iostream>
#include<fstream>
#include<list>
#include<array>
#include"BasicModel.h"
class Initializer
{
public:
	Initializer();
	~Initializer();
private:
	std::list<int> vaos;
	std::list<int> vbos;
	std::list<int> textures;
	
};

