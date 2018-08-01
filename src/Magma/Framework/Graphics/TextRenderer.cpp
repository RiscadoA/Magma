#include "TextRenderer.hpp"

struct Vertex
{
	float x, y;
	float u, v;
};

Magma::Framework::Graphics::TextRenderer::TextRenderer(RenderDevice* device, const Font * font, VertexShader * vertexShader)
{
	m_device = device;

	m_font = font;
	m_vertexShader = vertexShader;

	m_vertexBuffer = m_device->CreateVertexBuffer(sizeof(Vertex) * 6, nullptr, BufferUsage::Dynamic);

	VertexElement elements[2];

	elements[0].bufferIndex = 0;
	elements[0].name = "POSITION";
	elements[0].offset = offsetof(Vertex, x);
	elements[0].stride = sizeof(Vertex);
	elements[0].size = 2;
	elements[0].type = VertexElementType::Float;

	elements[1].bufferIndex = 0;
	elements[1].name = "UVS";
	elements[1].offset = offsetof(Vertex, u);
	elements[1].stride = sizeof(Vertex);
	elements[1].size = 2;
	elements[1].type = VertexElementType::Float;

	m_vertexLayout = m_device->CreateVertexLayout(2, elements, m_vertexShader);

	m_vertexArray = m_device->CreateVertexArray(1, &m_vertexBuffer, m_vertexLayout);
}

Magma::Framework::Graphics::TextRenderer::~TextRenderer()
{
	m_device->DestroyVertexArray(m_vertexArray);
	m_device->DestroyVertexBuffer(m_vertexBuffer);
	m_device->DestroyVertexLayout(m_vertexLayout);
}

void Magma::Framework::Graphics::TextRenderer::RenderU32(const String::U32Char * str, PixelBindingPoint * textureBP, float scale)
{
	float x = 0.0f;
	float y = 0.0f;

	auto it = str;
	while (*it != U'\0')
	{
		if (*it == 0)
			break;
		else if (*it == U'\n')
		{
			x = 0.0f;
			y -= m_font->GetHeight() * scale;
		}
		else if (*it == U'\r')
		{
			x = 0.0f;
		}
		else 
		{
			auto chr = m_font->Get(*it);

			float xpos = x + chr.bearing.x * scale;
			float ypos = -y - chr.bearing.y * scale;

			float w = chr.size.x * scale;
			float h = chr.size.y * scale;

			auto data = (Vertex*)m_vertexBuffer->Map();
			data[0] = { xpos,		-ypos,		chr.startUVs.x,	chr.startUVs.y, };
			data[1] = { xpos + w,	-ypos,		chr.endUVs.x,	chr.startUVs.y };
			data[2] = { xpos + w, -ypos - h,	chr.endUVs.x,	chr.endUVs.y };
			data[3] = { xpos,		-ypos,		chr.startUVs.x,	chr.startUVs.y };
			data[4] = { xpos,		-ypos - h,	chr.startUVs.x,	chr.endUVs.y };
			data[5] = { xpos + w, -ypos - h,	chr.endUVs.x,	chr.endUVs.y };
			m_vertexBuffer->Unmap();

			x += (chr.advance.x / 64.0f) * scale;
			y += (chr.advance.y / 64.0f) * scale;

			textureBP->BindTexture2D(chr.texture);
			m_device->SetVertexArray(m_vertexArray);
			m_device->DrawTriangles(0, 6);
		}

		++it;
	}
}
