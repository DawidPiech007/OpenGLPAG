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

	void CreateAllHouse(int n, int m);
	void CreateAllRoof(int n, int m);

	void SetPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z);
	void SetScale(float x, float y, float z);

	void AddLight(glm::vec3 position, glm::vec3 scale, glm::vec3 color);
	void AddMirror(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
	void AddGlass(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
	void AddChild(const std::shared_ptr<GraphNode>& child);

	void Update(float time, unsigned int houseBuffer, unsigned int roofBuffer);

	void Draw(Shader& shader, Shader& colorShader, Shader& lightShader, Shader& mirrorShader, Shader& glassShader, unsigned int cubemapTexture);
	void Draw(Shader& shader, Shader& orbitShader, Shader& sphereShader, int resolution);
	void Draw(Shader& shader, Shader& lightShader);
	void Draw(Shader& shader);

	glm::vec3 GetPosition(int index);

	void SetLight(int index, glm::vec3 lightColor);

	// teraz publiczne ¿eby da³o siê pobieraæ dane z œwiate³ w scenie i wysy³aæ dane z maina do œwiate³ w scenie
	std::vector<std::shared_ptr<GraphNode>> graphNodes;
	std::vector<std::shared_ptr<GraphNode>> children;

private:
	void CreateBaseScene();
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

	glm::mat4* modelMatricesHouse;
	glm::mat4* modelMatricesRoof;
};

#endif