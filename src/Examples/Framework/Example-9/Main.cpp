#include <Magma/Framework/Input/GLWindow.hpp>
#include <Magma/Framework/Graphics/1.X/OGL410RenderDevice.hpp>

#include <Magma/Framework/Input/D3DWindow.hpp>
#include <Magma/Framework/Graphics/1.X/D3D11RenderDevice.hpp>

#include <Magma/Framework/Audio/OALRenderDevice.hpp>

#include <Magma/Framework/Files/STDFileSystem.hpp>
#include <Magma/Framework/String/Conversion.hpp>
#include <iostream>

#include <Magma/Framework/Audio/WAVLoader.hpp>

#define USE_GL

using namespace Magma;

struct Scene
{
	Framework::Input::Window* window;
	Framework::Graphics_V1X::RenderDevice* Graphics_V1XDevice;
	Framework::Audio::RenderDevice* audioDevice;
	Framework::Files::FileSystem* fileSystem;
	bool running;

	Framework::Audio::Buffer* buffer;
	Framework::Audio::Source* source;
};

void LoadScene(Scene& scene)
{
	// Create filesystem
	{
		scene.fileSystem = new Framework::Files::STDFileSystem("../../../../../../resources/");
	}

	// Create window
	{
#ifdef USE_GL
		scene.window = new Framework::Input::GLWindow(800, 600, u8"Example-9", Framework::Input::Window::Mode::Windowed);
#else
		scene.window = new Framework::Input::D3DWindow(800, 600, u8"Example-9", Framework::Input::Window::Mode::Windowed);
#endif
		scene.running = true;
		scene.window->OnClose.AddListener([&scene]() { scene.running = false; });
	}

	// Create Graphics_V1X device
	{
		Framework::Graphics_V1X::RenderDeviceSettings settings;
#ifdef USE_GL
		scene.Graphics_V1XDevice = new Framework::Graphics_V1X::OGL410RenderDevice();
#else
		scene.Graphics_V1XDevice = new Framework::Graphics_V1X::D3D11RenderDevice();
#endif
		scene.Graphics_V1XDevice->Init(scene.window, settings);
	}

	// Create audio device
	{
		Framework::Audio::RenderDeviceSettings settings;
		scene.audioDevice = new Framework::Audio::OALRenderDevice();
		scene.audioDevice->Init(settings);
	}

	// Create buffer
	{
		// Load
		auto file = scene.fileSystem->OpenFile(Framework::Files::FileMode::Read, "/Example-9/pcm mono 16 bit 8kHz.wav");
		auto fileSize = scene.fileSystem->GetSize(file);
		char* fileData = new char[fileSize];
		scene.fileSystem->Read(file, fileData, fileSize);
		scene.fileSystem->CloseFile(file);

		// Parse
		Framework::Audio::WAVData data;
		Framework::Audio::LoadWAV(fileData, fileSize, data);

		scene.buffer = scene.audioDevice->CreateBuffer(data.dataChunk.pcmData, data.dataChunk.header.chunkSize, data.formatChunk.format, data.formatChunk.sampleRate);

		Framework::Audio::UnloadWAV(data);
	}

	// Create source
	{
		scene.source = scene.audioDevice->CreateSource();
		scene.source->QueueBuffer(scene.buffer);
		scene.source->Play();
	}
}

void CleanScene(Scene& scene)
{
	// Destroy audio buffer and source
	scene.audioDevice->DestroySource(scene.source);
	scene.audioDevice->DestroyBuffer(scene.buffer);

	// Destroy Graphics_V1X and audio devices, window and filesytem
	delete scene.audioDevice;
	delete scene.Graphics_V1XDevice;
	delete scene.window;
	delete scene.fileSystem;
}

int main(int argc, const char** argv) try
{
	if (mfInit(argc, argv) != MF_ERROR_OKAY)
		abort();

	Scene scene;

	LoadScene(scene);

	glm::vec3 sourcePos;
	sourcePos.x = 0.0f;
	sourcePos.y = 0.0f;
	sourcePos.z = 0.0f;

	scene.window->OnKeyDown.AddListener([&](Framework::Input::Keyboard code, Framework::Input::KeyModifiers mods)
	{
		if ((mods & Framework::Input::KeyModifiers::Shift) != Framework::Input::KeyModifiers::None)
		{
			switch (code)
			{
				case Framework::Input::Keyboard::P:
					scene.source->Pause();
					break;
				case Framework::Input::Keyboard::U:
					scene.source->Stop();
					break;
				case Framework::Input::Keyboard::O:
					scene.source->Play();
					break;
				case Framework::Input::Keyboard::I:
					scene.source->Rewind();
					break;

				case Framework::Input::Keyboard::A:
					sourcePos.x -= 1.0f;
					scene.source->SetPosition(sourcePos.x, sourcePos.y, sourcePos.z);
					break;
				case Framework::Input::Keyboard::D:
					sourcePos.x += 1.0f;
					scene.source->SetPosition(sourcePos.x, sourcePos.y, sourcePos.z);
					break;
				case Framework::Input::Keyboard::W:
					sourcePos.z += 1.0f;
					scene.source->SetPosition(sourcePos.x, sourcePos.y, sourcePos.z);
					break;
				case Framework::Input::Keyboard::S:
					sourcePos.z -= 1.0f;
					scene.source->SetPosition(sourcePos.x, sourcePos.y, sourcePos.z);
					break;
				case Framework::Input::Keyboard::E:
					sourcePos.y += 1.0f;
					scene.source->SetPosition(sourcePos.x, sourcePos.y, sourcePos.z);
					break;
				case Framework::Input::Keyboard::Q:
					sourcePos.y -= 1.0f;
					scene.source->SetPosition(sourcePos.x, sourcePos.y, sourcePos.z);
					break;
			}
		}
	});

	// Main loop
	while (scene.running)
	{
		// Poll events
		scene.window->PollEvents();

		// Clear screen
		scene.Graphics_V1XDevice->Clear(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

		// Swap screen back and front buffers
		scene.Graphics_V1XDevice->SwapBuffers();
	}

	CleanScene(scene);

	mfTerminate();
	return 0;
}
catch (Framework::Graphics_V1X::RenderDeviceError& e)
{
	std::cout << "Graphics_V1X render device error caught:" << std::endl;
	std::cout << e.what() << std::endl;
	getchar();
}
catch (Framework::Audio::RenderDeviceError& e)
{
	std::cout << "Audio render device error caught:" << std::endl;
	std::cout << e.what() << std::endl;
	getchar();
}
