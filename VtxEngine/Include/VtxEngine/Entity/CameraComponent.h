#pragma once
#include <VtxEngine/Prerequisites.h>
#include <VtxEngine/Entity/Component.h>
#include <VtxEngine/Math/Rect.h>
#include <VtxEngine/Math/Matrix4x4.h>

namespace Vtx
{
	class CameraComponent : public Component
	{
	public:
		CameraComponent();
		virtual ~CameraComponent();

		void SetFarPlane(float farPlane);
		float GetFarPlane();

		void SetNearPlane(float nearPlane);
		float GetNearPlane();

		void SetFOV(float fov);
		float GetFOV();

		void SetCameraType(const CameraType &type);
		CameraType GetCameraType();

		void SetScreenArea(const Rect &area);
		Rect GetScreenArea();

		void GetProjectionMatrix(Matrix4x4 &projection);
		void GetViewMatrix(Matrix4x4 &view);

	private:
		void ComputeProjectionMatrix();
	protected:
		void OnCreateInternal(); // for adding or removing camera components inside graphics engine

	private:
		float m_nearPlane = 0.01f;
		float m_farPlane = 100.0f;
		float m_FOV = 1.3f;
		CameraType m_type = CameraType::Perspective;
		Rect m_screenArea;

		Matrix4x4 m_projectionMatrix;
	};
}


