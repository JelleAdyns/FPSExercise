#pragma once
#include <string>
#include <memory>
#include "Singleton.h"
#include "Font.h"
#include "Texture2D.h"

namespace dae
{
	/*class Texture2D;
	class Font;*/
	class ResourceManager final : public Singleton<ResourceManager>
	{
	public:
		void Init(const std::string& data);
		std::unique_ptr<Texture2D> LoadTexture(const std::string& file) const;
		std::unique_ptr<Texture2D> LoadTextureFromFont(const std::string& text, const std::unique_ptr<Font>& font) const;
		std::unique_ptr<Font> LoadFont(const std::string& file, unsigned int size) const;
	private:
		friend class Singleton<ResourceManager>;
		ResourceManager() = default;
		std::string m_dataPath;
	};
}
