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

	Input::WindowHandle win = Input::CreateWindow(NULL, 800, 600, Input::WindowMode::Windowed, u8"Window");
	win.SetOnCloseCallback(OnWindowClose);

	Graphics::V2X::RenderDevice rd = NULL;

	try
	{
		Graphics::V2X::RenderDeviceDesc desc;
		desc.vsyncEnabled = true;
		rd = Graphics::V2X::CreateRenderDevice(NULL, win, &desc, Memory::StandardAllocator);
	}
	catch (std::runtime_error& err)
	{
		String::ErrStream.PutString(err.what());
	}

	while (!winShouldClose)
	{
		win.PollEvents();
		
		rd.ClearColor(0.0f, 0.2f, 0.4f, 1.0f);

		rd.SwapBuffers();
	}

	rd.Release();

	win.Release();

	Magma::Framework::Terminate();
	return 0;
}
