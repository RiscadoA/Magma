#include "Input.hpp"

Magma::GUI::Input::Input(
	Framework::Input::Window * window, 
	Root* root) :
	
	m_window(window),
	m_root(root)
{
	m_evtOnMouseEnter = m_window->OnMouseEnter.AddListener(std::bind(&Input::OnMouseEnter, this));
	m_evtOnMouseLeave = m_window->OnMouseLeave.AddListener(std::bind(&Input::OnMouseLeave, this));
	m_evtOnMouseMove = m_window->OnMouseMove.AddListener(std::bind(&Input::OnMouseMove, this, std::placeholders::_1, std::placeholders::_1));
	m_evtOnMouseScroll = m_window->OnMouseScroll.AddListener(std::bind(&Input::OnMouseScroll, this, std::placeholders::_1));
	m_evtOnMouseDown = m_window->OnMouseDown.AddListener(std::bind(&Input::OnMouseDown, this, std::placeholders::_1));
	m_evtOnMouseUp = m_window->OnMouseUp.AddListener(std::bind(&Input::OnMouseUp, this, std::placeholders::_1));
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
}

void Magma::GUI::Input::OnMouseLeave()
{
}

void Magma::GUI::Input::OnMouseMove(float x, float y)
{
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
