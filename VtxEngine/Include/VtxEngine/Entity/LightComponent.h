#pragma once
#include <VtxEngine/Entity/Component.h>
#include <VtxEngine/Math/Vector4D.h>

namespace Vtx
{
	class LightComponent : public Component
	{
	public:
		LightComponent();
		virtual ~LightComponent();

		void SetColor(const Vector4D &color);
		Vector4D GetColor();
	protected:
		virtual void OnCreateInternal();

	private:
		Vector4D m_color = Vector4D(1, 1, 1, 1);
	};
}

