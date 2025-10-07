#include <VtxEngine/Graphics/SwapChain.h>
#include <VtxEngine/Graphics/RenderSystem.h>
#include <exception>

namespace Vtx {
	SwapChain::SwapChain(RenderSystem* system, HWND hwnd, UINT WIDTH, UINT HEIGHT) : m_system(system)
	{
		ID3D11Device* device = m_system->m_d3dDevice.Get(); // get the device pointer for the create swap chain function

		DXGI_SWAP_CHAIN_DESC desc;
		ZeroMemory(&desc, sizeof(desc)); // fill all memory created by desc with zero incase of unknow initial values

		desc.BufferCount = 1; // since the monitor counts as a front buffer we only need 1 buffer, the back buffer.

		// set the width and height of the buffer to the size of the window
		desc.BufferDesc.Width = WIDTH;
		desc.BufferDesc.Height = HEIGHT;
		desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // gives 1 byte for each color channel for pixels, r, g, b, and alpha.
		desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; // allows for fullscreen

		desc.BufferDesc.RefreshRate.Numerator = 0; 
		desc.BufferDesc.RefreshRate.Denominator = 1;

		desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // tells directx how to use buffer of swap chain

		desc.OutputWindow = hwnd;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Windowed = TRUE;

		// create swap chainfor the window indicated for hwnd parameter
		HRESULT hr = m_system->m_dxgiFactory->CreateSwapChain(device, &desc, &m_swapChain);

		if (FAILED(hr)) {
			throw std::exception("Creation of Swap Chain Failed\n");
		}
		else {
			std::cout << "Swap chain creation successful\n";
		}

		ReloadBuffers(WIDTH, HEIGHT);
	}

	void SwapChain::SetFullscreen(bool fullscreen, UINT width, UINT height)
	{
		Resize(width, height); // call resize first to reload buffers
		m_swapChain->SetFullscreenState(fullscreen, nullptr);
	}

	void SwapChain::Resize(unsigned int width, unsigned int height)
	{
		m_rtv.Reset();
		m_dsv.Reset();

		m_swapChain->ResizeBuffers(1, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
		ReloadBuffers(width, height);
	}

	bool SwapChain::Present(bool vsync)
	{
		m_swapChain->Present(vsync, NULL);
		return true;
	}

	SwapChain::~SwapChain()
	{
		std::cout << "Released Swap Chain\n";

		std::cout << "Released RTV\n";

		std::cout << "Released DSV\n";
	}

	void SwapChain::ReloadBuffers(unsigned int width, unsigned int height)
	{
		ID3D11Device* device = m_system->m_d3dDevice.Get();

		ID3D11Texture2D* buffer = nullptr;
		HRESULT hr = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&buffer);

		if (FAILED(hr)) {
			throw std::exception("Back Buffer Retrieval Unsuccessful, in SwapChain.cpp\n");
		}
		else {
			std::cout << "Back Buffer Retrieval Successful\n";
		}

		hr = device->CreateRenderTargetView(buffer, nullptr, &m_rtv);
		buffer->Release();

		if (FAILED(hr)) {
			VTXError("RTV creation Unsuccessful, in SwapChain.cpp\n");
		}
		else {
			std::cout << "RTV creation Successful\n";
		}

		D3D11_TEXTURE2D_DESC texDesc = {};
		texDesc.Width = width;
		texDesc.Height = height;
		texDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		texDesc.Usage = D3D11_USAGE_DEFAULT;
		texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		texDesc.MipLevels = 1;
		texDesc.SampleDesc.Count = 1;
		texDesc.SampleDesc.Quality = 0;
		texDesc.MiscFlags = 0;
		texDesc.ArraySize = 1;
		texDesc.CPUAccessFlags = 0;

		hr = device->CreateTexture2D(&texDesc, nullptr, &buffer);

		if (FAILED(hr))
		{
			throw std::exception("Couldnt create texture 2D, error in swapChain.cpp\n");
		}

		hr = device->CreateDepthStencilView(buffer, nullptr, &m_dsv);
		buffer->Release();

		if (FAILED(hr))
		{
			throw std::exception("Couldn't create depth stencil view\n");
		}
	}
}

