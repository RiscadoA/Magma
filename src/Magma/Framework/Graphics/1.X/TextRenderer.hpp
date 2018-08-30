#pragma once

#include "Font.hpp"

#include "../../String/UTF32.hpp"

namespace Magma
{
	namespace Framework
	{
		namespace Graphics::V1X
		{
			/// <summary>
			///		TextRenderer class used to render text.
			///		The vertex shader being used must have as inputs a float2 "POSITION" and a float2 "UVS".
			/// </summary>
			class TextRenderer final
			{
			public:
				/// <summary>
				///		Creates a new text renderer for a specific font and vertex shader
				/// </summary>
				TextRenderer(RenderDevice* device, const Font* font, VertexShader* vertexShader);
				~TextRenderer();

				/// <summary>
				///		Renders a UTF-32 null terminated string
				/// </summary>
				/// <param name="str">UTF-32 null terminated string pointer</param>
				/// <param name="textureBP">Character texture binding point</param>
				void RenderU32(const String::UnicodePoint* str, PixelBindingPoint* textureBP, float scale = 24.0f);

			private:
				RenderDevice* m_device;

				const Font* m_font;
				VertexShader* m_vertexShader;

				VertexBuffer* m_vertexBuffer;
				VertexLayout* m_vertexLayout;
				VertexArray* m_vertexArray;
			};
		}
	}
}