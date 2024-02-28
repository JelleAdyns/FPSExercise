#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Scene.h" 
#include "Component.h"
#include "RenderComponent.h"
#include "TransformComponent.h"
#include "TextComponent.h"
#include "ImageComponent.h"
#include "FPSComponent.h"
#include "GameObject.h"

void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

	

	//Background
	auto go = std::make_shared<dae::GameObject>();
	go->AddComponent<dae::RenderComponent>();
	go->AddComponent<dae::ImageComponent>("background.tga") ;

	scene.Add(go);

	//DAE-Image
	go = std::make_shared<dae::GameObject>();
	go->AddComponent<dae::RenderComponent>();
	go->AddComponent<dae::ImageComponent>("logo.tga") ;
	go->GetTransform()->SetPosition(216, 180);

	scene.Add(go);

	//Info
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);

	go = std::make_shared<dae::GameObject>();
	go->AddComponent<dae::RenderComponent>();
	go->AddComponent<dae::TextComponent>("(Specify frame cap in the initialization of the Minigin)", font);
	go->GetTransform()->SetPosition(60, 450);

	scene.Add(go);


	font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	
	go = std::make_shared<dae::GameObject>();
	go->AddComponent<dae::RenderComponent>();
	go->AddComponent<dae::TextComponent>("Press space to toggle frame cap", font);
	go->GetTransform()->SetPosition(60, 400);

	scene.Add(go);

	//Title
	go = std::make_shared<dae::GameObject>();
	go->AddComponent<dae::RenderComponent>();
	go->AddComponent<dae::TextComponent>("Programming 4 Assignment", font);
	go->GetTransform()->SetPosition(80, 40);
	
	scene.Add(go);

	//FPSCounter
	go = std::make_shared<dae::GameObject>();
	go->AddComponent<dae::RenderComponent>();
	go->AddComponent<dae::TextComponent>("0 FPS", font);
	go->AddComponent<dae::FPSComponent>();
	
	scene.Add(go);
}

int main(int, char*[]) {
	dae::Minigin engine("../Data/", 0.02f, 60);
	engine.Run(load);
    return 0;
}