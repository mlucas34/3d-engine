#pragma once
#include <VtxEngine/Prerequisites.h>
#include <VtxEngine/Entity/Component.h>
#include <VtxEngine/Math/Vector3D.h>
#include <VtxEngine/Math/Matrix4x4.h>

namespace Vtx
{
	class TransformComponent : public Component
	{
	public:
		TransformComponent();
		virtual ~TransformComponent();

		void SetPosition(const Vector3D &position);
		Vector3D GetPosition();

		void SetRotation(const Vector3D &rotation);
		Vector3D GetRotation();

		void SetScale(const Vector3D &scale);
		Vector3D GetScale();

		void GetWorldMatrix(Matrix4x4 &mat);

	protected:
		void UpdateWorldMatrix();

	protected:
		Vector3D m_position;
		Vector3D m_scale = Vector3D(1.0f, 1.0f, 1.0f);
		Vector3D m_rotation = Vector3D(1.0f, 1.0f, 1.0f);

		Matrix4x4 m_worldMatrix;

	};
}