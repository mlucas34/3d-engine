#pragma once
#include <VtxEngine/Math/Poin>

namespace Vtx {
	class InputListener
	{
	public:
		InputListener()
		{

		}

		~InputListener()
		{

		}

		virtual void OnKeyDown(int key) = 0;
		virtual void OnKeyUp(int key) = 0;
		virtual void OnMouseMove(const Point& mousePos) = 0;
		virtual void OnMouseLeftClick(const Point& mousePos) = 0;
		virtual void OnMouseRightClick(const Point& mousePos) = 0;
		virtual void OnMouseLeftUp(const Point& mousePos) = 0;
		virtual void OnMouseRightUp(const Point& mousePos) = 0;

	};
}