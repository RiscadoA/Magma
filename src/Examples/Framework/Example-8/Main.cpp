#include <Magma/Framework/Input/GLWindow.hpp>
#include <Magma/Framework/Graphics/OGL410RenderDevice.hpp>
#include <Magma/Framework/Graphics/OGL410Assembler.hpp>

#include <Magma/Framework/Input/D3DWindow.hpp>
#include <Magma/Framework/Graphics/D3D11RenderDevice.hpp>
#include <Magma/Framework/Graphics/D3D11Assembler.hpp>

#include <Magma/Framework/Files/STDFileSystem.hpp>
#include <Magma/Framework/String/Conversion.hpp>
#include <iostream>

#include <glm/gtc/matrix_transform.hpp>

#include <Magma/Framework/Graphics/ShaderData.hpp>
#include <Magma/Framework/Graphics/ShaderCompiler.hpp>

#include <Magma/Framework/Graphics/Font.hpp>
#include <Magma/Framework/Graphics/TextRenderer.hpp>

#define USE_GL

using namespace Magma;
using namespace Magma::Framework;

struct Vertex
{
	float x, y, z;
	float u, v;
};

struct Scene
{
	Input::Window* window;
	Graphics::RenderDevice* device;
	Files::FileSystem* fileSystem;
	bool running;

	Graphics::VertexShader* vertexShader;
	Graphics::PixelShader* pixelShader;
	Graphics::Pipeline* pipeline;

	Graphics::Sampler2D* sampler;
	Graphics::PixelBindingPoint* textureBP;

	Graphics::ConstantBuffer* transformBuffer;
	Graphics::VertexBindingPoint* transformBP;

	Graphics::RasterState* rasterState;
	Graphics::DepthStencilState* depthStencilState;
	Graphics::BlendState* blendState;

	Graphics::Font* font;
	Graphics::TextRenderer* textRenderer;
};

struct Transform
{
	glm::mat4 mvp;
};

struct Material
{
	glm::vec4 diffuse;
};

void LoadScene(Scene& scene)
{
	// Create filesystem
	{
		scene.fileSystem = new Files::STDFileSystem("../../../../../../resources/");
	}

	// Create window
	{
#ifdef USE_GL
		scene.window = new Framework::Input::GLWindow(800, 600, u8"Example-8", Framework::Input::Window::Mode::Windowed);
#else
		scene.window = new Framework::Input::D3DWindow(800, 600, u8"Example-8", Framework::Input::Window::Mode::Windowed);
#endif
		scene.running = true;
		scene.window->OnClose.AddListener([&scene]() { scene.running = false; });
	}

	// Create context
	{
		Graphics::RenderDeviceSettings settings;
#ifdef USE_GL
		scene.device = new Framework::Graphics::OGL410RenderDevice();
#else
		scene.device = new Framework::Graphics::D3D11RenderDevice();
#endif
		scene.device->Init(scene.window, settings);
	}

	// Load vertex shader
	{
		char object[4096];
		size_t objectSize;

		{
			auto file = scene.fileSystem->OpenFile(Files::FileMode::Read, "/Shaders/Text/v-default.msl");
			auto size = scene.fileSystem->GetSize(file);
			auto code = new char[size + 1];
			scene.fileSystem->Read(file, code, size);
			scene.fileSystem->CloseFile(file);
			code[size] = '\0';
			objectSize = Graphics::ShaderCompiler::Run(code, object, sizeof(object));
			delete[] code;
		}

		Graphics::ShaderData shaderData(object, objectSize);

		try
		{
			scene.vertexShader = scene.device->CreateVertexShader(shaderData);
		}
		catch (Graphics::RenderDeviceError& err)
		{
			std::cout << err.what() << std::endl;
			getchar();
			exit(EXIT_FAILURE);
		}
	}

	// Load pixel shader
	{
		char object[4096];
		size_t objectSize;

		{
			auto file = scene.fileSystem->OpenFile(Files::FileMode::Read, "/Shaders/Text/p-basic.msl");
			auto size = scene.fileSystem->GetSize(file);
			auto code = new char[size + 1];
			scene.fileSystem->Read(file, code, size);
			scene.fileSystem->CloseFile(file);
			code[size] = '\0';
			objectSize = Graphics::ShaderCompiler::Run(code, object, sizeof(object));
			delete[] code;
		}

		Graphics::ShaderData shaderData(object, objectSize);

		try
		{
			scene.pixelShader = scene.device->CreatePixelShader(shaderData);
		}
		catch (Graphics::RenderDeviceError& err)
		{
			std::cout << err.what() << std::endl;
			getchar();
			exit(EXIT_FAILURE);
		}
	}

	// Create pipeline
	{
		scene.pipeline = scene.device->CreatePipeline(scene.vertexShader, scene.pixelShader);
	}

	// Create texture
	{
		scene.textureBP = scene.pixelShader->GetBindingPoint("TEXTURE");
		//scene.textureBP = nullptr;
	}

	// Create sampler
	{
		Graphics::Sampler2DDesc desc;

		desc.addressU = Graphics::TextureAdressMode::Repeat;
		desc.addressV = Graphics::TextureAdressMode::Repeat;
		desc.minFilter = Graphics::TextureFilter::Linear;
		desc.magFilter = Graphics::TextureFilter::Linear;
		desc.mipmapFilter = Graphics::TextureFilter::Linear;
		desc.border = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		desc.maxAnisotropy = scene.device->GetMaxAnisotropyLimit();

		scene.sampler = scene.device->CreateSampler2D(desc);
	}

	// Create constant buffer
	{
		scene.transformBuffer = scene.device->CreateConstantBuffer(sizeof(Transform));
		scene.transformBP = scene.vertexShader->GetBindingPoint("TRANSFORM");
	}

	// Create raster state
	{
		Graphics::RasterStateDesc desc;

		desc.rasterMode = Graphics::RasterMode::Fill;
		desc.cullEnabled = false;

		scene.rasterState = scene.device->CreateRasterState(desc);
	}

	// Create depth stencil state
	{
		Graphics::DepthStencilStateDesc desc;
		scene.depthStencilState = scene.device->CreateDepthStencilState(desc);
	}

	// Create blend state
	{
		Graphics::BlendStateDesc desc;
		desc.blendEnabled = true;
		desc.sourceFactor =	Graphics::BlendFactor::SourceAlpha;
		desc.destinationFactor = Graphics::BlendFactor::InverseSourceAlpha;
		scene.blendState = scene.device->CreateBlendState(desc);
	}

	// Create font
	{
		auto file = scene.fileSystem->OpenFile(Files::FileMode::Read, "/Fonts/Arial.ttf");
		auto size = scene.fileSystem->GetSize(file);
		auto data = new unsigned char[size];
		scene.fileSystem->Read(file, data, size);
		scene.fileSystem->CloseFile(file);
		scene.font = new Graphics::Font(scene.device, data, size, 0, 48, 1024, 1024);
		delete[] data;
	}

	// Create text renderer
	{
		scene.textRenderer = new Graphics::TextRenderer(scene.device, scene.font, scene.vertexShader);
	}
}

