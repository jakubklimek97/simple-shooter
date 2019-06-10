#pragma once
#include <GL/glew.h>
#include "Camera.h"
#include "Model.h"
#include "Shader.h"
#include <glm/glm.hpp>

class LightObject;
class Entity
{
public:
	Entity(Model& model, glm::vec3 position, float rotation, glm::vec3 scale);
	~Entity() = default;
	void rotateX(float radians);
	void rotateY(float radians);
	void rotateZ(float radians);
	void setRotationX(float radians);
	void setRotationY(float radians);
	void setRotationZ(float radians);
	void setShader(Shader& shader);
	void setShader(Shader* shader);
	virtual void Draw(glm::mat4 &projectionMatrix, glm::mat4& viewMatrix);
	virtual void Draw(glm::mat4 &projectionMatrix, glm::mat4& viewMatrix,const LightObject& lightObject, const Camera& camera);
	Shader* GetShader();
	glm::vec3 getPosition();
	void setPosition(const glm::vec3& pos);
protected:
	Model& model;
	void prepareModelMatrix();
	glm::mat4 modelMatrix;
	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 rotation;
	glm::vec3 rotationAngle;
	Shader* shader;
};

