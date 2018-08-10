#include <Magma/Framework/Input/GLWindow.hpp>
#include <Magma/Framework/Graphics/OGL410RenderDevice.hpp>

#include <Magma/Framework/Input/D3DWindow.hpp>
#include <Magma/Framework/Graphics/D3D11RenderDevice.hpp>

#include <Magma/Framework/Files/STDFileSystem.hpp>
#include <Magma/Framework/String/Conversion.hpp>
#include <iostream>

#include <Magma/Resources/Manager.hpp>

#include <Magma/Resources/Shader.hpp>
#include <Magma/Resources/AudioStream.hpp>

#include <Magma/Framework/Audio/OALRenderDevice.hpp>

#define USE_GL

using namespace Magma;

struct Scene
{
	Framework::Input::Window* window;
	Framework::Graphics::RenderDevice* graphicsDevice;
	Framework::Audio::RenderDevice* audioDevice;
	bool running;
};

void LoadScene(Scene& scene)
{
	// Init resource manager
	{
		Resources::ManagerSettings settings;
		settings.rootPath = "../../../../../resources/";
		Resources::Manager::Init(settings);
	}

	// Create window
	{
#ifdef USE_GL
		scene.window = new Framework::Input::GLWindow(800, 600, u8"Example-10"_ms, Framework::Input::Window::Mode::Windowed);
#else
		scene.window = new Framework::Input::D3DWindow(800, 600, u8"Example-10"_ms, Framework::Input::Window::Mode::Windowed);
#endif
		scene.running = true;
		scene.window->OnClose.AddListener([&scene]() { scene.running = false; });
	}

	// Create graphics render device
	{
		Framework::Graphics::RenderDeviceSettings settings;
#ifdef USE_GL
		scene.graphicsDevice = new Framework::Graphics::OGL410RenderDevice();
#else
		scene.graphicsDevice = new Framework::Graphics::D3D11RenderDevice();
#endif
		scene.graphicsDevice->Init(scene.window, settings);
	}

	// Create audio render device
	{
		Framework::Audio::RenderDeviceSettings settings;
		scene.audioDevice = new Framework::Audio::OALRenderDevice();
		scene.audioDevice->Init(settings);
	}

	// Add resource importers
	Resources::Manager::AddImporter<Resources::AudioStreamImporter>(scene.audioDevice);
	Resources::Manager::AddImporter<Resources::ShaderImporter>(scene.graphicsDevice);

	// Load permament resources
	Resources::Manager::Load();
}

void CleanScene(Scene& scene)
{

	// Destroy audio and graphics devices, window and filesytem
	delete scene.audioDevice;
	delete scene.graphicsDevice;
	delete scene.window;

	// Terminate resources manager
	Resources::Manager::Terminate();
}

void Main(int argc, char** argv) try
{
	Scene scene;
	
	LoadScene(scene);

	auto src = scene.audioDevice->CreateSource();

	Framework::Audio::Buffer* buffers[2];
	buffers[0] = scene.audioDevice->CreateBuffer();
	buffers[1] = scene.audioDevice->CreateBuffer();

	auto rsc = Resources::Manager::GetResource("test-music-1");
	auto claimGuard = Resources::AudioStreamClaimGuard(rsc->GetData<Resources::AudioStream>());

	for (size_t i = 0; i < 2; ++i)
	{
		rsc->GetData<Resources::AudioStream>()->FillBuffer(buffers[i]);
		src->QueueBuffer(buffers[i]);
	}

	src->Play();

	bool last = false;

	// Main loop
	while (scene.running)
	{
		while (src->GetProcessedBuffers() > 0)
		{
			auto buf = src->UnqueueBuffer();
			if (last == false)
			{
				last = rsc->GetData<Resources::AudioStream>()->FillBuffer(buf);
				src->QueueBuffer(buf);
				
				if (!src->IsPlaying())
					src->Play();
			}
			else claimGuard.Unclaim();
		}

		// Poll events
		scene.window->PollEvents();

		// Clear screen
		scene.graphicsDevice->Clear(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

		// Swap screen back and front buffers
		scene.graphicsDevice->SwapBuffers();
	}

	CleanScene(scene);
}
catch (Framework::Graphics::ShaderError& e)
{
	std::cout << "Shader error caught:" << std::endl;
	std::cout << e.what() << std::endl;
	getchar();
}
catch (Framework::Graphics::RenderDeviceError& e)
{
	std::cout << "Render device error caught:" << std::endl;
	std::cout << e.what() << std::endl;
	getchar();
}
catch (Framework::Graphics::TextError& e)
{
	std::cout << "Text error caught:" << std::endl;
	std::cout << e.what() << std::endl;
	getchar();
}