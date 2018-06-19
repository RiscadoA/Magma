#pragma once

#include "Context.hpp"

namespace Magma
{
	namespace Framework
	{
		namespace Graphics
		{
			/// <summary>
			///		OpenGL implementation of the rendering context
			/// </summary>
			class GLContext final : public Context
			{
				// Inherited via Context
				virtual void Init(Input::Window * window) final;
				virtual void Terminate() final;
			};
		}
	}
}