#pragma once

#include "Context.hpp"
#include "../String/UTF32.hpp"

#include <glm/vec2.hpp>
#include <vector>

namespace Magma
{
	namespace Framework
	{
		namespace Graphics
		{
			/// <summary>
			///		Holds info about a single font character.
			/// </summary>
			struct Character
			{
				/// <summary>
				///		Character unicode code point value.
				/// </summary>
				String::U32Char codePoint;

				/// <summary>
				///		Character texture handle.
				/// </summary>
				void* texture;

				/// <summary>
				///		Character top left texture coordinates
				/// </summary>
				glm::vec2 startUVs;

				/// <summary>
				///		Character bottom right texture coordinates
				/// </summary>
				glm::vec2 endUVs;

				/// <summary>
				///		Glyph size (in pixels).
				/// </summary>
				glm::ivec2 size;

				/// <summary>
				///		Offset brom baseline to left/top of glyph (in pixels).
				/// </summary>
				glm::ivec2 bearing;

				/// <summary>
				///		Offset to advance to next glyph (in 1/64th pixels).
				/// </summary>
				glm::ivec2 advance;
			};

			/// <summary>
			///		Holds and loads a list of characters from a true-type font.
			/// </summary>
			class Font
			{
			public:
				/// <summary>
				///		Loads a font from memory
				/// </summary>
				/// <param name="context">Graphics context</param>
				/// <param name="data">Data pointer</param>
				/// <param name="size">Data size</param>
				/// <param name="charWidth">Character width (set automatically if 0)</param>
				/// <param name="charHeight">Character height</param>
				/// <param name="atlasWidth">The atlas texture width</param>
				/// <param name="atlasHeight">The atlas texture height</param>
				Font(Context& context, const unsigned char* data, size_t size, size_t charWidth, size_t charHeight, size_t atlasWidth = 1024, size_t atlasHeight = 1024);

				/// <summary>
				///		Sets a character in a font
				/// </summary>
				/// <param name="character">Character to set</param>
				void Set(Character character);
				
				/// <summary>
				///		Gets a character in a font
				/// </summary>
				/// <param name="codePoint">Character unicode code point value</param>
				/// <returns>Character info</returns>
				Character Get(String::U32Char codePoint) const;

				/// <summary>
				///		Gets one of the atlas used by this font
				/// </summary>
				/// <param name="index">Atlas index</param>
				/// <returns>Atlas texture handle</returns>
				void* GetAtlas(size_t index) const;

				inline size_t GetHeight() const { return m_height; }

			private:
				Context& m_context;
				std::vector<Character> m_chars;
				void* m_atlas;
				size_t m_height;
			};

			/// <summary>
			///		Renders UTF-32 text.
			///		Shader takes as input:
			///		- Texture2D fontTexture (font atlas);
			///		- vec3 position (vertex position);
			///		- vec2 uvs (atlas UV coordinates);
			/// </summary>
			/// <param name="context">Context to render to</param>
			/// <param name="program">Shader program</param>
			/// <param name="font">Font to render text with</param>
			/// <param name="scale">Scaling to apply to the text</param>
			/// <param name="string">UTF-32 string to render</param>
			/// <param name="length">UTF-32 string length to render (if 0 renders until the null terminator character)</param>
			void RenderU32Text(Context& context, void* program, const Font& font, float scale, const String::U32Char* string, size_t length = 0);
		}
	}
}