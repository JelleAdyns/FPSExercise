#include "Game.h"

#include "SceneManager.h"
#include "ResourceManager.h"
#include "InputCommandBinder.h"
#include "Controller.h"
#include "Scene.h" 
#include "Components.h"
#include "Commands.h"
#include "KeyState.h"
#include "GameObject.h"
#include "Achievements.h"
#include "LevelLoader.h"
#include "CollisionTags.h"
#include <Minigin.h>
#include <SceneManager.h>
#include <functional>
#include <AudioLocator.h>
#include <SDLAudio.h>
#include <LogAudio.h>



//#pragma warning(push)
//#pragma warning (disable: 4996)
//#include <steam_api.h>
//#pragma warning(pop)

#include <iostream>

void Game::StartGame()
{
	SetScene(CurrScene::TitleScreen);
}

void Game::SetScene(Game::CurrScene newScene)
{
	m_ActiveScene = newScene;
	switch (m_ActiveScene)
	{
	case CurrScene::TitleScreen:
		LoadTitleScreen();
		break;
	case CurrScene::Menu:		
		LoadMainMenu();
		break;
	case CurrScene::Level1:
		LoadLevelOne();
		break;
	case CurrScene::Level2:
		LoadLevelTwo();
		break;
	case CurrScene::Level3:
		LoadLevelThree();
		break;
	case CurrScene::HighScore:
		break;
	}

	dae::SceneManager::GetInstance().RemoveNonActiveScenes();

	dae::SceneManager::GetInstance().Start();
}

void Game::LoadTitleScreen() const
{

#ifndef NDEBUG
	dae::AudioLocator::RegisterAudioService(std::make_unique<dae::LogAudio>( std::make_unique<dae::SDLAudio>()));
#else
	dae::AudioLocator::RegisterAudioService(std::make_unique<dae::SDLAudio>());
#endif // !NDEBUG

	auto& ss2 = dae::AudioLocator::GetAudioService();
	ss2.AddSound("Sounds/MainTheme.mp3", static_cast<dae::SoundID>( SoundEvent::MainTheme));
	ss2.PlaySoundClip(static_cast<dae::SoundID>(SoundEvent::MainTheme), 80, true);

	auto& sceneManager = dae::SceneManager::GetInstance();
	auto& scene = sceneManager.CreateScene("TitleScreen");
	auto& inputMan = dae::InputCommandBinder::GetInstance();
	inputMan.RemoveAllCommands();
	inputMan.AddController();

	auto titleLogo = std::make_unique<dae::GameObject>(static_cast<float>(dae::Minigin::GetWindowSize().x) / 2, static_cast<float>(dae::Minigin::GetWindowSize().y) / 2 - 20);
	titleLogo->AddRenderComponent(true);
	titleLogo->AddComponent<SpriteComponent>("Textures/TitleLogo.png", 6, 1, 0.2f);

	scene.AddGameObject(std::move(titleLogo));


	auto buttonHandler = std::make_unique<dae::GameObject>();
	buttonHandler->AddComponent<ButtonHandlerComponent>();

	std::shared_ptr<dae::Command> activateCommand = std::make_shared<ActivateButtonCommand>(buttonHandler);
	inputMan.AddKeyCommand(activateCommand, SDL_SCANCODE_SPACE, dae::KeyState::DownThisFrame);
	inputMan.AddControllerCommand(activateCommand, dae::ControllerButton::Start, dae::KeyState::DownThisFrame, 0);

	const auto& handlerComponent = buttonHandler->GetComponent<ButtonHandlerComponent>();

	std::unique_ptr<dae::Command> loadCommand = std::make_unique<LoadSceneCommand>(CurrScene::Menu);

	auto button1 = std::make_unique<dae::GameObject>(static_cast<float>(dae::Minigin::GetWindowSize().x) / 2, 180.f);
	button1->AddRenderComponent(true);
	button1->AddComponent<dae::TextComponent>(inputMan.IsKeyboardActive() ? "Press Space" : "Push Start Button", "Fonts/Pixel_NES.otf", 10);
	button1->AddComponent<ButtonComponent>(std::move(loadCommand));


	handlerComponent->AddButton(button1->GetComponent<ButtonComponent>());

	std::unique_ptr<dae::Command> setTextCommand = std::make_unique<SetTextCommand>(button1, "Push Start Button");
	inputMan.AddCommand_ChangingToController(std::move(setTextCommand));
	std::unique_ptr<dae::Command> setTextCommand2 = std::make_unique<SetTextCommand>(button1, "Press Space");
	inputMan.AddCommand_ChangingToKeyboard(std::move(setTextCommand2));


	auto credit = std::make_unique<dae::GameObject>(3.f, dae::Minigin::GetWindowSize().y - 8.f);
	credit->AddRenderComponent(false);
	credit->AddComponent<dae::TextComponent>("Made by Jelle Adyns", "Fonts/Pixel_NES.otf", 5);

	scene.AddGameObject(std::move(buttonHandler));
	scene.AddGameObject(std::move(button1));
	scene.AddGameObject(std::move(credit));
}

