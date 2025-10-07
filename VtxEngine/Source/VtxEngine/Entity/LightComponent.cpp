#include <VtxEngine/Entity/LightComponent.h>
#include <VtxEngine/Entity/Entity.h>
#include <VtxEngine/Game/World.h>
#include <VtxEngine/Game/Game.h>
#include <VtxEngine/Graphics/GraphicsEngine.h>

namespace Vtx
{
	LightComponent::LightComponent()
	{
	}

	LightComponent::~LightComponent()
	{
		m_entity->GetWorld()->GetGame()->GetGraphicsEngine()->RemoveComponent(this);
	}

	void LightComponent::SetColor(const Vector4D &color)
	{
		m_color = color;
	}

	Vector4D LightComponent::GetColor()
	{
		return m_color;
	}

	void LightComponent::OnCreateInternal()
	{
		m_entity->GetWorld()->GetGame()->GetGraphicsEngine()->AddComponent(this);
	}
}
