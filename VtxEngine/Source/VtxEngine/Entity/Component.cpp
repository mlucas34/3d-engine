#include <VtxEngine/Entity/Component.h>
#include <VtxEngine/Entity/Entity.h>

namespace Vtx {
	Component::Component()
	{
	}

	Component::~Component()
	{
	}

	void Component::ReleaseComponent()
	{
		m_entity->RemoveComponent(m_id);
	}

	Entity *Component::GetEntity()
	{
		return m_entity;
	}

	void Component::OnCreateInternal()
	{
	}
}
