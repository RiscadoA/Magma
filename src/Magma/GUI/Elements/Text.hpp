#pragma once

#include "../Element.hpp"
#include "../ElementRenderer.hpp"

#include <Magma/Framework/Graphics/1.X/Font.hpp>
#include <Magma/Framework/Graphics/1.X/TextRenderer.hpp>
#include <Magma/Framework/Graphics/1.X/RenderDevice.hpp>
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
				/// <summary>
				///		Creates a new graphical user interface text element.
				/// </summary>
				/// <param name="text">UTF-32 string to display</param>
				/// <param name="foregroundColor">Text foreground color</param>
				/// <param name="backgroundColor">Text background color</param>
				/// <param name="scale">Text scale</param>
				/// <param name="font">Text font resource</param>
				/// <param name="pixelShader">Text pixel shader resource</param>
				Text(const std::u32string& text, glm::vec4 foregroundColor, glm::vec4 backgroundColor, float scale, Resources::ResourceView font, Resources::ResourceView pixelShader);

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
				///		Sets this text current scale.
				/// </summary>
				/// <param name="size">New text scale</param>
				inline void SetScale(float scale) { m_scale = scale; }

				/// <summary>
				///		Gets this text current scale.
				/// </summary>
				/// <returns>Current text scale</returns>
				inline size_t GetScale() const { return m_scale; }

				/// <summary>
				///		Sets the string to display in this text element.
				/// </summary>
				/// <param name="text">New string to display</param>
				void SetText(const std::u32string& text);

				/// <summary>
				///		Resizes the bounding box to fit the text.
				/// </summary>
				void ResizeToFit();

				/// <summary>
				///		Gets the string that is being displayed by this text element.
				/// </summary>
				/// <returns>Text element string</returns>
				inline const std::u32string& GetText() const { return m_text; }

			private:
				friend class TextRenderer;

				std::u32string m_text;
				glm::vec4 m_backgroundColor;
				glm::vec4 m_foregroundColor;
				float m_scale;

				Resources::ResourceView m_font;

				Resources::ResourceView m_ps;
				Framework::Graphics::V1X::PixelBindingPoint* m_cbBP;
				Framework::Graphics::V1X::PixelBindingPoint* m_ftBP;
				Framework::Graphics::V1X::Pipeline* m_pp;
				Framework::Graphics::V1X::TextRenderer* m_tr;

			};

			/// <summary>
			///		Used to render text elements.
			///		The vertex shader takes as input a float2 POSITION, a float2 UVS and the constant buffer TEXT_DATA.
			///		The pixel shader takes as input the constant buffer TEXT_DATA and the grey scale texture TEXT_FONT (only has red component).
			///		
			///		Constant buffer TEXT_DATA:
			///			float44 transform;
			///			float4	backgroundColor;
			///			float4  foregroundColor;
			/// </summary>
			class TextRenderer final : public ElementRenderer
			{
			public:
				TextRenderer(Framework::Graphics::V1X::RenderDevice* renderDevice, Resources::ResourceView vertexShader);
				virtual ~TextRenderer() final;

				virtual void Render(Element* element) final;

			private:
				Framework::Graphics::V1X::RenderDevice* m_renderDevice;

				Framework::Graphics::V1X::ConstantBuffer* m_textDataCB;
				Framework::Graphics::V1X::Sampler2D* m_sampler;

				Resources::ResourceView m_vs;
				Framework::Graphics::V1X::VertexBindingPoint* m_cbBP;
				

				struct PipelineData
				{
					Framework::Graphics::V1X::Pipeline* pp;
					Framework::Graphics::V1X::VertexShader* vs;
					Framework::Graphics::V1X::PixelShader* ps;
				};

				struct FontData
				{
					Framework::Graphics::V1X::TextRenderer* renderer;
					Framework::Graphics::V1X::Font* font;
				};

				std::list<PipelineData> m_pps;
				std::list<FontData> m_fonts;

				Framework::Graphics::V1X::Pipeline* GetPipeline(Text* text);
				Framework::Graphics::V1X::TextRenderer* GetRenderer(Text* text);
			};
		}
	}
}
