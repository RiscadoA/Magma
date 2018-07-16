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
#include <Magma/Framework/Graphics/BytecodeAssembler.hpp>
#include <Magma/Framework/Graphics/MetaDataAssembler.hpp>

using namespace Magma;
using namespace Magma::Framework;

struct Vertex
{
	float x, y, z;
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

	Graphics::VertexArray* vertexArray;
	Graphics::VertexBuffer* vertexBuffer;
	Graphics::VertexBuffer* vertexBuffer2;
	Graphics::VertexLayout* vertexLayout;
	Graphics::IndexBuffer* indexBuffer;

	Graphics::RasterState* rasterState;
	Graphics::DepthStencilState* depthStencilState;
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
		scene.fileSystem = new Framework::Files::STDFileSystem("../../../../../resources/");
	}

	// Create window
	{
		scene.window = new Framework::Input::D3DWindow(800, 600, "Tetris", Framework::Input::Window::Mode::Windowed);
		scene.running = true;
		scene.window->OnClose.AddListener([&scene]() { scene.running = false; });
	}

	// Create context
	{
		
		Graphics::RenderDeviceSettings settings;
		scene.device = new Framework::Graphics::D3D11RenderDevice();
		scene.device->Init(scene.window, settings);
	}

	// Load vertex shader
	{
		char metaData[2048];
		size_t metaDataSize;

		{
			auto file = scene.fileSystem->OpenFile(Files::FileMode::Read, "/Example-1/vertex.mslmd");
			auto size = scene.fileSystem->GetSize(file);
			auto code = new char[size + 1];
			scene.fileSystem->Read(file, code, size);
			scene.fileSystem->CloseFile(file);
			code[size] = '\0';

			metaDataSize = Graphics::MetaDataAssembler::Assemble(code, metaData, sizeof(metaData));
		}

		char bytecode[2048];
		size_t bytecodeSize;

		{
			auto file = scene.fileSystem->OpenFile(Files::FileMode::Read, "/Example-1/vertex.mslbc");
			auto size = scene.fileSystem->GetSize(file);
			auto code = new char[size + 1];
			scene.fileSystem->Read(file, code, size);
			scene.fileSystem->CloseFile(file);
			code[size] = '\0';

			bytecodeSize = Graphics::BytecodeAssembler::Assemble(code, bytecode, sizeof(bytecode));
		}

		Graphics::ShaderData shaderData(bytecode, bytecodeSize, metaData, metaDataSize);

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
		char metaData[2048];
		size_t metaDataSize;

		{
			auto file = scene.fileSystem->OpenFile(Files::FileMode::Read, "/Example-1/pixel.mslmd");
			auto size = scene.fileSystem->GetSize(file);
			auto code = new char[size + 1];
			scene.fileSystem->Read(file, code, size);
			scene.fileSystem->CloseFile(file);
			code[size] = '\0';

			metaDataSize = Graphics::MetaDataAssembler::Assemble(code, metaData, sizeof(metaData));
		}

		char bytecode[2048];
		size_t bytecodeSize;

		{
			auto file = scene.fileSystem->OpenFile(Files::FileMode::Read, "/Example-1/pixel.mslbc");
			auto size = scene.fileSystem->GetSize(file);
			auto code = new char[size + 1];
			scene.fileSystem->Read(file, code, size);
			scene.fileSystem->CloseFile(file);
			code[size] = '\0';

			bytecodeSize = Graphics::BytecodeAssembler::Assemble(code, bytecode, sizeof(bytecode));
		}

		Graphics::ShaderData shaderData(bytecode, bytecodeSize, metaData, metaDataSize);

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

	// Load font
	/*{
		auto file = scene.fileSystem->OpenFile(Files::FileMode::Read, "/Consolas.ttf");
		auto size = scene.fileSystem->GetSize(file);
		auto data = new unsigned char[size];
		scene.fileSystem->Read(file, data, size);
		scene.fileSystem->CloseFile(file);
		scene.font = new Graphics::Font(*scene.context, data, size, 0, 60, 1024, 1024);
	}*/

	// Load vertex buffer
	{
		scene.vertexBuffer = scene.device->CreateVertexBuffer(48, nullptr, Graphics::BufferUsage::Dynamic);
	}

	// Load vertex buffer 2
	{
		glm::vec4 data[] =
		{
			glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),
			glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),
			glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),
			glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
		};

		scene.vertexBuffer2 = scene.device->CreateVertexBuffer(sizeof(data), data);
	}

	// Create vertex layout
	{
		Graphics::VertexElement elements[2];

		elements[0].bufferIndex = 0;
		elements[0].name = "position";
		elements[0].offset = offsetof(Vertex, x);
		elements[0].size = 3;
		elements[0].stride = sizeof(Vertex);
		elements[0].type = Graphics::VertexElementType::Float;

		elements[1].bufferIndex = 1;
		elements[1].name = "color";
		elements[1].offset = 0;
		elements[1].size = 4;
		elements[1].stride = sizeof(glm::vec4);
		elements[1].type = Graphics::VertexElementType::Float;

		scene.vertexLayout = scene.device->CreateVertexLayout(2, elements, scene.vertexShader);
	}

	// Create vertex array
	{
		Graphics::VertexBuffer* buffers[] =
		{
			scene.vertexBuffer,
			scene.vertexBuffer2,
		};
		scene.vertexArray = scene.device->CreateVertexArray(2, buffers, scene.vertexLayout);
	}

	// Create index buffer
	{
		unsigned int data[] =
		{
			2, 1, 0,
			2, 3, 1,
		};

		scene.indexBuffer = scene.device->CreateIndexBuffer(Graphics::IndexType::UInt, sizeof(data), data);
	}

	// Create raster state
	{
		Graphics::RasterStateDesc desc;

		//desc.rasterMode = Graphics::RasterMode::Line;

		scene.rasterState = scene.device->CreateRasterState(desc);
	}

	// Create depth stencil state
	{
		Graphics::DepthStencilStateDesc desc;
		
		scene.depthStencilState = scene.device->CreateDepthStencilState(desc);
	}
}

