#pragma once
#include <d3d11.h> 
#include <wrl.h>
#include <VtxEngine/Prerequisites.h>
#include <VtxEngine/Graphics/Texture2D.h>

namespace Vtx {

	class RenderSystem
	{
	public:
		RenderSystem();

		~RenderSystem();

	public:
		SwapChainPtr CreateSwapChain(HWND hwnd, UINT WIDTH, UINT HEIGHT);
		DeviceContextPtr GetImmediateDeviceContext();
		VertexBufferPtr CreateVertexBuffer(void* vertices, UINT vertexSize, UINT vertexListSize); // if you have errors change the implementaion of this function
		ConstantBufferPtr CreateConstantBuffer(void* buffer, UINT bufferSize);
		VertexShaderPtr CreateVertexShader(const wchar_t* fullPath, const char* entryPoint);
		PixelShaderPtr CreatePixelShader(const wchar_t* fullPath, const char* entryPoint);
		IndexBufferPtr CreateIndexBuffer(void* indicesList, UINT listSize);

		// three different kinds of textre, from file, for cubemap, and for render targets
		Texture2DPtr CreateTexture(const wchar_t* filePath);
		Texture2DPtr CreateTexture(const std::array<const wchar_t*, 6>& faces);
		Texture2DPtr CreateTexture(const Rect& size, Texture2D::Type type);

		//bool CompileVertexShader(const wchar_t* fileName, const char* entryPointName, void** shaderByteCode, SIZE_T* byteCodeLength);
		//bool CompilePixelShader(const wchar_t* fileName, const char* entryPointName, void** shaderByteCode, SIZE_T* byteCodeLength);

		//void ReleaseCompiledShader();

		void SetCullMode(const CullMode &mode);

	private:
		void CompilePrivateShaders();
		void InitDepthStencilState();
		void InitRasterizerStates();

	private:
		DeviceContextPtr m_immDeviceContext = nullptr; // used for binding resorces such as shaders to the pipline

	private:
		Microsoft::WRL::ComPtr<ID3D11Device> m_d3dDevice = nullptr; // needed for creating resources such as shaders, vertex buffers, etc. Its dx11's virtual adapter
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_immContext = nullptr;

		Microsoft::WRL::ComPtr<IDXGIDevice> m_dxgiDevice = nullptr;
		Microsoft::WRL::ComPtr<IDXGIAdapter> m_dxgiAdapter = nullptr;
		Microsoft::WRL::ComPtr<IDXGIFactory> m_dxgiFactory = nullptr;

		Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_cullFrontState = nullptr;
		Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_cullBackState = nullptr;
		Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_cullNoneState = nullptr;

		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_depthStencilState = nullptr;


		Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vs = nullptr;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> m_ps = nullptr;

		Microsoft::WRL::ComPtr<ID3DBlob> m_blob = nullptr; // where the buffer with the compiled shader and size if placed in memory 

		Microsoft::WRL::ComPtr<ID3DBlob> m_vsblob = nullptr;
		Microsoft::WRL::ComPtr<ID3DBlob> m_psblob = nullptr;

		unsigned char m_meshLayoutByteCode[1024];
		size_t m_meshLayoutSize = 0;

	private:
		friend class SwapChain;
		friend class VertexBuffer;
		friend class VertexShader;
		friend class PixelShader;
		friend class ConstantBuffer;
		friend class IndexBuffer;
		friend class Texture2D;
		friend class CubeMap;
	};

}

