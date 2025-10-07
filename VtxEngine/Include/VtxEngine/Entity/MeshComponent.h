#pragma once
#include <VtxEngine/Prerequisites.h>
#include <VtxEngine/Entity/Component.h>
#include <vector>

namespace Vtx{
	class MeshComponent : public Component
	{
	public:
		MeshComponent();
		virtual ~MeshComponent();

		void SetMesh(const MeshPtr &mesh);
		const MeshPtr &GetMesh();

		void AddMaterial(const MaterialPtr &material);
		void RemoveMaterial(int idx);

		const std::vector<MaterialPtr> &GetMaterialList();

	protected:
		virtual void OnCreateInternal(); // override on create in component class for own implementation

	private:
		MeshPtr m_mesh = nullptr;
		std::vector<MaterialPtr> m_materials;
	};
}

