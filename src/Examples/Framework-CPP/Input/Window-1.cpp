#include <Magma/Framework/Entry.hpp>
#include <Magma/Framework/Input/Window.hpp>
#include <Magma/Framework/Graphics/2.X/RenderDevice.hpp>
#include <Magma/Framework/String/Stream.hpp>

using namespace Magma::Framework;

bool winShouldClose = false;

void OnWindowClose(void* handle)
{
	winShouldClose = true;
}

int main(int argc, const char** argv)
{
	Magma::Framework::Init(argc, argv);

	Input::HWindow win = Input::CreateWindow(NULL, 800, 600, Input::WindowMode::Windowed, u8"Window");
	win.SetOnCloseCallback(OnWindowClose);

	while (!winShouldClose)
	{
		win.WaitForEvents();
	}

	win.Release();

	Magma::Framework::Terminate();
	return 0;
}
