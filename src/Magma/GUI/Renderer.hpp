#pragma once

#include <Magma/Framework/Graphics/RenderDevice.hpp>

namespace Magma
{
	namespace GUI
	{
		/// <summary>
		///		Used to render the graphical user interface
		/// </summary>
		class Renderer final
		{
		public:
			Renderer(Framework::Graphics::RenderDevice* device);
			~Renderer();
		};
	}
}