void CleanScene(Scene& scene)
{
	scene.device->DestroyDepthStencilState(scene.depthStencilState);
	scene.device->DestroyRasterState(scene.rasterState);

	scene.device->DestroyIndexBuffer(scene.indexBuffer);
	scene.device->DestroyVertexArray(scene.vertexArray);
	scene.device->DestroyVertexLayout(scene.vertexLayout);
	scene.device->DestroyVertexBuffer(scene.vertexBuffer);

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
	model = glm::translate(model, glm::vec3(-10.0f, 0.0f, 0.0f));
	view = glm::lookAt(glm::vec3(0.0f, 0.0f, 20.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	proj = glm::perspective(glm::radians(70.0f), 800.0f / 600.0f, 0.1f, 100.0f);
	Transform transform;
	
	float x = 0.0f;

	// Main loop
	while (scene.running)
	{
		model = glm::rotate(model, glm::radians(1.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		// Poll events
		scene.window->PollEvents();

		// Update transform
		transform.mvp = proj * view * model;
		//scene.device->UpdateConstantBuffer(scene.transformCBuffer, &transform, 0, sizeof(transform));

		// Clear screen
		//scene.device->SetRenderTargets(nullptr, 0);
		scene.device->Clear(glm::vec4(0.0f, 0.2f, 0.4f, 1.0f));

		// Set raster and depth stencil states
		scene.device->SetRasterState(scene.rasterState);
		scene.device->SetDepthStencilState(scene.depthStencilState);

		x += 0.00001f;

		// Update vertex buffer
		{
			auto data = (Vertex*)scene.vertexBuffer->Map();
			data[0].x = -0.5f + x; data[0].y = -0.5f; data[0].z = 0.0f;
			data[1].x = -0.5f + x; data[1].y = +0.5f; data[1].z = 0.0f;
			data[2].x = +0.5f + x; data[2].y = -0.5f; data[2].z = 0.0f;
			data[3].x = +0.5f + x; data[3].y = +0.5f; data[3].z = 0.0f;
			scene.vertexBuffer->Unmap();
		}

		// Set shader pipeline
		scene.device->SetPipeline(scene.pipeline);

		// Bind texture and sampler
		/*scene.context->BindTexture2D(scene.font->GetAtlas(0), scene.textureBP);
		scene.context->BindSampler(scene.sampler, scene.textureBP);*/

		// Bind transform and material
		/*scene.context->BindConstantBuffer(scene.transformCBuffer, scene.transformBP);
		scene.context->BindConstantBuffer(scene.materialCBuffer, scene.materialBP);*/

		// Bind vertex array and the index buffer and draw
		scene.device->SetVertexArray(scene.vertexArray);
		scene.device->SetIndexBuffer(scene.indexBuffer);
		scene.device->DrawTrianglesIndexed(0, 6);
		
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