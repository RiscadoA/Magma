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

			void OnMouseEnter();

			void OnMouseLeave();

			void OnMouseMove(float x, float y);

			void OnMouseScroll(float x);

			void OnMouseDown(Framework::Input::Mouse button);

			void OnMouseUp(Framework::Input::Mouse button);

		private:
			Framework::Input::Window* m_window;
			Root* m_root;

			size_t m_evtOnMouseEnter;
			size_t m_evtOnMouseLeave;
			size_t m_evtOnMouseMove;
			size_t m_evtOnMouseScroll;
			size_t m_evtOnMouseDown;
			size_t m_evtOnMouseUp;
		};
	}
}
