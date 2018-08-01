#pragma once

#include "RenderDevice.hpp"
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
				Texture2D* texture;

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
			class Font final
			{
			public:
				/// <summary>
				///		Loads a font from memory
				/// </summary>
				/// <param name="device">Graphics render device</param>
				/// <param name="data">Data pointer</param>
				/// <param name="size">Data size</param>
				/// <param name="charWidth">Character width (set automatically if 0)</param>
				/// <param name="charHeight">Character height</param>
				/// <param name="atlasWidth">The atlas texture width</param>
				/// <param name="atlasHeight">The atlas texture height</param>
				/// <param name="maxAtlasCount">The maximum atlas texture count</param>
				Font(RenderDevice* device, const unsigned char* data, size_t size, size_t charWidth, size_t charHeight, size_t atlasWidth = 1024, size_t atlasHeight = 1024, size_t maxAtlasCount = 2);
				~Font();

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
				Texture2D* GetAtlas(size_t index) const;

				/// <summary>
				///		Gets the number of atlas used by this font
				/// </summary>
				/// <returns>Atlas count</returns>
				inline size_t GetAtlasCount() const { return m_atlas.size(); }

				/// <summary>
				///		Gets the height of a character in pixels
				/// </summary>
				inline size_t GetHeight() const { return m_height; }

			private:
				RenderDevice* m_device;
				std::vector<Character> m_chars;
				std::vector<Texture2D*> m_atlas;
				size_t m_height;
			};
		}
	}
}