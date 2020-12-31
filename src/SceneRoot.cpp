
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
	//UpdateSolarSystem(time);

	// obracanie úiÍ úwiat≥a wokÛ≥ úrodka
	graphNodes[1]->SetPositionOnCircle(time * 0.4f, 260);
	graphNodes[2]->SetPositionOnCircle(0, 0);

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
	graphNodes[index]->SetLight(lightColor);
}


void SceneRoot::CreateSolarSystem()
{
	AddChild(NewObject(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f), "none"));																// [00] wszystkie Domki
	AddChild(NewObject(glm::vec3(-5.0f, 10.0f, -15.0f), glm::vec3(0.0f), glm::vec3(8.0f), "res/models/kostkaReady/kostka.obj"));				// [01] krπøπce punktowe úwiat≥o
	AddChild(NewObject(glm::vec3(0.0f, -5.5f, 0.0f), glm::vec3(0.0f), glm::vec3(450.0f, 0.00001f, 450.0f), "res/models/PodlozeReady/kostka.obj"));					// [02] podloze





	graphNodes[1]->SetLight(glm::vec3(0.1f, 0.6f, 0.9f));

	//AddChild(NewObject(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f), "none"));													// [00] S≥oÒce   
	//AddChild(NewObject(glm::vec3(0, 0, 0), glm::vec3(45.0f, 0.0f, -10.0f), glm::vec3(1.0f), "none"));								// [01] Orbita1  
	//AddChild(NewObject(glm::vec3(0, 0, 0), glm::vec3(10.0f, 0.0f, 0.0f), glm::vec3(1.0f), "none"));									// [02] Orbita2  
	//AddChild(NewObject(glm::vec3(0, 0, 0), glm::vec3(30.0f, 0.0f, 10.0f), glm::vec3(1.0f), "none"));								// [03] Orbita3  
	//AddChild(NewObject(glm::vec3(0, 0, 0), glm::vec3(0.0f, 0.0f, 20.0f), glm::vec3(1.0f), "none"));									// [04] Orbita4  
	//																																  	
	//graphNodes[1]->AddChild(NewObject(glm::vec3(0, 0, 0), glm::vec3(0.0f), glm::vec3(1.0f), "none"));								// [05] Planeta1_åcierzka
	//graphNodes[5]->AddChild(NewObject(glm::vec3(2, 0, 0), glm::vec3(45.0f, 0.0f, 0.0f), glm::vec3(1.0f), "none"));					// [06] Planeta1_Grupa
	//graphNodes[6]->AddChild(NewObject(glm::vec3(0, 0, 0), glm::vec3(0.0f), glm::vec3(1.0f), "res/models/kostkaReady/kostka.obj"));	// [07] Planeta1 
}



void SceneRoot::UpdateSolarSystem(float time)
{
	// PrÍdkoúÊ obiegu planet wokÛ≥ gwiazdy
	graphNodes[5]->SetRotation(0, time * 100, 0);		// Orbita1
	graphNodes[8]->SetRotation(0, time * 30, 0);		// Orbita2
	graphNodes[13]->SetRotation(0, time * 30, 0);		// Orbita3
	graphNodes[22]->SetRotation(0, time * 30, 0);		// Orbita4
	// PrÍdkoúÊ obrotowa planety
	graphNodes[7]->SetRotation(0, time * 200, 0);		// Planeta1
	graphNodes[10]->SetRotation(0, time * 90, 0);		// Planeta2
	graphNodes[15]->SetRotation(0, time * 130, 0);		// Planeta3
	graphNodes[24]->SetRotation(0, time * 170, 0);		// Planeta4
	// PrÍdkoúci obiegowe ksiÍøycÛw wokÛ≥ planet
	graphNodes[11]->SetRotation(0, time * 280, 0);		// Planeta1_Orbita1
	graphNodes[16]->SetRotation(0, time * 300, 0);		// Planeta3_Orbita1
	graphNodes[18]->SetRotation(0, time * 200, 0);		// Planeta3_Orbita2
	graphNodes[20]->SetRotation(0, time * 50, 0);		// Planeta3_Orbita3
	graphNodes[25]->SetRotation(0, time * 310, 0);		// Planeta4_Orbita1
	graphNodes[27]->SetRotation(0, time * 240, 0);		// Planeta4_Orbita2
	graphNodes[29]->SetRotation(0, time * 10, 0);		// Planeta4_Orbita3
	// PrÍdkoúÊ obrotowa ksiÍøycÛw
	graphNodes[12]->SetRotation(0, time * 700, 0);		// Planeta1_KúÍøyc1
	graphNodes[17]->SetRotation(0, time * 500, 0);		// Planeta3_Orbita1
	graphNodes[19]->SetRotation(0, time * 630, 0);		// Planeta3_Orbita2
	graphNodes[21]->SetRotation(0, time * 190, 0);		// Planeta3_Orbita3
	graphNodes[26]->SetRotation(0, time * 980, 0);		// Planeta4_Orbita1
	graphNodes[28]->SetRotation(0, time * 660, 0);		// Planeta4_Orbita2
	graphNodes[30]->SetRotation(0, time * 470, 0);		// Planeta4_Orbita3
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

