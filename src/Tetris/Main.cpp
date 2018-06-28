#include <Magma/Framework/Graphics/D3D11Context.hpp>
#include <Magma/Framework/Graphics/GLContext.hpp>
#include <Magma/Framework/Graphics/MSL/HLSLCompiler.hpp>
#include <Magma/Framework/Graphics/MSL/GLSLCompiler.hpp>
#include <Magma/Framework/Files/FileSystem.hpp>
#include <iostream>

#include <glm/gtc/matrix_transform.hpp>

using namespace Magma;

void Main(int argc, char** argv) try
{


	/*bool running = true;

	Framework::Input::Window* window = new Framework::Input::D3DWindow(800, 600, "Tetris", Framework::Input::Window::Mode::Windowed);

	Framework::Graphics::Context* context = new Framework::Graphics::D3D11Context();
	Framework::Graphics::ContextSettings contextSettings;
	contextSettings.multisampleCount = 4;
	contextSettings.enableVsync = true;
	context->Init(window, contextSettings);

	window->OnClose.AddListener([&running]() { running = false; });

	// Create program and shaders
	void* program = nullptr;
	void* vertexShader = nullptr;
	void* pixelShader = nullptr;
	{
		std::string source = R"msl(
		#version 1.5.0
		
		#define COLOR_R 1.0

		ConstantBuffer transform
		{
			mat4 mvp;
			vec4 color[4];
		}

		VertexOutput vertexOut
		{
			vec4 color;
			int index;
		}

		vec4 VertexShader(vec3 position, vec4 color, int index)
		{
			vertexOut.color = color;
			vertexOut.index = index;
			return mul(transform.mvp, vec4(position.xyz, 1.0));
		}
		
		vec4 PixelShader()
		{
			vec4 color = vertexOut.color * transform.color[vertexOut.index];
			return color;
		}
		
		)msl";

		vertexShader = context->CreateShader(Framework::Graphics::ShaderType::Vertex, source);
		pixelShader = context->CreateShader(Framework::Graphics::ShaderType::Pixel, source);

		program = context->CreateProgram();
		context->AttachShader(program, vertexShader);
		context->AttachShader(program, pixelShader);
		context->LinkProgram(program);
	}

	// Create vertex buffer
	void* vb = nullptr;
	{
		struct Vertex
		{
			float x, y, z;
			float r, g, b, a;
			int i;
		};

		Vertex data[] =
		{
			{ -0.5f, -0.5f, 0.0f,		1.0f, 1.0f, 1.0f, 1.0f,		2 },
			{ -0.5f, +0.5f, 0.0f,		1.0f, 1.0f, 1.0f, 1.0f,		2 },
			{  0.0f, -0.5f, 0.0f,		1.0f, 1.0f, 1.0f, 1.0f,		2 },
			{  0.0f, +0.5f, 0.0f,		1.0f, 1.0f, 1.0f, 1.0f,		2 },

			{ +0.5f, -0.5f, 0.0f,		1.0f, 1.0f, 1.0f, 1.0f,		1 },
			{ +0.5f, +0.5f, 0.0f,		1.0f, 1.0f, 1.0f, 1.0f,		1 },
			{  0.0f, -0.5f, 0.0f,		1.0f, 1.0f, 1.0f, 1.0f,		1 },
			{  0.0f, +0.5f, 0.0f,		1.0f, 1.0f, 1.0f, 1.0f,		1 },
		};

		Framework::Graphics::VertexLayout layout;
		layout.elements.emplace_back();
		layout.elements.back().format = Framework::Graphics::VertexElementFormat::Float3;
		layout.elements.back().name = "position";
		layout.elements.back().offset = offsetof(Vertex, x);

		layout.elements.emplace_back();
		layout.elements.back().format = Framework::Graphics::VertexElementFormat::Float4;
		layout.elements.back().name = "color";
		layout.elements.back().offset = offsetof(Vertex, r);

		layout.elements.emplace_back();
		layout.elements.back().format = Framework::Graphics::VertexElementFormat::Int1;
		layout.elements.back().name = "index";
		layout.elements.back().offset = offsetof(Vertex, i);

		layout.size = sizeof(Vertex);

		vb = context->CreateVertexBuffer(data, sizeof(data), layout, program);
	}

	// Create index buffer
	void* ib = nullptr;
	{
		unsigned int data[] =
		{
			0, 1, 2,
			1, 3, 2,

			6, 5, 4,
			6, 7, 5,
		};

		ib = context->CreateIndexBuffer(data, sizeof(data), Framework::Graphics::IndexFormat::UInt32);
	}

	// Create constant buffer
	struct
	{
		glm::mat4 mvp;
		glm::vec4 color[4];
	} cbData;

	cbData.color[0] = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	cbData.color[1] = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	cbData.color[2] = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	cbData.color[3] = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	void* transformcb = nullptr;
	{
		transformcb = context->CreateConstantBuffer(nullptr, sizeof(cbData));
	}

	// Create texture
	void* texture = nullptr;
	{
		float data[] =
		{
			1.0f, 0.0f, 0.0f, 1.0f,
			0.0f, 1.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
		};

		texture = context->CreateTexture2D(data, 2, 2, Framework::Graphics::TextureFormat::RGBA32Float);
	}

	// Create sampler
	void* sampler = nullptr;
	{
		Framework::Graphics::SamplerSettings settings;

		settings.adressU = Framework::Graphics::TextureAdressMode::Border;
		settings.adressV = Framework::Graphics::TextureAdressMode::Border;
		settings.minFilter = Framework::Graphics::TextureFilter::Linear;
		settings.magFilter = Framework::Graphics::TextureFilter::Linear;
		settings.borderColor = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);

		sampler = context->CreateSampler(settings);
	}

	glm::mat4 model, view, proj;

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	view = glm::mat4(1.0f);
	view = glm::lookAt(glm::vec3(0.0f, 0.0f, -5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	proj = glm::perspective(glm::radians(70.0f), 800.0f / 600.0f, 0.1f, 100.0f);

	auto transformbp = context->GetConstantBindingPoint(program, "transform");
	//auto texturebp = context->GetTextureBindingPoint(program, "texture0");

	while (running)
	{
		window->PollEvents();

		cbData.mvp = proj * view * model;

		context->UpdateConstantBuffer(transformcb, &cbData, 0, sizeof(cbData));

		context->SetRenderTarget(nullptr);
		context->SetClearColor(glm::vec4(0.0f, 0.2f, 0.4f, 1.0f));
		context->Clear(Framework::Graphics::BufferBit::Color | Framework::Graphics::BufferBit::Depth);

		context->BindProgram(program);
		context->BindVertexBuffer(vb);
		context->BindIndexBuffer(ib);
		context->BindConstantBuffer(transformcb, transformbp);
		//context->BindTexture2D(texture, texturebp);
		//context->BindSampler(sampler, texturebp);
		context->DrawIndexed(12, 0, Framework::Graphics::DrawMode::Triangles);

		context->SwapBuffers();
	}

	context->DestroyIndexBuffer(ib);
	context->DestroyShader(pixelShader);
	context->DestroyShader(vertexShader);
	context->DestroyProgram(program);
	context->DestroyVertexBuffer(vb);

	context->Terminate();
	delete context;
	delete window;*/
}
catch (std::exception& e)
{
	std::cout << e.what() << std::endl;
	getchar();
}