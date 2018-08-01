#include "Font.hpp"

#include <sstream>
#include <cmath>

#include <ft2build.h>
#include FT_FREETYPE_H

struct Vertex
{
	float x, y;
	float u, v;
};

Magma::Framework::Graphics::Font::Font(RenderDevice* device, const unsigned char* data, size_t size, size_t charWidth, size_t charHeight, size_t atlasWidth, size_t atlasHeight, size_t maxAtlasCount)
	: m_device(device), m_height(charHeight)
{
	if (maxAtlasCount < 1)
	{
		std::stringstream ss;
		ss << "Failed to create font:" << std::endl;
		ss << "The maximum atlas count must be at least 1";
		throw TextError(ss.str());
	}

	FT_Library ft;
	if (auto err = FT_Init_FreeType(&ft))
	{
		std::stringstream ss;
		ss << "Failed to create font:" << std::endl;
		ss << "FT_Init_FreeType returned error " << err;
		throw TextError(ss.str());
	}

	FT_Face face;
	if (auto err = FT_New_Memory_Face(ft, data, size, 0, &face))
	{
		std::stringstream ss;
		ss << "Failed to create font:" << std::endl;
		ss << "FT_New_Memory_Face returned error " << err;
		throw TextError(ss.str());
	}

	FT_Set_Pixel_Sizes(face, charWidth, charHeight);
	m_chars.clear();

	// Create atlas
	{
		m_atlas.push_back(m_device->CreateTexture2D(atlasWidth, atlasHeight, TextureFormat::R8UNorm, nullptr, BufferUsage::Dynamic));
		size_t x = 0;
		size_t y = 0;
		size_t maxH = 0;

		FT_UInt index;
		auto character = FT_Get_First_Char(face, &index);
		while (true)
		{
			// Load character
			if (auto err = FT_Load_Char(face, character, FT_LOAD_RENDER))
			{
				std::stringstream ss;
				ss << "Failed to create font:" << std::endl;
				ss << "FT_Load_Char returned error " << err << " on character " << character;
				throw TextError(ss.str());
			}

			// Check if the character fits on the current row
			if (x + face->glyph->bitmap.width > atlasWidth)
			{
				x = 0;
				y += maxH;
				maxH = 0;
			}

			// Check if the character fits on the current atlas
			if (y + face->glyph->bitmap.rows > atlasHeight)
			{
				if (m_atlas.size() + 1 > maxAtlasCount)
				{
					std::stringstream ss;
					ss << "Failed to create font:" << std::endl;
					ss << "Character U+" << std::hex << character << " doesn't fit on the atlas:" << std::endl;
					ss << "Maximum atlas count of " << std::dec << maxAtlasCount << " already achieved, try raising the maximum atlas count or increasing each atlas size" << std::endl;
					throw TextError(ss.str());
				}
				else
				{
					m_atlas.push_back(m_device->CreateTexture2D(atlasWidth, atlasHeight, TextureFormat::R8UNorm, nullptr, BufferUsage::Dynamic));
					x = 0;
					y = 0;
					maxH = 0;
				}
			}

			// Update max height
			if (face->glyph->bitmap.rows > maxH)
				maxH = face->glyph->bitmap.rows;
			
			// Store character info
			Character chr;
			chr.codePoint = character;
			chr.advance.x = face->glyph->advance.x;
			chr.advance.y = face->glyph->advance.y;
			chr.size.x = face->glyph->bitmap.width;
			chr.size.y = face->glyph->bitmap.rows;
			chr.bearing.x = face->glyph->bitmap_left;
			chr.bearing.y = face->glyph->bitmap_top;
			chr.startUVs = glm::vec2(x, y) / glm::vec2(atlasWidth, atlasHeight);
			chr.endUVs = glm::vec2(x + chr.size.x, y + chr.size.y) / glm::vec2(atlasWidth, atlasHeight);
			chr.texture = m_atlas.back();
			m_chars.push_back(chr);

			// Add character to atlas
			m_atlas.back()->Update(x, y, face->glyph->bitmap.width, face->glyph->bitmap.rows, face->glyph->bitmap.buffer);
			x += face->glyph->bitmap.width + 1;

			// Try to get next character
			character = FT_Get_Next_Char(face, character, &index);

			// Quit if end
			if (index == 0)
				break;
		}

		for (auto& t : m_atlas)
			t->GenerateMipmaps();
	}
}

Magma::Framework::Graphics::Font::~Font()
{
	for (auto& t : m_atlas)
		m_device->DestroyTexture2D(t);
	m_atlas.clear();
}

void Magma::Framework::Graphics::Font::Set(Magma::Framework::Graphics::Character character)
{
	for (auto& c : m_chars)
		if (c.codePoint == character.codePoint)
		{
			c = character;
			return;
		}
	m_chars.push_back(character);
}

Magma::Framework::Graphics::Character Magma::Framework::Graphics::Font::Get(Magma::Framework::String::U32Char codePoint) const
{
	for (const auto& c : m_chars)
		if (c.codePoint == codePoint)
			return c;
	std::stringstream ss;
	ss << "Failed to get font character:" << std::endl;
	ss << "Font has no character U+" << std::hex << (unsigned int)codePoint;
	throw TextError(ss.str());
}

Magma::Framework::Graphics::Texture2D * Magma::Framework::Graphics::Font::GetAtlas(size_t index) const
{
	return m_atlas.at(index);
}
