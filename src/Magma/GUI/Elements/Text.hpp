#pragma once

#include "../Element.hpp"
#include "../ElementRenderer.hpp"

#include <Magma/Framework/Graphics/Font.hpp>
#include <Magma/Framework/Graphics/TextRenderer.hpp>
#include <Magma/Framework/Graphics/RenderDevice.hpp>
#include <Magma/Resources/Resource.hpp>

namespace Magma
{
	namespace GUI
	{
		namespace Elements
		{
			/// <summary>
			///		Text graphical user interface element.
			/// </summary>
			class Text : public Element
			{
			public:
				Text(const std::string& text, glm::vec4 foregroundColor, glm::vec4 backgroundColor, size_t size, Resources::ResourceView font, Resources::ResourceView pixelShader);

				/// <summary>
				///		Sets this text background color.
				/// </summary>
				/// <param name="backgroundColor">New background color</param>
				inline void SetBackgroundColor(const glm::vec4& backgroundColor) { m_backgroundColor = backgroundColor; }

				/// <summary>
				///		Gets this text current background color.
				/// </summary>
				/// <returns>Current background color</returns>
				inline const glm::vec4& GetBackgroundColor() const { return m_backgroundColor; }

				/// <summary>
				///		Sets this text foreground color.
				/// </summary>
				/// <param name="backgroundColor">New foreground color</param>
				inline void SetForegroundColor(const glm::vec4& foregroundColor) { m_foregroundColor = foregroundColor; }

				/// <summary>
				///		Gets this text current foreground color.
				/// </summary>
				/// <returns>Current foreground color</returns>
				inline const glm::vec4& GetForegroundColor() const { return m_foregroundColor; }

				/// <summary>
				///		Sets this text current size.
				/// </summary>
				/// <param name="size">New text size</param>
				inline void SetSize(size_t size) { m_size = size; }

				/// <summary>
				///		Gets this text current size.
				/// </summary>
				/// <returns>Current text size</returns>
				inline size_t GetSize() const { return m_size; }

				/// <summary>
				///		Sets the string to display in this text element.
				/// </summary>
				/// <param name="text">New string to display</param>
				inline void SetText(const std::string& text) { m_text = text; }

				/// <summary>
				///		Gets the string that is being displayed by this text element.
				/// </summary>
				/// <returns>Text element string</returns>
				inline const std::string& GetText() const { return m_text; }

			private:
				friend class TextRenderer;

				std::string m_text;
				glm::vec4 m_backgroundColor;
				glm::vec4 m_foregroundColor;
				size_t m_size;

				Resources::ResourceView m_font;

				Resources::ResourceView m_ps;
				Framework::Graphics::PixelBindingPoint* m_cbBP;
				Framework::Graphics::Pipeline* m_pp;
				Framework::Graphics::TextRenderer* m_tr;

			};

			/// <summary>
			///		Used to render text elements.
			///		The vertex shader takes as input a float2 POSITION, a float2 UVS and the constant buffer TEXT_DATA.
			///		The pixel shader takes as input the constant buffer TEXT_DATA and the texture TEXT_FONT.
			///		s
			///		Constant buffer TEXT_DATA:
			///			float44 transform;
			///			float4	backgroundColor;
			///			float4  foregroundColor;
			///			float2	boxSize;
			/// </summary>
			class TextRenderer final : public ElementRenderer
			{
			public:
				TextRenderer(Framework::Graphics::RenderDevice* renderDevice, Resources::ResourceView vertexShader);
				virtual ~TextRenderer() final;

				virtual void Render(Element* element) final;

			private:
				Framework::Graphics::RenderDevice* m_renderDevice;

				Framework::Graphics::ConstantBuffer* m_textDataCB;

				Resources::ResourceView m_vs;
				Framework::Graphics::VertexBindingPoint* m_cbBP;

				struct PipelineData
				{
					Framework::Graphics::Pipeline* pp;
					Framework::Graphics::VertexShader* vs;
					Framework::Graphics::PixelShader* ps;
				};

				struct FontData
				{
					Framework::Graphics::TextRenderer* renderer;
					Framework::Graphics::Font* font;
				};

				std::list<PipelineData> m_pps;
				std::list<FontData> m_fonts;

				Framework::Graphics::Pipeline* GetPipeline(Text* text);
				Framework::Graphics::TextRenderer* GetRenderer(Text* text);
			};
		}
	}
}
