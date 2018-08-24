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

	Input::Window win = Input::CreateWindow(u8"d3d", 800, 600, Input::WindowMode::Windowed, u8"Window");
	win.SetOnCloseCallback(OnWindowClose);

	Graphics::V2X::RenderDevice rd = NULL;

	try
	{
		mfgV2XRenderDeviceDesc desc;
		desc.vsyncEnabled = MFM_TRUE;
		rd = Graphics::V2X::CreateRenderDevice(u8"d3d11", win, &desc, Memory::StandardAllocator);
	}
	catch (std::runtime_error& err)
	{
		String::ErrStream.PutString(err.what());
	}

	while (!winShouldClose)
	{
		win.PollEvents();
		
	}

	rd.Release();

	win.Release();

	Magma::Framework::Terminate();
	return 0;
}
