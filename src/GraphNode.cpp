#include "GraphNode.hpp"

GraphNode::GraphNode(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale)
	:position(position), scale(scale), isDirty(false)
{
	isDirty = false;
	orbit = false;
	sphere = false;
	light = false;
	enable = true;
	first = true;
	house = false;
	roof = false;
	mirror = false;
	glass = false;
	sprite = false;
	gate = false;
	gateOpen = false;
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

void GraphNode::SetRotationDir(glm::vec3 dir)
{
	rotation = glm::quatLookAt(dir, glm::vec3(0.0f, 1.0f, 0.0f));
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

void GraphNode::UniformShader_PointLight(string lightName, Shader& shader)
{
	shader.setBool(lightName + ".enable", enable);
	shader.setVec3(lightName + ".position", position);
	shader.setVec3(lightName + ".ambient", ambient);
	shader.setVec3(lightName + ".diffuse", diffuse);
	shader.setVec3(lightName + ".specular", specular);
	shader.setFloat(lightName + ".constant", constant);
	shader.setFloat(lightName + ".linear", linear);
	shader.setFloat(lightName + ".quadratic", quadratic);
}

void GraphNode::UniformShader_DirLight(string lightName, Shader& shader)
{
	shader.setBool(lightName + ".enable", enable);
	shader.setVec3(lightName + ".direction", lightDir);
	shader.setVec3(lightName + ".ambient", ambient);
	shader.setVec3(lightName + ".diffuse", diffuse);
	shader.setVec3(lightName + ".specular", specular);
}

void GraphNode::UniformShader_SpotLight(string lightName, Shader& shader)
{
	shader.setBool(lightName + ".enable", enable);
	shader.setVec3(lightName + ".position", position);
	shader.setVec3(lightName + ".direction", lightDir);
	shader.setVec3(lightName + ".ambient", ambient);
	shader.setVec3(lightName + ".diffuse", diffuse);
	shader.setVec3(lightName + ".specular", specular);
	shader.setFloat(lightName + ".constant", constant);
	shader.setFloat(lightName + ".linear", linear);
	shader.setFloat(lightName + ".quadratic", quadratic);
	shader.setFloat(lightName + ".cutOff", cutOff);
	shader.setFloat(lightName + ".outerCutOff", outerCutOff);
}

void GraphNode::ConfigLight(glm::vec3 color, glm::vec3 dir, bool enable)
{
	diffuse = color;
	ambient = color / 10.0f;

	dir = glm::normalize(dir);
	lightDir = dir;
	//SetRotation(dir.x * 180.0f, dir.y * 180.0f, dir.z * 180.f);
	SetRotationDir(dir);
	this->enable = enable;
}

void GraphNode::AddChild(const std::shared_ptr<GraphNode>& child)
{
	children.push_back(child);
}

void GraphNode::Update(bool parentIsDirty, glm::mat4 parentTransform, unsigned int houseBuffer, unsigned int roofBuffer)
{
	isDirty = isDirty | parentIsDirty;

	if (isDirty)
	{
		
		glm::mat4 P = glm::translate(glm::mat4(1.0f), position);
		glm::mat4 R = glm::mat4_cast(rotation);
		glm::mat4 S = glm::scale(glm::mat4(1.0f), scale);

		nodeTransform = P * R * S;
		transform = parentTransform * nodeTransform;

		if (house)
		{
			//std::cout <<" dom:" << index;
			//std::cout << " jegoDach:" << children[0]->index << endl;
			glBindBuffer(GL_ARRAY_BUFFER, houseBuffer);
			glBufferSubData(GL_ARRAY_BUFFER, index * sizeof(glm::mat4), sizeof(glm::mat4), &transform);
		}
		else if (roof)
		{
			//std::cout << " dach:" << index;
			glBindBuffer(GL_ARRAY_BUFFER, roofBuffer);
			glBufferSubData(GL_ARRAY_BUFFER, index * sizeof(glm::mat4), sizeof(glm::mat4), &transform);
			
		}
		else
		{
			//std::cout << " inne";
		}
	}

	if (first)
	{
		transform = parentTransform * nodeTransform;
		first = false;
	}

	for (auto& child : children)
	{
		child->Update(isDirty, transform, houseBuffer, roofBuffer);
	}
	isDirty = false;
}

void GraphNode::UpdateGate(float deltaTime, glm::vec3 playerPos)
{
	if (gate)
	{
		if (gateOpen == false)
		{
			if (glm::distance(position, playerPos) < 1.2f)
			{
				gateOpen = true;
				std::cout << "brama otwarta"<<endl;
			}
		}
		
		if (gateOpen == true)
		{
			if (glm::distance(position, targetGatePos) > 0.1f)
			{
				float speed = 1.0f * deltaTime;
				glm::vec3 dir = glm::vec3(targetGatePos.x - position.x, targetGatePos.y - position.y, targetGatePos.z - position.z);
				dir = glm::normalize(dir);
				SetPosition(position.x + dir.x * speed, position.y + dir.y * speed, position.z + dir.z * speed);
			}
		}
	}

	for (auto& child : children)
	{
		child->UpdateGate(deltaTime,playerPos);
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
			// sejder do rysowania krzta�t�w �r�de� �wiat�a
			lightShader.use();
			lightShader.setMat4("model", transform);
			lightShader.setVec3("lightColor", diffuse);
			GraphNode::model->Draw(lightShader);
		}
		else
		{
			// rysowanie szejderem z o�wietleniem
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

void GraphNode::Draw(Shader& shader, Shader& colorShader, Shader& lightShader, Shader& mirrorShader, Shader& glassShader, Shader& spriteShader, unsigned int cubemapTexture)
{
	if (model != nullptr)
	{
		if (light == true)
		{
			// sejder do rysowania krzta�t�w �r�de� �wiat�a
			lightShader.use();
			lightShader.setMat4("model", transform);
			lightShader.setVec3("lightColor", diffuse);
			GraphNode::model->Draw(lightShader);
		}
		else if (mirror == true)
		{
			// rysowanie szejderem dla luster
			mirrorShader.use();
			mirrorShader.setMat4("model", transform);

			glBindVertexArray(model->meshes[0].VAO);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
			glDrawArrays(GL_TRIANGLES, 0, model->meshes[0].indices.size());
		}
		else if (glass == true)
		{
			// rysowanie szejderem dla szyb
			glassShader.use();
			glassShader.setMat4("model", transform);

			glBindVertexArray(model->meshes[0].VAO);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
			glDrawArrays(GL_TRIANGLES, 0, model->meshes[0].indices.size());
		}
		else if (weapon == true)
		{
			// sejder do rysowania krzta�t�w �r�de� �wiat�a
			colorShader.use();
			colorShader.setMat4("model", transform);
			colorShader.setVec3("material.color", diffuse);
			GraphNode::model->Draw(colorShader);
		}
		else if (sprite == true)
		{
			// rysowanie szejderem dla sprit�w 2D
			spriteShader.use();
			spriteShader.setMat4("model", transform);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			GraphNode::model->Draw(shader);
			glDisable(GL_BLEND);
		}
		else
		{
			// rysowanie szejderem z o�wietleniem
			shader.use();
			shader.setMat4("model", transform);
			GraphNode::model->Draw(shader);
		}
	}

	for (auto& child : children)
	{
		child->Draw(shader, colorShader, lightShader, mirrorShader, glassShader, spriteShader, cubemapTexture);
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

void GraphNode::SetLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 lightDir, float cutOff, float outerCutOff, float constant, float linear, float quadratic)
{
	light = true;
	this->ambient = ambient;
	this->diffuse = diffuse;
	this->specular = specular;
	this->lightDir = lightDir;
	this->cutOff = glm::cos(glm::radians(cutOff));
	this->outerCutOff = glm::cos(glm::radians(outerCutOff));
	this->constant = constant;
	this->linear = linear;
	this->quadratic = quadratic;
}

void GraphNode::SetHouse(int index)
{
	house = true;
	this->index = index;
}

void GraphNode::SetRoof(int index)
{
	roof = true;
	this->index = index;
}

void GraphNode::SetMirror()
{
	mirror = true;
}

void GraphNode::SetGlass()
{
	glass = true;
}

void GraphNode::SetWeapon(glm::vec3 color)
{
	weapon = true;
	diffuse = color;
}

void GraphNode::SetSprite()
{
	sprite = true;
}

void GraphNode::SetGate(glm::vec3 targetGatePos)
{
	gate = true;
	GraphNode::targetGatePos = targetGatePos;
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
