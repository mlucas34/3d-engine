#include <VtxEngine/Input/InputSystem.h>
#include <Windows.h>

namespace Vtx {

	InputSystem::InputSystem()
	{
	}

	void InputSystem::Update()
	{
		POINT currentMousePos = {};
		::GetCursorPos(&currentMousePos); // gets the current position of the mouse cursor

		if (m_oldMousePos.m_x != currentMousePos.x || m_oldMousePos.m_y != currentMousePos.y) // checks if the mouse position has changed
		{
			m_deltaMousePos = Vector2D((float)currentMousePos.x - (float)m_oldMousePos.m_x, (float)currentMousePos.y - (float)m_oldMousePos.m_y);
		}
		else 
		{
			m_deltaMousePos = Vector2D(0, 0);
		}

		if (!m_cursorLocked) m_oldMousePos = Vector2D(currentMousePos.x, currentMousePos.y); // updates the old mouse position to the current mouse position
		else
		{
			SetCursorPos((int)m_lockAreaCenter.m_x, (int)m_lockAreaCenter.m_y);
			m_oldMousePos = m_lockAreaCenter;
		}

		for (unsigned int i = 0; i < 256; ++i)
		{
			m_keyboardState[i] = ::GetAsyncKeyState(i);

			if (m_keyboardState[i] & 0x8001) //  key is pressed
			{
				m_finalKeyStates[i] = 0; 
			} 
			else {
				if (m_keyboardState[i] != m_oldKeyboardState[i]) // key is not pressed
				{
					m_finalKeyStates[i] = 1;
				}
				else 
				{
					m_finalKeyStates[i] = 2; // key is in rest state

				}
			}
		::memcpy(m_oldKeyboardState, m_keyboardState, sizeof(unsigned char) * 256); // copies the current state of the keys to the old state
		}

		// check for cursor lock is here might want to change later
		if (IsKeyUp(Key::Escape))
		{
			ToggleCursorLock();
		}

	}

	bool InputSystem::IsKeyUp(const Key& key)
	{

		return m_finalKeyStates[GetKeyCode(key)] == 1;
	}

	bool InputSystem::IsKeyDown(const Key& key)
	{
		return m_finalKeyStates[GetKeyCode(key)] == 0;
	}

	Vector2D InputSystem::GetDeltaMousePos()
	{
		return m_deltaMousePos;
	}

	void InputSystem::LockCursor(bool lock)
	{
		m_cursorLocked = lock;
	}

	void InputSystem::SetLockArea(const Rect &area)
	{
		m_lockArea = area;
		m_lockAreaCenter = Vector2D(floor(area.left + (float)area.width / 2.0f), floor(area.top + (float)area.height / 2.0f));
	}

	void InputSystem::ToggleCursorLock() {
		m_cursorLocked = !m_cursorLocked;

		LockCursor(m_cursorLocked);

		if (m_cursorLocked)
		{
			ShowCursor(FALSE);
		}
		else
		{
			ShowCursor(TRUE);
		}

		
	}

	short InputSystem::GetKeyCode(const Key& key)
	{
		short winKey = 0;
		if (key >= Key::A && key <= Key::Z) winKey = 'A' + ((short)key - (short)Key::A);
		else if (key >= Key::_0 && key <= Key::_9) winKey = '0' + ((short)key - (short)Key::_0);
		else if (key == Key::Shift) winKey = VK_SHIFT;
		else if (key == Key::Escape) winKey = VK_ESCAPE;
		else if (key == Key::Tab) winKey = VK_TAB;
		else if (key == Key::Space) winKey = VK_SPACE;
		else if (key == Key::Enter) winKey = VK_RETURN;
		else if (key == Key::LeftMouseButton) winKey = VK_LBUTTON;
		else if (key == Key::RightMouseButton) winKey = VK_RBUTTON;
		else if (key == Key::MiddleMouseButton) winKey = VK_MBUTTON;



		return winKey;
	}

	InputSystem::~InputSystem()
	{
	}
}