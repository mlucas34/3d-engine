#include <VtxEngine/Entity/TransformComponent.h>

namespace Vtx
{
	TransformComponent::TransformComponent()
	{
	}

	TransformComponent::~TransformComponent()
	{
	}
	void TransformComponent::SetPosition(const Vector3D& position)
	{
		m_position = position;
		UpdateWorldMatrix();
	}

	Vector3D TransformComponent::GetPosition()
	{
		return m_position;
	}

	void TransformComponent::SetRotation(const Vector3D &rotation)
	{
		m_rotation = rotation;
		UpdateWorldMatrix();
	}

	Vector3D TransformComponent::GetRotation()
	{
		return m_rotation;
	}

	void TransformComponent::SetScale(const Vector3D &scale)
	{
		m_scale = scale;
		UpdateWorldMatrix();
	}

	Vector3D TransformComponent::GetScale()
	{
		return m_scale;
	}
	void TransformComponent::GetWorldMatrix(Matrix4x4 &mat)
	{
		mat.SetMatrix(m_worldMatrix);
	}
	void TransformComponent::UpdateWorldMatrix()
	{
		// order matters for matrix multiplcation dont change this order
		Matrix4x4 temp;
		
		// multiple scale matrix
		m_worldMatrix.SetIdentity();
		temp.SetIdentity();
		temp.SetScale(m_scale);
		m_worldMatrix *= temp;

		// rotation x
		temp.SetIdentity();
		temp.SetRotationX(m_rotation.m_x);
		m_worldMatrix *= temp;

		// rotation y
		temp.SetIdentity();
		temp.SetRotationY(m_rotation.m_y);
		m_worldMatrix *= temp;

		//------ for the reason this causes the world to rotate weird have to fix ----
		// 
		// rotation z
		/*temp.SetIdentity();
		temp.SetRotationZ(m_rotation.m_z);
		m_worldMatrix *= temp;*/

		// position
		temp.SetIdentity();
		temp.SetTranslation(m_position);
		m_worldMatrix *= temp;
	}
}
