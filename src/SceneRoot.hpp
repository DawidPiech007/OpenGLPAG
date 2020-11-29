#ifndef SCENEROOT_XD
#define SCENEROOT_XD

//#include <glm/mat4x4.hpp>
//#include <glm/gtc/quaternion.hpp>

#include <model_s.hpp>
#include "GraphNode.hpp"

//#include <memory>
//#include <vector>

class SceneRoot
{
public:
	SceneRoot(const glm::vec3& position = glm::vec3(0.0f),
		const glm::vec3& rotation = glm::vec3(0.0f),
		const glm::vec3& scale = glm::vec3(1.0f));

	void SetPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z);
	void SetScale(float x, float y, float z);

	void AddChild(const std::shared_ptr<GraphNode>& child);

	void Update();

	void Draw(Shader& shader);

private:
	void CreateSolarSystem();
	void UpdateSolarSystem();

	glm::vec3 position;
	glm::quat rotation;
	glm::vec3 scale;
	glm::mat4 nodeTransform;

	bool isDirty;

	std::vector<std::shared_ptr<GraphNode>> children;
	std::vector<std::shared_ptr<GraphNode>> graphNodes;

	std::vector<std::shared_ptr<Model>> models;
};

#endif