#pragma once
#include <VtxEngine/Prerequisites.h>
#include <VtxEngine/Math/Rect.h>
#include <Windows.h>

namespace Vtx {

	class Window {
	public:
		Window();
		virtual ~Window();


		Rect GetClientSize();
		Rect GetScreenSize();

		// event methods
		virtual void OnCreate();
		virtual void OnUpdate();
		virtual void OnDestroy();
		virtual void OnFocus();
		virtual void OnLostFocus();
		virtual void OnSize(const Rect& size);

	protected:
		void* m_hwnd;
		Rect m_size = { 0, 0, 1024, 768 };
	};
}