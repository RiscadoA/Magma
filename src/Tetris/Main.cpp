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

	// Set clear color
	context->SetClearColor(glm::vec4(0.0f, 0.2f, 0.4f, 1.0f));

	// Create program and shaders
	void* program = nullptr;
	void* vertexShader = nullptr;
	void* pixelShader = nullptr;
	{
		std::string source = R"mfx(
		#mfx version 1.0.0
				
		#context glsl
		
		#shader vertex
		#version 330 core

		in vec3 position;
		in vec4 color;

		out vec4 vertColor;

		layout(std140) uniform cbTransform
		{
			mat4 model;
		};
			
		void main()
		{
			gl_Position = model * vec4(position, 1.0);
			vertColor = color;
		}
	
		#shader pixel
		#version 330 core		

		in vec4 vertColor;
		
		out vec4 fragColor;

		layout(std140) uniform cbMaterial
		{
			vec4 diffuse;
		};

		void main()
		{
			fragColor = vertColor * diffuse;
		}

		#context hlsl

		cbuffer cbTransform
		{
			float4x4 model;
		};

		cbuffer cbMaterial
		{
			float4 diffuse;
		};
		
		struct VS_OUTPUT
		{
			float4 Pos : SV_POSITION;
			float4 Color : COLOR;
		};

		VS_OUTPUT VS(float3 inPos : POSITION, float4 inColor : COLOR)
		{
			VS_OUTPUT output;

			output.Pos = mul(model, float4(inPos, 1.0f));
			output.Color = inColor;

			return output;
		}

		float4 PS(VS_OUTPUT input) : SV_TARGET
		{
			return input.Color * diffuse;
		}

		)mfx";
		
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
		layout.elements.back().name = "position";
		layout.elements.back().offset = 0;

		layout.elements.emplace_back();
		layout.elements.back().format = Framework::Graphics::VertexElementFormat::Float4;
		layout.elements.back().name = "color";
		layout.elements.back().offset = sizeof(float) * 3;

		layout.size = sizeof(float) * 3 + sizeof(float) * 4;
		
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

	// Create constant buffers
	void* cbTransform = nullptr;
	{
		cbTransform = context->CreateConstantBuffer(nullptr, sizeof(glm::mat4));
	}

	void* cbMaterial = nullptr;
	{
		glm::vec4 material;
		material.r = 1.0f;
		material.g = 1.0f;
		material.b = 1.0f;
		material.a = 1.0f;
		cbMaterial = context->CreateConstantBuffer(&material[0], sizeof(glm::vec4));
	}

	// Get binding points
	void* cbTransformBind = context->GetBindingPoint(program, "cbTransform");
	void* cbMaterialBind = context->GetBindingPoint(program, "cbMaterial");

	glm::mat4 model = glm::mat4(1.0f);

	while (running)
	{
		window->PollEvents();

		context->Clear(Framework::Graphics::BufferBit::Color | Framework::Graphics::BufferBit::Depth);

		context->UpdateConstantBuffer(cbTransform, &model[0][0]);

		context->BindProgram(program);
		context->BindVertexBuffer(vb);
		context->BindIndexBuffer(ib);
		context->BindConstantBuffer(cbTransform, cbTransformBind);
		context->BindConstantBuffer(cbMaterial, cbMaterialBind);

		context->DrawIndexed(6, 0, Framework::Graphics::DrawMode::Triangles);

		context->BindIndexBuffer(nullptr);
		context->BindVertexBuffer(nullptr);
		context->BindProgram(nullptr);
		

		context->SwapBuffers();

		model = glm::translate(model, glm::vec3(-0.001f, 0.0f, 0.0f));
	}

	context->DestroyConstantBuffer(cbMaterial);
	context->DestroyConstantBuffer(cbTransform);
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