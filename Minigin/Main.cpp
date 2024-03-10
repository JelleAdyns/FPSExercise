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
#include "Components.h"
#include "GameObject.h"


void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

	auto gui = std::make_shared<dae::GameObject>();
	gui->AddComponent<dae::CacheExperimentComponent>();
	gui->AddComponent<dae::TrashTheCacheIntComponent>(10, "Exercise 1");
	scene.Add(gui);

	auto ggui = std::make_shared<dae::GameObject>();
	ggui->AddComponent<dae::CacheExperimentComponent>();
	ggui->AddComponent<dae::TrashTheCacheGameObjectsComponent>(10, "Exercise 2");
	scene.Add(ggui);

	//Background
	auto go = std::make_shared<dae::GameObject>();
	go->AddRenderComponent();
	go->AddComponent<dae::ImageComponent>("background.tga") ;

	scene.Add(go);
	  
	//DAE-Image
	go = std::make_shared<dae::GameObject>(216.f, 80.f);
	go->AddRenderComponent();
	go->AddComponent<dae::ImageComponent>("logo.tga") ;

	scene.Add(go);

	auto gob = std::make_shared<dae::GameObject>(10.f, 10.f);
	gob->AddRenderComponent();
	gob->AddComponent<dae::ImageComponent>("logo.tga");
	scene.Add(gob);

	gob->SetParent(go, false);

	// REMOVING CHILD FROM PARENT
	gob->SetParent(nullptr, true);

	////Info
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);

	go = std::make_shared<dae::GameObject>(60.f, 450.f);
	go->AddRenderComponent();
	go->AddComponent<dae::TextComponent>("(Specify frame cap in the initialization of the Minigin)", font);
	

	scene.Add(go);


	font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	
	go = std::make_shared<dae::GameObject>(60.f, 400.f);
	go->AddRenderComponent();
	go->AddComponent<dae::TextComponent>("Press space to toggle frame cap", font);

	scene.Add(go);

	//Title
	go = std::make_shared<dae::GameObject>(80.f, 40.f);
	go->AddRenderComponent();
	go->AddComponent<dae::TextComponent>("Programming 4 Assignment", font);
	
	scene.Add(go);

	//FPSCounter
	auto fpsCounter = std::make_shared<dae::GameObject>();
	fpsCounter = std::make_shared<dae::GameObject>();
	fpsCounter->AddRenderComponent();
	fpsCounter->AddComponent<dae::TextComponent>("0 FPS", font);
	fpsCounter->AddComponent<dae::FPSComponent>();
	
	scene.Add(fpsCounter);

	// Rotating Sprites
	{

		auto lo = std::make_shared<dae::GameObject>(300.f, 300.f);

		scene.Add(lo);

		auto ro = std::make_shared<dae::GameObject>();
		ro->AddRenderComponent(true);
		ro->AddComponent<dae::ImageComponent>("enemy.png");
		ro->AddComponent<dae::RotatorComponent>(75.f, 3.f);

		scene.Add(ro);

		ro->SetParent(lo, true);
		auto so = std::make_shared<dae::GameObject>();
		so->AddRenderComponent(true);
		so->AddComponent<dae::ImageComponent>("enemy2.png");
		so->AddComponent<dae::RotatorComponent>(75.f, -5.f);

		scene.Add(so);

		so->SetParent(ro, true);
		//so->SetParent(nullptr, true);
		auto bo = std::make_shared<dae::GameObject>();
		bo->AddRenderComponent(true);
		bo->AddComponent<dae::ImageComponent>("enemy.png");
		bo->AddComponent<dae::RotatorComponent>(75.f, 8.f);

		scene.Add(bo);
		bo->SetParent(so, false);
	}

	//so->SetParent(nullptr, true);
	//so->RemoveComponent<dae::RotatorComponent>();
}

int main(int, char*[]) {
	dae::Minigin engine("../Data/", 0.02f, 60);
	engine.Run(load);
    return 0;
}