void Game::LoadMainMenu() const
{
	auto& sceneManager = dae::SceneManager::GetInstance();
	auto& scene = sceneManager.CreateScene("MainMenu");

	auto& inputMan = dae::InputCommandBinder::GetInstance();
	inputMan.RemoveAllCommands();
	

	auto bubbleBobbleText = std::make_unique<dae::GameObject>(static_cast<float>(dae::Minigin::GetWindowSize().x) / 2- 30.f, 20.f);
	bubbleBobbleText->AddRenderComponent(false);
	bubbleBobbleText->AddComponent<dae::TextComponent>("Bubble Bobble", "Fonts/Pixel_NES.otf", 10);


	auto buttonHandler = std::make_unique<dae::GameObject>();
	buttonHandler->AddComponent<ButtonHandlerComponent>();

	
	const auto& handlerComponent = buttonHandler->GetComponent<ButtonHandlerComponent>();
	

	std::unique_ptr<dae::Command> loadCommand = std::make_unique<LoadSceneCommand>(CurrScene::Level1);

	auto button1 = std::make_unique<dae::GameObject>(0.f, 30.f);
	button1->AddRenderComponent(false);
	button1->AddComponent<dae::TextComponent>("Level 1", "Fonts/Pixel_NES.otf", 10);
	button1->AddComponent<ButtonComponent>(std::move(loadCommand));
	button1->SetParent(bubbleBobbleText, false);

	handlerComponent->AddButton(button1->GetComponent<ButtonComponent>());


	std::unique_ptr<dae::Command> loadCommand2 = std::make_unique<LoadSceneCommand>(CurrScene::Level2);

	auto button2 = std::make_unique<dae::GameObject>(0.f, 30.f);
	button2->AddRenderComponent(false);
	button2->AddComponent<dae::TextComponent>("Level 2", "Fonts/Pixel_NES.otf", 10);
	button2->AddComponent<ButtonComponent>(std::move(loadCommand2));
	button2->SetParent(button1, false);

	handlerComponent->AddButton(button2->GetComponent<ButtonComponent>());

	std::unique_ptr<dae::Command> loadCommand3 = std::make_unique<LoadSceneCommand>(CurrScene::Level3);

	auto button3 = std::make_unique<dae::GameObject>(0.f, 30.f);
	button3->AddRenderComponent(false);
	button3->AddComponent<dae::TextComponent>("Level 3", "Fonts/Pixel_NES.otf", 10);
	button3->AddComponent<ButtonComponent>(std::move(loadCommand3));
	button3->SetParent(button2, false);

	handlerComponent->AddButton(button3->GetComponent<ButtonComponent>());


	/*std::unique_ptr<dae::Command> loadCommand4 = std::make_unique<LoadSceneCommand>(CurrScene::TitleScreen);

	auto button4 = std::make_unique<dae::GameObject>(0.f, 30.f);
	button4->AddRenderComponent(false);
	button4->AddComponent<dae::TextComponent>("HighScores", "Fonts/Pixel_NES.otf", 10);
	button4->AddComponent<ButtonComponent>(std::move(loadCommand4));
	button4->SetParent(button3, false);

	handlerComponent->AddButton(button4->GetComponent<ButtonComponent>());*/

	

	auto icon = std::make_unique<dae::GameObject>(button1->GetWorldPosition().x - 20.f, button1->GetWorldPosition().y);
	icon->AddRenderComponent(false);
	icon->AddComponent<SpriteComponent>("Textures/Selector.png", 2,1,0.1f);


	std::shared_ptr<dae::Command> nextCommand = std::make_shared<SelectButtonCommand>(buttonHandler, icon, SelectButtonCommand::Direction::Next);
	inputMan.AddKeyCommand(nextCommand, SDL_SCANCODE_S, dae::KeyState::DownThisFrame);
	inputMan.AddControllerCommand(nextCommand, dae::ControllerButton::DpadDown, dae::KeyState::DownThisFrame, 0);

	std::shared_ptr<dae::Command> previousCommand = std::make_shared<SelectButtonCommand>(buttonHandler, icon, SelectButtonCommand::Direction::Previous);
	inputMan.AddKeyCommand(previousCommand, SDL_SCANCODE_W, dae::KeyState::DownThisFrame);
	inputMan.AddControllerCommand(previousCommand, dae::ControllerButton::DpadUp, dae::KeyState::DownThisFrame, 0);

	std::shared_ptr<dae::Command> activateCommand = std::make_shared<ActivateButtonCommand>(buttonHandler);
	inputMan.AddKeyCommand(activateCommand, SDL_SCANCODE_RETURN, dae::KeyState::UpThisFrame);
	inputMan.AddControllerCommand(activateCommand, dae::ControllerButton::A, dae::KeyState::UpThisFrame, 0);

	scene.AddGameObject(std::move(buttonHandler));
	scene.AddGameObject(std::move(bubbleBobbleText));
	scene.AddGameObject(std::move(icon));
	scene.AddGameObject(std::move(button1));
	scene.AddGameObject(std::move(button2));
	scene.AddGameObject(std::move(button3));
	//scene.AddGameObject(std::move(button4));

	auto info = std::make_unique<dae::GameObject>(static_cast<float>(dae::Minigin::GetWindowSize().x) / 2, 180.f);
	info->AddRenderComponent(true);
	info->AddComponent<dae::TextComponent>(inputMan.IsKeyboardActive() ? "Move: W/S | choose: ENTER" : "Move: DPAD | choose: A", "Fonts/Pixel_NES.otf", 10);

	std::unique_ptr<dae::Command> setTextCommand = std::make_unique<SetTextCommand>(info, "Move: DPAD | choose: A");
	inputMan.AddCommand_ChangingToController(std::move(setTextCommand));
	std::unique_ptr<dae::Command> setTextCommand2 = std::make_unique<SetTextCommand>(info,"Move: W/S | choose: ENTER" );
	inputMan.AddCommand_ChangingToKeyboard(std::move(setTextCommand2));

	scene.AddGameObject(std::move(info));
}

