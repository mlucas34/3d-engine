#pragma once
#include <d3d11.h>
#include <wrl.h>
#include <VtxEngine/Prerequisites.h>

namespace Vtx {

	class VertexShader
	{
	public:
		VertexShader(RenderSystem* system, const wchar_t* fullPath, const char* entryPoint);
		~VertexShader();

	private:
		Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vs = nullptr; // pointer to the vertex shader created by graphics engine
		RenderSystem* m_system = nullptr;

	private:
		friend class RenderSystem;
		friend class DeviceContext;
	};
}
