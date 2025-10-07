#include <VtxEngine/Graphics/VertexBuffer.h>
#include <VtxEngine/Graphics/RenderSystem.h>

namespace Vtx {
	VertexBuffer::VertexBuffer(RenderSystem* system, void* vertices, size_t vertexSize, size_t vertexListSize):m_layout(0), m_buffer(0), m_system(system)
	{
		// if  the buffer and layout are already used release them to load a new one for something else

		// creates a buffer description object to pass to create the vertex buffer
		D3D11_BUFFER_DESC bufferDesc = {};
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.ByteWidth = vertexSize * vertexListSize; // prdouct of the vertex sie and list, represetns intial size of buffer
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER; // indicates this buffer will be used as a vertex buffer
		bufferDesc.MiscFlags = 0;
		bufferDesc.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA initData = {};
		initData.pSysMem = vertices;

		// initalizes the member variables 
		m_vertexSize = vertexSize;
		m_vertexListSize = vertexListSize;

		if (FAILED(m_system->m_d3dDevice->CreateBuffer(&bufferDesc, &initData, &m_buffer)))
		{
			VTXError("Couldn't Create Vertex Buffer\n");
		}

		D3D11_INPUT_ELEMENT_DESC layout[] = // provides information on the the data is laid out in the vertex buffer
		{
			// following format for layout:
			// NAME    INDEX     FORMAT    INPUT SLOT	ALLGINED BYTE OFFSET    INPUT SLOT CCLASS    INSTANCE DATA STEP RATE
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{"TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{"BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 44, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		UINT layoutSize = ARRAYSIZE(layout);


		if (FAILED(m_system->m_d3dDevice->CreateInputLayout(layout, layoutSize, system->m_meshLayoutByteCode, system->m_meshLayoutSize, &m_layout)))
		{
			VTXError("Input Layout Creation Unsuccessful\n");
		}
		else
		{
			std::cout << "Input Layout Creation Successful\n";
		}
	}

	size_t VertexBuffer::GetVertexListSize()
	{
		return this->m_vertexListSize;
	}


	VertexBuffer::~VertexBuffer()
	{
	}
}