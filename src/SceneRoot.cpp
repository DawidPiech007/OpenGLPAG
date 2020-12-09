
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

void SceneRoot::Draw(Shader& shader, Shader& orbitShader, Shader& sphereShader, int resolution)
{
	for (auto& child : children)
	{
		child->Draw(shader, orbitShader, sphereShader, resolution);
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
	AddChild(NewObject(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f), "none"));													// [00] S≥oÒce   
	AddChild(NewObject(glm::vec3(0, 0, 0), glm::vec3(45.0f, 0.0f, -10.0f), glm::vec3(1.0f), "none"));								// [01] Orbita1  
	AddChild(NewObject(glm::vec3(0, 0, 0), glm::vec3(10.0f, 0.0f, 0.0f), glm::vec3(1.0f), "none"));									// [02] Orbita2  
	AddChild(NewObject(glm::vec3(0, 0, 0), glm::vec3(30.0f, 0.0f, 10.0f), glm::vec3(1.0f), "none"));								// [03] Orbita3  
	AddChild(NewObject(glm::vec3(0, 0, 0), glm::vec3(0.0f, 0.0f, 20.0f), glm::vec3(1.0f), "none"));									// [04] Orbita4  
																																	  	
	graphNodes[1]->AddChild(NewObject(glm::vec3(0, 0, 0), glm::vec3(0.0f), glm::vec3(1.0f), "none"));								// [05] Planeta1_åcierzka
	graphNodes[5]->AddChild(NewObject(glm::vec3(2, 0, 0), glm::vec3(45.0f, 0.0f, 0.0f), glm::vec3(1.0f), "none"));					// [06] Planeta1_Grupa
	graphNodes[6]->AddChild(NewObject(glm::vec3(0, 0, 0), glm::vec3(0.0f), glm::vec3(1.0f), "res/models/kostkaReady/kostka.obj"));	// [07] Planeta1 
																																		
	graphNodes[2]->AddChild(NewObject(glm::vec3(0, 0, 0), glm::vec3(0.0f), glm::vec3(1.0f), "none"));								// [08] Planeta2_åcierzka
	graphNodes[8]->AddChild(NewObject(glm::vec3(0, 0, 5), glm::vec3(0.0f), glm::vec3(1.0f), "none"));								// [09] Planeta2_Grupa
	graphNodes[9]->AddChild(NewObject(glm::vec3(0, 0, 0), glm::vec3(0.0f), glm::vec3(1.4f), "res/models/kostkaReady/kostka.obj"));	// [10] Planeta2
	graphNodes[9]->AddChild(NewObject(glm::vec3(0, 0, 0), glm::vec3(0.0f), glm::vec3(1.0f), "none"));								// [11] Planeta2_Orbita1
	graphNodes[11]->AddChild(NewObject(glm::vec3(2, 0, 0), glm::vec3(0.0f), glm::vec3(0.4f), "res/models/kostkaReady/kostka.obj"));	// [12] Planeta2_KsiÍøyc1

	graphNodes[3]->AddChild(NewObject(glm::vec3(0, 0, 0), glm::vec3(0.0f), glm::vec3(1.0f), "none"));								// [13] Planeta3_åcierzka
	graphNodes[13]->AddChild(NewObject(glm::vec3(10, 0, 10), glm::vec3(0.0f), glm::vec3(1.0f), "none"));							// [14] Planeta3_Grupa
	graphNodes[14]->AddChild(NewObject(glm::vec3(0, 0, 0), glm::vec3(0.0f), glm::vec3(1.4f), "res/models/Krzywa2/krzywa.obj"));		// [15] Planeta3
	graphNodes[14]->AddChild(NewObject(glm::vec3(0, 0, 0), glm::vec3(0.0f), glm::vec3(1.0f), "none"));								// [16] Planeta3_Orbita1
	graphNodes[16]->AddChild(NewObject(glm::vec3(2, 0, 0), glm::vec3(0.0f), glm::vec3(0.4f), "res/models/kostkaReady/kostka.obj"));	// [17] Planeta3_KsiÍøyc1
	graphNodes[14]->AddChild(NewObject(glm::vec3(0, 0, 0), glm::vec3(0.0f), glm::vec3(1.0f), "none"));								// [18] Planeta3_Orbita2
	graphNodes[18]->AddChild(NewObject(glm::vec3(3, 0, 0), glm::vec3(0.0f), glm::vec3(0.5f), "res/models/kostkaReady/kostka.obj"));	// [19] Planeta3_KsiÍøyc2
	graphNodes[14]->AddChild(NewObject(glm::vec3(0, 0, 0), glm::vec3(0.0f), glm::vec3(1.0f), "none"));								// [20] Planeta3_Orbita3
	graphNodes[20]->AddChild(NewObject(glm::vec3(5, 0, 0), glm::vec3(0.0f), glm::vec3(0.3f), "res/models/kostkaReady/kostka.obj"));	// [21] Planeta3_KsiÍøyc3

	graphNodes[4]->AddChild(NewObject(glm::vec3(0, 0, 0), glm::vec3(0.0f), glm::vec3(1.0f), "none"));								// [22] Planeta4_åcierzka
	graphNodes[22]->AddChild(NewObject(glm::vec3(20, 0, 30), glm::vec3(0.0f), glm::vec3(1.0f), "none"));							// [23] Planeta4_Grupa
	graphNodes[23]->AddChild(NewObject(glm::vec3(0, 0, 0), glm::vec3(0.0f), glm::vec3(3.1f), "res/models/KulaWcieta/KulaWcietaReady.obj"));	// [24] Planeta4
	graphNodes[23]->AddChild(NewObject(glm::vec3(0, 0, 0), glm::vec3(0.0f), glm::vec3(1.0f), "none"));								// [25] Planeta4_Orbita1
	graphNodes[25]->AddChild(NewObject(glm::vec3(2, 0, 0), glm::vec3(0.0f), glm::vec3(0.4f), "res/models/kostkaReady/kostka.obj"));	// [26] Planeta4_KsiÍøyc1
	graphNodes[23]->AddChild(NewObject(glm::vec3(0, 0, 0), glm::vec3(0.0f), glm::vec3(1.0f), "none"));								// [27] Planeta4_Orbita2
	graphNodes[27]->AddChild(NewObject(glm::vec3(3, 0, 0), glm::vec3(0.0f), glm::vec3(0.5f), "res/models/kostkaReady/kostka.obj"));	// [28] Planeta4_KsiÍøyc2
	graphNodes[23]->AddChild(NewObject(glm::vec3(0, 0, 0), glm::vec3(0.0f), glm::vec3(1.0f), "none"));								// [29] Planeta4_Orbita3
	graphNodes[29]->AddChild(NewObject(glm::vec3(5, 0, 0), glm::vec3(0.0f), glm::vec3(0.3f), "res/models/kostkaReady/kostka.obj"));	// [30] Planeta4_KsiÍøyc3

	graphNodes[0]->SetSphere(1.0f, 2.0f, glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));

	graphNodes[1]->SrtOrbit(2.0f, 15.0f,	glm::vec4(0.0f, 1.0f, 1.0f, 1.0f));
	graphNodes[2]->SrtOrbit(5.0f, 15.0f,	glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
	graphNodes[3]->SrtOrbit(14.141f, 15.0f,	glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	graphNodes[4]->SrtOrbit(36.055f, 15.0f,	glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));

	graphNodes[11]->SrtOrbit(2.0f, 15.0f,	glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));
	graphNodes[16]->SrtOrbit(2.0f, 15.0f,	glm::vec4(0.5f, 0.0f, 1.0f, 1.0f));
	graphNodes[18]->SrtOrbit(3.0f, 15.0f,	glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	graphNodes[20]->SrtOrbit(5.0f, 15.0f,	glm::vec4(0.5f, 1.0f, 0.0f, 1.0f));
	graphNodes[25]->SrtOrbit(2.0f, 15.0f,	glm::vec4(1.0f, 0.5f, 0.0f, 1.0f));
	graphNodes[27]->SrtOrbit(3.0f, 15.0f,	glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
	graphNodes[29]->SrtOrbit(5.0f, 15.0f,	glm::vec4(0.0f, 0.5f, 0.0f, 1.0f));

	//	Kolory
	//	1. gwiazda		- øÛ≥ty		1.0f, 1.0f, 0.0f
	//
	//	2. planeta 1	- b≥Íkitny  0.0f, 1.0f, 1.0f
	//	3. planeta 2	- niebieski 0.0f, 0.0f, 1.0f
	//	4. planeta 3	- czerwony  1.0f, 0.0f, 0.0f
	//	5. planeta 4	- zielony	0.0f, 1.0f, 0.0f
	//
	//	6. ksiÍøyc 2.1	- rÛøowy	1.0f, 0.0f, 1.0f
	//
	//	7. ksiÍøyc 3.1	- fioletowy	0.5f, 0.0f, 1.0f
	//	8. ksiÍøyc 3.2	- bia≥y		1.0f, 1.0f, 1.0f
	//	9. ksiÍøyc 3.3	- zielonkawy0.5f, 1.0f, 0.0f
	//
	//	10.ksiÍøyc 4.1	- pomaraÒczo1.0f, 0.5f, 0.0f
	//	11.ksiÍøyc 4.2	- szary		0.5f, 0.5f, 0.5f
	//	12.ksiÍøyc 4.3	- ciem.zielo0.0f, 0.5f, 0.0f
}



void SceneRoot::UpdateSolarSystem(float time)
{
	// PrÍdkoúÊ obiegu planet wokÛ≥ gwiazdy
	graphNodes[5]->SetRotation(0, time * 100, 0);		// Orbita1
	graphNodes[8]->SetRotation(0, time * 30, 0);		// Orbita2
	graphNodes[13]->SetRotation(0, time * 30, 0);		// Orbita3
	graphNodes[22]->SetRotation(0, time * 30, 0);		// Orbita4
	// PrÍdkoúÊ obrotowa planety
	graphNodes[7]->SetRotation(0, time * 70, 0);		// Planeta1
	graphNodes[10]->SetRotation(0, time * 50, 0);		// Planeta2
	graphNodes[15]->SetRotation(0, time * 20, 0);		// Planeta3
	graphNodes[24]->SetRotation(0, time * 15, 0);		// Planeta4
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

