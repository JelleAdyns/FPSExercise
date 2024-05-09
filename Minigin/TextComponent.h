#ifndef TEXTCOMPONENT_H
#define TEXTCOMPONENT_H

#include <string>
#include "Component.h"
#include "Font.h"
#include "Texture2D.h"

namespace dae
{
	class RenderComponent;

	class TextComponent final : public Component
	{
	public:
		explicit TextComponent(GameObject* pOwner, const std::string& text, const std::string& fontFile, unsigned int size);
		explicit TextComponent(GameObject* pOwner, const std::string& text, std::unique_ptr<Font>&& pFont, std::unique_ptr<Texture2D>&& pTexture);
		explicit TextComponent(GameObject* pOwner, const std::string& text, std::unique_ptr<Font>&& pFont);
		virtual ~TextComponent() {}

		TextComponent(const TextComponent&) = delete;
		TextComponent(TextComponent&&) noexcept = delete;
		TextComponent& operator= (const TextComponent&) = delete;
		TextComponent& operator= (TextComponent&&) noexcept = delete;

		virtual void Update() override;
		virtual void PrepareImGuiRender() override;


		void SetText(const std::string& text);
		Texture2D* GetTexture() const;

	private:
		bool m_needsUpdate;
		std::string m_text;

		std::unique_ptr<Font> m_pFont;
		std::unique_ptr<Texture2D> m_pTextTexture;

		RenderComponent* m_pRenderComponent;
	};
}
#endif // !TEXTCOMPONENT_H