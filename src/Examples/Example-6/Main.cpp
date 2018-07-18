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

	Graphics::VertexShader* screenVertexShader;
	Graphics::PixelShader* screenDifPixelShader;
	Graphics::Pipeline* screenDifPipeline;
	Graphics::PixelShader* screenUVsPixelShader;
	Graphics::Pipeline* screenUVsPipeline;

	Graphics::VertexArray* screenVertexArray;
	Graphics::VertexBuffer* screenVertexBuffer;
	Graphics::VertexLayout* screenVertexLayout;

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

	Graphics::PixelBindingPoint* diffuseBP;
	Graphics::PixelBindingPoint* uvsBP;

	Graphics::Texture2D* diffuseAttachment;
	Graphics::Texture2D* uvsAttachment;
	Graphics::DepthStencilBuffer* depthStencilBuffer;
	Graphics::Sampler2D* framebufferSampler;
	Graphics::Framebuffer* framebuffer;

	Graphics::ConstantBuffer* screenConstantBuffer;
	Graphics::VertexBindingPoint* screenConstantBufferBP;
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
#ifdef USE_GL
		scene.window = new Framework::Input::GLWindow(800, 600, "Example-6", Framework::Input::Window::Mode::Windowed);
#else
		scene.window = new Framework::Input::D3DWindow(800, 600, "Example-6", Framework::Input::Window::Mode::Windowed);
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
			auto file = scene.fileSystem->OpenFile(Files::FileMode::Read, "/Example-6/vertex.mslmd");
			auto size = scene.fileSystem->GetSize(file);
			auto code = new char[size + 1];
			scene.fileSystem->Read(file, code, size);
			scene.fileSystem->CloseFile(file);
			code[size] = '\0';

			metaDataSize = Graphics::MetaDataAssembler::Assemble(code, metaData, sizeof(metaData));
			delete[] code;
		}

		char bytecode[2048];
		size_t bytecodeSize;

		{
			auto file = scene.fileSystem->OpenFile(Files::FileMode::Read, "/Example-6/vertex.mslbc");
			auto size = scene.fileSystem->GetSize(file);
			auto code = new char[size + 1];
			scene.fileSystem->Read(file, code, size);
			scene.fileSystem->CloseFile(file);
			code[size] = '\0';

			bytecodeSize = Graphics::BytecodeAssembler::Assemble(code, bytecode, sizeof(bytecode));
			delete[] code;
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
			auto file = scene.fileSystem->OpenFile(Files::FileMode::Read, "/Example-6/pixel.mslmd");
			auto size = scene.fileSystem->GetSize(file);
			auto code = new char[size + 1];
			scene.fileSystem->Read(file, code, size);
			scene.fileSystem->CloseFile(file);
			code[size] = '\0';

			metaDataSize = Graphics::MetaDataAssembler::Assemble(code, metaData, sizeof(metaData));
			delete[] code;
		}

		char bytecode[2048];
		size_t bytecodeSize;

		{
			auto file = scene.fileSystem->OpenFile(Files::FileMode::Read, "/Example-6/pixel.mslbc");
			auto size = scene.fileSystem->GetSize(file);
			auto code = new char[size + 1];
			scene.fileSystem->Read(file, code, size);
			scene.fileSystem->CloseFile(file);
			code[size] = '\0';

			bytecodeSize = Graphics::BytecodeAssembler::Assemble(code, bytecode, sizeof(bytecode));
			delete[] code;
		}

		Graphics::ShaderData shaderData(bytecode, bytecodeSize, metaData, metaDataSize);

		try
		{
			scene.pixelShader = scene.device->CreatePixelShader(shaderData);
		}
		catch (Graphics::RenderDeviceError& err)
		{
			std::cout << err.what() << std::endl;
			exit(EXIT_FAILURE);
		}
	}

	// Create pipeline
	{
		scene.pipeline = scene.device->CreatePipeline(scene.vertexShader, scene.pixelShader);
	}

	// Load screen vertex shader
	{
		char metaData[2048];
		size_t metaDataSize;

		{
			auto file = scene.fileSystem->OpenFile(Files::FileMode::Read, "/Example-6/screen-vertex.mslmd");
			auto size = scene.fileSystem->GetSize(file);
			auto code = new char[size + 1];
			scene.fileSystem->Read(file, code, size);
			scene.fileSystem->CloseFile(file);
			code[size] = '\0';

			metaDataSize = Graphics::MetaDataAssembler::Assemble(code, metaData, sizeof(metaData));
			delete[] code;
		}

		char bytecode[2048];
		size_t bytecodeSize;

		{
			auto file = scene.fileSystem->OpenFile(Files::FileMode::Read, "/Example-6/screen-vertex.mslbc");
			auto size = scene.fileSystem->GetSize(file);
			auto code = new char[size + 1];
			scene.fileSystem->Read(file, code, size);
			scene.fileSystem->CloseFile(file);
			code[size] = '\0';

			bytecodeSize = Graphics::BytecodeAssembler::Assemble(code, bytecode, sizeof(bytecode));
			delete[] code;
		}

		Graphics::ShaderData shaderData(bytecode, bytecodeSize, metaData, metaDataSize);

		try
		{
			scene.screenVertexShader = scene.device->CreateVertexShader(shaderData);
		}
		catch (Graphics::RenderDeviceError& err)
		{
			std::cout << err.what() << std::endl;
			getchar();
			exit(EXIT_FAILURE);
		}
	}

	// Load screen diffuse pixel shader
	{
		char metaData[2048];
		size_t metaDataSize;

		{
			auto file = scene.fileSystem->OpenFile(Files::FileMode::Read, "/Example-6/screen-pixel-dif.mslmd");
			auto size = scene.fileSystem->GetSize(file);
			auto code = new char[size + 1];
			scene.fileSystem->Read(file, code, size);
			scene.fileSystem->CloseFile(file);
			code[size] = '\0';

			metaDataSize = Graphics::MetaDataAssembler::Assemble(code, metaData, sizeof(metaData));
			delete[] code;
		}

		char bytecode[2048];
		size_t bytecodeSize;

		{
			auto file = scene.fileSystem->OpenFile(Files::FileMode::Read, "/Example-6/screen-pixel-dif.mslbc");
			auto size = scene.fileSystem->GetSize(file);
			auto code = new char[size + 1];
			scene.fileSystem->Read(file, code, size);
			scene.fileSystem->CloseFile(file);
			code[size] = '\0';

			bytecodeSize = Graphics::BytecodeAssembler::Assemble(code, bytecode, sizeof(bytecode));
			delete[] code;
		}

		Graphics::ShaderData shaderData(bytecode, bytecodeSize, metaData, metaDataSize);

		try
		{
			scene.screenDifPixelShader = scene.device->CreatePixelShader(shaderData);
		}
		catch (Graphics::RenderDeviceError& err)
		{
			std::cout << err.what() << std::endl;
			getchar();
			exit(EXIT_FAILURE);
		}
	}

	// Create screen diffuse pipeline
	{
		scene.screenDifPipeline = scene.device->CreatePipeline(scene.screenVertexShader, scene.screenDifPixelShader);
	}

	// Load screen UVs pixel shader
	{
		char metaData[2048];
		size_t metaDataSize;

		{
			auto file = scene.fileSystem->OpenFile(Files::FileMode::Read, "/Example-6/screen-pixel-uvs.mslmd");
			auto size = scene.fileSystem->GetSize(file);
			auto code = new char[size + 1];
			scene.fileSystem->Read(file, code, size);
			scene.fileSystem->CloseFile(file);
			code[size] = '\0';

			metaDataSize = Graphics::MetaDataAssembler::Assemble(code, metaData, sizeof(metaData));
			delete[] code;
		}

		char bytecode[2048];
		size_t bytecodeSize;

		{
			auto file = scene.fileSystem->OpenFile(Files::FileMode::Read, "/Example-6/screen-pixel-uvs.mslbc");
			auto size = scene.fileSystem->GetSize(file);
			auto code = new char[size + 1];
			scene.fileSystem->Read(file, code, size);
			scene.fileSystem->CloseFile(file);
			code[size] = '\0';

			bytecodeSize = Graphics::BytecodeAssembler::Assemble(code, bytecode, sizeof(bytecode));
			delete[] code;
		}

		Graphics::ShaderData shaderData(bytecode, bytecodeSize, metaData, metaDataSize);

		try
		{
			scene.screenUVsPixelShader = scene.device->CreatePixelShader(shaderData);
		}
		catch (Graphics::RenderDeviceError& err)
		{
			std::cout << err.what() << std::endl;
			getchar();
			exit(EXIT_FAILURE);
		}
	}

	// Create screen UVs pipeline
	{
		scene.screenUVsPipeline = scene.device->CreatePipeline(scene.screenVertexShader, scene.screenUVsPixelShader);
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

	// Load screen vertex buffer
	{
		Vertex data[6];

		data[0].x = -1.0f; data[0].y = -1.0f; data[0].z = 0.0f; data[0].u = 0.0f; data[0].v = 0.0f;
		data[1].x = +0.0f; data[1].y = -1.0f; data[1].z = 0.0f; data[1].u = 1.0f; data[1].v = 0.0f;
		data[2].x = +0.0f; data[2].y = +0.0f; data[2].z = 0.0f; data[2].u = 1.0f; data[2].v = 1.0f;

		data[3].x = -1.0f; data[3].y = -1.0f; data[3].z = 0.0f; data[3].u = 0.0f; data[3].v = 0.0f;
		data[4].x = -1.0f; data[4].y = +0.0f; data[4].z = 0.0f; data[4].u = 0.0f; data[4].v = 1.0f;
		data[5].x = +0.0f; data[5].y = +0.0f; data[5].z = 0.0f; data[5].u = 1.0f; data[5].v = 1.0f;

		scene.screenVertexBuffer = scene.device->CreateVertexBuffer(sizeof(data), data, Graphics::BufferUsage::Static);
	}

	// Create screen vertex layout
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

		scene.screenVertexLayout = scene.device->CreateVertexLayout(2, elements, scene.screenVertexShader);
	}

	// Create screen vertex array
	{
		Graphics::VertexBuffer* buffers[] =
		{
			scene.screenVertexBuffer,
		};
		scene.screenVertexArray = scene.device->CreateVertexArray(1, buffers, scene.screenVertexLayout);
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

		desc.addressU = Graphics::TextureAdressMode::Clamp;
		desc.addressV = Graphics::TextureAdressMode::Clamp;
		desc.minFilter = Graphics::TextureFilter::Linear;
		desc.magFilter = Graphics::TextureFilter::Linear;
		desc.mipmapFilter = Graphics::TextureFilter::Linear;
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
		desc.depthEnabled = true;
		scene.depthStencilState = scene.device->CreateDepthStencilState(desc);
	}

	// Create blend state
	{
		Graphics::BlendStateDesc desc;
		desc.blendEnabled = false;
		desc.sourceFactor = Graphics::BlendFactor::SourceAlpha;
		desc.destinationFactor = Graphics::BlendFactor::InverseSourceAlpha;
		scene.blendState = scene.device->CreateBlendState(desc);
	}

	// Create diffuse attachment
	{
		scene.diffuseAttachment = scene.device->CreateTexture2D(scene.window->GetWidth() / 2, scene.window->GetHeight() / 2, Graphics::TextureFormat::RGBA8UNorm, nullptr, Graphics::BufferUsage::Default, true);
		scene.diffuseBP = scene.screenDifPixelShader->GetBindingPoint("diffuse");
	}

	// Create UVs attachment
	{
		scene.uvsAttachment = scene.device->CreateTexture2D(scene.window->GetWidth() / 2, scene.window->GetHeight() / 2, Graphics::TextureFormat::RGBA8UNorm, nullptr, Graphics::BufferUsage::Default, true);
		scene.uvsBP = scene.screenUVsPixelShader->GetBindingPoint("uvs");
	}

	// Create depth and stencil attachment
	{
		scene.depthStencilBuffer = scene.device->CreateDepthStencilBuffer(scene.window->GetWidth() / 2, scene.window->GetHeight() / 2, Graphics::DepthStencilFormat::Depth32Stencil8);
	}

	// Create framebuffer sampler
	{
		Graphics::Sampler2DDesc desc;
		desc.addressU = Graphics::TextureAdressMode::Clamp;
		desc.addressV = Graphics::TextureAdressMode::Clamp;
		desc.minFilter = Graphics::TextureFilter::Linear;
		desc.magFilter = Graphics::TextureFilter::Linear;
		scene.framebufferSampler = scene.device->CreateSampler2D(desc);
	}

	// Create framebuffer
	{
		Graphics::Texture2D* tex[2];
		tex[0] = scene.diffuseAttachment;
		tex[1] = scene.uvsAttachment;
		scene.framebuffer = scene.device->CreateFramebuffer(2, tex, scene.depthStencilBuffer);
	}

	// Create constant buffer
	{
		scene.screenConstantBuffer = scene.device->CreateConstantBuffer(sizeof(glm::mat4));
		auto& mvp = *(glm::mat4*)scene.screenConstantBuffer->Map();
		mvp = glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, 0.0f));
		scene.screenConstantBuffer->Unmap();
		scene.screenConstantBufferBP = scene.screenVertexShader->GetBindingPoint("transform");
	}
}

