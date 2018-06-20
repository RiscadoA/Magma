#include <Magma/Framework/Graphics/D3D11Context.hpp>
#include <Magma/Framework/Graphics/GLContext.hpp>
#include <iostream>

using namespace Magma;

void Main(int argc, char** argv)
{
	bool running = true;

	Framework::Input::Window* window = new Framework::Input::D3DWindow(800, 600, "Tetris", Framework::Input::Window::Mode::Windowed);
	
	Framework::Graphics::Context* context = new Framework::Graphics::D3D11Context();
	Framework::Graphics::ContextSettings contextSettings;
	contextSettings.multisampleCount = 4;
	context->Init(window, contextSettings);

	window->OnClose.AddListener([&running]() { running = false; });

	// Set clear color
	context->SetClearColor(glm::vec4(0.0f, 0.2f, 0.4f, 1.0f));

	// Create program and shaders
	void* program = nullptr;
	void* vertexShader = nullptr;
	void* pixelShader = nullptr;
	{
		std::string source = R"shader(

		struct VS_OUTPUT
		{
			float4 Pos : SV_POSITION;
			float4 Color : COLOR;
		};

		VS_OUTPUT VS(float3 inPos : POSITION, float4 inColor : COLOR)
		{
			VS_OUTPUT output;

			output.Pos = float4(inPos, 1.0f);
			output.Color = inColor;

			return output;
		}

		float4 PS(VS_OUTPUT input) : SV_TARGET
		{
			return input.Color;
		}

		)shader";

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
			0.0f, 0.0f, 0.0f,		1.0f, 0.0f, 0.0f, 1.0f,
			0.0f, 1.0f, 0.0f,		0.0f, 1.0f, 0.0f, 1.0f,
			1.0f, 1.0f, 0.0f,		0.0f, 0.0f, 1.0f, 1.0f,
			1.0f, 0.0f, 0.0f,       1.0f, 1.0f, 1.0f, 1.0f,
		};

		Framework::Graphics::VertexLayout layout;
		layout.elements.emplace_back();
		layout.elements.back().format = Framework::Graphics::VertexElementFormat::Float3;
		layout.elements.back().index = 0;
		layout.elements.back().name = "position";
		layout.elements.back().offset = 0;

		layout.elements.emplace_back();
		layout.elements.back().format = Framework::Graphics::VertexElementFormat::Float4;
		layout.elements.back().index = 0;
		layout.elements.back().name = "color";
		layout.elements.back().offset = sizeof(float) * 3;


		layout.size = sizeof(float) * 3 + sizeof(float) * 4;

		vb = context->CreateVertexBuffer(data, sizeof(data), layout, vertexShader);
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

	while (running)
	{
		window->PollEvents();

		context->Clear(Framework::Graphics::BufferBit::Color | Framework::Graphics::BufferBit::Depth);

		context->BindProgram(program);
		context->BindVertexBuffer(vb);
		context->BindIndexBuffer(ib);
		context->DrawIndexed(6, 0, Framework::Graphics::DrawMode::Triangles);
		context->BindIndexBuffer(nullptr);
		context->BindVertexBuffer(nullptr);
		context->BindProgram(nullptr);
		

		context->SwapBuffers();
	}

	context->DestroyVertexBuffer(vb);

	context->Terminate();
	delete context;
	delete window;
}