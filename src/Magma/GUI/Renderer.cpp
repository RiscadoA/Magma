#include "Renderer.hpp"
#include "Exception.hpp"

#include <sstream>

struct ElementConstantBuffer
{
	glm::mat4 transform;
	glm::vec4 backgroundColor;
};

Magma::GUI::Renderer::Renderer(Framework::Graphics::RenderDevice * device)
	: m_device(device)
{
	m_elementCB = m_device->CreateConstantBuffer(sizeof(ElementConstantBuffer));
}

Magma::GUI::Renderer::~Renderer()
{
	m_device->DestroyConstantBuffer(m_elementCB);
}

void Magma::GUI::Renderer::Render(Element * element)
{
	switch (element->GetType())
	{
		case Elements::Type::Root:
		case Elements::Type::Null:
			break; // Do nothing

		case Elements::Type::Box:
		{
			break;
		}

		case Elements::Type::Text:
		{
			break;
		}

		case Elements::Type::Invalid:
		{
			std::stringstream ss;
			ss << "Failed to render on GUI::Renderer:" << std::endl;
			ss << "Invalid GUI element type";
			throw ElementError(ss.str());
			break;
		}

		default:
		{
			std::stringstream ss;
			ss << "Failed to render on GUI::Renderer:" << std::endl;
			ss << "Unknown GUI element type";
			throw ElementError(ss.str());
			break;
		}		
	}

	// Render children
	auto child = element->GetFirstChild();
	while (child != nullptr)
	{
		this->Render(child);
		child = child->GetNext();
	}
}
