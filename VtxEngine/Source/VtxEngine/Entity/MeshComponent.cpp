#include <VtxEngine/Entity/MeshComponent.h>
#include <VtxEngine/Entity/Entity.h>
#include <VtxEngine/Game/World.h>
#include <VtxEngine/Game/Game.h>
#include <VtxEngine/Graphics/GraphicsEngine.h>

namespace Vtx
{
	MeshComponent::MeshComponent()
	{
	}

	MeshComponent::~MeshComponent()
	{
		m_entity->GetWorld()->GetGame()->GetGraphicsEngine()->RemoveComponent(this);
	}

	void MeshComponent::SetMesh(const MeshPtr &mesh)
	{
		m_mesh = mesh;
	}

	const MeshPtr &MeshComponent::GetMesh()
	{
		return m_mesh;
	}

	void MeshComponent::AddMaterial(const MaterialPtr &material)
	{
		m_materials.emplace_back(material);
	}

	void MeshComponent::RemoveMaterial(int idx)
	{
		if (idx >= m_materials.size()) return;

		m_materials.erase(m_materials.begin() + idx);
	}

	const std::vector<MaterialPtr> &MeshComponent::GetMaterialList()
	{
		return m_materials;
	}

	void MeshComponent::OnCreateInternal()
	{
		m_entity->GetWorld()->GetGame()->GetGraphicsEngine()->AddComponent(this);
	}
}