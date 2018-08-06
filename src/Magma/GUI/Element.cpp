#include "Element.hpp"

#include <glm/gtc/matrix_transform.hpp>

Magma::GUI::Element::Element(std::type_index type, bool isRenderable) :
	m_type(type),
	m_isRenderable(isRenderable)
{
	m_renderer = nullptr;

	m_parent = nullptr;
	m_child = nullptr;
	m_next = nullptr;

	m_enabled = true;
	m_valid = false;

	m_dirty = true;
}

Magma::GUI::Element::~Element()
{
	this->Destroy();
}

void Magma::GUI::Element::Destroy()
{
	// Remove this element from its parent
	this->SetParent(nullptr);

	// Remove children
	auto c = m_child;
	while (c != nullptr)
	{
		auto n = c->GetNext();
		this->RemoveChild(c);
		c = n;
	}
}

void Magma::GUI::Element::SetParent(Element * parent)
{
	if (m_parent != nullptr)
		m_parent->RemoveChild(this);
	m_parent = parent;
	if (m_parent != nullptr)
	{
		m_parent->AddChild(this);
		m_valid = true;
	}
	else
		m_valid = false;
}

void Magma::GUI::Element::AddChild(Element * child)
{
	auto c = m_child;
	if (c == nullptr)
		m_child = child;
	else
	{
		while (c->m_next != nullptr)
		{
			if (c == child)
				return;
			c = c->m_next;
		}

		if (c == child)
			return;

		c->m_next = child;
		child->m_next = nullptr;
	}
}

void Magma::GUI::Element::RemoveChild(Element * child)
{
	Element* prev = nullptr;
	auto c = m_child;
	while (c != nullptr)
	{
		if (c == child)
		{
			if (prev == nullptr)
			{
				m_child = c->m_next;
				c->m_next = nullptr;
				return;
			}
			else
			{
				prev->m_next = c->m_next;
				c->m_next = nullptr;
				return;
			}
		}
		else
		{
			prev = c;
			c = c->m_next;
		}
	}
}

bool Magma::GUI::Element::IsChild(Element * child) const
{
	auto c = m_child;
	while (c != nullptr)
	{
		if (c == child)
			return true;
		c = c->m_next;
	}
	
	return false;
}

Magma::GUI::BoundingBox Magma::GUI::Element::GetAbsoluteBoundingBox() const
{
	BoundingBox box;

	if (m_parent == nullptr)
	{
		box.left.relative = 0.0f;
		box.right.relative = 0.0f;
		box.top.relative = 0.0f;
		box.bottom.relative = 0.0f;
	}
	else
	{
		auto p = m_parent->GetAbsoluteBoundingBox();

		box.left.absolute += box.left.relative * (p.right.absolute - p.left.absolute);
		box.left.relative = 0.0f;

		box.right.absolute += box.right.relative * (p.right.absolute - p.left.absolute);
		box.right.relative = 0.0f;

		box.top.absolute += box.top.relative * (p.top.absolute - p.bottom.absolute);
		box.top.relative = 0.0f;

		box.bottom.absolute += box.bottom.relative * (p.top.absolute - p.bottom.absolute);
		box.bottom.relative = 0.0f;
	}

	return box;
}

bool Magma::GUI::Element::Contains(Point point, Element * relativeElement) const
{
	auto b1 = this->GetAbsoluteBoundingBox();

	if (relativeElement == nullptr)
		; // Do nothing, point is already ready to check
	else if (relativeElement == this)
	{
		point.x.absolute += point.x.relative * (b1.right.absolute - b1.left.absolute);
		point.y.absolute += point.y.relative * (b1.top.absolute - b1.bottom.absolute);

		point.x.absolute += b1.left.absolute;
		point.y.absolute += b1.bottom.absolute;
	}
	else
	{
		auto b2 = relativeElement->GetAbsoluteBoundingBox();

		point.x.absolute += point.x.relative * (b2.right.absolute - b2.left.absolute);
		point.y.absolute += point.y.relative * (b2.top.absolute - b2.bottom.absolute);

		point.x.absolute += b2.left.absolute;
		point.y.absolute += b2.bottom.absolute;
	}
	
	if (point.x.absolute < b1.left.absolute || point.x.absolute > b1.right.absolute ||
		point.y.absolute < b1.bottom.absolute || point.y.absolute > b1.top.absolute)
		return false;
	else
		return true;
}
void Magma::GUI::Element::UpdateTransform() const
{
	m_dirty = false;
	auto b = this->GetAbsoluteBoundingBox();
	m_transform = glm::translate(glm::mat4(1.0f), glm::vec3(b.left.absolute, b.bottom.absolute, 0.0f));
}

const glm::mat4 & Magma::GUI::Element::GetTransform() const
{
	if (m_dirty)
		this->UpdateTransform();
	return m_transform;
}