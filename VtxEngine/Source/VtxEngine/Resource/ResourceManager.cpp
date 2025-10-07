#include <VtxEngine/Resource/ResourceManager.h>
#include <VtxEngine/Resource/Mesh.h>
#include <VtxEngine/Resource/Texture.h>
#include <VtxEngine/Resource/Material.h>
#include <filesystem>
#include <array>

namespace Vtx{
	ResourceManager::ResourceManager(Game *game) : m_game(game)
	{
	}

	ResourceManager::~ResourceManager()
	{
	}

	Game* ResourceManager::GetGame()
	{
		return m_game;
	}

	ResourcePtr Vtx::ResourceManager::CreateResourceFromFileConcrete(const wchar_t* filePath)
	{
		std::filesystem::path resourcePath = filePath;
		std::filesystem::path ext = resourcePath.extension();

		auto it = m_resourceMap.find(filePath);

		if (it != m_resourceMap.end())
		{
			auto mat = std::dynamic_pointer_cast<Material>(it->second);
			if (mat) return std::make_shared<Material>(mat, this);

			return it->second;
		}

		if (!std::filesystem::exists(resourcePath)) return nullptr;

		ResourcePtr resPtr = nullptr;
		
		if (!ext.compare(L".obj"))
		{
			resPtr = std::make_shared<Mesh>(resourcePath.c_str(), this);
		} 
		else if (!ext.compare(L".jpg") || !ext.compare(L".png"))
		{
			resPtr = std::make_shared<Texture>(resourcePath.c_str(), this);
		}
		else if (!ext.compare(L".hlsl") || !ext.compare(L".fx"))
		{
			resPtr = std::make_shared<Material>(resourcePath.c_str(), this);
		}

		if (resPtr)
		{
			m_resourceMap.emplace(resourcePath, resPtr);
			return resPtr;
		}

		return nullptr;
	}

	ResourcePtr Vtx::ResourceManager::CreateResourceFromFileConcrete(const std::array<const wchar_t *, 6> &faces)
	{
		const std::array<const wchar_t *, 6> resourceArray = faces;

		std::wstring key;
		for (size_t i = 0; i < faces.size(); ++i) {
			if (faces[i]) {
				key += faces[i];
			}
			key.push_back(L'|'); // separator to avoid ambiguity
		}

		auto it = m_resourceMap.find(key);

		if (it != m_resourceMap.end())
		{
			auto mat = std::dynamic_pointer_cast<Material>(it->second);
			if (mat) return std::make_shared<Material>(mat, this);

			return it->second;
		}

		ResourcePtr resPtr = nullptr;

		resPtr = std::make_shared<Texture>(resourceArray, this);

		if (resPtr)
		{
			m_resourceMap.emplace(key, resPtr);
			return resPtr;
		}

		return nullptr;
	}
}
