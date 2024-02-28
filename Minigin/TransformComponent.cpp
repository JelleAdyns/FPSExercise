#include "TransformComponent.h"

namespace dae
{
	TransformComponent::TransformComponent(GameObject* pOwner):
		Component{pOwner}, m_position{}
	{}

	void TransformComponent::Update(){}
	void dae::TransformComponent::SetPosition(const float x, const float y)
	{
		m_position.x = x;
		m_position.y = y;
	}

}
