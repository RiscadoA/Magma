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
				Box(glm::vec4 backgroundColor, Resources::ResourceView pixelShader);

				/// <summary>
				///		Sets this box background color.
				/// </summary>
				/// <param name="backgroundColor">New background color</param>
				inline void SetBackgroundColor(const glm::vec4& backgroundColor) { m_backgroundColor = backgroundColor; }

				/// <summary>
				///		Gets this box current background color.
				/// </summary>
				/// <returns>Current background color</returns>
				inline const glm::vec4& GetBackgroundColor() const { return m_backgroundColor; }

				/// <summary>
				///		Gets the box size in pixels.
				/// </summary>
				glm::vec2 GetSize() const;

			private:
				friend class BoxRenderer;

				glm::vec4 m_backgroundColor;

				Resources::ResourceView m_ps;
				Framework::Graphics::PixelBindingPoint* m_cbBP;
				Framework::Graphics::Pipeline* m_pp;

			};

			/// <summary>
			///		Used to render box elements.
			///		The vertex shader takes as input a float2 POSITION and the constant buffer BOX_DATA.
			///		The pixel shader takes as input the constant buffer BOX_DATA.
			///		
			///		Constant buffer BOX_DATA:
			///			float44 transform;
			///			float4	backgroundColor;
			///			float2	boxSize;
			/// </summary>
			class BoxRenderer final : public ElementRenderer
			{
			public:
				BoxRenderer(Framework::Graphics::RenderDevice* renderDevice, Resources::ResourceView vertexShader);
				virtual ~BoxRenderer() final;

				virtual void Render(Element* element) final;

			private:
				Framework::Graphics::RenderDevice* m_renderDevice;

				Framework::Graphics::VertexArray* m_va;
				Framework::Graphics::VertexBuffer* m_vb;
				Framework::Graphics::VertexLayout* m_vl;			

				Framework::Graphics::ConstantBuffer* m_boxDataCB;

				Resources::ResourceView m_vs;
				Framework::Graphics::VertexBindingPoint* m_cbBP;

				struct PipelineData
				{
					Framework::Graphics::Pipeline* pp;
					Framework::Graphics::VertexShader* vs;
					Framework::Graphics::PixelShader* ps;
				};

				std::list<PipelineData> m_pps;

				Framework::Graphics::Pipeline* GetPipeline(Box* box);
			};
		}
	}
}