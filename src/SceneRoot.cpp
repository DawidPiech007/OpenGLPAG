
#include <SceneRoot.hpp>

SceneRoot::SceneRoot(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale)
	:position(position), rotation(rotation), scale(scale)
{
	isDirty = false;
	nodeTransform = glm::mat4(1.0f);

	CreateBaseScene();
}

void SceneRoot::CreateAllHouse(int n, int m)
{
	unsigned int amount = n * m;
	//modelMatricesHouse = new glm::mat4[amount];
	float offset = 2.0f;
	unsigned int index = 0;
	for (unsigned int i = 0; i < n; i++)
	{
		for (unsigned int j = 0; j < m; j++)
		{
			glm::vec3 position = glm::vec3(offset * i - 200, -5.0f, offset * j - 200);
			graphNodes[0]->AddChild(NewObject(position, glm::vec3(0.0f), glm::vec3(1.0f), "none"));

			graphNodes[6+index]->SetHouse(index);

			index++;
		}
	}
}

void SceneRoot::CreateAllRoof(int n, int m)
{
	unsigned int amount = n * m;
	//modelMatricesRoof = new glm::mat4[amount];
	float offset = 2.0f;
	unsigned int index = 0;
	for (unsigned int i = 0; i < n; i++)
	{
		for (unsigned int j = 0; j < m; j++)
		{
			glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
			// index + 6 to odpowiedni domek
			graphNodes[index+6]->AddChild(NewObject(position, glm::vec3(0.0f), glm::vec3(1.0f), "none"));
			graphNodes[6+amount + index]->SetRoof(index);


			//std::cout << " index:" << graphNodes[index + 6]->children[0]->index << endl;
			//std::cout << " index:" << graphNodes[index + 6]->index << " " << graphNodes[6 + amount + index]->index << endl;
			index++;
			//std::cout << " index:" << index << endl;
		}
	}
}

void SceneRoot::SetPosition(float x, float y, float z)
{
}

void SceneRoot::SetRotation(float x, float y, float z)
{
}

void SceneRoot::SetScale(float x, float y, float z)
{
}

void SceneRoot::AddLight(glm::vec3 position, glm::vec3 scale, glm::vec3 color)
{
	shared_ptr<GraphNode> light = NewObject(position, glm::vec3(0.0f), scale, "res/models/kostkaReady/kostka.obj");

	graphNodes[1]->AddChild(light);

	light->SetLight(glm::vec3(0.05f), // ambient
		color, // difiuse
		glm::vec3(1.0f), // specular
		glm::vec3(0.0f, 0.0f, 0.0f), // light dir	(zb�dne)
		0.0f, 0.0f, // cutOff i outherCutOff	(zb�dne)
		1.0f, 0.9f, 0.32f);// t�umienie
}

void SceneRoot::AddChild(const std::shared_ptr<GraphNode>& child)
{
	children.push_back(child);
}

void SceneRoot::Update(float time, unsigned int houseBuffer, unsigned int roofBuffer)
{
	// obracanie �i� �wiat�a wok� �rodka
	//graphNodes[1]->SetPositionOnCircle(time * 0.4f, 30);
	

	if (isDirty)
	{
		glm::mat4 P = glm::translate(glm::mat4(1.0f), position);
		glm::mat4 R = glm::mat4_cast(rotation);
		glm::mat4 S = glm::scale(glm::mat4(1.0f), scale);

		nodeTransform = P * R * S;

		isDirty = false;
	}

	for (auto& child : children)
	{
		child->Update(isDirty, nodeTransform, houseBuffer, roofBuffer);
	}
}

void SceneRoot::Draw(Shader& shader, Shader& orbitShader, Shader& sphereShader, int resolution)
{
	for (auto& child : children)
	{
		child->Draw(shader, orbitShader, sphereShader, resolution);
	}
}

void SceneRoot::Draw(Shader& shader, Shader& lightShader)
{
	for (auto& child : children)
	{
		child->Draw(shader, lightShader);
	}
}

void SceneRoot::Draw(Shader& shader)
{
	for (auto& child : children)
	{
		child->Draw(shader);
	}
}

glm::vec3 SceneRoot::GetPosition(int index)
{
	return graphNodes[index]->GetPosition();
}

void SceneRoot::SetLight(int index, glm::vec3 lightColor)
{
	graphNodes[index]->SetLight(glm::vec3(0.05f), // ambient
		lightColor, // difiuse
		glm::vec3(1.0f), // specular
		glm::vec3(0.0f, 0.0f, 0.0f), // light dir	(zb�dne)
		0.0f, 0.0f, // cutOff i outherCutOff	(zb�dne)
		1.0f, 0.0009f, 0.00032f);// t�umienie
}



