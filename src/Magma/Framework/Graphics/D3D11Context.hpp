#pragma once

#include "Context.hpp"

namespace Magma
{
	namespace Framework
	{
		namespace Graphics
		{
			/// <summary>
			///		Direct3D 11 implementation of the rendering context
			/// </summary>
			class D3D11Context : public Context
			{
				// Inherited via Context
				virtual void Init(Input::Window * window) final;
				virtual void Terminate() final;
			};
		}
	}
}