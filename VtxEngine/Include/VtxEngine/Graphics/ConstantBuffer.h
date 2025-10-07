#pragma once
#include <d3d11.h>
#include <wrl.h>
#include <memory>
#include <VtxEngine/Prerequisites.h>

namespace Vtx {

	class ConstantBuffer
	{
	public:
		ConstantBuffer(RenderSystem* system, void* buffer, UINT bufferSize);

		void Update(const DeviceContextPtr &context, void* buffer);

		~ConstantBuffer();

	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer = nullptr;

		RenderSystem* m_system = nullptr;

	private:
		friend class DeviceContext;
	};
}

