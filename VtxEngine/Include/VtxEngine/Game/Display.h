#pragma once  
#include <VtxEngine/Prerequisites.h>  
#include <VtxEngine/Math/Rect.h>
#include <VtxEngine/Window/Window.h>

namespace Vtx {  
	class Display : public Window
	{  
	public:  
		Display(Game *game);  
		~Display();  

	
	protected:
		virtual void OnSize(const Rect& size);

	private:  
		SwapChainPtr m_swapChain;  
		Game *m_game = nullptr;  

		friend class GraphicsEngine;
	};  
}