#include <Magma/Framework/Entry.hpp>
#include <Magma/Framework/Input/Window.hpp>
#include <Magma/Framework/File/FileSystem.hpp>
#include <Magma/Framework/File/FolderArchive.hpp>
#include <Magma/Framework/Graphics/2.X/RenderDevice.hpp>
#include <Magma/Framework/Graphics/2.X/MSL/Compiler.hpp>
#include <Magma/Framework/Graphics/2.X/PNGLoader.h>
#include <Magma/Framework/Graphics/Exception.hpp>
#include <Magma/Framework/String/Stream.hpp>
#include <Magma/Framework/String/StringStream.hpp>

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
		float2 uvs : uvs;
	};

	Output
	{
		float2 uvs : _out0;
		float4 position : _position;
	};

	void main()
	{
		Output.uvs = Input.uvs;
		Output.position = float4(Input.position.x, Input.position.y, Input.position.z, 1.0f);
	}
)";

const mfsUTF8CodeUnit* psSource = u8R"(
	Input
	{
		float2 uvs : _in0;
	};

	Output
	{
		float4 color : _target0;
	};

	Texture2D texture : texture;

	void main()
	{
		Output.color = sample2D(texture, Input.uvs);
	}	
)";

int run()
{
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
	Graphics::V2X::Texture2D tex = NULL;
	Graphics::V2X::Sampler sampler = NULL;
	void* texBP;

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
				float data[] =
				{
					0.0f, 0.0f, 0.0f,		0.0f, 0.0f,
					1.0f, 0.0f, 0.0f,		1.0f, 0.0f,
					1.0f, 1.0f, 0.0f,		1.0f, 1.0f,

					0.0f, 0.0f, 0.0f,		0.0f, 0.0f,
					0.0f, 1.0f, 0.0f,		0.0f, 1.0f,
					1.0f, 1.0f, 0.0f,		1.0f, 1.0f,
				};

				buf = rd.CreateVertexBuffer(sizeof(data), data, Graphics::V2X::Usage::Static);
			}

			// Create vertex layout
			{
				Graphics::V2X::VertexElement elements[2];
				elements[0].bufferIndex = 0;
				strcpy(elements[0].name, u8"position");
				elements[0].offset = 0;
				elements[0].stride = sizeof(float) * 5;
				elements[0].size = 3;
				elements[0].type = Graphics::V2X::Type::Float;

				elements[1].bufferIndex = 0;
				strcpy(elements[1].name, u8"uvs");
				elements[1].offset = sizeof(float) * 3;
				elements[1].stride = sizeof(float) * 5;
				elements[1].size = 2;
				elements[1].type = Graphics::V2X::Type::Float;

				layout = rd.CreateVertexLayout(2, elements, vs);
			}

			va = rd.CreateVertexArray(1, &buf, layout);
		}

		// Create texture
		{
			mfgPNGTextureData* texData;

			// Load texture data from file
			{
				auto file = File::GetFile(u8"/resources/Textures/test1.png");
				auto stream = File::OpenFile(file, File::FileMode::Read);
				mfError err = mfgLoadPNG(&stream.Get(), &texData, MFG_RGBA32FLOAT, NULL);
				if (err != MF_ERROR_OKAY)
					abort();
			}

			tex = rd.CreateTexture2D(texData->width, texData->height, Graphics::V2X::Format::RGBA32Float, texData->data, Graphics::V2X::Usage::Static);

			texBP = ps.GetBindingPoint("texture");

			mfgUnloadPNG(texData);
		}

		// Create sampler
		{
			Graphics::V2X::SamplerDesc desc;
			desc.minFilter = Graphics::V2X::TextureFilter::Nearest;
			desc.magFilter = Graphics::V2X::TextureFilter::Nearest;
			sampler = rd.CreateSampler(desc);
		}
	}
	catch (std::runtime_error& e)
	{
		String::OutStream.PutString(e.what());
		abort();
	}

	// Main loop
	while (!winShouldClose)
	{
		win.PollEvents();

		rd.ClearColor(0.0f, 0.2f, 0.4f, 1.0f);

		// Bind state
		rd.SetPipeline(pipeline);
		rd.BindTexture2D(texBP, tex);
		rd.BindSampler(texBP, sampler);
		rd.SetVertexArray(va);

		// Draw
		rd.DrawTriangles(0, 6);

		// Unbind state
		rd.BindTexture2D(texBP, nullptr);
		rd.BindSampler(texBP, nullptr);
		rd.SetVertexArray(nullptr);
		rd.SetPipeline(nullptr);

		rd.SwapBuffers();
	}

	return 0;
}

int main(int argc, const char** argv)
{
	Magma::Framework::Init(argc, argv);

	// Register archive on file system
	{
		mfsUTF8CodeUnit archivePath[256];
		auto ss = String::CreateStringStream(archivePath, sizeof(archivePath));
		ss.PutString(mffMagmaRootDirectory);
		ss.PutString(u8"/resources");
		ss.Release();

		auto archive = File::CreateFolderArchive(archivePath);
		File::RegisterArchive(archive, u8"resources");
	}

	int ret = run();

	Magma::Framework::Terminate();
	return ret;
}
