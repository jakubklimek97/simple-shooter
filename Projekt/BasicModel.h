#pragma once
class BasicModel
{
public:
	BasicModel(int _VAOid, int _VertexCount) :_VAOid(_VAOid), _VertexCount(_VertexCount) {};
	~BasicModel();
	BasicModel() = default; // zmienic! tylko po to zeby sie kompilowalo :)
	int getVAOid();
	int getVertexCount();
private:
	int _VAOid;
	int _VertexCount;
};

