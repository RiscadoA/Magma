﻿//#include <Magma/Framework/Graphics/D3D11Context.hpp>
//#include <Magma/Framework/Graphics/GLContext.hpp>
#include <Magma/Framework/Input/D3DWindow.hpp>
#include <Magma/Framework/Input/GLWindow.hpp>
#include <Magma/Framework/Graphics/OGL410RenderDevice.hpp>
#include <Magma/Framework/Graphics/OGL410Assembler.hpp>
#include <Magma/Framework/Files/STDFileSystem.hpp>
#include <Magma/Framework/String/UTF8.hpp>
#include <Magma/Framework/String/Conversion.hpp>
#include <iostream>

#include <glm/gtc/matrix_transform.hpp>

#include <Magma/Framework/Graphics/ShaderData.hpp>
#include <Magma/Framework/Graphics/BytecodeAssembler.hpp>

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

	Graphics::VertexArray* vertexArray;
	Graphics::VertexBuffer* vertexBuffer;
	Graphics::VertexLayout* vertexLayout;
	Graphics::IndexBuffer* indexBuffer;

	Graphics::Texture2D* texture;
	Graphics::Sampler2D* sampler;
	Graphics::PixelBindingPoint* textureBP;

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
		scene.fileSystem = new Files::STDFileSystem("../../../../../resources/");
	}

	// Create window
	{
		scene.window = new Input::GLWindow(800, 600, "Tetris", Input::Window::Mode::Windowed);
		scene.running = true;
		scene.window->OnClose.AddListener([&scene]() { scene.running = false; });
	}

	// Create context
	{
		Graphics::RenderDeviceSettings settings;
		scene.device = new Graphics::OGL410RenderDevice();
		scene.device->Init(scene.window, settings);
	}

	// Load vertex shader
	{
		char metaData[] =
		{
			0x00, 0x00, 0x00, 0x00, // Major version 0
			0x00, 0x00, 0x00, 0x01, // Minor version 1
			0x00, 0x00, 0x00, 0x00, // Vertex shader

			0x00, 0x00, 0x00, 0x02, // 2 input var
			0x00, 0x00, 0x00, 0x01, // Var index 1
			0x00, 0x00, 0x00, 0x08, // Var name size is 8
			'p', 'o', 's', 'i', 't', 'i', 'o', 'n',
			0x00, 0x00, 0x00, 0x09, // Var type is float3 (0x9)
			0x00, 0x00, 0x00, 0x02, // Var index 2
			0x00, 0x00, 0x00, 0x03, // Var name size is 3
			'u', 'v', 's',
			0x00, 0x00, 0x00, 0x08, // Var type is float2 (0x8)

			0x00, 0x00, 0x00, 0x01, // 1 output var
			0x00, 0x00, 0x00, 0x03, // Var index 3
			0x00, 0x00, 0x00, 0x03, // Var name size is 3
			'u', 'v', 's',
			0x00, 0x00, 0x00, 0x08, // Var type is float2 (0x8)

			0x00, 0x00, 0x00, 0x00, // 0 2D texture var

			0x00, 0x00, 0x00, 0x00,	// 0 constant buffers

			0x00, 0x00, 0x00, 0x00, // 0 constant buffer vars
		};

		auto file = scene.fileSystem->OpenFile(Files::FileMode::Read, "/vertex2.mslbc");
		auto size = scene.fileSystem->GetSize(file);
		auto code = new char[size + 1];
		scene.fileSystem->Read(file, code, size);
		scene.fileSystem->CloseFile(file);
		code[size] = '\0';

		char bytecode[2048];
		size_t bytecodeSize = Graphics::BytecodeAssembler::Assemble(code, bytecode, sizeof(bytecode));

		Graphics::ShaderData shaderData(bytecode, bytecodeSize, metaData, sizeof(metaData));

		file = scene.fileSystem->OpenFile(Files::FileMode::Write, "/vertex2.bc");
		scene.fileSystem->Write(file, bytecode, bytecodeSize);
		scene.fileSystem->CloseFile(file);
		
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
		char metaData[] =
		{
			0x00, 0x00, 0x00, 0x00, // Major version 0
			0x00, 0x00, 0x00, 0x01, // Minor version 1
			0x00, 0x00, 0x00, 0x01, // Pixel shader

			0x00, 0x00, 0x00, 0x01, // 1 input var
			0x00, 0x00, 0x00, 0x03, // Var index 3
			0x00, 0x00, 0x00, 0x03, // Var name size is 3
			'u', 'v', 's',
			0x00, 0x00, 0x00, 0x08, // Var type is float2 (0x8)

			0x00, 0x00, 0x00, 0x01, // 1 output var
			0x00, 0x00, 0x00, 0x00, // Var index 0
			0x00, 0x00, 0x00, 0x05, // Var name size is 5
			'c', 'o', 'l', 'o', 'r',
			0x00, 0x00, 0x00, 0x0A, // Var type is float4 (0xA)

			0x00, 0x00, 0x00, 0x01, // 1 2D texture var
			0x00, 0x00, 0x00, 0x01, // Var index 1
			0x00, 0x00, 0x00, 0x07, // Var name size is 7
			't', 'e', 'x', 't', 'u', 'r', 'e',

			0x00, 0x00, 0x00, 0x00,	// 0 constant buffers

			0x00, 0x00, 0x00, 0x00, // 0 constant buffer vars
		};

		auto file = scene.fileSystem->OpenFile(Files::FileMode::Read, "/pixel2.mslbc");
		auto size = scene.fileSystem->GetSize(file);
		auto code = new char[size + 1];
		scene.fileSystem->Read(file, code, size);
		scene.fileSystem->CloseFile(file);
		code[size] = '\0';

		char bytecode[2048];
		size_t bytecodeSize = Graphics::BytecodeAssembler::Assemble(code, bytecode, sizeof(bytecode));

		Graphics::ShaderData shaderData(bytecode, bytecodeSize, metaData, sizeof(metaData));

		file = scene.fileSystem->OpenFile(Files::FileMode::Write, "/pixel2.bc");
		scene.fileSystem->Write(file, bytecode, bytecodeSize);
		scene.fileSystem->CloseFile(file);

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
		scene.vertexBuffer = scene.device->CreateVertexBuffer(sizeof(Vertex) * 4, nullptr, Graphics::BufferUsage::Dynamic);
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

		elements[1].bufferIndex = 0;
		elements[1].name = "uvs";
		elements[1].offset = offsetof(Vertex, u);
		elements[1].size = 2;
		elements[1].stride = sizeof(Vertex);
		elements[1].type = Graphics::VertexElementType::Float;

		scene.vertexLayout = scene.device->CreateVertexLayout(2, elements, scene.vertexShader);
	}

	// Create vertex array
	{
		Graphics::VertexBuffer* buffers[] =
		{
			scene.vertexBuffer,
		};
		scene.vertexArray = scene.device->CreateVertexArray(1, buffers, scene.vertexLayout);
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

	{
		float data[] =
		{
			1.0f, 0.0f, 0.0f, 1.0f,		0.0f, 1.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 1.0f, 1.0f,		1.0f, 1.0f, 1.0f, 1.0f,
		};

		scene.texture = scene.device->CreateTexture2D(2, 2, Graphics::TextureFormat::RGBA32Float, data);
		scene.texture->GenerateMipmaps();
		scene.textureBP = scene.pixelShader->GetBindingPoint("texture");
	}

	// Create sampler
	{
		Graphics::Sampler2DDesc desc;

		desc.adressU = Graphics::TextureAdressMode::Border;
		desc.adressV = Graphics::TextureAdressMode::Border;
		desc.minFilter = Graphics::TextureFilter::Linear;
		desc.magFilter = Graphics::TextureFilter::Linear;
		desc.mipmapFilter = Graphics::TextureFilter::Nearest;
		desc.border = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);

		scene.sampler = scene.device->CreateSampler2D(desc);
	}

	// Create mask texture
	/*{
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

	// Destroy font
	delete scene.font;*/

	scene.device->DestroyDepthStencilState(scene.depthStencilState);
	scene.device->DestroyRasterState(scene.rasterState);

	scene.device->DestroySampler2D(scene.sampler);
	scene.device->DestroyTexture2D(scene.texture);

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
			data[0].x = -0.5f + x; data[0].y = -0.5f; data[0].z = 0.0f; data[0].u = 0.0f; data[0].v = 0.0f;
			data[1].x = -0.5f + x; data[1].y = +0.5f; data[1].z = 0.0f; data[1].u = 0.0f; data[1].v = 1.0f;
			data[2].x = +0.5f + x; data[2].y = -0.5f; data[2].z = 0.0f; data[2].u = 1.0f; data[2].v = 0.0f;
			data[3].x = +0.5f + x; data[3].y = +0.5f; data[3].z = 0.0f; data[3].u = 1.0f; data[3].v = 1.0f;
			scene.vertexBuffer->Unmap();
		}

		// Set shader pipeline
		scene.device->SetPipeline(scene.pipeline);

		// Bind texture and set sampler
		scene.textureBP->Bind(scene.texture);
		scene.textureBP->Bind(scene.sampler);

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