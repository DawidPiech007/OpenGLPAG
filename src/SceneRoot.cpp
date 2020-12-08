
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

void SceneRoot::Draw(Shader& shader, Shader& orbitShader, Shader& sphereShader)
{
	for (auto& child : children)
	{
		child->Draw(shader, orbitShader, sphereShader);
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
	AddChild(NewObject(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(2.0f), "res/models/kostkaReady/kostka.obj"));					// [00] S≥oÒce   
	AddChild(NewObject(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f), "none"));													// [01] Orbita1  
	AddChild(NewObject(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f), "none"));													// [02] Orbita2  
	AddChild(NewObject(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f), "none"));													// [03] Orbita3  
	AddChild(NewObject(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f), "none"));													// [04] Orbita4  
																																	  	
	graphNodes[1]->AddChild(NewObject(glm::vec3(2, 0, 0), glm::vec3(0.0f), glm::vec3(1.0f), "none"));								// [05] Planeta1_Grupa
	graphNodes[5]->AddChild(NewObject(glm::vec3(0, 0, 0), glm::vec3(0.0f), glm::vec3(1.0f), "res/models/kostkaReady/kostka.obj"));	// [06] Planeta1 
	graphNodes[1]->AddChild(NewObject(glm::vec3(0, 0, 0), glm::vec3(0.0f), glm::vec3(1.0f), "none"));								// [07] Planeta1_åcierzka
																																		
	graphNodes[2]->AddChild(NewObject(glm::vec3(0, 0, 5), glm::vec3(0.0f), glm::vec3(1.0f), "none"));								// [08] Planeta2_Grupa
	graphNodes[8]->AddChild(NewObject(glm::vec3(0, 0, 0), glm::vec3(0.0f), glm::vec3(1.4f), "res/models/kostkaReady/kostka.obj"));	// [09] Planeta2
	graphNodes[2]->AddChild(NewObject(glm::vec3(0, 0, 0), glm::vec3(0.0f), glm::vec3(1.0f), "none"));								// [10] Planeta2_åcierzka
	graphNodes[8]->AddChild(NewObject(glm::vec3(0, 0, 0), glm::vec3(0.0f), glm::vec3(1.0f), "none"));								// [11] Planeta2_Orbita1
	graphNodes[11]->AddChild(NewObject(glm::vec3(2, 0, 0), glm::vec3(0.0f), glm::vec3(0.4f), "res/models/kostkaReady/kostka.obj"));	// [12] Planeta2_KsiÍøyc1

	graphNodes[3]->AddChild(NewObject(glm::vec3(10, 0, 10), glm::vec3(0.0f), glm::vec3(1.0f), "none"));								// [13] Planeta3_Grupa
	graphNodes[13]->AddChild(NewObject(glm::vec3(0, 0, 0), glm::vec3(0.0f), glm::vec3(1.4f), "res/models/kostkaReady/kostka.obj"));	// [14] Planeta3
	graphNodes[3]->AddChild(NewObject(glm::vec3(0, 0, 0), glm::vec3(0.0f), glm::vec3(1.0f), "none"));								// [15] Planeta3_åcierzka
	graphNodes[13]->AddChild(NewObject(glm::vec3(0, 0, 0), glm::vec3(0.0f), glm::vec3(1.0f), "none"));								// [16] Planeta3_Orbita1
	graphNodes[16]->AddChild(NewObject(glm::vec3(2, 0, 0), glm::vec3(0.0f), glm::vec3(0.4f), "res/models/kostkaReady/kostka.obj"));	// [17] Planeta3_KsiÍøyc1
	graphNodes[13]->AddChild(NewObject(glm::vec3(0, 0, 0), glm::vec3(0.0f), glm::vec3(1.0f), "none"));								// [18] Planeta3_Orbita2
	graphNodes[18]->AddChild(NewObject(glm::vec3(3, 0, 0), glm::vec3(0.0f), glm::vec3(0.5f), "res/models/kostkaReady/kostka.obj"));	// [19] Planeta3_KsiÍøyc2
	graphNodes[13]->AddChild(NewObject(glm::vec3(0, 0, 0), glm::vec3(0.0f), glm::vec3(1.0f), "none"));								// [20] Planeta3_Orbita3
	graphNodes[21]->AddChild(NewObject(glm::vec3(5, 0, 0), glm::vec3(0.0f), glm::vec3(0.3f), "res/models/kostkaReady/kostka.obj"));	// [21] Planeta3_KsiÍøyc3

	graphNodes[3]->AddChild(NewObject(glm::vec3(20, 0, 30), glm::vec3(0.0f), glm::vec3(1.0f), "none"));								// [22] Planeta4_Grupa
	graphNodes[22]->AddChild(NewObject(glm::vec3(0, 0, 0), glm::vec3(0.0f), glm::vec3(1.1f), "res/models/kostkaReady/kostka.obj"));	// [23] Planeta4
	graphNodes[4]->AddChild(NewObject(glm::vec3(0, 0, 0), glm::vec3(0.0f), glm::vec3(1.0f), "none"));								// [24] Planeta4_åcierzka
	graphNodes[22]->AddChild(NewObject(glm::vec3(0, 0, 0), glm::vec3(0.0f), glm::vec3(1.0f), "none"));								// [25] Planeta4_Orbita1
	graphNodes[25]->AddChild(NewObject(glm::vec3(2, 0, 0), glm::vec3(0.0f), glm::vec3(0.4f), "res/models/kostkaReady/kostka.obj"));	// [26] Planeta4_KsiÍøyc1
	graphNodes[22]->AddChild(NewObject(glm::vec3(0, 0, 0), glm::vec3(0.0f), glm::vec3(1.0f), "none"));								// [27] Planeta4_Orbita2
	graphNodes[27]->AddChild(NewObject(glm::vec3(3, 0, 0), glm::vec3(0.0f), glm::vec3(0.5f), "res/models/kostkaReady/kostka.obj"));	// [28] Planeta4_KsiÍøyc2
	graphNodes[22]->AddChild(NewObject(glm::vec3(0, 0, 0), glm::vec3(0.0f), glm::vec3(1.0f), "none"));								// [29] Planeta4_Orbita3
	graphNodes[29]->AddChild(NewObject(glm::vec3(5, 0, 0), glm::vec3(0.0f), glm::vec3(0.3f), "res/models/kostkaReady/kostka.obj"));	// [30] Planeta4_KsiÍøyc3

	//graphNodes[7]->SrtOrbit(0.5f, 5.0f);
	//graphNodes[10]->SrtOrbit(1.0f, 5.0f);
	//graphNodes[15]->SrtOrbit(3.0f, 5.0f);
	//graphNodes[24]->SrtOrbit(10.0f, 5.0f);
}



