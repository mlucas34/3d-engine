#pragma once
#include <unordered_map>
#include <string>
#include <VtxEngine/Prerequisites.h>
#include <VtxEngine/Resource/Resource.h>

namespace Vtx {
	class ResourceManager
	{
	public:
		ResourceManager(Game *game);
		~ResourceManager();

		Game* GetGame();

		template<typename T>
		std::shared_ptr<T> CreateResourceFromFile(const wchar_t* filePath)
		{
			return std::dynamic_pointer_cast<T>(CreateResourceFromFileConcrete(filePath));
		}

		template<typename T>
		std::shared_ptr<T> CreateResourceFromFile(const std::array<const wchar_t *, 6> &faces)
		{
			return std::dynamic_pointer_cast<T>(CreateResourceFromFileConcrete(faces));
		}

	private:
		ResourcePtr CreateResourceFromFileConcrete(const wchar_t* filePath);
		ResourcePtr CreateResourceFromFileConcrete(const std::array<const wchar_t *, 6> &faces);

	private:
		std::unordered_map<std::wstring, ResourcePtr> m_resourceMap;
		std::unordered_map<std::wstring,  ResourcePtr> m_cubeResources;
		Game* m_game = nullptr;
	};
}

