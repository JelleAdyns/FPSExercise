#include "MaitaFallingState.h"
#include "MaitaRunState.h"
#include "SpriteComponent.h"
#include "FloorCheckingComponent.h"
#include "MaitaComponent.h"
#include <PhysicsComponent.h>
#include <CollisionComponent.h>
#include <GameObject.h>
#include <Minigin.h>

MaitaFallingState::MaitaFallingState(dae::GameObject* pEnemy, MaitaComponent* pEnemyComp, bool isAngry) :
	MaitaState{},
	m_IsAngry{ isAngry },
	m_pEnemy{ pEnemy },
	m_pEnemyComp{ pEnemyComp },
	m_pPhysicsComp{ pEnemy->GetComponent<dae::PhysicsComponent>() },
	m_pCollisionComp{ pEnemy->GetComponent<dae::CollisionComponent>() },
	m_pFloorCheckingComp{ pEnemy->GetComponent<FloorCheckingComponent>() }
{}


std::unique_ptr<MaitaState> MaitaFallingState::Update()
{
	//dae::GameObject* pCollidedObject = m_pCollisionComp->CheckForCollision(collisionTags::bubbleTag);
	//if (pCollidedObject)
	//{
	//	if (!pCollidedObject->GetComponent<BubbleComponent>()->IsOccupied())
	//	{
	//		return std::make_unique<MaitaCaughtState>(m_pEnemy, pCollidedObject);
	//	}
	//}

	if (m_pEnemy->GetWorldPosition().y > dae::Minigin::GetWindowSize().y)
	{
		m_pEnemy->SetLocalPos(m_pEnemy->GetLocalPosition().x, -50);
	}
	if (m_pFloorCheckingComp->IsOnGround())
	{
		return std::make_unique<MaitaRunState>(m_pEnemy, m_pEnemyComp, m_IsAngry);
	}

	return nullptr;
}
void MaitaFallingState::OnEnter()
{
	for (dae::Subject<PlayerComponent>* pSubject : m_pEnemyComp->GetPlayerSubjects())
	{
		pSubject->AddObserver(this);
	}
	m_pPhysicsComp->SetVelocityX(0);
}
void MaitaFallingState::OnExit()
{

	SpriteComponent* pSpriteComp = m_pEnemy->GetComponent<SpriteComponent>();

	if (m_PlayerXPos < m_pEnemy->GetWorldPosition().x) pSpriteComp->LookLeft(true);
	else pSpriteComp->LookLeft(false);

}

void MaitaFallingState::Notify(PlayerComponent* pSubject)
{
	auto subjectPos = pSubject->GetPos();
	m_PlayerXPos = subjectPos.x;
}

void MaitaFallingState::AddSubjectPointer(dae::Subject<PlayerComponent>* pSubject)
{
	m_pVecObservedSubjects.push_back(pSubject);
}

void MaitaFallingState::SetSubjectPointersInvalid()
{
	for (auto& pSubject : m_pVecObservedSubjects)
	{
		pSubject = nullptr;
	}
}