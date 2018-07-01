#include "Font.hpp"

#include <sstream>
#include <algorithm>

#include <ft2build.h>
#include FT_FREETYPE_H

Magma::Framework::Graphics::Font::Font(Context& context, const unsigned char* data, size_t size, size_t charWidth, size_t charHeight, size_t atlasWidth, size_t atlasHeight)
	: m_context(context), m_height(charHeight)
{
	FT_Library ft;
	if (auto err = FT_Init_FreeType(&ft))
	{
		std::stringstream ss;
		ss << "Failed to create font:" << std::endl;
		ss << "FT_Init_FreeType returned error " << err;
		throw std::runtime_error(ss.str());
	}

	FT_Face face;
	if (auto err = FT_New_Memory_Face(ft, data, size, 0, &face))
	{
		std::stringstream ss;
		ss << "Failed to create font:" << std::endl;
		ss << "FT_New_Memory_Face returned error " << err;
		throw std::runtime_error(ss.str());
	}

	FT_Set_Pixel_Sizes(face, charWidth, charHeight);

	m_chars.clear();

	m_atlas = m_context.CreateTexture2D(nullptr, atlasWidth, atlasHeight, TextureFormat::R8UInt);
	{
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
				throw std::runtime_error(ss.str());
			}

			// Check if fits
			if (x + face->glyph->bitmap.width > atlasWidth)
			{
				x = 0;
				y += maxH;
				maxH = 0;

				if (y + face->glyph->bitmap.rows > atlasHeight)
				{
					std::stringstream ss;
					ss << "Failed to create font:" << std::endl;
					ss << "Character '" << character << "' doesn't fit on the atlas";
					throw std::runtime_error(ss.str());
				}
			}

			// Update max height
			maxH = std::max(maxH, face->glyph->bitmap.rows);

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
			chr.texture = m_atlas;
			m_chars.push_back(chr);

			// Add character to atlas
			m_context.UpdateTexture2D(m_atlas, face->glyph->bitmap.buffer, face->glyph->bitmap.width, face->glyph->bitmap.rows, x, y, TextureFormat::R8UInt);
			x += face->glyph->bitmap.width + 1;

			// Try to get next character
			character = FT_Get_Next_Char(face, character, &index);

			// Got to teh end
			if (index == 0)
				break;
		}
	}
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
	throw std::runtime_error(ss.str());
}

void * Magma::Framework::Graphics::Font::GetAtlas(size_t index) const
{
	return m_atlas; /* MULTIPLE ATLAS NOT YET IMPLEMENTED */
}

void Magma::Framework::Graphics::RenderU32Text(Context& context, void* program, const Font & font, float scale, const String::U32Char * string, size_t length)
{
	struct Vertex
	{
		float x, y;
		float u, v;
	};

	static void* vertexBuffer = nullptr;
	if (vertexBuffer == nullptr)
	{
		Vertex data[] =
		{
			{ 0.0f, 0.0f,		0.0f, 0.0f },
			{ 1.0f, 0.0f,		1.0f, 0.0f },
			{ 1.0f, 1.0f,		1.0f, 1.0f },

			{ 0.0f, 0.0f,		0.0f, 0.0f },
			{ 0.0f, 1.0f,		0.0f, 1.0f },
			{ 1.0f, 1.0f,		1.0f, 1.0f },
		};

		Graphics::VertexLayout layout;
		
		layout.elements.emplace_back();
		layout.elements.back().format = Framework::Graphics::VertexElementFormat::Float2;
		layout.elements.back().name = "position";
		layout.elements.back().offset = offsetof(Vertex, x);

		layout.elements.emplace_back();
		layout.elements.back().format = Framework::Graphics::VertexElementFormat::Float2;
		layout.elements.back().name = "uvs";
		layout.elements.back().offset = offsetof(Vertex, u);

		layout.size = sizeof(Vertex);

		vertexBuffer = context.CreateVertexBuffer(data, sizeof(data), layout, program, Usage::Dynamic);
	}

	if (length == 0)
	{
		float x = 0.0f;
		float y = 0.0f;

		auto it = string;
		while (*it != 0)
		{
			if (*it == '\n')
			{
				x = 0.0f;
				y -= font.GetHeight() * scale;
				++it;
				continue;
			}

			auto chr = font.Get(*it);
			
			float xpos = x + chr.bearing.x * scale;
			float ypos = -y - chr.bearing.y * scale;

			float w = chr.size.x * scale;
			float h = chr.size.y * scale;

			Vertex data[] =
			{
				{ xpos,		-ypos,		chr.startUVs.x,	chr.startUVs.y, },
				{ xpos + w,	-ypos,		chr.endUVs.x,	chr.startUVs.y },
				{ xpos + w, -ypos - h,	chr.endUVs.x,	chr.endUVs.y },

				{ xpos,		-ypos,		chr.startUVs.x,	chr.startUVs.y },
				{ xpos,		-ypos - h,	chr.startUVs.x,	chr.endUVs.y },
				{ xpos + w, -ypos - h,	chr.endUVs.x,	chr.endUVs.y },
			};

			x += (chr.advance.x / 64.0f) * scale;
			y += (chr.advance.y / 64.0f) * scale;

			context.UpdateVertexBuffer(vertexBuffer, data, sizeof(data), 0);
			context.BindVertexBuffer(vertexBuffer);
			context.Draw(6, 0, DrawMode::Triangles);
			++it;
		}
	}
	else
	{
		for (size_t i = 0; i < length; ++i)
		{
			// TO DO
		}
	}
}
