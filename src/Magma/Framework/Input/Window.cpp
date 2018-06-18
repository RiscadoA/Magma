#include "Window.hpp"

#include <Config.hpp>
#include <iostream.hpp>

int main(int argc, char** argv)
{
#ifdef MAGMA_FRAMEWORK_USE_OPENGL
	std::cout << "opengl" << std::endl;
#elif MAGMA_FRAMEWORK_USE_DIRECTX
	std::cout << "directx" << std::endl;
#endif
	
	Main(argc, argv);
}