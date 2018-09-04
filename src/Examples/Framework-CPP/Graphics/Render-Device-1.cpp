#include <Magma/Framework/Entry.hpp>
#include <Magma/Framework/Input/Window.hpp>
#include <Magma/Framework/Graphics/2.X/RenderDevice.hpp>
#include <Magma/Framework/Graphics/2.X/MSL/Compiler.hpp>
#include <Magma/Framework/Graphics/Exception.hpp>
#include <Magma/Framework/String/Stream.hpp>

using namespace Magma::Framework;

bool winShouldClose = false;

void OnWindowClose(void* handle)
{
	winShouldClose = true;
}

const mfsUTF8CodeUnit* vsSource = u8R"(
	Input
	{
		float3 position : position;
	};

	Output
	{
		float4 position : _position;
	};

	void main()
	{
		Output.position = float4(Input.position.x, Input.position.y, Input.position.z, 1.0f);
	}
)";

const mfsUTF8CodeUnit* psSource = u8R"(
	Output
	{
		float4 color : _target0;
	};

	void main()
	{
		Output.color = float4(1.0f, 0.0f, 0.0f, 1.0f);
	}	
)";

int main(int argc, const char** argv)
{
	Magma::Framework::Init(argc, argv);

	Input::WindowHandle win = Input::CreateWindow(NULL, 800, 600, Input::WindowMode::Windowed, u8"Window");
	win.SetOnCloseCallback(OnWindowClose);

	Graphics::V2X::RenderDevice rd = NULL;

	try
	{
		Graphics::V2X::RenderDeviceDesc desc;
		desc.vsyncEnabled = true;
		rd = Graphics::V2X::CreateRenderDevice(NULL, win, &desc, Memory::StandardAllocator);
	}
	catch (std::runtime_error& err)
	{
		String::ErrStream.PutString(err.what());
		return 1;
	}

	Graphics::V2X::Pipeline pipeline = NULL;
	Graphics::V2X::VertexArray va = NULL;

	// Init render device objects
	try
	{
		Graphics::V2X::VertexShader vs = NULL;
		Graphics::V2X::PixelShader ps = NULL;

		// Create pipeline
		{
			// Create vertex shader
			{
				mfmU8 bBc[512];
				mfmU64 bBcSize;
				mfmU8 bMd[512];
				mfmU64 bMdSize;
				Graphics::V2X::MetaDataHandle md = NULL;

				Graphics::V2X::MSL::Compile(vsSource, bBc, sizeof(bBc), &bBcSize, bMd, sizeof(bMd), &bMdSize, Graphics::V2X::MSL::ShaderType::Vertex);
				md = Graphics::V2X::LoadMetaData(bMd, bMdSize);
				vs = rd.CreateVertexShader(bBc, bBcSize, md);
			}

			// Create pixel shader
			{
				mfmU8 bBc[512];
				mfmU64 bBcSize;
				mfmU8 bMd[512];
				mfmU64 bMdSize;
				Graphics::V2X::MetaDataHandle md = NULL;

				Graphics::V2X::MSL::Compile(psSource, bBc, sizeof(bBc), &bBcSize, bMd, sizeof(bMd), &bMdSize, Graphics::V2X::MSL::ShaderType::Pixel);
				md = Graphics::V2X::LoadMetaData(bMd, bMdSize);
				ps = rd.CreatePixelShader(bBc, bBcSize, md);
			}

			pipeline = rd.CreatePipeline(vs, ps);
		}

		// Create vertex array
		{
			Graphics::V2X::VertexBuffer buf = NULL;
			Graphics::V2X::VertexLayout layout = NULL;

			// Create vertex buffer
			{
				float positions[] =
				{
					0.0f, 0.0f, 0.0f,
					1.0f, 0.0f, 0.0f,
					1.0f, 1.0f, 0.0f,

					0.0f, 0.0f, 0.0f,
					0.0f, 1.0f, 0.0f,
					1.0f, 1.0f, 0.0f,
				};

				buf = rd.CreateVertexBuffer(sizeof(positions), positions, Graphics::V2X::Usage::Static);
			}

			// Create vertex layout
			{
				Graphics::V2X::VertexElement elements[1];
				elements[0].bufferIndex = 0;
				strcpy(elements[0].name, u8"position");
				elements[0].offset = 0;
				elements[0].stride = sizeof(float) * 3;
				elements[0].size = 3;
				elements[0].type = Graphics::V2X::Type::Float;
			
				layout = rd.CreateVertexLayout(1, elements, vs);
			}

			va = rd.CreateVertexArray(1, &buf, layout);
		}
	}
	catch (Graphics::RenderDeviceError& e)
	{
		String::OutStream.PutString(e.what());
		abort();
	}

	while (!winShouldClose)
	{
		win.PollEvents();
		
		rd.ClearColor(0.0f, 0.2f, 0.4f, 1.0f);

		rd.SetPipeline(pipeline);
		rd.SetVertexArray(va);
		rd.DrawTriangles(0, 6);
		rd.SetVertexArray(nullptr);
		rd.SetPipeline(nullptr);

		rd.SwapBuffers();
	}

	va.Release();
	pipeline.Release();

	rd.Release();

	win.Release();

	Magma::Framework::Terminate();
	return 0;
}
