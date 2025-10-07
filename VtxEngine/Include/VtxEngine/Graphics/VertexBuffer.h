#pragma once
#include <d3d11.h>
#include <wrl.h>
#include <VtxEngine/Prerequisites.h>

namespace Vtx {

	class VertexBuffer
	{
	public:
		VertexBuffer(RenderSystem* system, void* vertices, size_t vertexSize, size_t vertexListSize);
		size_t GetVertexListSize();
		~VertexBuffer();

	private:
		size_t m_vertexSize;
		size_t m_vertexListSize;

		Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer = nullptr;
		Microsoft::WRL::ComPtr<ID3D11InputLayout> m_layout = nullptr;

		RenderSystem* m_system = nullptr;

	private:
		friend class DeviceContext;
	};
}


