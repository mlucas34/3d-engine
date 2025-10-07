#pragma once
#include <VtxEngine/Math/Vector2D.h>
#include <VtxEngine/Math/Rect.h>
#include <VtxEngine/Prerequisites.h>

namespace Vtx {
	class InputSystem
	{
	public:
		InputSystem();
		~InputSystem();

	public:
		void Update();

		bool IsKeyUp(const Key& key);
		bool IsKeyDown(const Key& key);
		Vector2D GetDeltaMousePos();

		void LockCursor(bool lock);
		void SetLockArea(const Rect& area);

		void ToggleCursorLock();


	private:
		short GetKeyCode(const Key& key);

	private:
		short m_keyboardState[256] = {}; // stores the state of the keys
		short m_oldKeyboardState[256] = {}; // stores the previous state of the keys
		short m_finalKeyStates[256] = {}; // stores all the key states until next call to update
	
		bool m_cursorLocked = false;
		Rect m_lockArea;
		Vector2D m_lockAreaCenter;

		Vector2D m_oldMousePos;
		Vector2D m_deltaMousePos;

	};
}

