#include <Magma/Framework/Entry.hpp>
#include <Magma/Framework/Input/Window.hpp>

using namespace Magma::Framework;

int main(int argc, const char** argv)
{
	Magma::Framework::Init(argc, argv);

	auto win = Input::CreateWindow(u8"d3d", 800, 600, Input::WindowMode::Windowed, u8"D3D Window");

	while (true)
	{
		win.WaitForEvents();
	}

	Magma::Framework::Terminate();
	return 0;
}