void Game::LoadLevelOne() const
{
	auto& inputMan = dae::InputCommandBinder::GetInstance();
	inputMan.RemoveAllCommands();

	auto& sceneManager = dae::SceneManager::GetInstance();

	auto& scene = sceneManager.CreateScene("Level1");


	LoadLevel("Levels.txt", scene, 1);


	////Info
	//auto go = std::make_unique<dae::GameObject>(2.f, 28.f);
	//go->AddRenderComponent();
	//go->AddComponent<dae::TextComponent>("Move Bub with WASD, Press E to die and R and T to pick stuff up.", "Lingua.otf", 8);

	//scene.AddGameObject(std::move(go));

	//go = std::make_unique<dae::GameObject>(2.f, 35.f);
	//go->AddRenderComponent();
	//go->AddComponent<dae::TextComponent>("Move Bob with the DPAD, Press X to die and A and B to pick stuff up.", "Lingua.otf", 8);

	//scene.AddGameObject(std::move(go));

	//go = std::make_unique<dae::GameObject>(15.f, 210.f);
	//go->AddRenderComponent();
	//go->AddComponent<dae::TextComponent>("(Specify frame cap in the initialization of the Minigin)", "Lingua.otf", 8);

	//scene.AddGameObject(std::move(go));

	//go = std::make_unique<dae::GameObject>(15.f, 200.f);
	//go->AddRenderComponent();
	//go->AddComponent<dae::TextComponent>("Press space to toggle frame cap", "Lingua.otf", 8);

	//scene.AddGameObject(std::move(go));


	////Title
	//go = std::make_unique<dae::GameObject>(30.f, 15.f);
	//go->AddRenderComponent();
	//go->AddComponent<dae::TextComponent>("Programming 4 Assignment", "Lingua.otf", 8);

	//scene.AddGameObject(std::move(go));

	//FPSCounter
	auto fpsCounter = std::make_unique<dae::GameObject>();
	fpsCounter->AddRenderComponent();
	fpsCounter->AddComponent<dae::TextComponent>("0 FPS", "Fonts/Lingua.otf", 8);
	fpsCounter->AddComponent<FPSComponent>();

	scene.AddGameObject(std::move(fpsCounter));

	//// Rotating Sprites
	//{

	//	auto lo = std::make_unique<dae::GameObject>(75.f, 75.f);


	//	auto ro = std::make_unique<dae::GameObject>();
	//	ro->AddRenderComponent(true);
	//	ro->AddComponent<dae::ImageComponent>("enemy.png");
	//	ro->AddComponent<RotatorComponent>(20.f, 3.f);

	//	ro->SetParent(lo, true);

	//	auto so = std::make_unique<dae::GameObject>();
	//	so->AddRenderComponent(true);
	//	so->AddComponent<dae::ImageComponent>("enemy2.png");
	//	so->AddComponent<RotatorComponent>(20.f, -5.f);

	//	so->SetParent(ro, true);

	//	scene.AddGameObject(std::move(lo));
	//	scene.AddGameObject(std::move(ro));

	//	//so->SetParent(nullptr, true);
	//	auto bo = std::make_unique<dae::GameObject>();
	//	bo->AddRenderComponent(true);
	//	bo->AddComponent<dae::ImageComponent>("enemy.png");
	//	bo->AddComponent<RotatorComponent>(30.f, 8.f);

	//	bo->SetParent(so, false);
	//	scene.AddGameObject(std::move(bo));
	//	scene.AddGameObject(std::move(so));
	//}

	//Input Objects
	MakePlayer(scene);


	auto info = std::make_unique<dae::GameObject>();
	info->AddRenderComponent();
	info->AddComponent<dae::TextComponent>(inputMan.IsKeyboardActive() ? "SPACE: Jump, A/D: Move, W: Shoot " : "Y: Jump, DPAD: Move, A: Shoot", "Fonts/Pixel_NES.otf", 10);

	std::unique_ptr<dae::Command> setTextCommand = std::make_unique<SetTextCommand>(info, "Y: Jump, DPAD: Move, A: Shoot");
	inputMan.AddCommand_ChangingToController(std::move(setTextCommand));
	std::unique_ptr<dae::Command> setTextCommand2 = std::make_unique<SetTextCommand>(info, "SPACE: Jump, A/D: Move, W: Shoot ");
	inputMan.AddCommand_ChangingToKeyboard(std::move(setTextCommand2));

	scene.AddGameObject(std::move(info));

}

