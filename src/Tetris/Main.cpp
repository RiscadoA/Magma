#include <Magma/Framework/Graphics/D3D11Context.hpp>
#include <Magma/Framework/Graphics/GLContext.hpp>
#include <iostream>

#include <glm/gtc/matrix_transform.hpp>

using namespace Magma;

void Main(int argc, char** argv) try
{
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
		#version 1.0.0

		#define COLOR_R 1.0

		int(position.x.y.z)

		/*VertexOutput vertexOut
		{
			
		}

		int test()
		{
			return -1.0f;
		}*/

		/*vec4 VertexShader(vec3 position)
		{
			return vec4(position.x, position.y, position.z, 1.0);
		}

		vec4 PixelShader()
		{
			return vec4(COLOR_R, COLOR_R, 0.0, 1.0);
		}*/
		
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
			-1.0f, -1.0f, 0.0f,
			-1.0f, +1.0f, 0.0f,
			+1.0f, +1.0f, 0.0f,
			+1.0f, -1.0f, 0.0f,
		};

		Framework::Graphics::VertexLayout layout;
		layout.elements.emplace_back();
		layout.elements.back().format = Framework::Graphics::VertexElementFormat::Float3;
		layout.elements.back().name = "position";
		layout.elements.back().offset = 0;

		layout.size = sizeof(float) * 3;

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

	glm::mat4 model, view, proj;
	glm::mat4 mvp;

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(2.0f, 0.0f, -2.0f));
	view = glm::mat4(1.0f);
	view = glm::lookAt(glm::vec3(0.0f, 0.0f, -5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	proj = glm::perspective(glm::radians(70.0f), 800.0f / 600.0f, 0.1f, 100.0f);

	while (running)
	{
		window->PollEvents();

		context->SetRenderTarget(nullptr);
		context->SetClearColor(glm::vec4(0.0f, 0.2f, 0.4f, 1.0f));
		context->Clear(Framework::Graphics::BufferBit::Color | Framework::Graphics::BufferBit::Depth);

		context->BindProgram(program);
		context->BindVertexBuffer(vb);
		context->BindIndexBuffer(ib);
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
catch (std::exception& e)
{
	std::cout << e.what() << std::endl;
	getchar();
	return;
}