void SceneRoot::CreateBaseScene()
{
	AddChild(NewObject(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f), "none"));																// [00] wszystkie Domki
	AddChild(NewObject(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f), "none"));																// [01] wszystkie �wiat�a
	AddChild(NewObject(glm::vec3(0.0f, -6.0f, 0.0f), glm::vec3(0.0f), glm::vec3(100.0f, 1.0f, 100.0f), "res/models/PodlozeReady/kostka.obj"));	// [02] podloze
	AddChild(NewObject(glm::vec3(0.0f, -4.0f, 0.0f), glm::vec3(180.0f, 0.0f, 0.0f), glm::vec3(100.0f, 1.0f, 100.0f), "res/models/PodlozeReady/kostka.obj"));	// [03] sufit
	//AddChild(NewObject(glm::vec3(-5.0f, 1.0f, -15.0f), glm::vec3(0.0f), glm::vec3(1.0f), "res/models/kostkaReady/kostka.obj"));				// [01] kr���ce punktowe �wiat�o
	//AddChild(NewObject(glm::vec3(0.0f, 15.0f, 0.0f), glm::vec3(0.0f), glm::vec3(3.0f), "res/models/Refrektor/kostka.obj"));					// [03] �wiat�o kierunkowe
	//AddChild(NewObject(glm::vec3(5.0f, 1.0f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f), "res/models/Refrektor/kostka.obj"));					// [04] �wiat�o refretorowe 1
	//AddChild(NewObject(glm::vec3(-5.0f, 1.0f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f), "res/models/Refrektor/kostka.obj"));					// [05] �wiat�o refretorowe 2

	//// �wiat�o punktowe
	//graphNodes[1]->SetLight(glm::vec3(0.05f), // ambient
	//	glm::vec3(0.1f, 0.6f, 0.9f), // difiuse
	//	glm::vec3(1.0f), // specular
	//	glm::vec3(0.0f,0.0f,0.0f), // light dir	(zb�dne)
	//	0.0f, 0.0f, // cutOff i outherCutOff	(zb�dne)
	//	1.0f, 0.0009f, 0.032f);// t�umienie
	//
	//// �wiat�o kierunkowe
	//graphNodes[3]->SetLight(glm::vec3(0.05f), // ambient
	//	glm::vec3(0.1f, 0.6f, 0.9f), // difiuse
	//	glm::vec3(0.5f), // specular
	//	glm::vec3(0.0f, -1.0f, 0.0f), // light dir
	//	0.0f, 0.0f, // cutOff i outherCutOff	(zb�dne)
	//	0.0f, 0.0f, 0.0f);// t�umienie			(zb�dne)
	//
	//// �wiat�o kierunkowe refretorowe 1
	//graphNodes[4]->SetLight(glm::vec3(0.05f), // ambient
	//	glm::vec3(0.1f, 0.6f, 0.9f), // difiuse
	//	glm::vec3(1.0f), // specular
	//	glm::vec3(1.0f, -1.0f, 0.0f), // light dir
	//	10.0f, 15.0f, // cutOff i outherCutOff	
	//	1.0f, 0.0009f, 0.032f);// t�umienie
	//
	//// �wiat�o kierunkowe refretorowe 2
	//graphNodes[5]->SetLight(glm::vec3(0.05f), // ambient
	//	glm::vec3(0.1f, 0.6f, 0.9f), // difiuse
	//	glm::vec3(1.0f), // specular
	//	glm::vec3(1.0f, -1.0f, 0.0f), // light dir
	//	10.0f, 15.0f, // cutOff i outherCutOff	
	//	1.0f, 0.0009f, 0.032f);// t�umienie
				
	
	//graphNodes[2]->SetPositionOnCircle(0, 0);

	//graphNodes[1]->AddChild(NewObject(glm::vec3(0, 0, 0), glm::vec3(0.0f), glm::vec3(1.0f), "none"));								// [05] Planeta1_�cierzka
	//graphNodes[5]->AddChild(NewObject(glm::vec3(2, 0, 0), glm::vec3(45.0f, 0.0f, 0.0f), glm::vec3(1.0f), "none"));					// [06] Planeta1_Grupa
	//graphNodes[6]->AddChild(NewObject(glm::vec3(0, 0, 0), glm::vec3(0.0f), glm::vec3(1.0f), "res/models/kostkaReady/kostka.obj"));	// [07] Planeta1 
}

shared_ptr<GraphNode> SceneRoot::NewObject(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale, const std::string& path)
{
	std::shared_ptr<GraphNode> object = std::make_shared<GraphNode>(position, rotation, scale);	// wska�nik na obiekt
	graphNodes.push_back(object);	// zapisanie obiektu w wektorze

	if (path != "none")
	{
		std::shared_ptr<Model> model = std::make_shared<Model>(path);						// wska�nik na model
		object->SetModel(model);	// przypisanie obiektowi modelu
		models.push_back(model);		// zapisanie modelu w wektorze
	}
	else
	{
		models.push_back(nullptr);		// zapisanie modelu w wektorze
	}
	
	return object;
}

