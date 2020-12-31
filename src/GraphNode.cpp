#include "GraphNode.hpp"

GraphNode::GraphNode(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale)
	:position(position), scale(scale), isDirty(false)
{
	isDirty = false;
	orbit = false;
	sphere = false;
	light = false;
	this->rotation = glm::angleAxis(glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f)) *
		glm::angleAxis(glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f)) *
		glm::angleAxis(glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 P = glm::translate(glm::mat4(1.0f), position);
	glm::mat4 R = glm::mat4_cast(this->rotation);
	glm::mat4 S = glm::scale(glm::mat4(1.0f), scale);

	nodeTransform = P * R * S;
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

void GraphNode::SetPositionOnCircle(float angle, float radius)
{
	float x = std::cos(angle) * radius;
	float z = std::sin(angle) * radius;

	position = glm::vec3(x, position.y, z);
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

	transform = parentTransform * nodeTransform;

	for (auto& child : children)
	{
		child->Update(isDirty, transform);
	}
}

void GraphNode::Draw(Shader& shader)
{
	if (model != nullptr)
	{
		shader.use();
		//PrintMatrix(transform);
		shader.setMat4("model", transform);
		GraphNode::model->Draw(shader);
	}

	for (auto& child : children)
	{
		child->Draw(shader);
	}
}

void GraphNode::Draw(Shader& shader, Shader& lightShader)
{
	if (model != nullptr)
	{
		if (light == true)
		{
			// sejder do rysowania krzta³tów Ÿróde³ œwiat³a
			lightShader.use();
			lightShader.setMat4("model", transform);
			lightShader.setVec3("lightColor", lightColor);
			GraphNode::model->Draw(lightShader);
		}
		else
		{
			// rysowanie szejderem z oœwietleniem
			shader.use();
			shader.setMat4("model", transform);
			GraphNode::model->Draw(shader);
		}
	}

	for (auto& child : children)
	{
		child->Draw(shader,lightShader);
	}
}

void GraphNode::Draw(Shader& shader, Shader& orbitShader, Shader& sphereShader, int resolution)
{
	if (model != nullptr)
	{
		shader.use();
		//PrintMatrix(transform);

		shader.setMat4("model", transform);
		GraphNode::model->Draw(shader);
	}
	if (orbit)
	{
		DrawByGeometryShader(orbitShader, resolution);
	}
	if (sphere)
	{
		DrawByGeometryShader(sphereShader, resolution);
	}


	for (auto& child : children)
	{
		child->Draw(shader,orbitShader, sphereShader, resolution);
	}
}

void GraphNode::DrawByGeometryShader(Shader& geometryShader, int resolution)
{
	float point[] = { 0.0f, 0.0f, 0.0f };
	
	
	unsigned int VBO, VAO; 
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	
	glBindVertexArray(VAO);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(point), point, GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	


	geometryShader.use();
	geometryShader.setFloat("r", r);
	//vertexN = resolution;
	geometryShader.setInt("vertexN", resolution); 
	geometryShader.setMat4("model", transform);
	geometryShader.setVec4("color", color);
	
	glBindVertexArray(VAO);
	glDrawArrays(GL_POINTS, 0, 1);

}

void GraphNode::SetModel(const std::shared_ptr<Model>& newModel)
{
	//model = std::make_shared<Model>(newModel);
	model = newModel;
}

void GraphNode::SrtOrbit(float r, float vertexN, glm::vec4 color)
{
	orbit = true;
	this->r = r;
	this->vertexN = vertexN;
	this->color = color;
}

void GraphNode::SetSphere(float r, float vertexN, glm::vec4 color)
{
	sphere = true;
	this->r = r;
	this->vertexN = vertexN;
	this->color = color;
}

void GraphNode::SetLight(glm::vec3 lightColor)
{
	light = true;
	this->lightColor = lightColor;
}

glm::vec3 GraphNode::GetPosition()
{
	return position;
}

void GraphNode::PrintMatrix(glm::mat4 M)
{
	cout << "================================" << endl;
	cout << M[0][0] << "  " << M[0][1] << "  " << M[0][2] << "  " << M[0][3] << endl;
	cout << M[1][0] << "  " << M[1][1] << "  " << M[1][2] << "  " << M[1][3] << endl;
	cout << M[2][0] << "  " << M[2][1] << "  " << M[2][2] << "  " << M[2][3] << endl;
	cout << M[3][0] << "  " << M[3][1] << "  " << M[3][2] << "  " << M[3][3] << endl;
}

void GraphNode::PrintVector(glm::vec4 V)
{
	cout << V[0] << "  " << V[1] << "  " << V[2] << "  " << V[3] << endl;
}
