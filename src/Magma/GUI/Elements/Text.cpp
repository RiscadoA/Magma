#include "Text.hpp"
#include "../Exception.hpp"

#include <Magma/Resources/Shader.hpp>
#include <Magma/Resources/Font.hpp>

#include <sstream>

struct TextDataCB
{
	glm::mat4 transform;
	glm::vec4 backgroundColor;
	glm::vec4 foregroundColor;
};

Magma::GUI::Elements::Text::Text(
	const std::u32string & text,
	glm::vec4 foregroundColor,
	glm::vec4 backgroundColor, 
	float scale,
	Resources::ResourceView font,
	Resources::ResourceView pixelShader) :

	Element(std::type_index(typeid(Text)), true),
	m_foregroundColor(foregroundColor),
	m_backgroundColor(backgroundColor),
	m_scale(scale),
	m_font(font),
	m_ps(pixelShader),

	m_tr(nullptr),
	m_pp(nullptr)
{
	// Check if the passed pixel shader is really a pixel shader
	if (m_ps->GetData<Resources::Shader>()->type != Framework::Graphics::ShaderType::Pixel)
	{
		std::stringstream ss;
		ss << "Failed to create Text element:" << std::endl;
		ss << "The pixel shader resource '" << m_ps->GetName() << "' is not a pixel shader";
		throw RenderingError(ss.str());
	}

	// Get constant buffer binding point
	m_cbBP = m_ps->GetData<Resources::Shader>()->pixelShader->GetBindingPoint("TEXT_DATA");
	if (m_cbBP == nullptr)
	{
		std::stringstream ss;
		ss << "Failed to create Text element:" << std::endl;
		ss << "The pixel shader resource '" << m_ps->GetName() << "' must have a constant buffer named 'BOX_DATA'";
		throw RenderingError(ss.str());
	}

	// Get font binding point
	m_ftBP = m_ps->GetData<Resources::Shader>()->pixelShader->GetBindingPoint("TEXT_FONT");
	if (m_ftBP == nullptr)
	{
		std::stringstream ss;
		ss << "Failed to create Text element:" << std::endl;
		ss << "The pixel shader resource '" << m_ps->GetName() << "' must have a texture named 'TEXT_FONT'";
		throw RenderingError(ss.str());
	}

	this->SetText(text);
	this->ResizeToFit();
}

void Magma::GUI::Elements::Text::SetText(const std::u32string & text)
{
	m_text = text;
}

void Magma::GUI::Elements::Text::ResizeToFit()
{
	// Get size of string and resize box accordingly
	size_t width = 0;
	size_t height = 0;
	for (auto& c : m_text)
	{
		auto chr = m_font->GetData<Resources::Font>()->font.Get(c);
		width += ((chr.advance.x / 64.0f) / (float)m_font->GetData<Resources::Font>()->font.GetHeight()) * m_scale;
		height += ((chr.advance.y / 64.0f) / (float)m_font->GetData<Resources::Font>()->font.GetHeight()) * m_scale;
		if (height < (chr.size.y / (float)m_font->GetData<Resources::Font>()->font.GetHeight()) * m_scale)
			height = (chr.size.y / (float)m_font->GetData<Resources::Font>()->font.GetHeight()) * m_scale;
		if (width < (chr.size.x / (float)m_font->GetData<Resources::Font>()->font.GetHeight()) * m_scale)
			width = (chr.size.x / (float)m_font->GetData<Resources::Font>()->font.GetHeight()) * m_scale;
	}

	auto box = this->GetBox();
	box.right.relative = box.left.relative;
	box.right.absolute = box.left.absolute + width;
	box.top.relative = box.bottom.relative;
	box.top.absolute = box.bottom.absolute + height;
	this->SetBox(box);
}

