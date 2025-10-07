#pragma once
#include <d3d11.h>
#include <wrl.h>
#include <VtxEngine/Prerequisites.h>

namespace Vtx {

	class IndexBuffer
	{
	public:
		IndexBuffer(RenderSystem* system, void* indicesList, size_t listSize);
		size_t GetIndexListSize();
		~IndexBuffer();

	private:
		size_t m_listSize;

		Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer = nullptr;

		RenderSystem* m_system = nullptr;

	private:
		friend class DeviceContext;
	};
}

