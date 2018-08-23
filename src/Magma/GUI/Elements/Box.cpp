#include "Box.hpp"
#include "../Exception.hpp"

#include <Magma/Resources/Shader.hpp>
#include <sstream>

struct BoxDataCB
{
	glm::mat4 transform;
	glm::vec4 backgroundColor;
	glm::vec2 boxSize;
};

Magma::GUI::Elements::Box::Box(
	glm::vec4 backgroundColor,
	Resources::ResourceView pixelShader) :

	Element(std::type_index(typeid(Box)), true),
	m_backgroundColor(backgroundColor),
	m_ps(pixelShader),
	m_pp(nullptr)
{
	// Check if the passed pixel shader is really a pixel shader
	if (m_ps->GetData<Resources::Shader>()->type != Framework::Graphics_V1X::ShaderType::Pixel)
	{
		std::stringstream ss;
		ss << "Failed to create Box element:" << std::endl;
		ss << "The pixel shader resource '" << m_ps->GetName() << "' is not a pixel shader";
		throw RenderingError(ss.str());
	}
	
	// Get constant buffer binding point
	m_cbBP = m_ps->GetData<Resources::Shader>()->pixelShader->GetBindingPoint("BOX_DATA");
	if (m_cbBP == nullptr)
	{
		std::stringstream ss;
		ss << "Failed to create Box element:" << std::endl;
		ss << "The pixel shader resource '" << m_ps->GetName() << "' must have a constant buffer named 'BOX_DATA'";
		throw RenderingError(ss.str());
	}
}

glm::vec2 Magma::GUI::Elements::Box::GetSize() const
{
	auto bb = this->GetAbsoluteBoundingBox();
	return glm::vec2(bb.right.absolute - bb.left.absolute, bb.top.absolute - bb.bottom.absolute);
}

Magma::GUI::Elements::BoxRenderer::BoxRenderer(
	Framework::Graphics_V1X::RenderDevice * renderDevice,
	Resources::ResourceView vertexShader) :

	ElementRenderer(std::type_index(typeid(Box))),
	m_renderDevice(renderDevice),
	m_vs(vertexShader)
{
	// Check if the passed vertex shader is really a vertex shader
	if (m_vs->GetData<Resources::Shader>()->type != Framework::Graphics_V1X::ShaderType::Vertex)
	{
		std::stringstream ss;
		ss << "Failed to create BoxRenderer:" << std::endl;
		ss << "The vertex shader resource '" << m_vs->GetName() << "' is not a vertex shader";
		throw RenderingError(ss.str());
	}

	// Create vertex buffer
	{
		glm::vec2 positions[6] =
		{
			{ 0.0f, 0.0f, },
			{ 1.0f, 0.0f, },
			{ 1.0f, 1.0f, },

			{ 1.0f, 1.0f, },
			{ 0.0f, 1.0f, },
			{ 0.0f, 0.0f, },
		};

		m_vb = m_renderDevice->CreateVertexBuffer(sizeof(glm::vec2) * 6, positions, Framework::Graphics_V1X::BufferUsage::Static);
	}

	// Create vertex layout
	{
		Framework::Graphics_V1X::VertexElement elements[1];

		elements[0].bufferIndex = 0;
		elements[0].name = "POSITION";
		elements[0].offset = 0;
		elements[0].size = 2;
		elements[0].stride = sizeof(glm::vec2);
		elements[0].type = Framework::Graphics_V1X::VertexElementType::Float;

		m_vl = m_renderDevice->CreateVertexLayout(1, elements, m_vs->GetData<Resources::Shader>()->vertexShader);
	}

	// Create vertex array
	m_va = m_renderDevice->CreateVertexArray(1, &m_vb, m_vl);

	// Create constant buffer
	m_boxDataCB = m_renderDevice->CreateConstantBuffer(sizeof(BoxDataCB), nullptr, Framework::Graphics_V1X::BufferUsage::Dynamic);

	// Get constant buffer binding point
	m_cbBP = m_vs->GetData<Resources::Shader>()->vertexShader->GetBindingPoint("BOX_DATA");
	if (m_cbBP == nullptr)
	{
		std::stringstream ss;
		ss << "Failed to create BoxRenderer:" << std::endl;
		ss << "The vertex shader resource '" << m_vs->GetName() << "' must have a constant buffer named 'BOX_DATA'";
		throw RenderingError(ss.str());
	}
}

Magma::GUI::Elements::BoxRenderer::~BoxRenderer()
{
	for (auto& pp : m_pps)
		m_renderDevice->DestroyPipeline(pp.pp);

	m_renderDevice->DestroyConstantBuffer(m_boxDataCB);

	m_renderDevice->DestroyVertexArray(m_va);
	m_renderDevice->DestroyVertexBuffer(m_vb);
	m_renderDevice->DestroyVertexLayout(m_vl);
}

Magma::Framework::Graphics_V1X::Pipeline * Magma::GUI::Elements::BoxRenderer::GetPipeline(Box * box)
{
	for (auto& pp : m_pps)
		if (pp.ps == box->m_ps->GetData<Resources::Shader>()->pixelShader &&
			pp.vs == m_vs->GetData<Resources::Shader>()->vertexShader)
			return pp.pp;

	m_pps.emplace_back();
	m_pps.back().ps = box->m_ps->GetData<Resources::Shader>()->pixelShader;
	m_pps.back().vs = m_vs->GetData<Resources::Shader>()->vertexShader;
	m_pps.back().pp = m_renderDevice->CreatePipeline(m_pps.back().vs, m_pps.back().ps);
	return m_pps.back().pp;
}

void Magma::GUI::Elements::BoxRenderer::Render(Element * element)
{
	auto box = static_cast<Box*>(element);

	// If box still hasn't a shader pipeline, get one
	if (box->m_pp == nullptr)
		box->m_pp = GetPipeline(box);

	// Update box data constant buffer
	auto boxData = (BoxDataCB*)m_boxDataCB->Map();
	boxData->transform = box->GetTransform();
	boxData->backgroundColor = box->GetBackgroundColor();
	boxData->boxSize = box->GetSize();
	m_boxDataCB->Unmap();

	// Render box
	m_cbBP->BindConstantBuffer(m_boxDataCB);
	box->m_cbBP->BindConstantBuffer(m_boxDataCB);
	m_renderDevice->SetPipeline(box->m_pp);
	m_renderDevice->SetVertexArray(m_va);
	m_renderDevice->DrawTriangles(0, 6);
}
