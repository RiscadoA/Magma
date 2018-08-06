#include "Box.hpp"

Magma::GUI::Elements::Box::Box(glm::vec4 backgroundColor) :
	Element(std::type_index(typeid(Box)), true),
	m_backgroundColor(backgroundColor)
{

}

Magma::GUI::Elements::BoxRenderer::BoxRenderer(
	Framework::Graphics::RenderDevice * renderDevice,
	Resources::ResourceView vertexShader,
	Resources::ResourceView pixelShader) :

	ElementRenderer(std::type_index(typeid(Box))),
	m_renderDevice(renderDevice),

	m_vs(vertexShader),
	m_ps(pixelShader)
{
	glm::vec2 positions[6] =
	{
		{ 0.0f, 0.0f, },
		{ 1.0f, 0.0f, },
		{ 1.0f, 1.0f, },

		{ 0.0f, 0.0f, },
		{ 1.0f, 0.0f, },
		{ 1.0f, 1.0f, },

	};

	m_vb = m_renderDevice->CreateVertexBuffer(sizeof(glm::vec2) * 6, positions, Framework::Graphics::BufferUsage::Static);

}

Magma::GUI::Elements::BoxRenderer::~BoxRenderer()
{
	m_renderDevice->DestroyVertexArray(m_va);
	m_renderDevice->DestroyVertexBuffer(m_vb);
	m_renderDevice->DestroyVertexLayout(m_vl);
}

void Magma::GUI::Elements::BoxRenderer::Render(Element * element)
{
	auto box = static_cast<Box*>(element);


}