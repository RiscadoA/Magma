#pragma once

#include "../Input/Window.hpp"

namespace Magma
{
	namespace Framework
	{
		namespace Graphics
		{
			/// <summary>
			///		Abstract class for low level rendering calls
			/// </summary>
			class Context
			{
			public:
				Context() = default;
				virtual ~Context() = default;

				/// <summary>
				///		Inits the rendering context on a window
				/// </summary>
				/// <param name="window">Window</param>
				virtual void Init(Input::Window* window) = 0;

				/// <summary>
				///		Terminates the rendering context
				/// </summary>
				virtual void Terminate() = 0;
			};
		}
	}
}