#pragma once

#include "../Element.hpp"
#include "../ElementRenderer.hpp"

#include <Magma/Framework/Graphics/RenderDevice.hpp>
#include <Magma/Resources/Resource.hpp>

namespace Magma
{
	namespace GUI
	{
		namespace Elements
		{
			/// <summary>
			///		Box graphical user interface element.
			/// </summary>
			class Box : public Element
			{
			public:
				Box(glm::vec4 backgroundColor);

				/// <summary>
				///		Sets this box background color
				/// </summary>
				/// <param name="backgroundColor">New background color</param>
				inline void SetBackgroundColor(const glm::vec4& backgroundColor) { m_backgroundColor = backgroundColor; }

				/// <summary>
				///		Gets this box current background color
				/// </summary>
				/// <returns>Current background color</returns>
				inline const glm::vec4& GetBackgroundColor() const { return m_backgroundColor; }

			private:
				glm::vec4 m_backgroundColor;
			};

			/// <summary>
			///		Used to render box elements.
			/// </summary>
			class BoxRenderer final : public ElementRenderer
			{
			public:
				BoxRenderer(Framework::Graphics::RenderDevice* renderDevice, Resources::ResourceView vertexShader, Resources::ResourceView pixelShader);
				virtual ~BoxRenderer() final;

				virtual void Render(Element* element) final;

			private:
				Framework::Graphics::RenderDevice* m_renderDevice;

				Framework::Graphics::VertexArray* m_va;
				Framework::Graphics::VertexBuffer* m_vb;
				Framework::Graphics::VertexLayout* m_vl;

				Resources::ResourceView m_vs;
				Resources::ResourceView m_ps;
			};
		}
	}
}