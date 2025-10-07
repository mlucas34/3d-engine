#pragma once
#include <VtxEngine/Prerequisites.h>
#include <map>
#include <set>

namespace Vtx {
	class World
	{
	public:
		World(Game *game);
		~World();

		template <typename T>
		T* CreateEntity()
		{
			static_assert(std::is_base_of<Entity, T>::value, "Templated class must derive from Entity");
			size_t id = typeid(T).hash_code();
			auto e = new T();
			CreateEntityInternal(e, id);
			return e;
		}

		void Update(float deltaTime);

		Game *GetGame();

	private:
		void CreateEntityInternal(Entity* entity, size_t id);	
		void RemoveEntity(Entity* entity);

	private:
		std::map<size_t, std::map<Entity*, std::unique_ptr<Entity>>> m_entities;
		std::set<Entity*> m_entitiesToDestroy;

		Game *m_game = nullptr;

		friend class Entity;
	};
}

