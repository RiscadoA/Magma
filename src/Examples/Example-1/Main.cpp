//#include <Magma/Framework/Graphics/D3D11Context.hpp>
//#include <Magma/Framework/Graphics/GLContext.hpp>
#include <Magma/Framework/Input/D3DWindow.hpp>
#include <Magma/Framework/Input/GLWindow.hpp>
#include <Magma/Framework/Graphics/OGLRenderDevice.hpp>
#include <Magma/Framework/Files/STDFileSystem.hpp>
#include <Magma/Framework/String/UTF8.hpp>
#include <Magma/Framework/String/Conversion.hpp>
#include <iostream>

#include <glm/gtc/matrix_transform.hpp>

using namespace Magma;
using namespace Magma::Framework;

struct Scene
{
	Input::Window* window;
	Graphics::RenderDevice* device;
	Files::FileSystem* fileSystem;
	bool running;

	Graphics::VertexShader* vertexShader;
	Graphics::PixelShader* pixelShader;
	Graphics::Pipeline* pipeline;

	Graphics::VertexArray* vertexArray;
	Graphics::VertexBuffer* vertexBuffer;
	Graphics::VertexLayout* vertexLayout;
	Graphics::IndexBuffer* indexBuffer;
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
		scene.fileSystem = new Framework::Files::STDFileSystem("../../../../resources/");
	}

	// Create window
	{
		scene.window = new Framework::Input::D3DWindow(800, 600, "Tetris", Framework::Input::Window::Mode::Windowed);
		scene.running = true;
		scene.window->OnClose.AddListener([&scene]() { scene.running = false; });
	}

	// Create context
	/*{
		Graphics::ContextSettings contextSettings;
		contextSettings.multisampleCount = 4;
		contextSettings.enableVsync = true;
		scene.context = new Framework::Graphics::D3D11Context();
		scene.context->Init(scene.window, contextSettings);
	}*/

	// Load pipeline and shaders
	{
		auto file = scene.fileSystem->OpenFile(Files::FileMode::Read, "/Mask.msl");
		auto size = scene.fileSystem->GetSize(file);
		auto data = new char[size + 1];
		scene.fileSystem->Read(file, data, size);
		scene.fileSystem->CloseFile(file);
		data[size] = '\0';

		scene.vertexShader = scene.device->CreateVertexShader(data);
		scene.pixelShader = scene.device->CreatePixelShader(data);
		scene.pipeline = scene.device->CreatePipeline(scene.vertexShader, scene.pixelShader);
	}

	// Load font
	/*{
		auto file = scene.fileSystem->OpenFile(Files::FileMode::Read, "/Consolas.ttf");
		auto size = scene.fileSystem->GetSize(file);
		auto data = new unsigned char[size];
		scene.fileSystem->Read(file, data, size);
		scene.fileSystem->CloseFile(file);
		scene.font = new Graphics::Font(*scene.context, data, size, 0, 60, 1024, 1024);
	}*/

	struct Vertex
	{
		float x, y, z;
	};

	// Load vertex buffer
	{
		Vertex data[] =
		{
			{ -0.5f, -0.5f, 0.0f, },
			{ -0.5f, +0.5f, 0.0f, },
			{ +0.5f, -0.5f, 0.0f, },
			{ +0.5f, +0.5f, 0.0f, },
		};

		scene.vertexBuffer = scene.device->CreateVertexBuffer(sizeof(data), data);
	}

	// Create vertex layout
	{
		Graphics::VertexElement elements[1];

		elements[0].bufferIndex = 0;
		elements[0].name = "position";
		elements[0].offset = offsetof(Vertex, x);
		elements[0].size = 3;
		elements[0].stride = sizeof(Vertex);
		elements[0].type = Graphics::VertexElementType::Float;

		scene.vertexLayout = scene.device->CreateVertexLayout(1, elements, scene.vertexShader);
	}

	// Create vertex array
	{
		scene.vertexArray = scene.device->CreateVertexArray(1, &scene.vertexBuffer, scene.vertexLayout);
	}

	// Create index buffer
	{
		unsigned int data[] =
		{
			0, 1, 2,
			1, 3, 2,
		};

		scene.indexBuffer = scene.device->CreateIndexBuffer(Graphics::IndexType::UInt, sizeof(data), data);
	}

	// Create sampler
	/*{
		Framework::Graphics::SamplerSettings settings;

		settings.adressU = Framework::Graphics::TextureAdressMode::Border;
		settings.adressV = Framework::Graphics::TextureAdressMode::Border;
		settings.minFilter = Framework::Graphics::TextureFilter::Linear;
		settings.magFilter = Framework::Graphics::TextureFilter::Linear;
		settings.borderColor = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);

		scene.sampler = scene.context->CreateSampler(settings);
	}

	// Create mask texture
	{
		unsigned char data[] =
		{
			0,
			255,
			255,
			255,
		};

		scene.texture = scene.context->CreateTexture2D(data, 2, 2, Framework::Graphics::TextureFormat::R8UInt);
	}

	// Get mask texture binding point
	{
		scene.textureBP = scene.context->GetTextureBindingPoint(scene.program, "fontTexture");
	}

	// Create transform constant buffer and get its binding point
	{
		scene.transformCBuffer = scene.context->CreateConstantBuffer(nullptr, sizeof(Transform));
		scene.transformBP = scene.context->GetConstantBindingPoint(scene.program, "transform");
	}

	// Create material constant buffer and get its binding point
	{
		Material mat;
		mat.diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

		scene.materialCBuffer = scene.context->CreateConstantBuffer(&mat, sizeof(Material));
		scene.materialBP = scene.context->GetConstantBindingPoint(scene.program, "material");
	}

	{
		scene.context->CreateVertexLayout(desc, program);
		scene.context->CreateVertexBuffer(nullptr, 1, Graphics::Usage::Default);
		scene.context->BindVertexBuffer(buffer, layout);
	}*/
}

