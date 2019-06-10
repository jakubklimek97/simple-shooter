#include "HUD.h"


int HUD::lifenumber = 0;
int HUD::HP = 500000;
float HUD::_incrementvalue = 0.0f;
HUD::HUD()
{
	loaded = false;
}
HUD::HUD(vector<reference_wrapper<const Shader>>LifeShader,const string &heart, const string &container,float Rotation,int Lifeamount ):_Lifeamount(Lifeamount)
{

	_LifeShader.reserve(_Lifeamount);
	copy(LifeShader.begin(), LifeShader.end(), back_inserter(_LifeShader));
	  SetData();
	  ++lifenumber;
 	  InitBuffer();
	  texture1 = LoadTexture(heart, "res/img", false);
	  texture2 = LoadTexture(container, "res/img", false);
	  HealthID.push_back(texture1);
	  HealthID.push_back(texture2);
	  InitHUD(_LifeShader);
	  RotationIntensity();
	  
}
unsigned int HUD::LoadTexture(const std::string & path, const std::string & directory, bool gamma) {
	unsigned int textureID;
	glGenTextures(1, &textureID);
	int width = 1, height = 1, nrComponents = 1;
	std::string filename = path;
	filename = directory + '/' + filename;
		SDL_Surface* ptr = IMG_Load(filename.c_str());
		if (!ptr) {
			std::cout << "ERROR::TEXTURE_LOADER:: " << IMG_GetError() << std::endl;
		}
		width = ptr->w;
		height = ptr->h;
		nrComponents = ptr->format->BytesPerPixel;
		if (ptr)
		{
			GLenum format;
			if (nrComponents == 1)
				format = GL_RED;
			else if (nrComponents == 3)
				format = GL_RGB;
			else if (nrComponents == 4)
				format = GL_RGBA;

			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, ptr->pixels);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			loaded = true;
		
		}
		else
		{
			std::cout << "Texture failed to load " << std::endl;
		}
		SDL_FreeSurface(ptr);
		return textureID;

}

void HUD::RotationIntensity()
{
	_gamma = static_cast<GLfloat>(sin(_incrementvalue) / 2) + 0.5f;

}

void HUD::InitHUD(vector<reference_wrapper<const Shader>>_LifeShader)
{
	Shader Program;
	for_each(_LifeShader.begin(), _LifeShader.end(), StructShader);
	
}

void HUD::Incrementvalue()
{
	_incrementvalue += 0.01f;
	if (_incrementvalue == 1.1f) {
		_incrementvalue = 0.0f;
	}
	HP -= 1;
	if (HP < -2300)
		HP = -1;
}



void HUD::RenderHUD(vector<reference_wrapper<const Shader>>_LifeShader)
{
	Shader Program;
	for (LifeIterator = _LifeShader.begin(); LifeIterator != _LifeShader.end(); ++LifeIterator) {
		auto i = distance(_LifeShader.begin(), LifeIterator);
		glActiveTexture(GL_TEXTURE0+i);
		glBindTexture(GL_TEXTURE_2D, HealthID[i]);
	}
		
		RotationIntensity();

		for (LifeIterator = _LifeShader.begin(); LifeIterator != _LifeShader.end(); ++LifeIterator) {
			auto i = distance(_LifeShader.begin(), LifeIterator);
			Program = _LifeShader[i];
			Program.use();
			glUniform1f(glGetUniformLocation(Program.ID, "gamma"), _gamma);
			Program.setInt("texture2", 1);
			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); 
		}
}

void HUD::SetIndices()
{
	indices = { 0,1,3, // 1 triangle
			  1,2,3 }; // 2 triangle
}

void HUD::SetVertices()
{
	if (lifenumber == 0) { //first 

		vertices = {
			// positions          // colors           // texture coords
		-0.7f, -1.0f, 0.0f,    1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
		-0.7f, -0.7f, 0.0f,    0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
		-1.0f, -0.7f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
		-1.0f, -1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
		};
	}

	else if (lifenumber == 1) {//second

		vertices = {
			// positions          // colors           // texture coords
			-0.4f, -1.0f, 0.0f,    1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
			-0.4f, -0.7f, 0.0f,    0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
			-0.7f, -0.7f, 0.0f,    0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
			-0.7f, -1.0f, 0.0f,    1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
		};

	}
	else {//third

		vertices = {
			// positions          // colors           // texture coords
			-0.1f, -1.0f, 0.0f,    1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
			-0.1f, -0.7f, 0.0f,    0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
			-0.4f, -0.7f, 0.0f,    0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
			-0.4f, -1.0f, 0.0f,    1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
		};
	}

	}

void HUD::SetData()
{
	SetIndices();
	SetVertices();
}

HUD::~HUD()
{
	lifenumber--;
}

void HUD::InitBuffer()
{


	
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * 2 * sizeof(float), &vertices.front(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * 2 * sizeof(int), &indices.front(), GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

}