void CleanScene(Scene& scene)
{
	delete scene.textRenderer;
	delete scene.font;

	scene.device->DestroyBlendState(scene.blendState);
	scene.device->DestroyDepthStencilState(scene.depthStencilState);
	scene.device->DestroyRasterState(scene.rasterState);

	scene.device->DestroySampler2D(scene.sampler);

	scene.device->DestroyPipeline(scene.pipeline);
	scene.device->DestroyPixelShader(scene.pixelShader);
	scene.device->DestroyVertexShader(scene.vertexShader);

	// Destroy context, window and filesytem
	delete scene.device;
	delete scene.window;
	delete scene.fileSystem;
}

void Main(int argc, char** argv) try
{
	Scene scene;

	LoadScene(scene);

	// Create transformation matrices
	glm::mat4 model, view, proj;
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	view = glm::lookAt(glm::vec3(0.0f, 0.0f, 7.5f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	proj = glm::perspective(glm::radians(70.0f), 800.0f / 600.0f, 0.1f, 100.0f);

	// Main loop
	while (scene.running)
	{
		model = glm::rotate(model, glm::radians(0.3f * (1.0f / 60.0f)), glm::vec3(0.0f, 1.0f, 0.0f));

		// Poll events
		scene.window->PollEvents();

		// Clear screen
		scene.device->Clear(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

		// Set raster and depth stencil states
		scene.device->SetRasterState(scene.rasterState);
		scene.device->SetDepthStencilState(scene.depthStencilState);
		scene.device->SetBlendState(scene.blendState);

		// Set shader pipeline
		scene.device->SetPipeline(scene.pipeline);

		// Bind sampler
		scene.textureBP->BindSampler2D(scene.sampler);

		// Update transform and draw
		{
			auto transform = (Transform*)scene.transformBuffer->Map();
			transform->mvp = glm::ortho(0.0f, (float)scene.window->GetWidth(), 0.0f, (float)scene.window->GetHeight());
			transform->mvp = glm::translate(transform->mvp, glm::vec3(0.0f, scene.window->GetHeight() / 2, 0.0f));
			scene.transformBuffer->Unmap();
			scene.transformBP->BindConstantBuffer(scene.transformBuffer);

			scene.textRenderer->RenderU32(U"Samplé tèxt", scene.textureBP, 48.0f);
		}

		// Swap screen back and front buffers
		scene.device->SwapBuffers();
	}

	CleanScene(scene);
}
catch (Graphics::ShaderError& e)
{
	std::cout << "Shader error caught:" << std::endl;
	std::cout << e.what() << std::endl;
	getchar();
}
catch (Graphics::RenderDeviceError& e)
{
	std::cout << "Render device error caught:" << std::endl;
	std::cout << e.what() << std::endl;
	getchar();
}
catch (Graphics::TextError& e)
{
	std::cout << "Text error caught:" << std::endl;
	std::cout << e.what() << std::endl;
	getchar();
}