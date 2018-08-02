#include "Manager.hpp"

#include <sstream>

Magma::Resources::Manager* Magma::Resources::Manager::s_manager = nullptr;

void Magma::Resources::Manager::Init()
{
	if (s_manager != nullptr)
	{
		// TO DO: THROW ERROR
	}

	// TO DO: INIT MANAGER
}

void Magma::Resources::Manager::Terminate()
{
	if (s_manager == nullptr)
	{
		// TO DO: THROW ERROR
	}

	// TO DO: TERMINATE MANAGER

	s_manager = nullptr;
}
