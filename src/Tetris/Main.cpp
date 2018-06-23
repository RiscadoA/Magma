#include <Magma/Framework/Graphics/D3D11Context.hpp>
#include <Magma/Framework/Graphics/GLContext.hpp>
#include <iostream>

#include <glm/gtc/matrix_transform.hpp>

using namespace Magma;

void Main(int argc, char** argv) try
{
	bool running = true;

	Framework::Input::Window* window = new Framework::Input::D3DWindow(800, 600, "Tetris", Framework::Input::Window::Mode::Windowed);
	
	Framework::Graphics::Context* context = new Framework::Graphics::D3D11Context();
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
		in vec2 texcoords;
		in vec4 color;

		out vec2 vertTexCoords;
		out vec4 vertColor;

		layout(std140) uniform cbTransform
		{
			mat4 mvp;
		};
			
		void main()
		{
			gl_Position = mvp * vec4(position, 1.0);
			vertTexCoords = texcoords;
			vertColor = color;
		}
	
		#shader pixel
		#version 330 core		

		in vec2 vertTexCoords;
		in vec4 vertColor;
		
		out vec4 fragColor;

		uniform sampler2D gTexture;

		layout(std140) uniform cbMaterial
		{
			vec4 diffuse;
		};

		void main()
		{
			vec4 color = vertColor * diffuse * texture2D(gTexture, vertTexCoords);
			if (color.a == 0.0)
				discard;
			else
				fragColor = color;
		}

		#context hlsl

		Texture2D<float4> gTexture;
		SamplerState gTexture_sampler;

		cbuffer cbTransform
		{
			float4x4 mvp;
		};

		cbuffer cbMaterial
		{
			float4 diffuse;
		};
		
		struct VS_OUTPUT
		{
			float4 Pos : SV_POSITION;
			float4 UVs : TEXCOORDS;
			float4 Color : COLOR;
		};

		VS_OUTPUT VS(float3 inPos : POSITION, float2 inUVs : TEXCOORDS, float4 inColor : COLOR)
		{
			VS_OUTPUT output;

			output.Pos =  mul(mvp, float4(inPos, 1.0f));
			output.UVs = float4(inUVs.xy, 0.0f, 1.0f);
			output.Color = inColor;

			return output;
		}

		float4 PS(VS_OUTPUT input) : SV_TARGET
		{
			float4 color = input.Color * diffuse * gTexture.Sample(gTexture_sampler, input.UVs);
			if (color.a == 0)
				discard;
			return color;
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
			-1.0f, -1.0f, 0.0f,		-0.5f, -0.5f,		1.0f, 1.0f, 1.0f, 1.0f,
			-1.0f, +1.0f, 0.0f,		-0.5f, +1.5f,		1.0f, 1.0f, 1.0f, 1.0f,
			+1.0f, +1.0f, 0.0f,		+1.5f, +1.5f,		1.0f, 1.0f, 1.0f, 1.0f,
			+1.0f, -1.0f, 0.0f,     +1.5f, -0.5f,		1.0f, 1.0f, 1.0f, 1.0f,
		};

		Framework::Graphics::VertexLayout layout;
		layout.elements.emplace_back();
		layout.elements.back().format = Framework::Graphics::VertexElementFormat::Float3;
		layout.elements.back().name = "position";
		layout.elements.back().offset = 0;

		layout.elements.emplace_back();
		layout.elements.back().format = Framework::Graphics::VertexElementFormat::Float2;
		layout.elements.back().name = "texcoords";
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

	// Create sampler
	void* sampler = nullptr;
	{
		using namespace Framework::Graphics;

		SamplerSettings settings;
		settings.adressU = TextureAdressMode::Border;
		settings.adressV = TextureAdressMode::Border;
		settings.borderColor = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);
		settings.minFilter = TextureFilter::Nearest;
		settings.magFilter = TextureFilter::Nearest;

		sampler = context->CreateSampler(settings);
	}

	//Create texture
	void* texture = nullptr;
	{
		/*float data[] =
		{
		//	R		G		B		A
			1.0f,	0.0f,	0.0f,	1.0f,
			0.0f,	1.0f,	0.0f,	1.0f,
			0.0f,	0.0f,	1.0f,	1.0f,
			1.0f,	1.0f,	1.0f,	1.0f,
		};*/

		unsigned char data[] =
		{
			//	R		G		B		A
			255,	0,		0,		255,
			0,		255,	0,		0,
			0,		0,		255,	0,
			255,	255,	255,	255,
		};

		using namespace Framework::Graphics;

		texture = context->CreateTexture2D(data, 2, 2, TextureFormat::RGBA8UInt);
	}

	void* texture2 = nullptr;
	{
		/*float data[] =
		{
		//	R		G		B		A
		1.0f,	0.0f,	0.0f,	1.0f,
		0.0f,	1.0f,	0.0f,	1.0f,
		0.0f,	0.0f,	1.0f,	1.0f,
		1.0f,	1.0f,	1.0f,	1.0f,
		};*/

		unsigned char data[] =
		{
		//	R		G		B		A
			0,		0,		255,	255,
			0,		255,	0,		255,
			255,	0,		0,		255,
			0,		0,		0,		255,
		};

		using namespace Framework::Graphics;

		texture2 = context->CreateTexture2D(data, 2, 2, TextureFormat::RGBA8UInt);
	}

	// Get binding points
	void* cbTransformBind = context->GetConstantBindingPoint(program, "cbTransform");
	void* cbMaterialBind = context->GetConstantBindingPoint(program, "cbMaterial");
	void* textureBind = context->GetTextureBindingPoint(program, "gTexture");

	glm::mat4 model, view, proj;
	glm::mat4 mvp;

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(2.0f, 0.0f, -2.0f));
	view = glm::mat4(1.0f);
	view = glm::lookAt(glm::vec3(5.0f, 5.0f, -5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	proj = glm::perspective(glm::radians(70.0f), 800.0f / 600.0f, 0.1f, 100.0f);

	while (running)
	{
		

		window->PollEvents();

		context->Clear(Framework::Graphics::BufferBit::Color | Framework::Graphics::BufferBit::Depth);

		context->BindProgram(program);
		context->BindVertexBuffer(vb);
		context->BindIndexBuffer(ib);
		context->BindConstantBuffer(cbTransform, cbTransformBind);
		context->BindConstantBuffer(cbMaterial, cbMaterialBind);
		context->BindSampler(sampler, textureBind);

		context->BindTexture2D(texture, textureBind);
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -1.0f));
		mvp = proj * view * model;
		context->UpdateConstantBuffer(cbTransform, &mvp[0][0]);
		context->DrawIndexed(6, 0, Framework::Graphics::DrawMode::Triangles);

		context->BindTexture2D(texture2, textureBind);
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		mvp = proj * view * model;
		context->UpdateConstantBuffer(cbTransform, &mvp[0][0]);
		context->DrawIndexed(6, 0, Framework::Graphics::DrawMode::Triangles);

		context->SwapBuffers();
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