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
#include "InputCommandBinder.h"
#include "Controller.h"
#include "Scene.h" 
#include "Components.h"
#include "Commands.h"
#include "KeyState.h"
#include "GameObject.h"
#include "Achievement.h"



#pragma warning(push)
#pragma warning (disable: 4996)
#include <steam_api.h>
#pragma warning(pop)

void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

	//auto gui = std::make_unique<dae::GameObject>();
	//gui->AddComponent<dae::CacheExperimentComponent>();
	//gui->AddComponent<dae::TrashTheCacheIntComponent>(10, "Exercise 1");
	//scene.Add(std::move(gui));

	//auto ggui = std::make_unique<dae::GameObject>();
	//ggui->AddComponent<dae::CacheExperimentComponent>();
	//ggui->AddComponent<dae::TrashTheCacheGameObjectsComponent>(10, "Exercise 2");
	//scene.Add(std::move(ggui));

	//Background
	auto go = std::make_unique<dae::GameObject>();
	go->AddRenderComponent();
	go->AddComponent<dae::ImageComponent>("background.tga") ;

	scene.Add(std::move(go));
	  
	//DAE-Image
	go = std::make_unique<dae::GameObject>(216.f, 160.f);
	go->AddRenderComponent();
	go->AddComponent<dae::ImageComponent>("logo.tga") ;


	//auto gob = std::make_unique<dae::GameObject>(10.f, 10.f);
	//gob->AddRenderComponent();
	//gob->AddComponent<dae::ImageComponent>("logo.tga");

	//gob->SetParent(go, false);
	//// REMOVING CHILD FROM PARENT
	//gob->SetParent(nullptr, true);

	scene.Add(std::move(go));
	//scene.Add(std::move(gob));


	////Info
	go = std::make_unique<dae::GameObject>(10.f, 90.f);
	go->AddRenderComponent();
	go->AddComponent<dae::TextComponent>("Move Bub with WASD, Press E to die and R and T to pick stuff up.", "Lingua.otf", 15);

	scene.Add(std::move(go));

	go = std::make_unique<dae::GameObject>(10.f, 110.f);
	go->AddRenderComponent();
	go->AddComponent<dae::TextComponent>("Move Bob with the DPAD, Press X to die and A and B to pick stuff up.", "Lingua.otf", 15);

	scene.Add(std::move(go));

	go = std::make_unique<dae::GameObject>(60.f, 450.f);
	go->AddRenderComponent();
	go->AddComponent<dae::TextComponent>("(Specify frame cap in the initialization of the Minigin)", "Lingua.otf", 20);
	
	scene.Add(std::move(go));
	
	go = std::make_unique<dae::GameObject>(60.f, 400.f);
	go->AddRenderComponent();
	go->AddComponent<dae::TextComponent>("Press space to toggle frame cap", "Lingua.otf", 36);

	scene.Add(std::move(go));


	//Title
	go = std::make_unique<dae::GameObject>(80.f, 40.f);
	go->AddRenderComponent();
	go->AddComponent<dae::TextComponent>("Programming 4 Assignment", "Lingua.otf", 36);
	
	scene.Add(std::move(go));

	//FPSCounter
	auto fpsCounter = std::make_unique<dae::GameObject>();
	fpsCounter->AddRenderComponent();
	fpsCounter->AddComponent<dae::TextComponent>("0 FPS", "Lingua.otf", 36);
	fpsCounter->AddComponent<dae::FPSComponent>();
	
	scene.Add(std::move(fpsCounter));

	// Rotating Sprites
	{

		//auto lo = std::make_unique<dae::GameObject>(300.f, 300.f);


		//auto ro = std::make_unique<dae::GameObject>();
		//ro->AddRenderComponent(true);
		//ro->AddComponent<dae::ImageComponent>("enemy.png");
		//ro->AddComponent<dae::RotatorComponent>(75.f, 3.f);

		//ro->SetParent(lo, true);

		//auto so = std::make_unique<dae::GameObject>();
		//so->AddRenderComponent(true);
		//so->AddComponent<dae::ImageComponent>("enemy2.png");
		//so->AddComponent<dae::RotatorComponent>(75.f, -5.f);

		//so->SetParent(ro, true);

		//scene.Add(std::move(lo));
		//scene.Add(std::move(ro));

		////so->SetParent(nullptr, true);
		//auto bo = std::make_unique<dae::GameObject>();
		//bo->AddRenderComponent(true);
		//bo->AddComponent<dae::ImageComponent>("enemy.png");
		//bo->AddComponent<dae::RotatorComponent>(75.f, 8.f);

		//bo->SetParent(so, false);
		//scene.Add(std::move(bo));
		//scene.Add(std::move(so));
	}

	//Input Objects
	{
		int speedObject1 = 150;
		int speedObject2 = speedObject1 * 2;

		int pickUpScore1 = 50;
		int pickUpScore2 = pickUpScore1 * 2;

		int starterLives = 3;

		auto& inputMan = dae::InputCommandBinder::GetInstance();


		//PLAYER 1
		auto player1 = std::make_unique<dae::GameObject>(150.f, 150.f);
		player1->AddRenderComponent();
		player1->AddComponent<dae::LivesComponent>(starterLives);
		player1->AddComponent<dae::PickUpComponent>();
		player1->AddComponent<dae::ImageComponent>("Bubby.png");

		std::unique_ptr<dae::Command> killCommand = std::make_unique<dae::KillCommand>(player1);
		inputMan.AddKeyCommand(std::move(killCommand), SDL_SCANCODE_E, dae::KeyState::UpThisFrame);
	
		std::unique_ptr<dae::Command> pickUpCommand = std::make_unique<dae::PickUpCommand>(player1, pickUpScore1);
		inputMan.AddKeyCommand(std::move(pickUpCommand), SDL_SCANCODE_R, dae::KeyState::UpThisFrame);

		std::unique_ptr<dae::Command> pickUpCommand2 = std::make_unique<dae::PickUpCommand>(player1, pickUpScore2);
		inputMan.AddKeyCommand(std::move(pickUpCommand2), SDL_SCANCODE_T, dae::KeyState::UpThisFrame);


		auto livesDisplay = std::make_unique<dae::GameObject>(10.f, 150.f);
		livesDisplay->AddRenderComponent();
		livesDisplay->AddComponent<dae::LivesUIComponent>();
		livesDisplay->AddComponent<dae::TextComponent>("Remaining lives: " + std::to_string(starterLives), "Lingua.otf", 15);

		auto scoreDisplay = std::make_unique<dae::GameObject>(0.f, 20.f);
		scoreDisplay->AddRenderComponent();
		scoreDisplay->AddComponent<dae::ScoreUIComponent>();
		scoreDisplay->AddComponent<dae::TextComponent>("Score: 0", "Lingua.otf", 15);

		scoreDisplay->SetParent(livesDisplay, false);

		player1->GetComponent<dae::LivesComponent>()->AddObserver(livesDisplay->GetComponent<dae::LivesUIComponent>());
		player1->GetComponent<dae::PickUpComponent>()->AddObserver(scoreDisplay->GetComponent<dae::ScoreUIComponent>());
		player1->GetComponent<dae::PickUpComponent>()->AddObserver(&dae::Achievement::GetInstance());


		scene.Add(std::move(livesDisplay));
		scene.Add(std::move(scoreDisplay));



		//PLAYER 2
		auto player2 = std::make_unique<dae::GameObject>(150.f, 200.f);
		player2->AddRenderComponent();
		player2->AddComponent<dae::LivesComponent>(starterLives);
		player2->AddComponent<dae::PickUpComponent>();
		player2->AddComponent<dae::ImageComponent>("Bobby.png");

		inputMan.AddController();


		std::unique_ptr<dae::Command> killCommand2 = std::make_unique<dae::KillCommand>(player2);
		inputMan.AddControllerCommand(std::move(killCommand2), dae::ControllerButton::X, dae::KeyState::UpThisFrame, 0);

		pickUpCommand = std::make_unique<dae::PickUpCommand>(player2, pickUpScore1);
		inputMan.AddControllerCommand(std::move(pickUpCommand), dae::ControllerButton::A, dae::KeyState::UpThisFrame, 0);

		pickUpCommand2 = std::make_unique<dae::PickUpCommand>(player2, pickUpScore2);
		inputMan.AddControllerCommand(std::move(pickUpCommand2), dae::ControllerButton::B, dae::KeyState::UpThisFrame, 0);

		auto livesDisplay2 = std::make_unique<dae::GameObject>(10.f, 200.f);
		livesDisplay2->AddRenderComponent();
		livesDisplay2->AddComponent<dae::LivesUIComponent>();
		livesDisplay2->AddComponent<dae::TextComponent>("Remaining lives: " + std::to_string(starterLives), "Lingua.otf", 15);

		auto scoreDisplay2 = std::make_unique<dae::GameObject>(0.f, 20.f);
		scoreDisplay2->AddRenderComponent();
		scoreDisplay2->AddComponent<dae::ScoreUIComponent>();
		scoreDisplay2->AddComponent<dae::TextComponent>("Score: 0", "Lingua.otf", 15);

		scoreDisplay2->SetParent(livesDisplay2, false);


		player2->GetComponent<dae::LivesComponent>()->AddObserver(livesDisplay2->GetComponent<dae::LivesUIComponent>());
		player2->GetComponent<dae::PickUpComponent>()->AddObserver(scoreDisplay2->GetComponent<dae::ScoreUIComponent>());
		player2->GetComponent<dae::PickUpComponent>()->AddObserver(&dae::Achievement::GetInstance());







		scene.Add(std::move(livesDisplay2));
		scene.Add(std::move(scoreDisplay2));
		std::unique_ptr<dae::Command> moveCommand = std::make_unique<dae::MoveCommand>(player1, speedObject1, 0);
		inputMan.AddKeyCommand(std::move(moveCommand), SDL_SCANCODE_D, dae::KeyState::Pressed);
		moveCommand = std::make_unique<dae::MoveCommand>(player1, -speedObject1, 0);
		inputMan.AddKeyCommand(std::move(moveCommand), SDL_SCANCODE_A, dae::KeyState::Pressed);
		moveCommand = std::make_unique<dae::MoveCommand>(player1, 0, -speedObject1);
		inputMan.AddKeyCommand(std::move(moveCommand), SDL_SCANCODE_W, dae::KeyState::Pressed);
		moveCommand = std::make_unique<dae::MoveCommand>(player1, 0, speedObject1);
		inputMan.AddKeyCommand(std::move(moveCommand), SDL_SCANCODE_S, dae::KeyState::Pressed);

		/*inputMan.AddController();
		std::unique_ptr<dae::Command> moveCommand = std::make_unique<dae::MoveCommand>(player1, speedObject1, 0);
		inputMan.AddControllerCommand(std::move(moveCommand), dae::ControllerButtons::DpadRight, dae::KeyState::Pressed, 0);
		moveCommand = std::make_unique<dae::MoveCommand>(player1, -speedObject1, 0);
		inputMan.AddControllerCommand(std::move(moveCommand), dae::ControllerButtons::DpadLeft, dae::KeyState::Pressed, 0);
		moveCommand = std::make_unique<dae::MoveCommand>(player1, 0, -speedObject1);
		inputMan.AddControllerCommand(std::move(moveCommand), dae::ControllerButtons::DpadUp, dae::KeyState::Pressed, 0);
		moveCommand = std::make_unique<dae::MoveCommand>(player1, 0, speedObject1);
		inputMan.AddControllerCommand(std::move(moveCommand), dae::ControllerButtons::DpadDown, dae::KeyState::Pressed, 0);*/

		moveCommand = std::make_unique<dae::MoveCommand>(player2, speedObject2, 0);
		inputMan.AddControllerCommand(std::move(moveCommand), dae::ControllerButton::DpadRight, dae::KeyState::Pressed, 0);
		moveCommand = std::make_unique<dae::MoveCommand>(player2, -speedObject2, 0);
		inputMan.AddControllerCommand(std::move(moveCommand), dae::ControllerButton::DpadLeft, dae::KeyState::Pressed, 0);
		moveCommand = std::make_unique<dae::MoveCommand>(player2, 0, -speedObject2);
		inputMan.AddControllerCommand(std::move(moveCommand), dae::ControllerButton::DpadUp, dae::KeyState::Pressed, 0);
		moveCommand = std::make_unique<dae::MoveCommand>(player2, 0, speedObject2);
		inputMan.AddControllerCommand(std::move(moveCommand), dae::ControllerButton::DpadDown, dae::KeyState::Pressed, 0);


		scene.Add(std::move(player1));
		scene.Add(std::move(player2));
	}

}

int main(int, char* []) {

	if (!SteamAPI_Init())
	{
		std::cerr << "Fatal Error - Steam must be running to play this game (SteamAPI_Init() failed)." << std::endl;
		return 1;
	}
	else
	{
		dae::Achievement::GetInstance().SetSteamAchievements( new CSteamAchievements(dae::Achievement::g_Achievements, 4));
		std::cout << "Successfully initialized steam." << std::endl;
	}

	dae::Minigin engine("../Data/", 0.02f, 60);
	engine.Run(load);

	// Shutdown Steam
	SteamAPI_Shutdown();

    return 0;
}