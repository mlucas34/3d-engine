#include <VtxEngine/Graphics/IndexBuffer.h>

#include <VtxEngine/Graphics/RenderSystem.h>
#include <exception>

namespace Vtx {
	IndexBuffer::IndexBuffer(RenderSystem* system, void* indicesList, size_t listSize) : m_buffer(0), m_system(system)
	{
		// creates a buffer description object to pass to create the index buffer
		D3D11_BUFFER_DESC bufferDesc = {};
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.ByteWidth = 4 * listSize; // represents intial size of buffer
		bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER; // indicates this buffer will be used as an index buffer
		bufferDesc.MiscFlags = 0;
		bufferDesc.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA initData = {};
		initData.pSysMem = indicesList;

		// initalizes the member variables 
		m_listSize = listSize;

		if (FAILED(m_system->m_d3dDevice->CreateBuffer(&bufferDesc, &initData, &m_buffer)))
		{
			throw std::exception("Couldn't Create Index Buffer, error in IndexBuffer cpp\n");
		}
		else {
			std::cout << "Index Buffer creaation Successful\n";
		}
	}


	size_t IndexBuffer::GetIndexListSize()
	{
		return this->m_listSize;
	}


	IndexBuffer::~IndexBuffer()
	{
		std::cout << "Index Buffer released\n";
	}
}