void CleanScene(Scene& scene)
{
	scene.device->DestroyConstantBuffer(scene.screenConstantBuffer);

	scene.device->DestroyFramebuffer(scene.framebuffer);
	scene.device->DestroyDepthStencilBuffer(scene.depthStencilBuffer);
	scene.device->DestroyTexture2D(scene.uvsAttachment);
	scene.device->DestroyTexture2D(scene.diffuseAttachment);

	scene.device->DestroyBlendState(scene.blendState);
	scene.device->DestroyDepthStencilState(scene.depthStencilState);
	scene.device->DestroyRasterState(scene.rasterState);

	scene.device->DestroySampler2D(scene.sampler);
	scene.device->DestroyTexture2D(scene.texture);

	scene.device->DestroyIndexBuffer(scene.indexBuffer);
	scene.device->DestroyVertexArray(scene.vertexArray);
	scene.device->DestroyVertexLayout(scene.vertexLayout);
	scene.device->DestroyVertexBuffer(scene.vertexBuffer);

	scene.device->DestroyPipeline(scene.screenDifPipeline);
	scene.device->DestroyPixelShader(scene.screenDifPixelShader);
	scene.device->DestroyPipeline(scene.screenUVsPipeline);
	scene.device->DestroyPixelShader(scene.screenUVsPixelShader);
	scene.device->DestroyVertexShader(scene.screenVertexShader);

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
		model1 = glm::rotate(model1, glm::radians(0.01f), glm::vec3(0.0f, 1.0f, 0.0f));

		// Poll events
		scene.window->PollEvents();

		// Set raster and depth stencil states
		scene.device->SetRasterState(scene.rasterState);
		scene.device->SetDepthStencilState(scene.depthStencilState);
		scene.device->SetBlendState(scene.blendState);

		// Clear framebuffer
		scene.device->SetFramebuffer(scene.framebuffer);
		scene.device->Clear(glm::vec4(0.0f, 0.2f, 0.4f, 1.0f));

		// Draw objects
		scene.device->SetPipeline(scene.pipeline);
		scene.textureBP->BindTexture2D(scene.texture);
		scene.textureBP->BindSampler2D(scene.sampler);
		scene.device->SetVertexArray(scene.vertexArray);
		scene.device->SetIndexBuffer(scene.indexBuffer);

		// Object 2
		{
			auto transform = (Transform*)scene.transformBuffer->Map();
			transform->mvp = proj * view * model2;
			scene.transformBuffer->Unmap();
			scene.transformBP->BindConstantBuffer(scene.transformBuffer);
			scene.device->DrawTrianglesIndexed(0, 6);
		}

		// Object 1
		{
			auto transform = (Transform*)scene.transformBuffer->Map();
			transform->mvp = proj * view * model1;
			scene.transformBuffer->Unmap();
			scene.transformBP->BindConstantBuffer(scene.transformBuffer);
			scene.device->DrawTrianglesIndexed(0, 6);
		}

		// Draw screen quads
		scene.device->SetFramebuffer(nullptr);
		scene.device->Clear(glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));

		// Draw screen diffuse quad
		{
			auto& mvp = *(glm::mat4*)scene.screenConstantBuffer->Map();
			mvp = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			scene.screenConstantBuffer->Unmap();
		}

		scene.device->SetPipeline(scene.screenDifPipeline);
		scene.diffuseBP->BindTexture2D(scene.diffuseAttachment);
		scene.diffuseBP->BindSampler2D(scene.framebufferSampler);
		scene.screenConstantBufferBP->BindConstantBuffer(scene.screenConstantBuffer);
		scene.device->SetVertexArray(scene.screenVertexArray);
		scene.device->DrawTriangles(0, 6);
		scene.diffuseBP->BindTexture2D(nullptr);

		// Draw screen UVs quad
		{
			auto& mvp = *(glm::mat4*)scene.screenConstantBuffer->Map();
			mvp = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			scene.screenConstantBuffer->Unmap();
		}

		scene.device->SetPipeline(scene.screenUVsPipeline);
		scene.uvsBP->BindTexture2D(scene.uvsAttachment);
		scene.uvsBP->BindSampler2D(scene.framebufferSampler);
		scene.screenConstantBufferBP->BindConstantBuffer(scene.screenConstantBuffer);
		scene.device->SetVertexArray(scene.screenVertexArray);
		scene.device->DrawTriangles(0, 6);
		scene.uvsBP->BindTexture2D(nullptr);

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