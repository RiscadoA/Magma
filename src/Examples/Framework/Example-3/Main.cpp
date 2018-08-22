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

	Graphics::VertexArray* vertexArray;
	Graphics::VertexBuffer* vertexBuffer;
	Graphics::VertexLayout* vertexLayout;
	Graphics::IndexBuffer* indexBuffer;

	Graphics::Texture2D* texture;
	Graphics::Sampler2D* sampler;
	Graphics::PixelBindingPoint* textureBP;

	Graphics::ConstantBuffer* transformBuffer;
	Graphics::VertexBindingPoint* transformBP;

	Graphics::RasterState* rasterState;
	Graphics::DepthStencilState* depthStencilState;
	Graphics::BlendState* blendState;
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
		scene.window = new Framework::Input::GLWindow(800, 600, u8"Example-3", Framework::Input::Window::Mode::Windowed);
#else
		scene.window = new Framework::Input::D3DWindow(800, 600, u8"Example-3", Framework::Input::Window::Mode::Windowed);
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
		char metaData[2048];
		size_t metaDataSize;

		{
			auto file = scene.fileSystem->OpenFile(Files::FileMode::Read, "/Example-3/vertex.mslmd");
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
			auto file = scene.fileSystem->OpenFile(Files::FileMode::Read, "/Example-3/vertex.mslbc");
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
			auto file = scene.fileSystem->OpenFile(Files::FileMode::Read, "/Example-3/pixel.mslmd");
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
			auto file = scene.fileSystem->OpenFile(Files::FileMode::Read, "/Example-3/pixel.mslbc");
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

	// Load vertex buffer
	{
		Vertex data[4];
		data[0].x = -0.5f; data[0].y = -0.5f; data[0].z = 0.0f; data[0].u = 0.0f; data[0].v = 0.0f;
		data[1].x = -0.5f; data[1].y = +0.5f; data[1].z = 0.0f; data[1].u = 0.0f; data[1].v = 1.0f;
		data[2].x = +0.5f; data[2].y = -0.5f; data[2].z = 0.0f; data[2].u = 1.0f; data[2].v = 0.0f;
		data[3].x = +0.5f; data[3].y = +0.5f; data[3].z = 0.0f; data[3].u = 1.0f; data[3].v = 1.0f;

		scene.vertexBuffer = scene.device->CreateVertexBuffer(sizeof(data), data, Graphics::BufferUsage::Static);
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

	// Create texture
	{
		float data[] =
		{
			1.0f, 0.0f, 0.0f, 0.5f,		0.0f, 1.0f, 0.0f, 0.5f,
			0.0f, 0.0f, 1.0f, 0.5f,		1.0f, 1.0f, 1.0f, 0.5f,
		};

		scene.texture = scene.device->CreateTexture2D(2, 2, Graphics::TextureFormat::RGBA32Float, data);
		scene.texture->GenerateMipmaps();
		scene.textureBP = scene.pixelShader->GetBindingPoint("texture");
	}

	// Create sampler
	{
		Graphics::Sampler2DDesc desc;

		desc.addressU = Graphics::TextureAdressMode::Clamp;
		desc.addressV = Graphics::TextureAdressMode::Clamp;
		desc.minFilter = Graphics::TextureFilter::Linear;
		desc.magFilter = Graphics::TextureFilter::Linear;
		desc.mipmapFilter = Graphics::TextureFilter::Nearest;
		desc.border = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

		scene.sampler = scene.device->CreateSampler2D(desc);
	}

	// Create constant buffer
	{
		scene.transformBuffer = scene.device->CreateConstantBuffer(sizeof(Transform));
		scene.transformBP = scene.vertexShader->GetBindingPoint("transform");
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
}

void CleanScene(Scene& scene)
{
	scene.device->DestroyBlendState(scene.blendState);
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
	glm::mat4 model1, model2, view, proj;
	model1 = glm::mat4(1.0f);
	model1 = glm::translate(model1, glm::vec3(0.0f, 0.0f, +0.0f));
	model2 = glm::mat4(1.0f);
	model2 = glm::translate(model2, glm::vec3(0.0f, 0.0f, -5.0f));
	view = glm::lookAt(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	proj = glm::perspective(glm::radians(70.0f), 800.0f / 600.0f, 0.1f, 100.0f);

	// Main loop
	while (scene.running)
	{
		model1 = glm::rotate(model1, glm::radians(90.0f * (1.0f / 60.0f)), glm::vec3(0.0f, 1.0f, 0.0f));

		// Poll events
		scene.window->PollEvents();

		// Clear screen
		//scene.device->SetRenderTargets(nullptr, 0);
		scene.device->Clear(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

		// Set raster and depth stencil states
		scene.device->SetRasterState(scene.rasterState);
		scene.device->SetDepthStencilState(scene.depthStencilState);
		scene.device->SetBlendState(scene.blendState);

		// Set shader pipeline
		scene.device->SetPipeline(scene.pipeline);

		// Bind texture and set sampler
		scene.textureBP->BindTexture2D(scene.texture);
		scene.textureBP->BindSampler2D(scene.sampler);

		// Bind vertex array and the index buffer
		scene.device->SetVertexArray(scene.vertexArray);
		scene.device->SetIndexBuffer(scene.indexBuffer);

		// Update transform and draw
		{
			auto transform = (Transform*)scene.transformBuffer->Map();
			transform->mvp = proj * view * model2;
			scene.transformBuffer->Unmap();
			scene.transformBP->BindConstantBuffer(scene.transformBuffer);
			scene.device->DrawTrianglesIndexed(0, 6);
		}

		// Update transform and draw
		{
			auto transform = (Transform*)scene.transformBuffer->Map();
			transform->mvp = proj * view * model1;
			scene.transformBuffer->Unmap();
			scene.transformBP->BindConstantBuffer(scene.transformBuffer);
			scene.device->DrawTrianglesIndexed(0, 6);
		}

		// Swap screen back and front buffers
		scene.device->SwapBuffers();
	}

	CleanScene(scene);

	mfTerminate();
	return 0;
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