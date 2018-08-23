#include <Magma/Framework/Input/GLWindow.hpp>
#include <Magma/Framework/Graphics/1.X/OGL410RenderDevice.hpp>

#include <Magma/Framework/Input/D3DWindow.hpp>
#include <Magma/Framework/Graphics/1.X/D3D11RenderDevice.hpp>

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
	Framework::Graphics_V1X::RenderDevice* Graphics_V1XDevice;
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

	// Create Graphics_V1X render device
	{
		Framework::Graphics_V1X::RenderDeviceSettings settings;
#ifdef USE_GL
		scene.Graphics_V1XDevice = new Framework::Graphics_V1X::OGL410RenderDevice();
#else
		scene.Graphics_V1XDevice = new Framework::Graphics_V1X::D3D11RenderDevice();
#endif
		scene.Graphics_V1XDevice->Init(scene.window, settings);
	}

	// Create audio render device
	{
		Framework::Audio::RenderDeviceSettings settings;
		scene.audioDevice = new Framework::Audio::OALRenderDevice();
		scene.audioDevice->Init(settings);
	}

	// Add resource importers
	Resources::Manager::AddImporter<Resources::AudioStreamImporter>(scene.audioDevice);
	Resources::Manager::AddImporter<Resources::ShaderImporter>(scene.Graphics_V1XDevice);
	Resources::Manager::AddImporter<Resources::FontImporter>(scene.Graphics_V1XDevice);

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
		scene.guiRenderer = new GUI::Renderer(scene.Graphics_V1XDevice);
		scene.guiRenderer->AddRenderer<GUI::Elements::BoxRenderer>(scene.Graphics_V1XDevice, Resources::Manager::GetResource("v-box"));
		scene.guiRenderer->AddRenderer<GUI::Elements::TextRenderer>(scene.Graphics_V1XDevice, Resources::Manager::GetResource("v-text"));
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

	// Destroy audio and Graphics_V1X devices, window and filesytem
	delete scene.audioDevice;
	delete scene.Graphics_V1XDevice;
	delete scene.window;

	// Terminate resources manager
	Resources::Manager::Terminate();
}

int main(int argc, const char** argv) try
{
	if (mfInit(argc, argv) != MF_ERROR_OKAY)
		abort();

	Scene scene;
	
	LoadScene(scene);

	// Main loop
	while (scene.running)
	{
		// Poll events
		scene.window->PollEvents();

		// Clear screen
		scene.Graphics_V1XDevice->Clear(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

		// Draw GUI
		scene.guiRenderer->Render(scene.guiRoot);

		// Swap screen back and front buffers
		scene.Graphics_V1XDevice->SwapBuffers();
	}

	CleanScene(scene);

	mfTerminate();
	return 0;
}
catch (Framework::Graphics_V1X::ShaderError& e)
{
	std::cout << "Shader error caught:" << std::endl;
	std::cout << e.what() << std::endl;
	abort();
}
catch (Framework::Graphics_V1X::RenderDeviceError& e)
{
	std::cout << "Render device error caught:" << std::endl;
	std::cout << e.what() << std::endl;
	abort();
}
catch (Framework::Graphics_V1X::TextError& e)
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