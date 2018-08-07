#pragma once

#include <Magma/Framework/Input/Window.hpp>

#include "Root.hpp"

namespace Magma
{
	namespace GUI
	{
		class Input final
		{
		public:
			/// <summary>
			///		Constructs a new graphical user interface Input manager.
			/// </summary>
			/// <param name="window">Window from where the events will be polled</param>
			/// <param name="root">GUI element tree root</param>
			Input(Framework::Input::Window* window, Root* root);
			~Input();

			/// <summary>
			///		Called when the mouse leaves the window.
			/// </summary>
			void OnMouseLeave();

			/// <summary>
			///		Called when the mouse moves.
			/// </summary>
			/// <param name="x">Mouse X coordinate</param>
			/// <param name="y">Mouse Y coordinate</param>
			void OnMouseMove(float x, float y);

			/// <summary>
			///		Called when the mouse wheel is scrolled.
			/// </summary>
			/// <param name="x">Mouse wheel delta</param>
			void OnMouseScroll(float x);

			/// <summary>
			///		Called when a mouse button is down.
			/// </summary>
			/// <param name="button">Mouse button code</param>
			void OnMouseDown(Framework::Input::Mouse button);

			/// <summary>
			///		Called when a mouse button is up.
			/// </summary>
			/// <param name="button">Mouse button code</param>
			void OnMouseUp(Framework::Input::Mouse button);

		private:
			void OnMouseLeaveRecursive(Element* element);
			void OnMouseMoveRecursive(Element* element, bool inside);
			bool OnMouseScrollRecursive(Element* element, float x);
			bool OnMouseDownRecursive(Element* element, Framework::Input::Mouse button);
			bool OnMouseUpRecursive(Element* element, Framework::Input::Mouse button);

			Framework::Input::Window* m_window;
			Root* m_root;

			size_t m_evtOnMouseLeave;
			size_t m_evtOnMouseMove;
			size_t m_evtOnMouseScroll;
			size_t m_evtOnMouseDown;
			size_t m_evtOnMouseUp;

			float m_mX;
			float m_mY;
		};
	}
}
