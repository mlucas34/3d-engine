#include <VtxEngine/Window/Window.h>
#include <Windows.h>

namespace Vtx {
	LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
		switch (msg) {
		case WM_CREATE:
		{
			break;
		}

		case WM_SETFOCUS:
		{
			// event filled when window gets focus
			Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
			if (window) window->OnFocus();
			break;
		}
		case WM_SIZE:
		{
			Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
			if (window) window->OnSize(window->GetClientSize());
			break;
		}
		case WM_KILLFOCUS:
		{
			// event filled when window lost focus
			Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
			window->OnLostFocus();
			break;
		}

		case WM_DESTROY:
		{
			// event filled when window destroyed
			Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
			window->OnDestroy();
			::PostQuitMessage(0);
			break;
		}
		case WM_CLOSE:
		{
			PostQuitMessage(0);
			break;
		}


		default:
			return ::DefWindowProc(hwnd, msg, wparam, lparam);
		}

		return NULL;
	}

	Window::Window() {
		WNDCLASSEX wc = {};

		wc.cbSize = sizeof(WNDCLASSEX);
		wc.lpszClassName = L"MyWindowClass";
		wc.lpfnWndProc = &WndProc;


		auto classID = RegisterClassEx(&wc);

		if (!classID) VTXError("Window Creaation Unsuccessful\n");

		RECT rc = { 0, 0, m_size.width, m_size.height };
		AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, false);

		m_hwnd = ::CreateWindowEx(NULL, MAKEINTATOM(classID), L"Vtx Application", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, NULL, NULL);

		if (!m_hwnd) VTXError("Window Creaation Unsuccessful\n");

		HWND hwnd = static_cast<HWND>(m_hwnd);

		SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)this);

		::ShowWindow(hwnd, SW_SHOW);
		::UpdateWindow(hwnd);
	}

	//bool Window::Broadcast() {

	//	MSG msg;

	//	if (!this->m_isInit) {
	//		// Event fired when the window is created
	//		SetWindowLongPtr(m_hwnd, GWLP_USERDATA, (LONG_PTR)this);
	//		this->OnCreate();

	//		this->m_isInit = true;
	//	}

	//	this->OnUpdate();

	//	while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0) {
	//		TranslateMessage(&msg);
	//		DispatchMessage(&msg);
	//	}

	//	Sleep(1);
	//	return true;
	//}

	//bool Window::IsRunning() 
	//{
	//	if (m_isRunning) {
	//		Broadcast();
	//	}

	//	return m_isRunning;
	//}
	
	Rect Window::GetClientSize()
	{
		RECT rc = {};
		HWND hwnd = static_cast<HWND>(m_hwnd);
		::GetClientRect(hwnd, &rc);
		::ClientToScreen(hwnd, (LPPOINT)&rc.left);
		::ClientToScreen(hwnd, (LPPOINT)&rc.right);

		return { rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top };
	}

	Rect Window::GetScreenSize()
	{
		RECT rc = {};
		rc.right = ::GetSystemMetrics(SM_CXSCREEN);
		rc.bottom = ::GetSystemMetrics(SM_CYSCREEN);

		return { 0, 0, rc.right - rc.left, rc.bottom - rc.top };
	}

	void Window::OnDestroy()
	{
	}

	void Window::OnFocus()
	{
	}

	void Window::OnLostFocus()
	{
	}

	void Window::OnSize(const Rect& size)
	{
	}

	void Window::OnUpdate()
	{

	}

	void Window::OnCreate()
	{

	}

	Window::~Window() {
		DestroyWindow(static_cast<HWND>(m_hwnd));
	}
}


