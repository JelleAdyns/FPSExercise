#include "Spawners.h"
#include "Components.h"

#include <SceneManager.h>
#include <GameObject.h>
#include <CollisionComponent.h>
#include "CollisionTags.h"
#include "BubbleComponent.h"
#include "SpriteComponent.h"

#include "WallCheckingComponent.h"
#include "FloorCheckingComponent.h"
#include "ProjectileComponent.h"
#include "LevelState.h"

namespace spawners
{

	void SpawnEnemy(const glm::vec2& spawnPos)
	{
		dae::Scene* pScene = dae::SceneManager::GetInstance().GetActiveScene();

		auto enemy = std::make_unique<dae::GameObject>(spawnPos);
		enemy->AddRenderComponent();
		enemy->AddPhysicsComponent();
		enemy->AddComponent<ZenChanComponent>();
		//enemy->AddComponent<MovementComponent>(-160.f, 60.f);
		ZenChanComponent* enemyComp = enemy->GetComponent<ZenChanComponent>();
		enemyComp->AddPlayerObserver(LevelState::GetPlayerOne()->GetComponent<PlayerComponent>());
		enemy->AddComponent<SpriteComponent>("Textures/Zen-ChanStates.png", 4, 7, 0.1f);
		SpriteComponent* enemySpriteComp = enemy->GetComponent<SpriteComponent>();

		const auto& enemyDestRctSize = enemySpriteComp->GetDestRectSize();
		float collisionOffset{ LevelState::GetCollisionOffset() };

		enemy->AddComponent<dae::CollisionComponent>(
			glm::vec2{ collisionOffset ,collisionOffset },
			glm::vec2{ enemyDestRctSize.x - collisionOffset * 2 ,enemyDestRctSize.y - collisionOffset * 2 },
			collisionTags::enemyTag);

		enemy->AddComponent<WallCheckingComponent>(glm::vec2{ 0,enemyDestRctSize.y / 4 }, glm::vec2{ enemyDestRctSize.x,enemyDestRctSize.y / 2 });
		enemy->AddComponent<FloorCheckingComponent>(glm::vec2{ enemyDestRctSize.x / 4,0 }, glm::vec2{ enemyDestRctSize.x / 2,enemyDestRctSize.y });

		pScene->AddGameObject(std::move(enemy));
	}

	void SpawnBubble(const glm::vec2& spawnPos, bool left)
	{
		dae::Scene* pScene = dae::SceneManager::GetInstance().GetActiveScene();

		auto pBubble = std::make_unique<dae::GameObject>(spawnPos);
		pBubble->AddRenderComponent();
		pBubble->AddPhysicsComponent();
		pBubble->AddComponent<BubbleComponent>(left);
		pBubble->AddComponent<SpriteComponent>("Textures/Bubble.png", 3, 11, 0.1f, true, true);
		SpriteComponent* pSpriteComp = pBubble->GetComponent<SpriteComponent>();
		auto destRectSize = pSpriteComp->GetDestRectSize();
		pBubble->AddComponent<dae::CollisionComponent>(glm::vec2{}, destRectSize, collisionTags::bubbleTag);
		pBubble->AddComponent<WallCheckingComponent>(glm::vec2{ 0, destRectSize.y / 4 }, glm::vec2{ destRectSize.x, destRectSize.y / 2 });

		pBubble->Start();

		pScene->AddGameObject(std::move(pBubble));
	}

	void SpawnPickUp(const glm::vec2& spawnPos, PickUpComponent::PickUpType pickUpType)
	{
		dae::Scene* pScene = dae::SceneManager::GetInstance().GetActiveScene();

		auto pPickUp = std::make_unique<dae::GameObject>(spawnPos);
		pPickUp->AddRenderComponent();
		pPickUp->AddPhysicsComponent();
		pPickUp->AddComponent<SpriteComponent>("Textures/PickUps.png", 2, 3, 0.1f, false, false);
		SpriteComponent* pSpriteComp = pPickUp->GetComponent<SpriteComponent>();
		auto destRectSize = pSpriteComp->GetDestRectSize();
		pPickUp->AddComponent<dae::CollisionComponent>(glm::vec2{}, destRectSize, collisionTags::pickUp);
		pPickUp->AddComponent<FloorCheckingComponent>(glm::vec2{ destRectSize.x / 4, 0.f }, glm::vec2{ destRectSize.x / 2, destRectSize.y });
		pPickUp->AddComponent<PickUpComponent>(pickUpType, nullptr);

		pPickUp->Start();

		pScene->AddGameObject(std::move(pPickUp));
	}

	void SpawnProjectile(const glm::vec2& spawnPos, bool left)
	{
		dae::Scene* pScene = dae::SceneManager::GetInstance().GetActiveScene();

		auto pBoulder = std::make_unique<dae::GameObject>(spawnPos);
		pBoulder->AddRenderComponent();
		pBoulder->AddPhysicsComponent();
		pBoulder->AddComponent<SpriteComponent>("Textures/Boulder.png", 4, 2, 0.1f, true, false);
		SpriteComponent* pSpriteComp = pBoulder->GetComponent<SpriteComponent>();
		auto destRectSize = pSpriteComp->GetDestRectSize();
		pBoulder->AddComponent<dae::CollisionComponent>(glm::vec2{}, destRectSize, collisionTags::projectileTag);
		pBoulder->AddComponent<WallCheckingComponent>(glm::vec2{ 0.f, destRectSize.y / 4 }, glm::vec2{ destRectSize.x , destRectSize.y / 2 });
		pBoulder->AddComponent<ProjectileComponent>(left);

		pBoulder->Start();

		pScene->AddGameObject(std::move(pBoulder));
	}
}