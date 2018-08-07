#include "Text.hpp"
#include "../Exception.hpp"

#include <Magma/Resources/Shader.hpp>
#include <Magma/Resources/Font.hpp>

#include <sstream>

Magma::GUI::Elements::Text::Text(
	const std::string & text,
	glm::vec4 foregroundColor,
	glm::vec4 backgroundColor, 
	size_t size,
	Resources::ResourceView font,
	Resources::ResourceView pixelShader) :

	Element(std::type_index(typeid(Text)), true),
	m_text(text),
	m_foregroundColor(foregroundColor),
	m_backgroundColor(backgroundColor),
	m_size(size),
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
}

Magma::GUI::Elements::TextRenderer::TextRenderer(
	Framework::Graphics::RenderDevice * renderDevice,
	Resources::ResourceView vertexShader) :

	ElementRenderer(std::type_index(typeid(Text))),
	m_renderDevice(renderDevice),
	m_vs(vertexShader)
{

}

Magma::GUI::Elements::TextRenderer::~TextRenderer()
{

}

void Magma::GUI::Elements::TextRenderer::Render(Element * element)
{

}

Magma::Framework::Graphics::Pipeline * Magma::GUI::Elements::TextRenderer::GetPipeline(Text * text)
{
	return nullptr;
}

Magma::Framework::Graphics::TextRenderer * Magma::GUI::Elements::TextRenderer::GetRenderer(Text * text)
{
	return nullptr;
}
