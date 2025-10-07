#include <VtxEngine/Graphics/ConstantBuffer.h>
#include <VtxEngine/Graphics/RenderSystem.h>
#include <VtxEngine/Graphics/DeviceContext.h>
#include <exception>


namespace Vtx {
	Vtx::ConstantBuffer::ConstantBuffer(RenderSystem* system, void* buffer, UINT bufferSize) : m_system(system)
	{
		// creates a buffer description object to pass to create the vertex buffer
		D3D11_BUFFER_DESC bufferDesc = {};
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.ByteWidth = bufferSize; // prdouct of the vertex sie and list, represetns intial size of buffer
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER; // indicates this buffer will be used as a vertex buffer
		bufferDesc.MiscFlags = 0;
		bufferDesc.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA initData = {};
		initData.pSysMem = buffer;

		if (FAILED(m_system->m_d3dDevice->CreateBuffer(&bufferDesc, &initData, &m_buffer)))
		{
			throw std::runtime_error("Couldn't create Constant Buffer");
		}
		else {
			std::cout << "Constant Buffer Creation Successful\n";
		}

	}

	void Vtx::ConstantBuffer::Update(const DeviceContextPtr &context, void* buffer)
	{

		// if you are getting error conext should probably be a unique pointer of device context
		context->m_deviceContext->UpdateSubresource(this->m_buffer.Get(), NULL, NULL, buffer, NULL, NULL);
	}

	Vtx::ConstantBuffer::~ConstantBuffer()
	{
		std::cout << "Constant Buffer Released\n";
	}
}
