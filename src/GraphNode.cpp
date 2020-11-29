#include "GraphNode.hpp"
/*
GraphNode::GraphNode(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale)
	:position(position), rotation(rotation), scale(scale), model(nullptr), isDirty(false)
{
	isDirty = false;
	nodeTransform = glm::mat4(1.0f);
	//children = std::vector< std::shared_ptr<GraphNode>>();
}

void GraphNode::SetPosition(float x, float y, float z)
{
	position = glm::vec3(x, y, z);
	isDirty = true;
}

void GraphNode::SetRotation(float x, float y, float z)
{
	rotation =	glm::angleAxis(glm::radians(x), glm::vec3(1.0f, 0.0f, 0.0f)) *
				glm::angleAxis(glm::radians(y), glm::vec3(0.0f, 1.0f, 0.0f)) *
				glm::angleAxis(glm::radians(z), glm::vec3(0.0f, 0.0f, 1.0f));
	isDirty = true;
}

void GraphNode::SetScale(float x, float y, float z)
{
	scale = glm::vec3(x, y, z);
	isDirty = true;
}

void GraphNode::AddChild(const std::shared_ptr<GraphNode>& child)
{
	children.push_back(child);
}

void GraphNode::Update(bool parentIsDirty, glm::mat4 parentTransform)
{
	isDirty = isDirty | parentIsDirty;

	if (isDirty)
	{
		glm::mat4 P = glm::translate(glm::mat4(1.0f), position);
		glm::mat4 R = glm::mat4_cast(rotation);
		glm::mat4 S = glm::scale(glm::mat4(1.0f), scale);

		nodeTransform = P * R * S;

		isDirty = false;
	}

	transform = nodeTransform * parentTransform;

	for (auto& child : children)
	{
		child->Update(isDirty, transform);
	}
}

void GraphNode::Draw(Shader& shader)
{
	if (model != nullptr)
	{
		model->Draw(shader);
		for (auto& child : children)
		{
			child->Draw(shader);
		}
	}
}

void GraphNode::SetModel(const std::shared_ptr<Model>& newModel)
{
	//model = std::make_shared<Model>(newModel);
}
*/