void Game::LoadLevelTwo() const
{

	auto& inputMan = dae::InputCommandBinder::GetInstance();
	inputMan.RemoveAllCommands();

	auto& sceneManager = dae::SceneManager::GetInstance();

	auto& scene = sceneManager.CreateScene("Level2");


	LoadLevel("Levels.txt", scene, 2);

	MakePlayer(scene);

	auto info = std::make_unique<dae::GameObject>();
	info->AddRenderComponent();
	info->AddComponent<dae::TextComponent>(inputMan.IsKeyboardActive() ? "SPACE: Jump, A/D: Move, W: Shoot " : "Y: Jump, DPAD: Move, A: Shoot", "Fonts/Pixel_NES.otf", 10);

	std::unique_ptr<dae::Command> setTextCommand = std::make_unique<SetTextCommand>(info, "Y: Jump, DPAD: Move, A: Shoot");
	inputMan.AddCommand_ChangingToController(std::move(setTextCommand));
	std::unique_ptr<dae::Command> setTextCommand2 = std::make_unique<SetTextCommand>(info, "SPACE: Jump, A/D: Move, W: Shoot ");
	inputMan.AddCommand_ChangingToKeyboard(std::move(setTextCommand2));

	scene.AddGameObject(std::move(info));
}
void Game::LoadLevelThree() const
{

	auto& inputMan = dae::InputCommandBinder::GetInstance();
	inputMan.RemoveAllCommands();

	auto& sceneManager = dae::SceneManager::GetInstance();

	auto& scene = sceneManager.CreateScene("Level3");


	LoadLevel("Levels.txt", scene, 3);

	MakePlayer(scene);
	

	auto info = std::make_unique<dae::GameObject>();
	info->AddRenderComponent();
	info->AddComponent<dae::TextComponent>(inputMan.IsKeyboardActive() ? "SPACE: Jump, A/D: Move, W: Shoot " : "Y: Jump, DPAD: Move, A: Shoot", "Fonts/Pixel_NES.otf", 10);

	std::unique_ptr<dae::Command> setTextCommand = std::make_unique<SetTextCommand>(info, "Y: Jump, DPAD: Move, A: Shoot");
	inputMan.AddCommand_ChangingToController(std::move(setTextCommand));
	std::unique_ptr<dae::Command> setTextCommand2 = std::make_unique<SetTextCommand>(info, "SPACE: Jump, A/D: Move, W: Shoot ");
	inputMan.AddCommand_ChangingToKeyboard(std::move(setTextCommand2));

	scene.AddGameObject(std::move(info));
}

