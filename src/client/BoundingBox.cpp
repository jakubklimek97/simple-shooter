#include "BoundingBox.h"
#include <limits>

BoundingBox::BoundingBox(Entity & entity): entityRef(entity)
{
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
}
