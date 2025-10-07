#pragma once
#include <VtxEngine/Prerequisites.h>
#include <set>

namespace Vtx
{
	class PhysicsEngine
	{
	public:
		PhysicsEngine(Game *game);
		virtual ~PhysicsEngine();

		void Update();
	public:
		void AddComponent(Component *component);
		void RemoveComponent(Component *component);

	private:
		Game *m_game = nullptr;
		std::set<Component *> m_components;
	};
}

