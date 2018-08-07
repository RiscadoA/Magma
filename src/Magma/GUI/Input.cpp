#include "Input.hpp"

Magma::GUI::Input::Input(
	Framework::Input::Window * window, 
	Root* root) :
	
	m_window(window),
	m_root(root)
{
	// Init events
	m_evtOnMouseEnter = m_window->OnMouseEnter.AddListener(std::bind(&Input::OnMouseEnter, this));
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
	m_window->OnMouseEnter.RemoveListener(m_evtOnMouseEnter);
	m_window->OnMouseLeave.RemoveListener(m_evtOnMouseLeave);
	m_window->OnMouseMove.RemoveListener(m_evtOnMouseMove);
	m_window->OnMouseScroll.RemoveListener(m_evtOnMouseScroll);
	m_window->OnMouseDown.RemoveListener(m_evtOnMouseDown);
	m_window->OnMouseUp.RemoveListener(m_evtOnMouseUp);
}


void Magma::GUI::Input::OnMouseEnter()
{
	// Do nothing (still not needed)
}

void Magma::GUI::Input::OnMouseLeave()
{
	OnMouseLeaveRecursive(m_root);
}

void Magma::GUI::Input::OnMouseMove(float x, float y)
{
	// Update mouse position
	m_mX = x;
	m_mY = y;

	// If the mouse is inside the GUI root element
	Point mouse;
	mouse.x.absolute = m_mX;
	mouse.y.absolute = m_mY;
	if (m_root->Contains(mouse))
	{
		if (!m_root->IsMouseOver())
		{
			m_root->OnMouseEnter.Fire();
			printf("Mouse entered \"%s\"\n", m_root->GetType().name());
		}
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
		{
			m_root->OnMouseLeave.Fire();
			printf("Mouse left \"%s\"\n", m_root->GetType().name());
		}

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
}

void Magma::GUI::Input::OnMouseDown(Framework::Input::Mouse button)
{
}

void Magma::GUI::Input::OnMouseUp(Framework::Input::Mouse button)
{
}


void Magma::GUI::Input::OnMouseEnterRecursive(Element* element)
{
	// Do nothing (still not needed)
}

void Magma::GUI::Input::OnMouseLeaveRecursive(Element* element)
{
	if (element->IsMouseOver())
	{
		element->OnMouseLeave.Fire();
		printf("Mouse left \"%s\"\n", m_root->GetType().name());
	}

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
		{
			element->OnMouseLeave.Fire();
			printf("Mouse left \"%s\"\n", element->GetType().name());
		}
	}
	else
	{
		if (element->Contains(mouse))
		{
			// If mouse wasn't hovering the element, trigger OnMouseEnter
			if (!element->IsMouseOver())
			{
				element->OnMouseEnter.Fire();
				printf("Mouse entered \"%s\"\n", element->GetType().name());
			}
			// Trigger OnMouseMove event
			element->OnMouseMove.Fire(m_mX, m_mY);
		}
		else
		{
			// If mouse was hovering the element, trigger OnMouseLeave
			if (element->IsMouseOver())
			{
				element->OnMouseLeave.Fire();
				printf("Mouse left \"%s\"\n", element->GetType().name());
			}
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
	return true;
}

bool Magma::GUI::Input::OnMouseDownRecursive(Element* element, Framework::Input::Mouse button)
{
	return true;
}

bool Magma::GUI::Input::OnMouseUpRecursive(Element* element, Framework::Input::Mouse button)
{
	return true;
}
