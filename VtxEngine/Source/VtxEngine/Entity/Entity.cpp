#include <VtxEngine/Entity/Entity.h>
#include <VtxEngine/Entity/Component.h>
#include <VtxEngine/Entity/TransformComponent.h>
#include <VtxEngine/Game/World.h>
#include <VtxEngine/Game/Game.h>

namespace Vtx {
	
	Entity::Entity()
	{
		m_transform = CreateComponent<TransformComponent>();
	}

	Entity::~Entity()
	{
	}

	World* Entity::GetWorld()
	{
		return m_world;
	}

	TransformComponent *Entity::GetTransform()
	{
		return m_transform;
	}

	InputSystem *Entity::GetInputSystem()
	{
		return m_world->GetGame()->GetInputSystem();
	}

	void Entity::CreateComponentInternal(Component* component, size_t id)
	{
		std::unique_ptr<Component> componentPtr = std::unique_ptr<Component>(component);
		m_components.emplace(id, std::move(componentPtr));

		component->m_id = id;
		component->m_entity = this;

		component->OnCreateInternal(); // see header for information
	}

	Component* Entity::GetComponentInternal(size_t id)
	{
		auto it = m_components.find(id);

		if (it != m_components.end()) return it->second.get();

		return nullptr;
	}

	void Entity::RemoveComponent(size_t id)
	{
		m_components.erase(id);
	}

	void Entity::ReleaseEntity()
	{
		m_world->RemoveEntity(this);
		VTXInfo("relased entity");
	}
}