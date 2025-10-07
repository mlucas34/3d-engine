#pragma once
#include <d3d11.h>
#include <VtxEngine/Graphics/RenderSystem.h>

namespace Vtx {

	class SwapChain
	{
	public:
		SwapChain(RenderSystem* system, HWND hwnd, UINT WIDTH, UINT HEIGHT);

		void SetFullscreen(bool fullscreen, UINT width, UINT height);
		void Resize(unsigned int width, unsigned int height);
		bool Present(bool vsync);

		~SwapChain();

	private:
		void ReloadBuffers(unsigned int width, unsigned int height);

	private:
		Microsoft::WRL::ComPtr<IDXGISwapChain> m_swapChain = nullptr;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_rtv = nullptr; // creates pointer for the render target view
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_dsv = nullptr;
		RenderSystem* m_system = nullptr;

	private:
		friend class DeviceContext;
		friend class GraphicsEngine;
	};
}

