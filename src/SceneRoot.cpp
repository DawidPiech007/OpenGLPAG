
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
	UpdateSolarSystem(time);

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

void SceneRoot::Draw(Shader& shader)
{
	for (auto& child : children)
	{
		child->Draw(shader);
	}
}

void SceneRoot::CreateSolarSystem()
{
	//// Tworzenie danych
	//std::shared_ptr<GraphNode> kostkaNode = std::make_shared<GraphNode>(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f));	// wskaünik na obiekt
	//std::shared_ptr<Model> kostkaModel =  std::make_shared<Model>("res/models/kostkaReady/kostka.obj");						// wskaünik na model
	//kostkaNode->SetModel(kostkaModel);	// przypisanie obiektowi modelu
	//// ZapamiÍtywanie
	//graphNodes.push_back(kostkaNode);	// zapisanie obiektu w wektorze
	//models.push_back(kostkaModel);		// zapisanie modelu w wektorze
	
	
	// Uk≥adanie grafu sceny
	//AddChild(kostkaNode);

	AddChild(NewObject(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(2.0f), "res/models/kostkaReady/kostka.obj"));				// S≥oÒce   [0]
	AddChild(NewObject(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f), "none"));												// Orbita1  [1]
	AddChild(NewObject(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f), "none"));												// Orbita2  [2]
	AddChild(NewObject(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f), "none"));												// Orbita3  [3]
	//
	children[0]->AddChild(NewObject(glm::vec3(2.0f), glm::vec3(0.0f), glm::vec3(1.0f), "res/models/kostkaReady/kostka.obj"));	// Planeta1 [4]
}



void SceneRoot::UpdateSolarSystem(float time)
{
	//(float)glfwGetTime();
	graphNodes[1]->SetRotation(0, time*100, 0);
	graphNodes[4]->SetRotation(0, time*50, 0);

	graphNodes[4]->PrintMatrix(graphNodes[4]->transform);
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

