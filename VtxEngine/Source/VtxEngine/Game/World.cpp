#include <VtxEngine/Game/World.h>
#include <VtxEngine/Entity/Entity.h>

namespace Vtx {
	World::World(Game *game) : m_game(game)
	{
	}

	World::~World()
	{
	}

	void World::Update(float deltaTime)
	{
		// loop through all the entities and destroy any which need to be
		for (Entity *entity : m_entitiesToDestroy)
		{
			m_entities[entity->m_id].erase(entity);
		}

		m_entitiesToDestroy.clear();

		for (auto&& [typeID, entities] : m_entities)
		{
			for (auto&& [ptr, entity] : entities)
			{
				ptr->OnUpdate(deltaTime);
			}
		}
	}

	void World::CreateEntityInternal(Entity* entity, size_t id)
	{
		std::unique_ptr<Entity> entityPtr = std::unique_ptr<Entity>(entity);
		m_entities[id].emplace(entity, std::move(entityPtr));

		entity->m_id = id;
		entity->m_world = this;
		entity->OnCreate();
	}

	void World::RemoveEntity(Entity* entity)
	{
		m_entitiesToDestroy.emplace(entity);
	}

	Game *World::GetGame()
	{
		return m_game;
	}
}