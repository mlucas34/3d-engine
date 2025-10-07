#include <VtxEngine/Game/Game.h>
#include <VtxEngine/Window/Window.h>
#include <VtxEngine/Graphics/GraphicsEngine.h>
#include <Windows.h>

namespace Vtx {
	void Game::Run() {
		MSG msg;

		OnCreate();

		while (isRunning) {
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) 
			{

				if (msg.message == WM_QUIT) 
				{
					isRunning = false;
					continue;
				}
				else
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}

			OnInternalUpdate();
		}

		OnQuit();
	}
}