#include <Magma/Framework/Input/GLWindow.hpp>
#include <iostream>

using namespace Magma;

void Main(int argc, char** argv)
{
	bool running = true;

	auto window = Framework::Input::GLWindow(240, 800, "Tetris");
	window.OnClose.AddListener([&running]() { running = false; });

	while (running)
	{
		window.PollEvents();
	}
}