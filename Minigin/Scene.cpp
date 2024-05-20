#include "Scene.h"

#include <algorithm>

using namespace dae;

unsigned int Scene::m_IdCounter = 0;

Scene::Scene() { ++m_IdCounter; }

Scene::~Scene() { --m_IdCounter; }

void Scene::AddGameObject(std::unique_ptr<GameObject>&& object)
{
	m_pObjects.emplace_back(std::move(object));
}

void Scene::Remove(std::unique_ptr<GameObject> object)
{
	m_pObjects.erase(std::remove(m_pObjects.begin(), m_pObjects.end(), object), m_pObjects.end());
}

void Scene::RemoveAll()
{
	m_pObjects.clear();
}


void dae::Scene::Start()
{
	for (auto& object : m_pObjects)
	{
		object->Start();
	}
}

void Scene::Update()
{
	for(auto& object : m_pObjects)
	{
		object->Update();
	}

	m_pObjects.erase(
		std::remove_if(m_pObjects.begin(), m_pObjects.end(),
			[&](const std::unique_ptr<GameObject>& pObject) {return pObject->IsDead(); }
		), 
		m_pObjects.cend()
	);
}

void dae::Scene::PrepareImGuiRender()
{
	for (auto& object : m_pObjects)
	{
		object->PrepareImGuiRender();
	}
}

void Scene::Render() const
{
	for (const auto& object : m_pObjects)
	{
		object->Render();
	}
}

void dae::Scene::FixedUpdate()
{
	for (const auto& object : m_pObjects)
	{
		object->FixedUpdate();
	}

	m_pObjects.erase(
		std::remove_if(m_pObjects.begin(), m_pObjects.end(),
			[&](const std::unique_ptr<GameObject>& pObject) {return pObject->IsDead(); }
		),
		m_pObjects.cend()
	);
}

bool dae::Scene::IsDestroyed() const
{
	return m_IsDestroyed;
}

void dae::Scene::SetDestroyed()
{
	m_IsDestroyed = true;
}

