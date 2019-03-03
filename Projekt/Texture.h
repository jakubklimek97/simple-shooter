#pragma once
class Texture
{
public:
	Texture(int texture):_textureID(texture) {};
	Texture() = default; // zmienic! tylko po to zeby sie kompilowalo :)
	~Texture();
	int getID() { return _textureID; };
	float GetShineDamper() { return _ShineDamper; };
	float GetSReflectivity() { return _Reflectivity; };
	void SetShineDamper(float _ShineDamper);
	void SetReflectivity(float _Reflectivity);
private:
	int _textureID;
	float _ShineDamper = 1;
	float _Reflectivity = 0;
};

