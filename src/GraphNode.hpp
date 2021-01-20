#ifndef GRAPHNODE
#define GRAPHNODE

#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>

#include "model_s.hpp"

#include <memory>
#include <vector>

class GraphNode
{
public:
	//GraphNode(const glm::vec3& position = glm::vec3(0.0f),
	//	const glm::vec3& rotation = glm::vec3(0.0f),
	//	const glm::vec3& scale = glm::vec3(1.0f));

	GraphNode(const glm::vec3& position,
		const glm::vec3& rotation,
		const glm::vec3& scale);

	void SetPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z);
	void SetRotationDir(glm::vec3 dir);
	void SetScale(float x, float y, float z);
	void SetPositionOnCircle(float angle, float radius);

	void UniformShader_PointLight(string lightName, Shader& shader);
	void UniformShader_DirLight(string lightName, Shader& shader);
	void UniformShader_SpotLight(string lightName, Shader& shader);

	void ConfigLight(glm::vec3 color, glm::vec3 dir, bool enable);

	void AddChild(const std::shared_ptr<GraphNode>& child);

	void Update(bool parentIsDirty, glm::mat4 parentTransform, unsigned int houseBuffer, unsigned int roofBuffer);
	void UpdateGate(float deltaTime, glm::vec3 playerPos);

	void Draw(Shader& shader);
	void Draw(Shader& shader, Shader& lightShader);
	void Draw(Shader& shader, Shader& colorShader, Shader& lightShader, Shader& mirrorShader, Shader& glassShader, Shader& spriteShader, unsigned int cubemapTexture);
	void Draw(Shader& shader, Shader& orbitShader, Shader& sphereShader, int resolution);
	void DrawByGeometryShader(Shader& geometryShader, int resolution);

	void SetModel(const std::shared_ptr<Model>& newModel);
	void SrtOrbit(float r, float vertexN, glm::vec4 color);
	void SetSphere(float r, float vertexN, glm::vec4 color);
	void SetLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,	glm::vec3 lightDir, float cutOff, float outerCutOff, float constant, float linear, float quadratic);
	void SetHouse(int index);
	void SetRoof(int index);
	void SetMirror();
	void SetGlass();
	void SetWeapon(glm::vec3 color);
	void SetSprite();
	void SetGate(glm::vec3 targetGatePos);

	glm::vec3 GetPosition();

	// publiczne do debugowania
	std::vector<std::shared_ptr<GraphNode>> children;
	int index;

private:
	void PrintMatrix(glm::mat4 M);
	void PrintVector(glm::vec4 V);

	glm::mat4 transform;
	glm::vec3 position;
	glm::quat rotation;
	glm::vec3 scale;
	glm::mat4 nodeTransform;

	bool isDirty;
	bool first;

	bool house;
	bool roof;

	bool mirror;
	bool glass;
	bool weapon;
	bool sprite;
	bool gate;
	bool gateOpen;
	glm::vec3 targetGatePos;

	bool orbit;
	bool sphere;

	bool light;
	bool enable;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	glm::vec3 lightDir;
	float cutOff;
	float outerCutOff;
	float constant;
	float linear;
	float quadratic;

	float r;
	float vertexN;
	glm::vec4 color;


	std::shared_ptr<Model> model;
};

#endif