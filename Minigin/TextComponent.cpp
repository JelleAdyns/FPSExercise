#include "TextComponent.h"
//#include <stdexcept>
#include <SDL_ttf.h>
#include "RenderComponent.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "Font.h"
//#include "Texture2D.h"

namespace dae
{
	TextComponent::TextComponent(const std::shared_ptr<GameObject>& pOwner, const std::string& text, const std::shared_ptr<Font>& font)
		: Component{pOwner}, m_needsUpdate(true), m_text(text), m_pFont(font), m_pTextTexture(nullptr)
	{
		
		using ThisType = std::remove_reference<decltype(*this)>::type;
		m_pOwner.lock()->GetComponent<RenderComponent>()->AddTexture<ThisType>(m_pTextTexture);
	}
	//void TextComponent::FixedUpdate(float fixedTimeStep){}

	void TextComponent::Update()
	{
		if (m_needsUpdate)
		{
			m_pTextTexture = ResourceManager::GetInstance().LoadTextureFromFont(m_text, m_pFont);

			using ThisType = std::remove_reference<decltype(*this)>::type;
			m_pOwner.lock()->GetComponent<RenderComponent>()->AddTexture<ThisType>(m_pTextTexture);

			m_needsUpdate = false;
		}
	}


	// This implementation uses the "dirty flag" pattern
	void TextComponent::SetText(const std::string& text)
	{
		m_text = text;
		m_needsUpdate = true;
	}
	std::shared_ptr<Texture2D> TextComponent::GetTexture() const
	{
		return m_pTextTexture;
	}

}
	
