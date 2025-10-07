#include <VtxEngine/Graphics/PixelShader.h>
#include <VtxEngine/Graphics/RenderSystem.h>
#include <d3dcompiler.h>
#include <exception>

namespace Vtx {
	PixelShader::PixelShader(RenderSystem* system, const wchar_t* fullPath, const char* entryPoint) : m_system(system)
	{
		Microsoft::WRL::ComPtr<ID3DBlob> blob = nullptr;
		Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr;

		// try to compile shader 
		D3DCompileFromFile(fullPath, nullptr, nullptr, entryPoint, "ps_5_0", 0, 0, &blob, &errorBlob);

		// use the blobs to check if the compile was successful
		if (errorBlob) VTXWarning("Pixel shader " << fullPath << " compiled with errors: \n" << (char*)errorBlob->GetBufferPointer());
		if (!blob) VTXError("Pixel shader " << fullPath << " couldn't compile\n");

		// try to create shader
		if (FAILED(m_system->m_d3dDevice->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &m_ps)))
		{
			VTXError("Pixel shader " << fullPath << " could not be created");
		}
	}

	PixelShader::~PixelShader()
	{
		std::cout << "Pixel shader released\n";
	}
}