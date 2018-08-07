#include "Renderer.hpp"
#include "Exception.hpp"

#include <sstream>
#include <Magma/Resources/Manager.hpp>

#include "Elements/Box.hpp"

struct ElementConstantBuffer
{
	glm::mat4 transform;
	glm::vec4 backgroundColor;
};

Magma::GUI::Renderer::Renderer(Framework::Graphics::RenderDevice* device)
	: m_device(device)
{
	// Create depth stencil state
	{
		Framework::Graphics::DepthStencilStateDesc desc;
		desc.depthEnabled = false;
		m_depthStencilState = m_device->CreateDepthStencilState(desc);
	}

	// Create blend state
	{
		Framework::Graphics::BlendStateDesc desc;
		desc.blendEnabled = true;
		desc.sourceFactor = Framework::Graphics::BlendFactor::SourceAlpha;
		desc.destinationFactor = Framework::Graphics::BlendFactor::InverseSourceAlpha;
		m_blendState = m_device->CreateBlendState(desc);
	}
}

Magma::GUI::Renderer::~Renderer()
{
	m_device->DestroyDepthStencilState(m_depthStencilState);

	for (auto& r : m_renderers)
		delete r;
	m_renderers.clear();
}

void Magma::GUI::Renderer::Render(Root * root)
{
	m_device->SetDepthStencilState(m_depthStencilState);
	m_device->SetBlendState(m_blendState);

	// Render GUI tree elements
	auto child = root->GetFirstChild();
	while (child != nullptr)
	{
		this->RenderRecursive(child);
		child = child->GetNext();
	}

	m_device->SetBlendState(nullptr);
	m_device->SetDepthStencilState(nullptr);
}

void Magma::GUI::Renderer::RenderRecursive(Element * element)
{
	// If the element is renderable, render it
	if (element->IsRenderable())
	{
		// If element doesn't have a renderer yet, search for one
		if (element->GetRenderer() == nullptr)
		{
			for (auto& r : m_renderers)
				if (r->GetType() == element->GetType())
					element->SetRenderer(r);
			if (element->GetRenderer() == nullptr)
			{
				std::stringstream ss;
				ss << "Failed to render GUI:" << std::endl;
				ss << "Failed to render element:" << std::endl;
				ss << "Couldn't find an element renderer with the type '" << element->GetType().name() << "'";
				throw RenderingError(ss.str());
			}
		}

		// Render the element
		auto renderer = element->GetRenderer();
		if (renderer != nullptr)
			renderer->Render(element);
	}

	// Render the element's children
	auto child = element->GetFirstChild();
	while (child != nullptr)
	{
		this->RenderRecursive(child);
		child = child->GetNext();
	}
}
