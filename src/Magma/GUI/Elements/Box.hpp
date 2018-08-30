#pragma once

#include "../Element.hpp"
#include "../ElementRenderer.hpp"

#include <Magma/Framework/Graphics/1.X/RenderDevice.hpp>
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
				Framework::Graphics::V1X::PixelBindingPoint* m_cbBP;
				Framework::Graphics::V1X::Pipeline* m_pp;

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
				BoxRenderer(Framework::Graphics::V1X::RenderDevice* renderDevice, Resources::ResourceView vertexShader);
				virtual ~BoxRenderer() final;

				virtual void Render(Element* element) final;

			private:
				Framework::Graphics::V1X::RenderDevice* m_renderDevice;

				Framework::Graphics::V1X::VertexArray* m_va;
				Framework::Graphics::V1X::VertexBuffer* m_vb;
				Framework::Graphics::V1X::VertexLayout* m_vl;			

				Framework::Graphics::V1X::ConstantBuffer* m_boxDataCB;

				Resources::ResourceView m_vs;
				Framework::Graphics::V1X::VertexBindingPoint* m_cbBP;

				struct PipelineData
				{
					Framework::Graphics::V1X::Pipeline* pp;
					Framework::Graphics::V1X::VertexShader* vs;
					Framework::Graphics::V1X::PixelShader* ps;
				};

				std::list<PipelineData> m_pps;

				Framework::Graphics::V1X::Pipeline* GetPipeline(Box* box);
			};
		}
	}
}
