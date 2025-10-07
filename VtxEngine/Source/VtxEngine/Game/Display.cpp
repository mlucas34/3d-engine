#include <VtxEngine/Game/Display.h>
#include <VtxEngine/Game/Game.h>
#include <VtxEngine/Graphics/GraphicsEngine.h>
#include <VtxEngine/Graphics/RenderSystem.h>
#include <VtxEngine/Graphics/SwapChain.h>

namespace Vtx {
	Display::Display(Game *game) : m_game(game)
	{
		Rect size = GetClientSize();
		m_swapChain = game->GetGraphicsEngine()->GetRenderSystem()->CreateSwapChain(static_cast<HWND>(m_hwnd), size.width, size.height);
	}

	void Display::OnSize(const Rect& size)
	{
		m_swapChain->Resize(size.width, size.height);
		m_game->OnDisplaySize(size);
	}

	Display::~Display()
	{

	}
}