Magma::GUI::Elements::TextRenderer::TextRenderer(
	Framework::Graphics::RenderDevice * renderDevice,
	Resources::ResourceView vertexShader) :

	ElementRenderer(std::type_index(typeid(Text))),
	m_renderDevice(renderDevice),
	m_vs(vertexShader)
{
	// Check if the passed vertex shader is really a vertex shader
	if (m_vs->GetData<Resources::Shader>()->type != Framework::Graphics::ShaderType::Vertex)
	{
		std::stringstream ss;
		ss << "Failed to create TextRenderer:" << std::endl;
		ss << "The vertex shader resource '" << m_vs->GetName() << "' is not a vertex shader";
		throw RenderingError(ss.str());
	}

	// Create constant buffer
	m_textDataCB = m_renderDevice->CreateConstantBuffer(sizeof(TextDataCB), nullptr, Framework::Graphics::BufferUsage::Dynamic);

	// Get constant buffer binding point
	m_cbBP = m_vs->GetData<Resources::Shader>()->vertexShader->GetBindingPoint("TEXT_DATA");
	if (m_cbBP == nullptr)
	{
		std::stringstream ss;
		ss << "Failed to create TextRenderer:" << std::endl;
		ss << "The vertex shader resource '" << m_vs->GetName() << "' must have a constant buffer named 'TEXT_DATA'";
		throw RenderingError(ss.str());
	}

	// Create sampler 2D for the font
	{
		Framework::Graphics::Sampler2DDesc desc;
		
		desc.addressU = Framework::Graphics::TextureAdressMode::Repeat;
		desc.addressV = Framework::Graphics::TextureAdressMode::Repeat;
		desc.minFilter = Framework::Graphics::TextureFilter::Linear;
		desc.magFilter = Framework::Graphics::TextureFilter::Linear;
		desc.mipmapFilter = Framework::Graphics::TextureFilter::Linear;
		desc.border = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		desc.maxAnisotropy = m_renderDevice->GetMaxAnisotropyLimit();

		m_sampler = m_renderDevice->CreateSampler2D(desc);
	}
}

Magma::GUI::Elements::TextRenderer::~TextRenderer()
{
	for (auto& fonts : m_fonts)
		delete fonts.renderer;
	for (auto& pp : m_pps)
		m_renderDevice->DestroyPipeline(pp.pp);

	m_renderDevice->DestroySampler2D(m_sampler);
	m_renderDevice->DestroyConstantBuffer(m_textDataCB);
}

void Magma::GUI::Elements::TextRenderer::Render(Element * element)
{
	auto text = static_cast<Text*>(element);

	// If text still hasn't a shader pipeline, get one
	if (text->m_pp == nullptr)
		text->m_pp = GetPipeline(text);

	// If text still hasn't a text renderer, get one
	if (text->m_tr == nullptr)
		text->m_tr = GetRenderer(text);

	// Update text data constant buffer
	auto textData = (TextDataCB*)m_textDataCB->Map();
	textData->transform = text->GetTransform();
	textData->backgroundColor = text->GetBackgroundColor();
	textData->foregroundColor = text->GetForegroundColor();
	m_textDataCB->Unmap();

	// Render text
	m_cbBP->BindConstantBuffer(m_textDataCB);
	text->m_cbBP->BindConstantBuffer(m_textDataCB);
	text->m_ftBP->BindSampler2D(m_sampler);
	m_renderDevice->SetPipeline(text->m_pp);
	text->m_tr->RenderU32(text->GetText().c_str(), text->m_ftBP, text->GetScale());
}

Magma::Framework::Graphics::Pipeline * Magma::GUI::Elements::TextRenderer::GetPipeline(Text * text)
{
	for (auto& pp : m_pps)
		if (pp.ps == text->m_ps->GetData<Resources::Shader>()->pixelShader &&
			pp.vs == m_vs->GetData<Resources::Shader>()->vertexShader)
			return pp.pp;

	m_pps.emplace_back();
	m_pps.back().ps = text->m_ps->GetData<Resources::Shader>()->pixelShader;
	m_pps.back().vs = m_vs->GetData<Resources::Shader>()->vertexShader;
	m_pps.back().pp = m_renderDevice->CreatePipeline(m_pps.back().vs, m_pps.back().ps);
	return m_pps.back().pp;
}

Magma::Framework::Graphics::TextRenderer * Magma::GUI::Elements::TextRenderer::GetRenderer(Text * text)
{
	for (auto& font : m_fonts)
		if (font.font == &text->m_font->GetData<Resources::Font>()->font)
			return font.renderer;

	m_fonts.emplace_back();
	m_fonts.back().font = &text->m_font->GetData<Resources::Font>()->font;
	m_fonts.back().renderer = new Framework::Graphics::TextRenderer(m_renderDevice, m_fonts.back().font, m_vs->GetData<Resources::Shader>()->vertexShader);
	return m_fonts.back().renderer;
}
