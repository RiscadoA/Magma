#include <Magma/Framework/Input/GLWindow.hpp>
#include <Magma/Framework/Graphics/OGL410RenderDevice.hpp>

#include <Magma/Framework/Input/D3DWindow.hpp>
#include <Magma/Framework/Graphics/D3D11RenderDevice.hpp>

#include <Magma/Framework/Audio/OALRenderDevice.hpp>

#include <Magma/Framework/Files/STDFileSystem.hpp>
#include <Magma/Framework/String/Conversion.hpp>
#include <iostream>

#define USE_GL

using namespace Magma;

struct Scene
{
	Framework::Input::Window* window;
	Framework::Graphics::RenderDevice* graphicsDevice;
	Framework::Audio::RenderDevice* audioDevice;
	Framework::Files::FileSystem* fileSystem;
	bool running;
};

void LoadScene(Scene& scene)
{
	// Create filesystem
	{
		scene.fileSystem = new Framework::Files::STDFileSystem("../../../../../resources/");
	}

	// Create window
	{
#ifdef USE_GL
		scene.window = new Framework::Input::GLWindow(800, 600, "Example-9", Framework::Input::Window::Mode::Windowed);
#else
		scene.window = new Framework::Input::D3DWindow(800, 600, "Example-9", Framework::Input::Window::Mode::Windowed);
#endif
		scene.running = true;
		scene.window->OnClose.AddListener([&scene]() { scene.running = false; });
	}

	// Create graphics device
	{
		Framework::Graphics::RenderDeviceSettings settings;
#ifdef USE_GL
		scene.graphicsDevice = new Framework::Graphics::OGL410RenderDevice();
#else
		scene.graphicsDevice = new Framework::Graphics::D3D11RenderDevice();
#endif
		scene.graphicsDevice->Init(scene.window, settings);
	}

	// Create audio device
	{
		Framework::Audio::RenderDeviceSettings settings;
		scene.audioDevice = new Framework::Audio::OALRenderDevice();
		scene.audioDevice->Init(settings);
	}
}

void CleanScene(Scene& scene)
{
	// Destroy graphics and audio devices, window and filesytem
	delete scene.audioDevice;
	delete scene.graphicsDevice;
	delete scene.window;
	delete scene.fileSystem;
}

void Main(int argc, char** argv) try
{
	Scene scene;

	LoadScene(scene);

	// Main loop
	while (scene.running)
	{
		// Poll events
		scene.window->PollEvents();

		// Clear screen
		scene.graphicsDevice->Clear(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

		// Swap screen back and front buffers
		scene.graphicsDevice->SwapBuffers();
	}

	CleanScene(scene);
}
catch (Framework::Graphics::RenderDeviceError& e)
{
	std::cout << "Graphics render device error caught:" << std::endl;
	std::cout << e.what() << std::endl;
	getchar();
}
catch (Framework::Audio::RenderDeviceError& e)
{
	std::cout << "Audio render device error caught:" << std::endl;
	std::cout << e.what() << std::endl;
	getchar();
}
