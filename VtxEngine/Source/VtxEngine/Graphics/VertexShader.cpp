#include <VtxEngine/Graphics/VertexShader.h>
#include <VtxEngine/Graphics/RenderSystem.h>
#include <d3dcompiler.h>
#include <exception>

namespace Vtx {
	VertexShader::VertexShader(RenderSystem* system, const wchar_t* fullPath, const char* entryPoint) : m_system(system)
	{
		Microsoft::WRL::ComPtr<ID3DBlob> blob = nullptr;
		Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr;

		// try to compile shader 
		D3DCompileFromFile(fullPath, nullptr, nullptr, entryPoint, "vs_5_0", 0, 0, &blob, &errorBlob);


		// use the blobs to check if the compile was successful
		if (errorBlob) VTXWarning("Vertex shader " << fullPath << " compiled with errors: \n" << (char*)errorBlob->GetBufferPointer());
		if (!blob) 
		{

			VTXError("Vertex shader " << fullPath << " couldn't compile\n");
		}
		// try to create shader
		if (FAILED(m_system->m_d3dDevice->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &m_vs)))
		{
			VTXError("Vertex shader " << fullPath << " could not be created");
		}
	}

	VertexShader::~VertexShader()
	{
		std::cout << "Released Vertex Shader\n";
	}
}