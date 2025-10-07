#pragma once
#include <VtxEngine/Prerequisites.h>

namespace Vtx {
	class Component
	{
	public:
		Component();
		virtual	~Component();

		void ReleaseComponent();

		Entity *GetEntity();

	protected:
		virtual void OnCreateInternal(); // called on creation of component to add component to graphics engine

		size_t m_id = 0;
		Entity *m_entity = nullptr;

		friend class Entity;
	};
}

