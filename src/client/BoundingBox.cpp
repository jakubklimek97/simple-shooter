#include "BoundingBox.h"
#include <limits>

BoundingBox::BoundingBox(Entity & entity): entityRef(entity)
{
	
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	unsigned int indices[] = {
		0, 1, 2,
		1, 2, 3,
		0, 2, 4,
		2, 6, 4,
		2, 3, 6,
		3, 6, 7,
		1, 3, 7,
		1, 5, 7,
		0, 1, 4,
		1, 4, 5,
		4, 5, 6,
		5, 6, 7
	};
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36 * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
	glBindVertexArray(0);
	calculateBoundingBox();
}

BoundingBox::~BoundingBox()
{
}

void BoundingBox::calculateBoundingBox()
{
	minX = minY = minZ = std::numeric_limits<float>::max();
	maxX = maxY = maxZ = std::numeric_limits<float>::lowest();
	entityRef.prepareModelMatrix();
	for (const Mesh& mesh : entityRef.model.meshes) {
		for (Vertex vertex : mesh.vertices) {
			vertex.Position = glm::vec3(entityRef.modelMatrix * glm::vec4(vertex.Position, 1.0f));
			if (vertex.Position.x < minX)
				minX = vertex.Position.x;
			else if (vertex.Position.x > maxX)
				maxX = vertex.Position.x;
			if (vertex.Position.y < minY)
				minY = vertex.Position.y;
			else if (vertex.Position.y > maxY)
				maxY = vertex.Position.y;
			if (vertex.Position.z < minZ)
				minZ = vertex.Position.z;
			else if (vertex.Position.z > maxZ)
				maxZ = vertex.Position.z;
		}
	}
	prepareMesh();
}

void BoundingBox::Draw(glm::mat4 &projectionMatrix, glm::mat4& viewMatrix, Shader &shader)
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	shader.use();
	shader.setMat4("projection", projectionMatrix);
	shader.setMat4("view", viewMatrix);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void BoundingBox::prepareMesh()
{
	glm::vec3 pos[8] = {
		glm::vec3(minX, minY, minZ),
		glm::vec3(maxX, minY, minZ),
		glm::vec3(minX, maxY, minZ),
		glm::vec3(maxX, maxY, minZ),
		glm::vec3(minX, minY, maxZ),
		glm::vec3(maxX, minY, maxZ),
		glm::vec3(minX, maxY, maxZ),
		glm::vec3(maxX, maxY, maxZ),
	};
	
	glBindVertexArray(VAO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*8, &pos[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
}
