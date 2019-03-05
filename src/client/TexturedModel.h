#pragma once
#include "BasicModel.h"
#include "Texture.h"
class TexturedModel
{
public:
	TexturedModel(BasicModel Model, Texture texture) :Model(Model), Texture(texture) {};
	BasicModel getBasicModel() { return Model; };
	Texture getTexture() { return Texture; };
	~TexturedModel();

private:
	BasicModel Model;
	Texture Texture;
};

