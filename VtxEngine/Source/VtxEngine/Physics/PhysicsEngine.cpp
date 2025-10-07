#include <VtxEngine/Physics/PhysicsEngine.h>
#include <VtxEngine/Entity/TerrainComponent.h>
#include <VtxEngine/Game/Game.h>

/*

PHYSIC ENGINES NEEDS TO BE REWORKED

*/

namespace Vtx
{
	PhysicsEngine::PhysicsEngine(Game *game) : m_game(game)
	{
	}

	PhysicsEngine::~PhysicsEngine()
	{
	}

	void PhysicsEngine::Update()
	{

	}

	void PhysicsEngine::AddComponent(Component *component)
	{
		if (TerrainComponent *c = dynamic_cast<TerrainComponent *>(component))
		{
			m_components.emplace(c);
		}
	}

	void PhysicsEngine::RemoveComponent(Component *component)
	{
		m_components.erase(component);
	}
}