#ifndef SCENEROOT_XD
#define SCENEROOT_XD

//#include <glm/mat4x4.hpp>
//#include <glm/gtc/quaternion.hpp>

#include <model_s.hpp>
#include "GraphNode.hpp"
#include <GLFW/glfw3.h>
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

	void Update(float time);

	void Draw(Shader& shader, Shader& orbitShader, Shader& sphereShader, int resolution);
	void Draw(Shader& shader, Shader& lightShader);
	void Draw(Shader& shader);

	glm::vec3 GetPosition(int index);

	void SetLight(int index, glm::vec3 lightColor);

	// teraz publiczne ¿eby da³o siê pobieraæ dane z œwiate³ w scenie i wysy³aæ dane z maina do œwiate³ w scenie
	std::vector<std::shared_ptr<GraphNode>> graphNodes;
	std::vector<std::shared_ptr<GraphNode>> children;

private:
	void CreateSolarSystem();
	shared_ptr<GraphNode> NewObject(const glm::vec3& position,
		const glm::vec3& rotation,
		const glm::vec3& scale, 
		const std::string& path);

	glm::vec3 position;
	glm::quat rotation;
	glm::vec3 scale;
	glm::mat4 nodeTransform;

	bool isDirty;


	std::vector<std::shared_ptr<Model>> models;
};

#endif