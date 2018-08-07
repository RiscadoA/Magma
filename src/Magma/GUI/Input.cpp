#include "Input.hpp"

Magma::GUI::Input::Input(
	Framework::Input::Window * window, 
	Root* root) :
	
	m_window(window),
	m_root(root)
{
	// Init events
	m_evtOnMouseLeave = m_window->OnMouseLeave.AddListener(std::bind(&Input::OnMouseLeave, this));
	m_evtOnMouseMove = m_window->OnMouseMove.AddListener(std::bind(&Input::OnMouseMove, this, std::placeholders::_1, std::placeholders::_2));
	m_evtOnMouseScroll = m_window->OnMouseScroll.AddListener(std::bind(&Input::OnMouseScroll, this, std::placeholders::_1));
	m_evtOnMouseDown = m_window->OnMouseDown.AddListener(std::bind(&Input::OnMouseDown, this, std::placeholders::_1));
	m_evtOnMouseUp = m_window->OnMouseUp.AddListener(std::bind(&Input::OnMouseUp, this, std::placeholders::_1));

	// Init mouse pos
	m_mX = 0.0f;
	m_mY = 0.0f;
}

Magma::GUI::Input::~Input()
{
	m_window->OnMouseLeave.RemoveListener(m_evtOnMouseLeave);
	m_window->OnMouseMove.RemoveListener(m_evtOnMouseMove);
	m_window->OnMouseScroll.RemoveListener(m_evtOnMouseScroll);
	m_window->OnMouseDown.RemoveListener(m_evtOnMouseDown);
	m_window->OnMouseUp.RemoveListener(m_evtOnMouseUp);
}

void Magma::GUI::Input::OnMouseLeave()
{
	OnMouseLeaveRecursive(m_root);
}

void Magma::GUI::Input::OnMouseMove(float x, float y)
{
	// Update mouse position
	m_mX = x;
	m_mY = m_window->GetHeight() - y;

	// If the mouse is inside the GUI root element
	Point mouse;
	mouse.x.absolute = m_mX;
	mouse.y.absolute = m_mY;
	if (m_root->Contains(mouse))
	{
		if (!m_root->IsMouseOver())
			m_root->OnMouseEnter.Fire();
		m_root->OnMouseMove.Fire(m_mX, m_mY);

		auto c = m_root->GetFirstChild();
		while (c != nullptr)
		{
			OnMouseMoveRecursive(c, true);
			c = c->GetNext();
		}
	}
	// Otherwise
	else
	{
		if (m_root->IsMouseOver())

			m_root->OnMouseLeave.Fire();

		auto c = m_root->GetFirstChild();
		while (c != nullptr)
		{
			OnMouseMoveRecursive(c, false);
			c = c->GetNext();
		}
	}
}

void Magma::GUI::Input::OnMouseScroll(float x)
{
	OnMouseScrollRecursive(m_root, x);
}

void Magma::GUI::Input::OnMouseDown(Framework::Input::Mouse button)
{
	OnMouseDownRecursive(m_root, button);
}

void Magma::GUI::Input::OnMouseUp(Framework::Input::Mouse button)
{
	OnMouseUpRecursive(m_root, button);
}

void Magma::GUI::Input::OnMouseLeaveRecursive(Element* element)
{
	if (element->IsMouseOver())
		element->OnMouseLeave.Fire();

	auto c = element->GetFirstChild();
	while (c != nullptr)
	{
		OnMouseLeaveRecursive(c);
		c = c->GetNext();
	}
}

void Magma::GUI::Input::OnMouseMoveRecursive(Element* element, bool inside)
{
	// Check if the mouse is inside any bounding box
	Point mouse;
	mouse.x.absolute = m_mX;
	mouse.y.absolute = m_mY;

	if (!inside)
	{
		// If mouse was hovering the element, trigger OnMouseLeave
		if (element->IsMouseOver())
			element->OnMouseLeave.Fire();
	}
	else
	{
		if (element->Contains(mouse))
		{
			// If mouse wasn't hovering the element, trigger OnMouseEnter
			if (!element->IsMouseOver())
				element->OnMouseEnter.Fire();

			auto bb = element->GetAbsoluteBoundingBox();
			float mx = m_mX - bb.left.absolute;
			float my = m_mY - bb.bottom.absolute;

			// Trigger OnMouseMove event
			element->OnMouseMove.Fire(mx, my);
		}
		else
		{
			// If mouse was hovering the element, trigger OnMouseLeave
			if (element->IsMouseOver())
				element->OnMouseLeave.Fire();
			inside = false;
		}
	}

	auto c = element->GetFirstChild();
	while (c != nullptr)
	{
		OnMouseMoveRecursive(c, inside);
		c = c->GetNext();
	}
}

bool Magma::GUI::Input::OnMouseScrollRecursive(Element* element, float x)
{
	Point mouse;
	mouse.x.absolute = m_mX;
	mouse.y.absolute = m_mY;

	// If this element contains the mouse
	if (element->Contains(mouse))
	{
		//	Check if its children contain it
		bool childrenHave = false;
		auto c = element->GetFirstChild();
		while (c != nullptr)
		{
			if (OnMouseScrollRecursive(c, x))
			{
				childrenHave = true;
				break;
			}
			c = c->GetNext();
		}

		if (!childrenHave)
		{
			element->OnMouseScroll.Fire(x);
		}

		return true;
	}
	else return false;
}

bool Magma::GUI::Input::OnMouseDownRecursive(Element* element, Framework::Input::Mouse button)
{
	Point mouse;
	mouse.x.absolute = m_mX;
	mouse.y.absolute = m_mY;

	// If this element contains the mouse
	if (element->Contains(mouse))
	{
		//	Check if its children contain it
		bool childrenHave = false;
		auto c = element->GetFirstChild();
		while (c != nullptr)
		{
			if (OnMouseDownRecursive(c, button))
			{
				childrenHave = true;
				break;
			}
			c = c->GetNext();
		}

		if (!childrenHave)
		{
			element->OnMouseDown.Fire(button);
		}

		return true;
	}
	else return false;
}

bool Magma::GUI::Input::OnMouseUpRecursive(Element* element, Framework::Input::Mouse button)
{
	Point mouse;
	mouse.x.absolute = m_mX;
	mouse.y.absolute = m_mY;

	// If this element contains the mouse
	if (element->Contains(mouse))
	{
		//	Check if its children contain it
		bool childrenHave = false;
		auto c = element->GetFirstChild();
		while (c != nullptr)
		{
			if (OnMouseUpRecursive(c, button))
			{
				childrenHave = true;
				break;
			}
			c = c->GetNext();
		}

		if (!childrenHave)
		{
			element->OnMouseUp.Fire(button);
		}

		return true;
	}
	else return false;
}
