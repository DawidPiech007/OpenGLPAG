
//#include <SceneRoot.hpp>
/*
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

void SceneRoot::Update()
{
	UpdateSolarSystem();

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
		//child->Update(isDirty, nodeTransform);
	}
}

void SceneRoot::Draw(Shader& shader)
{
	for (auto& child : children)
	{
		//child->Draw(shader);
	}
}

void SceneRoot::CreateSolarSystem()
{
	// Tworzenie danych
	//std::shared_ptr<GraphNode> kostkaNode = std::make_shared<GraphNode>(new GraphNode());								// wskaünik na obiekt
	//std::shared_ptr<Model> kostkaModel =  std::make_shared<Model>(new Model("res/models/kostkaReady/kostka.obj"));	// wskaünik na model
	//kostkaNode->SetModel(kostkaModel);	// przypisanie obiektowi modelu
	//
	//graphNodes.push_back(kostkaNode);	// zapisanie obiektu w wektorze
	//models.push_back(kostkaModel);		// zapisanie modelu w wektorze




	// Uk≥adanie grafu sceny
	//AddChild(kostkaNode);
}

void SceneRoot::UpdateSolarSystem()
{

}
*/