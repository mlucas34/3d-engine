#pragma once
#include <VtxEngine/HighLevelClasses.h>

namespace Vtx {
	class Player : public Entity
	{
	public:
		Player();
		virtual ~Player();

	protected:
		virtual void OnCreate();
		virtual void OnUpdate(float deltaTime);

	private:
		Entity* m_entity = nullptr;
		float m_elapsedTime = 0.0f;

		float m_forward = 0.0f;
		float m_leftward = 0.0f;
	};
}

