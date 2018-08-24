#include <Magma/Framework/Input/Old/OGLWindow.hpp>
#include <Magma/Framework/Graphics/1.X/OGL410RenderDevice.hpp>
#include <Magma/Framework/Graphics/1.X/OGL410Assembler.hpp>

#include <Magma/Framework/Input/Old/D3DWindow.hpp>
#include <Magma/Framework/Graphics/1.X/D3D11RenderDevice.hpp>
#include <Magma/Framework/Graphics/1.X/D3D11Assembler.hpp>

#include <Magma/Framework/Files/STDFileSystem.hpp>
#include <Magma/Framework/String/Conversion.hpp>
#include <iostream>

#include <glm/gtc/matrix_transform.hpp>

#include <Magma/Framework/Graphics/1.X/ShaderData.hpp>
#include <Magma/Framework/Graphics/1.X/BytecodeAssembler.hpp>
#include <Magma/Framework/Graphics/1.X/MetaDataAssembler.hpp>

#include <Magma/Framework/Entry.hpp>

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
	Graphics_V1X::RenderDevice* device;
	Files::FileSystem* fileSystem;
	bool running;

	Graphics_V1X::VertexShader* vertexShader;
	Graphics_V1X::PixelShader* pixelShader;
	Graphics_V1X::Pipeline* pipeline;

	Graphics_V1X::VertexArray* vertexArray;
	Graphics_V1X::VertexBuffer* vertexBuffer;
	Graphics_V1X::VertexLayout* vertexLayout;
	Graphics_V1X::IndexBuffer* indexBuffer;

	Graphics_V1X::Texture2D* texture;
	Graphics_V1X::Sampler2D* sampler;
	Graphics_V1X::PixelBindingPoint* textureBP;

	Graphics_V1X::RasterState* rasterState;
	Graphics_V1X::DepthStencilState* depthStencilState;
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
		scene.window = new Framework::Input::OGLWindow(800, 600, u8"Example-2", Framework::Input::Window::Mode::Windowed);
#else
		scene.window = new Framework::Input::D3DWindow(800, 600, u8"Example-2", Framework::Input::Window::Mode::Windowed);
