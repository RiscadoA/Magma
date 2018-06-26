#include <Magma/Framework/Graphics/D3D11Context.hpp>
#include <Magma/Framework/Graphics/GLContext.hpp>
#include <Magma/Framework/Graphics/MSL/GLSLCompiler.hpp>
#include <iostream>

#include <glm/gtc/matrix_transform.hpp>

using namespace Magma;

void Main(int argc, char** argv)// try
{
	/*Framework::Graphics::MSL::GLSLCompiler compiler;

	compiler.Load(R"msl(
		#version 1.2.0

		#define COLOR_R 1.0

		Texture2D texture0;

		ConstantBuffer transform
		{
			mat4 mvp;
		}

		VertexOutput vertexOut
		{
			vec2 uvs;
			vec4 color;
		}
		
		int test(int x)
		{
			return x;
		}

		vec4 VertexShader(vec3 position, vec2 uvs, vec4 color)
		{
			//vertexOut.uvs = uvs;
			//vertexOut.color = color;
			return transform.mvp * vec4(position.xyz, 1.0);
		}
		
		vec4 PixelShader()
		{
			vec4 color = Sample2D(texture0, vertexOut.uvs) * vertexOut.color;
			bool shouldDiscard = color.r == 1.0;
			if (shouldDiscard == false)
			{
				discard;
			}
			else
			{
				color.r = 1.0;
			}
			return color;
		}
		
		)msl"
	);

	compiler.SetShaderType(Framework::Graphics::ShaderType::Vertex);
	compiler.Compile();
	compiler.PrintTree();
	std::cout << compiler.GetOutput() << std::endl << std::endl << std::endl;
	compiler.SetShaderType(Framework::Graphics::ShaderType::Pixel);
	compiler.Compile();
	compiler.PrintTree();
	std::cout << compiler.GetOutput() << std::endl << std::endl << std::endl;
	getchar();

	return;*/

	// end test

	bool running = true;

	Framework::Input::Window* window = new Framework::Input::GLWindow(800, 600, "Tetris", Framework::Input::Window::Mode::Windowed);

	Framework::Graphics::Context* context = new Framework::Graphics::GLContext();
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
		#version 1.2.0
		
		#define COLOR_R 1.0

		Texture2D texture0;

		ConstantBuffer transform
		{
			mat4 mvp;
		}

		VertexOutput vertexOut
		{
			vec2 uvs;
			vec4 color;
		}
		
		int test(int x)
		{
			return x;
		}

		vec4 VertexShader(vec3 position, vec2 uvs, vec4 color)
		{
			vertexOut.uvs = uvs;
			vertexOut.color = color;
			return transform.mvp * vec4(position.xyz, 1.0);
		}
		
		vec4 PixelShader()
		{
			vec4 color = Sample2D(texture0, vertexOut.uvs) * vertexOut.color;
			if (color.r == 1.0)
				discard;
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
		float data[] =
		{
			-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		1.0f, 1.0f, 1.0f, 1.0f,
			-0.5f, +0.5f, 0.0f,		0.0f, 1.0f,		1.0f, 1.0f, 1.0f, 1.0f,
			+0.5f, +0.5f, 0.0f,		1.0f, 1.0f,		1.0f, 1.0f, 1.0f, 1.0f,
			+0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		1.0f, 1.0f, 1.0f, 1.0f,
		};

		Framework::Graphics::VertexLayout layout;
		layout.elements.emplace_back();
		layout.elements.back().format = Framework::Graphics::VertexElementFormat::Float3;
		layout.elements.back().name = "position";
		layout.elements.back().offset = 0;

		layout.elements.emplace_back();
		layout.elements.back().format = Framework::Graphics::VertexElementFormat::Float2;
		layout.elements.back().name = "uvs";
		layout.elements.back().offset = sizeof(float) * 3;

		layout.elements.emplace_back();
		layout.elements.back().format = Framework::Graphics::VertexElementFormat::Float4;
		layout.elements.back().name = "color";
		layout.elements.back().offset = sizeof(float) * 3 + sizeof(float) * 2;

		layout.size = sizeof(float) * 3 + sizeof(float) * 2 + sizeof(float) * 4;

		vb = context->CreateVertexBuffer(data, sizeof(data), layout, program);
	}

	// Create index buffer
	void* ib = nullptr;
	{
		unsigned int data[] =
		{
			0, 1, 2,
			2, 3, 0,
		};

		ib = context->CreateIndexBuffer(data, sizeof(data), Framework::Graphics::IndexFormat::UInt32);
	}

	// Create constant buffer
	void* transformcb = nullptr;
	{
		transformcb = context->CreateConstantBuffer(nullptr, sizeof(glm::mat4));
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
		settings.minFilter = Framework::Graphics::TextureFilter::Nearest;
		settings.magFilter = Framework::Graphics::TextureFilter::Nearest;
		settings.borderColor = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);

		sampler = context->CreateSampler(settings);
	}

	glm::mat4 model, view, proj;
	glm::mat4 mvp;

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	view = glm::mat4(1.0f);
	view = glm::lookAt(glm::vec3(0.0f, 0.0f, -5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	proj = glm::perspective(glm::radians(70.0f), 800.0f / 600.0f, 0.1f, 100.0f);

	auto transformbp = context->GetConstantBindingPoint(program, "transform");
	auto texturebp = context->GetTextureBindingPoint(program, "texture0");

	while (running)
	{
		window->PollEvents();

		mvp = proj * view * model;
		context->UpdateConstantBuffer(transformcb, &mvp[0][0]);

		context->SetRenderTarget(nullptr);
		context->SetClearColor(glm::vec4(0.0f, 0.2f, 0.4f, 1.0f));
		context->Clear(Framework::Graphics::BufferBit::Color | Framework::Graphics::BufferBit::Depth);

		context->BindProgram(program);
		context->BindVertexBuffer(vb);
		context->BindIndexBuffer(ib);
		context->BindConstantBuffer(transformcb, transformbp);
		context->BindTexture2D(texture, texturebp);
		context->BindSampler(sampler, texturebp);
		context->DrawIndexed(6, 0, Framework::Graphics::DrawMode::Triangles);

		context->SwapBuffers();
	}

	context->DestroyIndexBuffer(ib);
	context->DestroyShader(pixelShader);
	context->DestroyShader(vertexShader);
	context->DestroyProgram(program);
	context->DestroyVertexBuffer(vb);

	context->Terminate();
	delete context;
	delete window;
}
/*catch (std::exception& e)
{
	std::cout << e.what() << std::endl;
	getchar();
	return;
}*/