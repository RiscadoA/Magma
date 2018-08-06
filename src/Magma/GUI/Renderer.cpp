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

Magma::GUI::Renderer::Renderer()
{

}

Magma::GUI::Renderer::~Renderer()
{
	for (auto& r : m_renderers)
		delete r;
	m_renderers.clear();
}

void Magma::GUI::Renderer::Render(Root * root)
{
	// Render GUI tree elements
	auto child = root->GetFirstChild();
	while (child != nullptr)
	{
		this->RenderRecursive(child);
		child = child->GetNext();
	}
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