#endif
		scene.running = true;
		scene.window->OnClose.AddListener([&scene]() { scene.running = false; });
	}

	// Create context
	{
		Graphics_V1X::RenderDeviceSettings settings;
#ifdef USE_GL
		scene.device = new Framework::Graphics_V1X::OGL410RenderDevice();
#else
		scene.device = new Framework::Graphics_V1X::D3D11RenderDevice();
#endif
		scene.device->Init(scene.window, settings);
	}

	// Load vertex shader
	{
		char metaData[2048];
		size_t metaDataSize;

		{
			auto file = scene.fileSystem->OpenFile(Files::FileMode::Read, "/Example-2/vertex.mslmd");
			auto size = scene.fileSystem->GetSize(file);
			auto code = new char[size + 1];
			scene.fileSystem->Read(file, code, size);
			scene.fileSystem->CloseFile(file);
			code[size] = '\0';

			metaDataSize = Graphics_V1X::MetaDataAssembler::Assemble(code, metaData, sizeof(metaData));
		}

		char bytecode[2048];
		size_t bytecodeSize;

		{
			auto file = scene.fileSystem->OpenFile(Files::FileMode::Read, "/Example-2/vertex.mslbc");
			auto size = scene.fileSystem->GetSize(file);
			auto code = new char[size + 1];
			scene.fileSystem->Read(file, code, size);
			scene.fileSystem->CloseFile(file);
			code[size] = '\0';

			bytecodeSize = Graphics_V1X::BytecodeAssembler::Assemble(code, bytecode, sizeof(bytecode));
		}

		Graphics_V1X::ShaderData shaderData(bytecode, bytecodeSize, metaData, metaDataSize);

		try
		{
			scene.vertexShader = scene.device->CreateVertexShader(shaderData);
		}
		catch (Graphics_V1X::RenderDeviceError& err)
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
			auto file = scene.fileSystem->OpenFile(Files::FileMode::Read, "/Example-2/pixel.mslmd");
			auto size = scene.fileSystem->GetSize(file);
			auto code = new char[size + 1];
			scene.fileSystem->Read(file, code, size);
			scene.fileSystem->CloseFile(file);
			code[size] = '\0';

			metaDataSize = Graphics_V1X::MetaDataAssembler::Assemble(code, metaData, sizeof(metaData));
		}

		char bytecode[2048];
		size_t bytecodeSize;

		{
			auto file = scene.fileSystem->OpenFile(Files::FileMode::Read, "/Example-2/pixel.mslbc");
			auto size = scene.fileSystem->GetSize(file);
			auto code = new char[size + 1];
			scene.fileSystem->Read(file, code, size);
			scene.fileSystem->CloseFile(file);
			code[size] = '\0';

			bytecodeSize = Graphics_V1X::BytecodeAssembler::Assemble(code, bytecode, sizeof(bytecode));
		}

		Graphics_V1X::ShaderData shaderData(bytecode, bytecodeSize, metaData, metaDataSize);

		try
		{
			scene.pixelShader = scene.device->CreatePixelShader(shaderData);
		}
		catch (Graphics_V1X::RenderDeviceError& err)
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

	// Load vertex buffer
	{
		scene.vertexBuffer = scene.device->CreateVertexBuffer(sizeof(Vertex) * 4, nullptr, Graphics_V1X::BufferUsage::Dynamic);
	}

	// Create vertex layout
	{
		Graphics_V1X::VertexElement elements[2];

		elements[0].bufferIndex = 0;
		elements[0].name = "position";
		elements[0].offset = offsetof(Vertex, x);
		elements[0].size = 3;
		elements[0].stride = sizeof(Vertex);
		elements[0].type = Graphics_V1X::VertexElementType::Float;

		elements[1].bufferIndex = 0;
		elements[1].name = "uvs";
		elements[1].offset = offsetof(Vertex, u);
		elements[1].size = 2;
		elements[1].stride = sizeof(Vertex);
		elements[1].type = Graphics_V1X::VertexElementType::Float;

		scene.vertexLayout = scene.device->CreateVertexLayout(2, elements, scene.vertexShader);
	}

	// Create vertex array
	{
		Graphics_V1X::VertexBuffer* buffers[] =
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

		scene.indexBuffer = scene.device->CreateIndexBuffer(Graphics_V1X::IndexType::UInt, sizeof(data), data);
	}

	// Create raster state
	{
		Graphics_V1X::RasterStateDesc desc;

		//desc.rasterMode = Graphics_V1X::RasterMode::Line;

		scene.rasterState = scene.device->CreateRasterState(desc);
	}

	// Create depth stencil state
	{
		Graphics_V1X::DepthStencilStateDesc desc;
		
		scene.depthStencilState = scene.device->CreateDepthStencilState(desc);
	}

	// Create texture
	{
		float data[] =
		{
			1.0f, 0.0f, 0.0f, 1.0f,		0.0f, 1.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 1.0f, 1.0f,		1.0f, 1.0f, 1.0f, 1.0f,
		};

		scene.texture = scene.device->CreateTexture2D(2, 2, Graphics_V1X::TextureFormat::RGBA32Float, data);
		scene.texture->GenerateMipmaps();
		scene.textureBP = scene.pixelShader->GetBindingPoint("texture");
	}

	// Create sampler
	{
		Graphics_V1X::Sampler2DDesc desc;

		desc.addressU = Graphics_V1X::TextureAdressMode::Clamp;
		desc.addressV = Graphics_V1X::TextureAdressMode::Clamp;
		desc.minFilter = Graphics_V1X::TextureFilter::Linear;
		desc.magFilter = Graphics_V1X::TextureFilter::Linear;
		desc.mipmapFilter = Graphics_V1X::TextureFilter::Linear;
		desc.maxAnisotropy = 1;
		desc.border = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

		scene.sampler = scene.device->CreateSampler2D(desc);
	}
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

int main(int argc, const char** argv) try
{
	if (mfInit(argc, argv) != MF_ERROR_OKAY)
		abort();

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
		scene.textureBP->BindTexture2D(scene.texture);
		scene.textureBP->BindSampler2D(scene.sampler);

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

	mfTerminate();
	return 0;
}
catch (Graphics_V1X::ShaderError& e)
{
	std::cout << "Shader error caught:" << std::endl;
	std::cout << e.what() << std::endl;
	getchar();
}
catch (Graphics_V1X::RenderDeviceError& e)
{
	std::cout << "Render device error caught:" << std::endl;
	std::cout << e.what() << std::endl;
	getchar();
}