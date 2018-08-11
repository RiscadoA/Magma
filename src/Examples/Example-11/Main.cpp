#include <Magma/Framework/Input/GLWindow.hpp>
#include <Magma/Framework/Graphics/OGL410RenderDevice.hpp>

#include <Magma/Framework/Input/D3DWindow.hpp>
#include <Magma/Framework/Graphics/D3D11RenderDevice.hpp>

#include <Magma/Framework/Files/STDFileSystem.hpp>
#include <Magma/Framework/String/Conversion.hpp>
#include <iostream>

#include <Magma/Resources/Manager.hpp>

#include <Magma/GUI/Renderer.hpp>
#include <Magma/GUI/Input.hpp>
#include <Magma/GUI/Elements/Box.hpp>
#include <Magma/GUI/Elements/Text.hpp>

#include <Magma/Resources/Shader.hpp>
#include <Magma/Resources/AudioStream.hpp>
#include <Magma/Resources/Font.hpp>

#include <Magma/Framework/Audio/OALRenderDevice.hpp>

//#define USE_GL

using namespace Magma;

struct Scene
{
	Framework::Input::Window* window;
	Framework::Graphics::RenderDevice* graphicsDevice;
	Framework::Audio::RenderDevice* audioDevice;
	bool running;

	GUI::Root* guiRoot;
	GUI::Renderer* guiRenderer;
	GUI::Input* guiInput;
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
		scene.window = new Framework::Input::GLWindow(800, 600, u8"Example-11", Framework::Input::Window::Mode::Windowed);
#else
		scene.window = new Framework::Input::D3DWindow(800, 600, u8"Example-11", Framework::Input::Window::Mode::Windowed);
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
	Resources::Manager::AddImporter<Resources::FontImporter>(scene.graphicsDevice);

	// Load permament resources<
	Resources::Manager::Load();

	// Create GUI Root
	{
		scene.guiRoot = new GUI::Root();
		GUI::BoundingBox bb;
		bb.left.absolute = 0.0f;
		bb.right.absolute = scene.window->GetWidth();
		bb.top.absolute = scene.window->GetHeight();
		bb.bottom.absolute = 0.0f;
		scene.guiRoot->SetBox(bb);
	}

	// Create GUI Renderer
	{
		scene.guiRenderer = new GUI::Renderer(scene.graphicsDevice);
		scene.guiRenderer->AddRenderer<GUI::Elements::BoxRenderer>(scene.graphicsDevice, Resources::Manager::GetResource("v-box"));
		scene.guiRenderer->AddRenderer<GUI::Elements::TextRenderer>(scene.graphicsDevice, Resources::Manager::GetResource("v-text"));
	}

	// Create GUI Input
	{
		scene.guiInput = new GUI::Input(scene.window, scene.guiRoot);
	}

	// Create GUI box element
	{
		GUI::BoundingBox bb;

		auto element1 = scene.guiRoot->CreateElement<GUI::Elements::Box>(nullptr, glm::vec4(0.3f, 0.3f, 0.3f, 1.0f), Resources::Manager::GetResource("p-box"));
		bb.left.absolute = -50.0f;
		bb.left.relative = 0.5f;
		bb.right.absolute = 50.0f;
		bb.right.relative = 0.5f;
		bb.bottom.absolute = -50.0f;
		bb.bottom.relative = 0.25f;
		bb.top.absolute = 50.0f;
		bb.top.relative = 0.25f;
		element1->SetBox(bb);
		element1->OnMouseDown.AddListener([](Framework::Input::Mouse button)
		{
			if (button == Framework::Input::Mouse::Left)
				printf("Mouse down on element 1\n");
		});
		element1->OnMouseUp.AddListener([](Framework::Input::Mouse button)
		{
			if (button == Framework::Input::Mouse::Left)
				printf("Mouse up on element 1\n");
		});
		element1->OnMouseScroll.AddListener([](float delta)
		{
			printf("Mouse scrolled (%f) on element 1\n", delta);
		});

		auto element2 = scene.guiRoot->CreateElement<GUI::Elements::Text>(
			element1,
			U"",
			glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
			glm::vec4(0.0f, 0.0f, 0.0f, 0.0f),
			48.0f,
			Resources::Manager::GetResource("arial"),
			Resources::Manager::GetResource("p-text"));

		bb.left.absolute = 0.0f;
		bb.left.relative = 0.0f;
		bb.bottom.absolute = 0.0f;
		bb.bottom.relative = 0.0f;
		element2->SetBox(bb);

		element2->SetText(U"text");
		element2->ResizeToFit();

		element2->OnMouseDown.AddListener([](Framework::Input::Mouse button)
		{
			if (button == Framework::Input::Mouse::Left)
				printf("Mouse down on element 2\n");
		});
		element2->OnMouseUp.AddListener([](Framework::Input::Mouse button)
		{
			if (button == Framework::Input::Mouse::Left)
				printf("Mouse up on element 2\n");
		});
		element2->OnMouseScroll.AddListener([](float delta)
		{
			printf("Mouse scrolled (%f) on element 2\n", delta);
		});
	}
}

void CleanScene(Scene& scene)
{
	// Delete GUI stuff
	delete scene.guiInput;
	delete scene.guiRenderer;
	delete scene.guiRoot;

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

	// Main loop
	while (scene.running)
	{
		// Poll events
		scene.window->PollEvents();

		// Clear screen
		scene.graphicsDevice->Clear(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

		// Draw GUI
		scene.guiRenderer->Render(scene.guiRoot);

		// Swap screen back and front buffers
		scene.graphicsDevice->SwapBuffers();
	}

	CleanScene(scene);
}
catch (Framework::Graphics::ShaderError& e)
{
	std::cout << "Shader error caught:" << std::endl;
	std::cout << e.what() << std::endl;
	abort();
}
catch (Framework::Graphics::RenderDeviceError& e)
{
	std::cout << "Render device error caught:" << std::endl;
	std::cout << e.what() << std::endl;
	abort();
}
catch (Framework::Graphics::TextError& e)
{
	std::cout << "Text error caught:" << std::endl;
	std::cout << e.what() << std::endl;
	abort();
}
catch (std::runtime_error& e)
{
	std::cout << "Runtime error caught:" << std::endl;
	std::cout << e.what() << std::endl;
	abort();
}