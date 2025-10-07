#include <VtxEngine/Graphics/RenderSystem.h>
#include <VtxEngine/Graphics/GraphicsEngine.h>
#include <VtxEngine/Graphics/SwapChain.h>
#include <VtxEngine/Graphics/DeviceContext.h>
#include <VtxEngine/Graphics/VertexShader.h>
#include <VtxEngine/Graphics/PixelShader.h>
#include <VtxEngine/Graphics/IndexBuffer.h>
#include <VtxEngine/Graphics/ConstantBuffer.h>
#include <VtxEngine/Graphics/VertexBuffer.h>
#include <VtxEngine/Math/Rect.h>
#include <d3dcompiler.h>
#include <exception>


namespace Vtx {

	RenderSystem::RenderSystem() {
		D3D_DRIVER_TYPE driverTypes[] =
		{
			D3D_DRIVER_TYPE_HARDWARE,
			D3D_DRIVER_TYPE_WARP,
			D3D_DRIVER_TYPE_REFERENCE
		};
		UINT numDriverTypes = ARRAYSIZE(driverTypes);


		D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_0
		};
		UINT numFeatureLevels = ARRAYSIZE(featureLevels);

		HRESULT res = 0;

		D3D_FEATURE_LEVEL featureLevel = {};

		for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; ) {
			res = D3D11CreateDevice(NULL, driverTypes[driverTypeIndex], NULL, D3D11_CREATE_DEVICE_DEBUG, featureLevels, 
									numFeatureLevels, D3D11_SDK_VERSION, &m_d3dDevice, &featureLevel, &m_immContext);

			if (SUCCEEDED(res)) {
				break;
			}

			++driverTypeIndex;
		}

		if (FAILED(res)) {
			VTXError("Render System creation Unsuccessful\n");
		}

		m_immDeviceContext = std::make_shared<DeviceContext>(m_immContext.Get(), this);

		if (m_immDeviceContext) {
			std::cout << "Device Context Initaliztion Successful\n";
		}

		m_d3dDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&m_dxgiDevice);
		m_dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&m_dxgiAdapter);
		m_dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&m_dxgiFactory);

		InitRasterizerStates();
		InitDepthStencilState();
		CompilePrivateShaders();
	}

	RenderSystem::~RenderSystem() 
	{
		
	}

	SwapChainPtr RenderSystem::CreateSwapChain(HWND hwnd, UINT WIDTH, UINT HEIGHT)
	{
		return std::make_shared<SwapChain>(this, hwnd, WIDTH, HEIGHT);
	}

	DeviceContextPtr RenderSystem::GetImmediateDeviceContext()
	{
		return m_immDeviceContext;
	}

	VertexBufferPtr RenderSystem::CreateVertexBuffer(void* vertices, UINT vertexSize, UINT vertexListSize)
	{
		return std::make_shared<VertexBuffer>(this, vertices, vertexSize, vertexListSize);
	}

	ConstantBufferPtr RenderSystem::CreateConstantBuffer(void* buffer, UINT bufferSize)
	{
		return std::make_shared<ConstantBuffer>(this, buffer, bufferSize);
	}

	VertexShaderPtr RenderSystem::CreateVertexShader(const wchar_t* fullPath, const char* entryPoint)
	{
		return std::make_shared<VertexShader>(this, fullPath, entryPoint);
	}

	PixelShaderPtr RenderSystem::CreatePixelShader(const wchar_t* fullPath, const char* entryPoint)
	{
		return std::make_shared<PixelShader>(this, fullPath, entryPoint);
	}

	IndexBufferPtr RenderSystem::CreateIndexBuffer(void* indicesList, UINT listSize)
	{
		return std::make_shared<IndexBuffer>(this, indicesList, listSize);
	}

	Texture2DPtr RenderSystem::CreateTexture(const wchar_t* filePath)
	{
		return std::make_shared<Texture2D>(filePath, this);
	}

	Texture2DPtr RenderSystem::CreateTexture(const std::array<const wchar_t*, 6>& faces)
	{
		return std::make_shared<Texture2D>(faces, this);
	}

	Texture2DPtr RenderSystem::CreateTexture(const Rect& size, Texture2D::Type type)
	{
		return std::make_shared<Texture2D>(size, type, this);
	}

	//bool RenderSystem::CompileVertexShader(const wchar_t* fileName, const char* entryPointName, void** shaderByteCode, SIZE_T* byteCodeLength)
	//{
	//	// entry point is the name of the function of vertex shader

	//	ID3DBlob* errorBlob = nullptr;

	//	if (FAILED(::D3DCompileFromFile(fileName, nullptr, nullptr, entryPointName, "vs_5_0", 0, 0, &m_blob, &errorBlob))) {
	//		std::cout << "Failed to compile vertex shader, failed in file RenderSystem.cpp\n";

	//		if (errorBlob) errorBlob->Release(); // error most likely with eror blob

	//		return false;
	//	}
	//	else {
	//		std::cout << "Vertex Shader Compiled Successfully\n";
	//	}

	//	// get the shader byet code and length from m blob

	//	*shaderByteCode = m_blob->GetBufferPointer();
	//	*byteCodeLength = m_blob->GetBufferSize();

	//	return true;
	//}

	//bool RenderSystem::CompilePixelShader(const wchar_t* fileName, const char* entryPointName, void** shaderByteCode, SIZE_T* byteCodeLength)
	//{
	//	ID3DBlob* errorBlob = nullptr; // initalized for handling errors with compiling shaders

	//	if (FAILED(::D3DCompileFromFile(fileName, nullptr, nullptr, entryPointName, "ps_5_0", 0, 0, &m_blob, &errorBlob)))
	//	{
	//		std::cout << "Failed to compile pixel shader, failed in file RenderSystem.cpp\n";
	//		errorBlob->Release(); // error most likely with eror blob;
	//		return false;

	//	}
	//	else {
	//		std::cout << "Pixel shader compiled successfully\n";
	//	}

	//	*shaderByteCode = m_blob->GetBufferPointer();
	//	*byteCodeLength = m_blob->GetBufferSize();

	//	return true;
	//}

	//void RenderSystem::ReleaseCompiledShader()
	//{
	//	if (m_blob) {
	//		m_blob->Release();
	//	}
	//}

	void RenderSystem::SetCullMode(const CullMode &mode)
	{
		if (mode == CullMode::Front)
		{
			m_immContext->RSSetState(m_cullFrontState.Get());
		}
		else if (mode == CullMode::Back)
		{
			m_immContext->RSSetState(m_cullBackState.Get());
		}
		else if (mode == CullMode::None)
		{

		}

	}

	void RenderSystem::CompilePrivateShaders()
	{
		Microsoft::WRL::ComPtr<ID3DBlob> blob = nullptr;
		Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr;

		// raw string for vertex mesh shader so it cant be accessed by anthing else
		auto meshLayoutCode = R"(struct VS_INPUT
								{
									float4 position : POSITION0;
									float2 texcoord : TEXCOORD0;
									float3 normal   : NORMAL0;
									float3 tangent : TANGENT0;
									float3 binormal : BINORMAL0;
								};

								struct VS_OUTPUT
								{
									float4 position: SV_POSITION;
									float2 texcoord : TEXCOORD0;
								};

								VS_OUTPUT vsmain( VS_INPUT input )
								{
									VS_OUTPUT output = (VS_OUTPUT)0;

									return output;
								})";

		size_t codeSize = strlen(meshLayoutCode);

		if (FAILED(D3DCompile(meshLayoutCode, codeSize, "VertexMeshLayoutShader", nullptr, nullptr, "vsmain", "vs_5_0", 0, 0, &blob, &errorBlob))) 
		{
			VTXError("'VertexMeshLayoutShader could not compile");
		}
		
		// copy layout from blob to meshLayoutByteCode and set the meshLayoutSize
		memcpy(m_meshLayoutByteCode, blob->GetBufferPointer(), blob->GetBufferSize());
		m_meshLayoutSize = blob->GetBufferSize();
	}

	void RenderSystem::InitDepthStencilState()
	{
		D3D11_DEPTH_STENCIL_DESC dsDesc = {};
		dsDesc.DepthEnable = TRUE;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
		dsDesc.StencilEnable = FALSE;

		HRESULT hr = m_d3dDevice->CreateDepthStencilState(&dsDesc, &m_depthStencilState);

		if (FAILED(hr)) {
			VTXError("Failed to create Ddepth Stencil State\n");
		}

		m_immDeviceContext->SetDepthStencilState(m_depthStencilState.Get());
	}
	void RenderSystem::InitRasterizerStates()
	{
		D3D11_RASTERIZER_DESC desc = {};
		desc.DepthClipEnable = true;
		desc.FillMode = D3D11_FILL_SOLID;
		desc.FrontCounterClockwise = true;

		desc.CullMode = D3D11_CULL_FRONT;
		m_d3dDevice->CreateRasterizerState(&desc, &m_cullFrontState);

		desc.CullMode = D3D11_CULL_BACK;
		m_d3dDevice->CreateRasterizerState(&desc, &m_cullBackState);

		desc.CullMode = D3D11_CULL_NONE;
		m_d3dDevice->CreateRasterizerState(&desc, &m_cullNoneState);
	}
}