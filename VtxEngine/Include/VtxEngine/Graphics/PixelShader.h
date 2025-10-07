#pragma once
#include <d3d11.h>
#include <wrl.h>
#include <VtxEngine/Prerequisites.h>

namespace Vtx {

	class PixelShader
	{
	public:
		PixelShader(RenderSystem* system, const wchar_t* fullPath, const char* entryPoint);
		~PixelShader();

	private:
		Microsoft::WRL::ComPtr<ID3D11PixelShader> m_ps = nullptr;

		RenderSystem* m_system = nullptr;

	private:
		friend class RenderSystem;
		friend class DeviceContext;
	};
}

