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
			/// <summary>
			///		Constructs a new graphical user interface Renderer
			/// </summary>
			/// <param name="device">Render device pointer</param>
			Renderer(Framework::Graphics::RenderDevice* device);
			~Renderer();

			/// <summary>
			///		Renders the graphical user interface element tree
			/// </summary>
			/// <param name="element">Tree root pointer</param>
			void Render(Element* element);

		private:
			Framework::Graphics::RenderDevice* m_device;

			Framework::Graphics::ConstantBuffer* m_elementCB;

			Framework::Graphics::VertexArray* m_boxVA;
			Framework::Graphics::VertexBuffer* m_boxVB;
			Framework::Graphics::VertexLayout* m_boxVL;
		};
	}
}
