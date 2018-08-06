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
			///		Constructs a new graphical user interface Input manager
			/// </summary>
			Input(Framework::Input::Window* window);
			~Input();

		private:
			Framework::Input::Window* m_window;
		};
	}
}
