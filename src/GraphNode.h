#ifndef GRAPHNODE
#define GRAPHNODE

#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>

#include <model_s.h>

#include <memory>
#include <vector>

class GraphNode
{
public:
	GraphNode(const glm::vec3& position = glm::vec3(0.0f),
		const glm::vec3& rotation = glm::vec3(0.0f),
		const glm::vec3& scale = glm::vec3(1.0f));

	void SetPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z);
	void SetScale(float x, float y, float z);

	void AddChild(std::shared_ptr<GraphNode> child);

	void Update(bool parentIsDirty, glm::mat4 parentTransform);

	void Draw(Shader& shader);

private:
	glm::vec3 position;
	glm::quat rotation;
	glm::vec3 scale;
	glm::mat4 nodeTransform;

	bool isDirty;

	std::vector<std::shared_ptr<GraphNode>> children;

	std::shared_ptr<Model> model;
};

#endif