#include <VtxEngine/Entity/CameraComponent.h>
#include <VtxEngine/Entity/TransformComponent.h>
#include <VtxEngine/Entity/Entity.h>
#include <VtxEngine/Game/World.h>
#include <VtxEngine/Game/Game.h>
#include <VtxEngine/Graphics/GraphicsEngine.h>

namespace Vtx
{
	CameraComponent::CameraComponent()
	{
	}

	CameraComponent::~CameraComponent()
	{
		m_entity->GetWorld()->GetGame()->GetGraphicsEngine()->RemoveComponent(this);
	}

	void CameraComponent::SetFarPlane(float farPlane)
	{
		m_farPlane = farPlane;
		ComputeProjectionMatrix();
	}

	float CameraComponent::GetFarPlane()
	{
		return m_farPlane;
	}

	void CameraComponent::SetNearPlane(float nearPlane)
	{
		m_nearPlane = nearPlane;
		ComputeProjectionMatrix();
	}

	float CameraComponent::GetNearPlane()
	{
		return m_nearPlane;
	}

	void CameraComponent::SetFOV(float fov)
	{
		m_FOV = fov;
		ComputeProjectionMatrix();
	}

	float CameraComponent::GetFOV()
	{
		return m_FOV;
	}

	void CameraComponent::SetCameraType(const CameraType &type)
	{
		m_type = type;
		ComputeProjectionMatrix();
	}

	CameraType CameraComponent::GetCameraType()
	{
		return m_type;
	}

	void CameraComponent::SetScreenArea(const Rect &area)
	{
		m_screenArea = area;
		ComputeProjectionMatrix();
	}

	Rect CameraComponent::GetScreenArea()
	{
		return m_screenArea;
	}

	void CameraComponent::GetProjectionMatrix(Matrix4x4 &projection)
	{
		projection = m_projectionMatrix;
	}

	void CameraComponent::GetViewMatrix(Matrix4x4 &view)
	{
		m_entity->GetTransform()->GetWorldMatrix(view);
		view.Inverse();
	}

	void CameraComponent::ComputeProjectionMatrix()
	{
		if (m_type == CameraType::Perspective)
		{
			float aspect = (float)m_screenArea.width / (float)m_screenArea.height;
			m_projectionMatrix.SetPerspectiveFOV(m_FOV, aspect, m_nearPlane, m_farPlane);
		}
		else if (m_type == CameraType::Orthogonal)
		{
			m_projectionMatrix.SetOrtho((float)m_screenArea.width, (float)m_screenArea.height, m_nearPlane, m_farPlane);
		}
	}

	void CameraComponent::OnCreateInternal()
	{
		m_entity->GetWorld()->GetGame()->GetGraphicsEngine()->AddComponent(this);
	}
}