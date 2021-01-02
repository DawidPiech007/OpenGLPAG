
#include <SceneRoot.hpp>

SceneRoot::SceneRoot(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale)
	:position(position), rotation(rotation), scale(scale)
{
	isDirty = false;
	nodeTransform = glm::mat4(1.0f);

	CreateSolarSystem();
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

void SceneRoot::AddChild(const std::shared_ptr<GraphNode>& child)
{
	children.push_back(child);
}

void SceneRoot::Update(float time)
{
	// obracanie úiÍ úwiat≥a wokÛ≥ úrodka
	graphNodes[1]->SetPositionOnCircle(time * 0.4f, 260);
	

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
		child->Update(isDirty, nodeTransform);
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
		glm::vec3(0.0f, 0.0f, 0.0f), // light dir	(zbÍdne)
		0.0f, 0.0f, // cutOff i outherCutOff	(zbÍdne)
		1.0f, 0.0009f, 0.00032f);// t≥umienie
}


void SceneRoot::CreateSolarSystem()
{
	AddChild(NewObject(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f), "none"));																// [00] wszystkie Domki
	AddChild(NewObject(glm::vec3(-5.0f, 10.0f, -15.0f), glm::vec3(0.0f), glm::vec3(8.0f), "res/models/kostkaReady/kostka.obj"));				// [01] krπøπce punktowe úwiat≥o
	AddChild(NewObject(glm::vec3(0.0f, -6.5f, 0.0f), glm::vec3(0.0f), glm::vec3(450.0f, 1.0f, 450.0f), "res/models/PodlozeReady/kostka.obj"));	// [02] podloze
	AddChild(NewObject(glm::vec3(0.0f, 15.0f, 0.0f), glm::vec3(0.0f), glm::vec3(8.0f), "res/models/kostkaReady/kostka.obj"));					// [03] úwiat≥o kierunkowe
	AddChild(NewObject(glm::vec3(15.0f, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(3.0f), "res/models/kostkaReady/kostka.obj"));					// [04] úwiat≥o refretorowe 1
	AddChild(NewObject(glm::vec3(-15.0f, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(3.0f), "res/models/kostkaReady/kostka.obj"));					// [05] úwiat≥o refretorowe 2

	// úwiat≥o punktowe
	graphNodes[1]->SetLight(glm::vec3(0.05f), // ambient
		glm::vec3(0.1f, 0.6f, 0.9f), // difiuse
		glm::vec3(1.0f), // specular
		glm::vec3(0.0f,0.0f,0.0f), // light dir	(zbÍdne)
		0.0f, 0.0f, // cutOff i outherCutOff	(zbÍdne)
		1.0f, 0.0009f, 0.00032f);// t≥umienie

	// úwiat≥o kierunkowe
	graphNodes[3]->SetLight(glm::vec3(0.05f), // ambient
		glm::vec3(0.1f, 0.6f, 0.9f), // difiuse
		glm::vec3(1.0f), // specular
		glm::vec3(0.0f, -1.0f, 0.0f), // light dir
		0.0f, 0.0f, // cutOff i outherCutOff	(zbÍdne)
		0.0f, 0.0f, 0.0f);// t≥umienie			(zbÍdne)

	// úwiat≥o kierunkowe refretorowe 1
	graphNodes[4]->SetLight(glm::vec3(0.05f), // ambient
		glm::vec3(0.1f, 0.6f, 0.9f), // difiuse
		glm::vec3(1.0f), // specular
		glm::vec3(1.0f, -1.0f, 0.0f), // light dir
		10.0f, 15.0f, // cutOff i outherCutOff	
		1.0f, 0.0009f, 0.00032f);// t≥umienie

	// úwiat≥o kierunkowe refretorowe 2
	graphNodes[5]->SetLight(glm::vec3(0.05f), // ambient
		glm::vec3(0.1f, 0.6f, 0.9f), // difiuse
		glm::vec3(1.0f), // specular
		glm::vec3(1.0f, -1.0f, 0.0f), // light dir
		10.0f, 15.0f, // cutOff i outherCutOff	
		1.0f, 0.0009f, 0.00032f);// t≥umienie
				
	
	//graphNodes[2]->SetPositionOnCircle(0, 0);

	//graphNodes[1]->AddChild(NewObject(glm::vec3(0, 0, 0), glm::vec3(0.0f), glm::vec3(1.0f), "none"));								// [05] Planeta1_åcierzka
	//graphNodes[5]->AddChild(NewObject(glm::vec3(2, 0, 0), glm::vec3(45.0f, 0.0f, 0.0f), glm::vec3(1.0f), "none"));					// [06] Planeta1_Grupa
	//graphNodes[6]->AddChild(NewObject(glm::vec3(0, 0, 0), glm::vec3(0.0f), glm::vec3(1.0f), "res/models/kostkaReady/kostka.obj"));	// [07] Planeta1 
}

shared_ptr<GraphNode> SceneRoot::NewObject(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale, const std::string& path)
{
	std::shared_ptr<GraphNode> object = std::make_shared<GraphNode>(position, rotation, scale);	// wskaünik na obiekt
	graphNodes.push_back(object);	// zapisanie obiektu w wektorze

	if (path != "none")
	{
		std::shared_ptr<Model> model = std::make_shared<Model>(path);						// wskaünik na model
		object->SetModel(model);	// przypisanie obiektowi modelu
		models.push_back(model);		// zapisanie modelu w wektorze
	}
	else
	{
		models.push_back(nullptr);		// zapisanie modelu w wektorze
	}
	
	return object;
}

