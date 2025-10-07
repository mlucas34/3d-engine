#include <VtxEngine/Game/Player.h>

namespace Vtx {
	Player::Player()
	{
	}

	void Player::OnCreate()
	{
		Entity::OnCreate();
		CameraComponent *cam = CreateComponent<CameraComponent>();
		cam->SetFarPlane(2000.0f);
		GetTransform()->SetPosition(Vector3D(0, 0, -10.0f));
	}

	void Player::OnUpdate(float deltaTime)
	{
		Entity::OnUpdate(deltaTime);

		TransformComponent *transform = GetTransform();

		m_forward = 0.0f;
		m_leftward = 0.0f;

		if (GetInputSystem()->IsKeyDown(Key::W))
		{
			m_forward = 1.0f;
		}
		if (GetInputSystem()->IsKeyDown(Key::S))
		{
			m_forward = -1.0f;
		}
		if (GetInputSystem()->IsKeyDown(Key::A))
		{
			m_leftward = -1.0f;
		}
		if (GetInputSystem()->IsKeyDown(Key::D))
		{
			m_leftward = 1.0f;
		}


		Vector2D deltaPos = GetInputSystem()->GetDeltaMousePos();
		Vector3D rot = transform->GetRotation();
		rot = rot + Vector3D(deltaPos.m_y * 0.001f, deltaPos.m_x * 0.001f, 0);

		Matrix4x4 world;
		transform->GetWorldMatrix(world);

		Vector3D pos = transform->GetPosition();
		pos = pos + world.GetZDirection() * m_forward * 35.0f * deltaTime;
		pos = pos + world.GetXDirection() * m_leftward * 35.0f * deltaTime;

		transform->SetPosition(pos);
		transform->SetRotation(rot);
	}

	Player::~Player()
	{
	}
}
