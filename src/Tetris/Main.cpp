#include <Magma/Framework/Input/D3DWindow.hpp>
#include <Magma/Framework/Input/GLWindow.hpp>
#include <Magma/Framework/Graphics/D3D11Context.hpp>
#include <Magma/Framework/Graphics/GLContext.hpp>
#include <iostream>

using namespace Magma;

void Main(int argc, char** argv)
{
	bool running = true;

	Framework::Input::Window* window = new Framework::Input::D3DWindow(800, 600, "Tetris", Framework::Input::Window::Mode::Windowed);
	
	Framework::Graphics::Context* context = new Framework::Graphics::D3D11Context();
	context->Init(window);

	window->OnClose.AddListener([&running]() { running = false; });
	
	while (running)
	{
		window->PollEvents();
	}

	context->Terminate();
	delete context;
	delete window;
}