void SceneRoot::UpdateSolarSystem(float time)
{
	// PrÍdkoúÊ obiegu planet wokÛ≥ gwiazdy
	graphNodes[1]->SetRotation(time * 100, time * 100, 0);		// Orbita1
	graphNodes[2]->SetRotation(0, time * 30, 0);		// Orbita2
	graphNodes[3]->SetRotation(0, time * 30, 0);		// Orbita3
	graphNodes[4]->SetRotation(0, time * 30, 0);		// Orbita4
	// PrÍdkoúÊ obrotowa planety
	graphNodes[6]->SetRotation(0, time * 70, 0);		// Planeta1
	graphNodes[9]->SetRotation(0, time * 50, 0);		// Planeta2
	graphNodes[12]->SetRotation(0, time * 20, 0);		// Planeta3
	graphNodes[22]->SetRotation(0, time * 15, 0);		// Planeta4
	// PrÍdkoúci obiegowe ksiÍøycÛw wokÛ≥ planet
	graphNodes[11]->SetRotation(0, time * 280, 0);		// Planeta1_Orbita1
	graphNodes[16]->SetRotation(0, time * 300, 0);		// Planeta3_Orbita1
	graphNodes[18]->SetRotation(0, time * 200, 0);		// Planeta3_Orbita2
	graphNodes[20]->SetRotation(0, time * 50, 0);		// Planeta3_Orbita3
	graphNodes[25]->SetRotation(0, time * 310, 0);		// Planeta4_Orbita1
	graphNodes[27]->SetRotation(0, time * 240, 0);		// Planeta4_Orbita2
	graphNodes[29]->SetRotation(0, time * 10, 0);		// Planeta4_Orbita3
	// PrÍdkoúÊ obrotowa ksiÍøycÛw
	graphNodes[12]->SetRotation(0, time * 100, 0);		// Planeta1_KúÍøyc1
	graphNodes[17]->SetRotation(0, time * 130, 0);		// Planeta3_Orbita1
	graphNodes[19]->SetRotation(0, time * 70, 0);		// Planeta3_Orbita2
	graphNodes[21]->SetRotation(0, time * 50, 0);		// Planeta3_Orbita3
	graphNodes[26]->SetRotation(0, time * 200, 0);		// Planeta4_Orbita1
	graphNodes[28]->SetRotation(0, time * 170, 0);		// Planeta4_Orbita2
	graphNodes[30]->SetRotation(0, time * 20, 0);		// Planeta4_Orbita3
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