void Game::MakePlayer(dae::Scene& scene) const
{

	//int starterLives = 3;


	//PLAYER 1
	/*auto livesDisplay = std::make_unique<dae::GameObject>(2.f, 50.f);
	livesDisplay->AddRenderComponent();
	livesDisplay->AddComponent<LivesUIComponent>();
	livesDisplay->AddComponent<dae::TextComponent>("Remaining lives: " + std::to_string(starterLives), "Lingua.otf", 8);

	auto scoreDisplay = std::make_unique<dae::GameObject>(0.f, 10.f);
	scoreDisplay->AddRenderComponent();
	scoreDisplay->AddComponent<ScoreUIComponent>(&Achievements::GetInstance());
	scoreDisplay->AddComponent<dae::TextComponent>("Score: 0", "Lingua.otf", 8);

	scoreDisplay->SetParent(livesDisplay, false);*/

	const float collisionOffset{ 3.f };

	auto player1 = std::make_unique<dae::GameObject>(38.f, 50.f);
	player1->AddRenderComponent();
	player1->AddPhysicsComponent();
	player1->AddComponent<MovementComponent>(-160.f, 60.f);
	dae::PhysicsComponent::SetGravity(300);
	player1->AddComponent<PlayerComponent>();
	PlayerComponent* playerComp = player1->GetComponent<PlayerComponent>();
	player1->AddComponent<SpriteComponent>("Textures/BubStates.png", 4, 8, 0.1f, true, false);
	SpriteComponent* spriteComp = player1->GetComponent<SpriteComponent>();
	spriteComp->AddObserver(playerComp);
	spriteComp->SetHeightMarkers(0, 64*2);

	const auto& destRctSize = spriteComp->GetDestRectSize();

	player1->AddComponent<dae::CollisionComponent>(
		glm::vec2{ collisionOffset ,collisionOffset }, 
		glm::vec2{ destRctSize.x - collisionOffset *2 ,destRctSize.y - collisionOffset * 2 },
		collisionTags::playerTag);

	player1->AddComponent<WallCheckingComponent>(glm::vec2{ 0,destRctSize.y/4 }, glm::vec2{ destRctSize.x,destRctSize.y/2 });
	player1->AddComponent<FloorCheckingComponent>(glm::vec2{ destRctSize.x/4,0 }, glm::vec2{ destRctSize.x/2,destRctSize.y });



	auto enemy = std::make_unique<dae::GameObject>(128, 120);
	enemy->AddRenderComponent();
	enemy->AddPhysicsComponent();
	enemy->AddComponent<ZenChanComponent>();
	//enemy->AddComponent<MovementComponent>(-160.f, 60.f);
	ZenChanComponent* enemyComp = enemy->GetComponent<ZenChanComponent>();
	enemyComp->AddPlayerObserver(playerComp);
	enemy->AddComponent<SpriteComponent>("Textures/Zen-ChanStates.png", 4, 7, 0.1f);
	SpriteComponent* enemySpriteComp = enemy->GetComponent<SpriteComponent>();

	const auto& enemyDestRctSize = enemySpriteComp->GetDestRectSize();

	enemy->AddComponent<dae::CollisionComponent>(
		glm::vec2{ collisionOffset ,collisionOffset },
		glm::vec2{ enemyDestRctSize.x - collisionOffset * 2 ,enemyDestRctSize.y - collisionOffset * 2 },
		collisionTags::enemyTag);

	enemy->AddComponent<WallCheckingComponent>(glm::vec2{ 0,enemyDestRctSize.y/4 }, glm::vec2{ enemyDestRctSize.x,enemyDestRctSize.y/2 });
	enemy->AddComponent<FloorCheckingComponent>(glm::vec2{ enemyDestRctSize.x/4,0 }, glm::vec2{ enemyDestRctSize.x/2,enemyDestRctSize.y });

	scene.AddGameObject(std::move(enemy));

	auto enemy2 = std::make_unique<dae::GameObject>(200, 120);
	enemy2->AddRenderComponent();
	enemy2->AddPhysicsComponent();
	enemy2->AddComponent<ZenChanComponent>();
	//enemy2->AddComponent<MovementComponent>(-160.f, 60.f);
	ZenChanComponent* enemy2Comp = enemy2->GetComponent<ZenChanComponent>();
	enemy2Comp->AddPlayerObserver(playerComp);
	enemy2->AddComponent<SpriteComponent>("Textures/Zen-ChanStates.png", 4, 7, 0.1f);
	SpriteComponent* enemy2SpriteComp = enemy2->GetComponent<SpriteComponent>();

	const auto& enemy2DestRctSize = enemy2SpriteComp->GetDestRectSize();

	enemy2->AddComponent<dae::CollisionComponent>(
		glm::vec2{ collisionOffset ,collisionOffset },
		glm::vec2{ enemy2DestRctSize.x - collisionOffset * 2 ,enemy2DestRctSize.y - collisionOffset * 2 },
		collisionTags::enemyTag);

	enemy2->AddComponent<WallCheckingComponent>(glm::vec2{ 0,enemy2DestRctSize.y / 4 }, glm::vec2{ enemy2DestRctSize.x,enemy2DestRctSize.y / 2 });
	enemy2->AddComponent<FloorCheckingComponent>(glm::vec2{ enemy2DestRctSize.x / 4,0 }, glm::vec2{ enemy2DestRctSize.x / 2,enemy2DestRctSize.y });

	scene.AddGameObject(std::move(enemy2));

	auto enemy3 = std::make_unique<dae::GameObject>(220, 120);
	enemy3->AddRenderComponent();
	enemy3->AddPhysicsComponent();
	enemy3->AddComponent<MaitaComponent>();
	//enemy3->AddComponent<MovementComponent>(-160.f, 60.f);
	MaitaComponent* enemy3Comp = enemy3->GetComponent<MaitaComponent>();
	enemy3Comp->AddPlayerObserver(playerComp);
	enemy3->AddComponent<SpriteComponent>("Textures/MaitaStates.png", 5, 9, 0.1f);
	SpriteComponent* enemy3SpriteComp = enemy3->GetComponent<SpriteComponent>();

	auto enemy3DestRctSize = enemy3SpriteComp->GetDestRectSize();
	enemy3DestRctSize.x = 16;

	enemy3->AddComponent<dae::CollisionComponent>(
		glm::vec2{ collisionOffset ,collisionOffset },
		glm::vec2{ enemy3DestRctSize.x - collisionOffset * 2 ,enemy3DestRctSize.y - collisionOffset * 2 },
		collisionTags::enemyTag);

	enemy3->AddComponent<WallCheckingComponent>(glm::vec2{ 0,enemy3DestRctSize.y / 4 }, glm::vec2{ enemy3DestRctSize.x,enemy3DestRctSize.y / 2 });
	enemy3->AddComponent<FloorCheckingComponent>(glm::vec2{ enemy3DestRctSize.x / 4,0 }, glm::vec2{ enemy3DestRctSize.x / 2,enemy3DestRctSize.y });

	scene.AddGameObject(std::move(enemy3));


	//uint8_t playerIndex = playerComp->GetPlayerIndex();

	//auto& inputMan = dae::InputCommandBinder::GetInstance();

	//std::shared_ptr<dae::Command> shootCommand = std::make_shared<ShootCommand>(player1);
	//inputMan.AddKeyCommand(shootCommand, SDL_SCANCODE_W, dae::KeyState::DownThisFrame);
	//inputMan.AddControllerCommand(shootCommand, dae::ControllerButton::A, dae::KeyState::DownThisFrame, playerIndex);

	//std::shared_ptr<dae::Command> moveCommand = std::make_shared<MoveCommand>(player1, playerComp->GetMoveVelocity());
	//inputMan.AddKeyCommand(moveCommand, SDL_SCANCODE_D, dae::KeyState::Pressed);
	//inputMan.AddControllerCommand(moveCommand, dae::ControllerButton::DpadRight, dae::KeyState::Pressed, playerIndex);

	//moveCommand = std::make_shared<MoveCommand>(player1, -playerComp->GetMoveVelocity());
	//inputMan.AddKeyCommand(moveCommand, SDL_SCANCODE_A, dae::KeyState::Pressed);
	//inputMan.AddControllerCommand(moveCommand, dae::ControllerButton::DpadLeft, dae::KeyState::Pressed, playerIndex);


	//std::shared_ptr<dae::Command> stopMovingCommand = std::make_shared<StopMovingCommand>(player1);
	//inputMan.AddKeyCommand(stopMovingCommand, SDL_SCANCODE_D, dae::KeyState::UpThisFrame);
	//inputMan.AddControllerCommand(stopMovingCommand, dae::ControllerButton::DpadRight, dae::KeyState::UpThisFrame, playerComp->GetPlayerIndex());

	//stopMovingCommand = std::make_shared<StopMovingCommand>(player1);
	//inputMan.AddKeyCommand(stopMovingCommand, SDL_SCANCODE_A, dae::KeyState::UpThisFrame);
	//inputMan.AddControllerCommand(stopMovingCommand, dae::ControllerButton::DpadLeft, dae::KeyState::UpThisFrame, playerComp->GetPlayerIndex());
	// 
	// 
	//player1->AddComponent<LivesComponent>(starterLives, livesDisplay->GetComponent<LivesUIComponent>());
	//player1->AddComponent<PickUpComponent>(PickUpComponent::PickUpType::Melon, scoreDisplay->GetComponent<ScoreUIComponent>());

	//std::shared_ptr<dae::Command> killCommand = std::make_shared<KillCommand>(player1);
	//inputMan.AddKeyCommand(killCommand, SDL_SCANCODE_E, dae::KeyState::UpThisFrame);
	//inputMan.AddControllerCommand(killCommand, dae::ControllerButton::X, dae::KeyState::UpThisFrame, 0);

	//std::shared_ptr<dae::Command> pickUpCommand = std::make_shared<PickUpCommand>(player1);
	//inputMan.AddKeyCommand(pickUpCommand, SDL_SCANCODE_R, dae::KeyState::UpThisFrame);
	//inputMan.AddControllerCommand(pickUpCommand, dae::ControllerButton::A, dae::KeyState::UpThisFrame, 0);

	//std::shared_ptr<dae::Command> pickUpCommand2 = std::make_shared<PickUpCommand>(player1);
	//inputMan.AddKeyCommand(pickUpCommand2, SDL_SCANCODE_T, dae::KeyState::UpThisFrame);
	//inputMan.AddControllerCommand(pickUpCommand2, dae::ControllerButton::B, dae::KeyState::UpThisFrame, 0);


	//scene.AddGameObject(std::move(livesDisplay));
	//scene.AddGameObject(std::move(scoreDisplay));



	/*std::shared_ptr<dae::Command> moveCommand = std::make_shared<MoveCommand>(player1, speedObject1);
	inputMan.AddKeyCommand(moveCommand, SDL_SCANCODE_D, dae::KeyState::Pressed);
	inputMan.AddControllerCommand(moveCommand, dae::ControllerButton::DpadRight, dae::KeyState::Pressed, 0);

	moveCommand = std::make_shared<MoveCommand>(player1, -speedObject1);
	inputMan.AddKeyCommand(moveCommand, SDL_SCANCODE_A, dae::KeyState::Pressed);
	inputMan.AddControllerCommand(moveCommand, dae::ControllerButton::DpadLeft, dae::KeyState::Pressed, 0);*/

	/*moveCommand = std::make_shared<JumpCommand>(player1, -160.f);
	inputMan.AddKeyCommand(moveCommand, SDL_SCANCODE_SPACE, dae::KeyState::Pressed);
	inputMan.AddControllerCommand(moveCommand, dae::ControllerButton::Y, dae::KeyState::Pressed, 0);*/



	scene.AddGameObject(std::move(player1));
}
