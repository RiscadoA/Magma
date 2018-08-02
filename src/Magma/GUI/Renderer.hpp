#pragma once

#include <Magma/Framework/Graphics/RenderDevice.hpp>

#include "Root.hpp"

namespace Magma
{
	namespace GUI
	{
		/// <summary>
		///		Used to render the graphical user interface element tree
		/// </summary>
		class Renderer final
		{
		public:
			Renderer(Framework::Graphics::RenderDevice* device);
			~Renderer();

			void Render(Root* root);
		};
	}
}