void CleanScene(Scene& scene)
{
	// Destroy constant buffers
	/*scene.context->DestroyConstantBuffer(scene.materialCBuffer);
	scene.context->DestroyConstantBuffer(scene.transformCBuffer);

	// Destroy texture and sampler
	scene.context->DestroyTexture2D(scene.texture);
	scene.context->DestroySampler(scene.sampler);

	// Destroy index and vertex buffers
	scene.context->DestroyIndexBuffer(scene.indexBuffer);
	scene.context->DestroyVertexBuffer(scene.vertexBuffer);

	// Destroy font
	delete scene.font;

	// Destroy program and shaders
	scene.context->DestroyProgram(scene.program);
	scene.context->DestroyShader(scene.vertexShader);
	scene.context->DestroyShader(scene.pixelShader);*/

	// Destroy context, window and filesytem
	delete scene.context;
	delete scene.window;
	delete scene.fileSystem;
}

void Main(int argc, char** argv) try
{
	/*Scene scene;

	LoadScene(scene);

	// Create transformation matrices
	glm::mat4 model, view, proj;
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-10.0f, 0.0f, 0.0f));
	view = glm::lookAt(glm::vec3(0.0f, 0.0f, 20.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	proj = glm::perspective(glm::radians(70.0f), 800.0f / 600.0f, 0.1f, 100.0f);
	Transform transform;*

	// Main loop
	while (scene.running)
	{
		model = glm::rotate(model, glm::radians(1.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		// Poll events
		scene.window->PollEvents();

		// Update transform
		transform.mvp = proj * view * model;
		scene.context->UpdateConstantBuffer(scene.transformCBuffer, &transform, 0, sizeof(transform));

		// Clear screen
		scene.context->SetRenderTarget(nullptr);
		scene.context->SetClearColor(glm::vec4(0.0f, 0.2f, 0.4f, 1.0f));
		scene.context->Clear(Framework::Graphics::BufferBit::Color | Framework::Graphics::BufferBit::Depth);

		// Bind shader program
		scene.context->BindProgram(scene.program);			

		// Bind texture and sampler
		scene.context->BindTexture2D(scene.font->GetAtlas(0), scene.textureBP);
		scene.context->BindSampler(scene.sampler, scene.textureBP);

		// Bind transform and material
		scene.context->BindConstantBuffer(scene.transformCBuffer, scene.transformBP);
		scene.context->BindConstantBuffer(scene.materialCBuffer, scene.materialBP);

		// Bind vertex and index buffers and draw
		scene.context->BindVertexBuffer(scene.vertexBuffer);
		scene.context->BindIndexBuffer(scene.indexBuffer);
		scene.context->DrawIndexed(6, 0, Framework::Graphics::DrawMode::Triangles);
		Graphics::RenderU32Text(*scene.context, scene.program, *scene.font, 0.04f, U"Sample text\nMulti-line text\nÁâÂãõóàçÇ,.!?");
		
		// Swap screen back and front buffers
		scene.context->SwapBuffers();
	}

	CleanScene(scene);*/
}
catch (std::exception& e)
{
	std::cout << e.what() << std::endl;
	getchar();
}