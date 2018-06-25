#include "D3D11Context.hpp"

#include <Config.hpp>

#if defined(MAGMA_FRAMEWORK_USE_DIRECTX)
#include <windows.h>
#include <windowsx.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")
#pragma comment (lib, "dxguid.lib")

#include <glm/gtc/type_ptr.hpp>
#include <sstream>
#include <map>
#include <comdef.h>

#include "MSL/HLSLCompiler.hpp"

#define MAX_BINDING_POINTS 16

using namespace Magma::Framework::Graphics;

struct Shader
{
	ID3D10Blob* blob;
	void* shader;
	ShaderType type;
	ID3D11ShaderReflection* reflection;
};

struct VertexBuffer
{
	ID3D11Buffer* buffer;
	ID3D11InputLayout* layout;
	unsigned int vertexSize;
};

struct IndexBuffer
{
	ID3D11Buffer* buffer;
	DXGI_FORMAT format;
};

struct ConstantBuffer
{
	ID3D11Buffer* buffer;
};

struct BindingPoint
{
	size_t index;
	size_t samplerIndex;
	std::string name;
	bool exists[(size_t)ShaderType::Count];
	bool occupied;
};

struct ShaderProgram
{
	Shader* shaders[(size_t)ShaderType::Count];
	BindingPoint bindingPoints[MAX_BINDING_POINTS];
};

struct Texture2D
{
	ID3D11Texture2D* texture;
	ID3D11ShaderResourceView* view;
	ID3D11RenderTargetView* renderTargetView;
	ID3D11Texture2D* depthStencilBuffer;
	ID3D11DepthStencilView* depthStencilView;
};

struct Sampler
{
	ID3D11SamplerState* samplerState;
};

Magma::Framework::Graphics::D3D11Context::D3D11Context()
{
	m_swapChain = nullptr;
	m_device = nullptr;
	m_deviceContext = nullptr;
	m_defaultRenderTargetView = nullptr;
	m_renderTargetView = nullptr;
	m_depthStencilBuffer = nullptr;
	m_depthStencilState = nullptr;
	m_defaultDepthStencilView = nullptr;
	m_rasterState = nullptr;
}

void Magma::Framework::Graphics::D3D11Context::Init(Input::Window * window, const ContextSettings& settings)
{
	HRESULT hr;

	m_settings = settings;
	m_window = dynamic_cast<Input::D3DWindow*>(window);
	if (m_window == nullptr)
		throw std::runtime_error("Failed to init D3D11Context:\nCouldn't cast from Magma::Framework::Input::Window* to Magma::Framework::Input::D3DWindow*");

	IDXGISwapChain* swapChain = nullptr;
	ID3D11Device* device = nullptr;
	ID3D11DeviceContext* deviceContext = nullptr;

	// Create Swap Chain
	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	scd.BufferCount = 1;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.OutputWindow = (HWND)m_window->GetHWND();
	scd.SampleDesc.Count = settings.multisampleCount;
	scd.Windowed = (window->GetMode() == Input::Window::Mode::Fullscreen) ? FALSE : TRUE;

	hr = D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		NULL,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&scd,
		&swapChain,
		&device,
		NULL,
		&deviceContext);
	if (FAILED(hr))
	{
		std::stringstream ss;
		ss << "Failed to create init D3D11Context:\nCouldn't create device and swap chain:\nError: " << hr;
		throw std::runtime_error(ss.str());
	}

	// Create back buffer
	ID3D11Texture2D* backBuffer;
	hr = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
	if (FAILED(hr))
	{
		swapChain->Release();
		device->Release();
		deviceContext->Release();
		std::stringstream ss;
		ss << "Failed to create init D3D11Context:\nFailed to get back buffer:\nError: " << hr;
		throw std::runtime_error(ss.str());
	}

	// Create render target view
	ID3D11RenderTargetView* renderTargetView;
	hr = device->CreateRenderTargetView(backBuffer, NULL, &renderTargetView);
	backBuffer->Release();
	if (FAILED(hr))
	{
		swapChain->Release();
		device->Release();
		deviceContext->Release();
		std::stringstream ss;
		ss << "Failed to create init D3D11Context:\nFailed to create render target view:\nError: " << hr;
		throw std::runtime_error(ss.str());
	}

	// Create depth stencil buffer and depth stencil view
	ID3D11DepthStencilView* depthStencilView;
	ID3D11Texture2D* depthStencilBuffer;

	D3D11_TEXTURE2D_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
	depthStencilDesc.Width = m_window->GetWidth();
	depthStencilDesc.Height = m_window->GetHeight();
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = settings.multisampleCount;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	hr = device->CreateTexture2D(&depthStencilDesc, NULL, &depthStencilBuffer);
	if (FAILED(hr))
	{
		swapChain->Release();
		device->Release();
		deviceContext->Release();
		renderTargetView->Release();
		std::stringstream ss;
		ss << "Failed to create init D3D11Context:\nFailed to create depth stencil texture:\nError: " << hr;
		throw std::runtime_error(ss.str());
	}

	hr = device->CreateDepthStencilView(depthStencilBuffer, NULL, &depthStencilView);
	if (FAILED(hr))
	{
		depthStencilBuffer->Release();
		swapChain->Release();
		device->Release();
		deviceContext->Release();
		renderTargetView->Release();
		std::stringstream ss;
		ss << "Failed to create init D3D11Context:\nFailed to create depth stencil view:\nError: " << hr;
		throw std::runtime_error(ss.str());
	}

	// Set render target
	deviceContext->OMSetRenderTargets(1, &renderTargetView, depthStencilView);

	// Set viewport
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = m_window->GetWidth();
	viewport.Height = m_window->GetHeight();
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	deviceContext->RSSetViewports(1, &viewport);

	// Set rasterizer state
	D3D11_RASTERIZER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));

	desc.FillMode = D3D11_FILL_SOLID;
	desc.CullMode = D3D11_CULL_BACK;
	desc.FrontCounterClockwise = TRUE;
	desc.MultisampleEnable = TRUE;
	desc.AntialiasedLineEnable = TRUE;

	ID3D11RasterizerState* rasterState;
	hr = device->CreateRasterizerState(&desc, &rasterState);
	if (FAILED(hr))
	{
		depthStencilView->Release();
		depthStencilBuffer->Release();
		swapChain->Release();
		device->Release();
		deviceContext->Release();
		renderTargetView->Release();
		std::stringstream ss;
		ss << "Failed to create init D3D11Context:\nFailed to create rasterizer state:\nError: " << hr;
		throw std::runtime_error(ss.str());
	}
	deviceContext->RSSetState(rasterState);

	// Store variables
	m_swapChain = swapChain;
	m_device = device;
	m_deviceContext = deviceContext;
	m_defaultRenderTargetView = renderTargetView;
	m_renderTargetView = renderTargetView;
	m_depthStencilBuffer = depthStencilBuffer;
	m_defaultDepthStencilView = depthStencilView;
	m_depthStencilView = depthStencilView;
	m_rasterState = rasterState;

	// Create MSL compiler
	m_compiler = new MSL::HLSLCompiler();
}

void Magma::Framework::Graphics::D3D11Context::Terminate()
{
	delete (MSL::HLSLCompiler*)m_compiler;

	((ID3D11RasterizerState*)m_rasterState)->Release();
	((ID3D11Texture2D*)m_depthStencilBuffer)->Release();
	((ID3D11DepthStencilView*)m_defaultDepthStencilView)->Release();
	((ID3D11RenderTargetView*)m_defaultRenderTargetView)->Release();
	((IDXGISwapChain*)m_swapChain)->Release();
	((ID3D11Device*)m_device)->Release();
	((ID3D11DeviceContext*)m_deviceContext)->Release();
}

void Magma::Framework::Graphics::D3D11Context::SetClearColor(glm::vec4 color)
{
	m_clearColor = color;
}

void Magma::Framework::Graphics::D3D11Context::Clear(BufferBit mask)
{
	if ((mask & BufferBit::Color) != BufferBit::None)
		((ID3D11DeviceContext*)m_deviceContext)->ClearRenderTargetView((ID3D11RenderTargetView*)m_renderTargetView, glm::value_ptr(m_clearColor));
	UINT depthStencilFlags = 0;
	if ((mask & BufferBit::Depth) != BufferBit::None)
		depthStencilFlags |= D3D11_CLEAR_DEPTH;
	if ((mask & BufferBit::Stencil) != BufferBit::None)
		depthStencilFlags |= D3D11_CLEAR_STENCIL;
	if (depthStencilFlags != 0)
	{
		if (m_depthStencilView == nullptr)
			throw std::runtime_error("Failed to clear on D3D11Context:\nBuffer bit mask has depth or stencil even though current target has no depth stencil buffer");
		((ID3D11DeviceContext*)m_deviceContext)->ClearDepthStencilView((ID3D11DepthStencilView*)m_depthStencilView, depthStencilFlags, 1.0f, 0);
	}
}

void Magma::Framework::Graphics::D3D11Context::SwapBuffers()
{
	((IDXGISwapChain*)m_swapChain)->Present(m_settings.enableVsync ? 1 : 0, 0);
}

void * Magma::Framework::Graphics::D3D11Context::CreateVertexBuffer(void * data, size_t size, const VertexLayout & layout, void* program)
{
	HRESULT hr = 0;

	auto buffer = new VertexBuffer();

	// Crate buffer
	D3D11_BUFFER_DESC vbDesc;
	ZeroMemory(&vbDesc, sizeof(vbDesc));

	vbDesc.Usage = D3D11_USAGE_DEFAULT;
	vbDesc.ByteWidth = size;
	vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbDesc.CPUAccessFlags = 0;
	vbDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vbData;
	ZeroMemory(&vbData, sizeof(vbData));
	vbData.pSysMem = data;

	hr = ((ID3D11Device*)m_device)->CreateBuffer(&vbDesc, &vbData, &buffer->buffer);
	if (FAILED(hr))
	{
		delete buffer;
		std::stringstream ss;
		ss << "Failed to create vertex buffer on D3D11Context:\nCouldn't create buffer:\nError: " << hr;
		throw std::runtime_error(ss.str());
	}
	buffer->vertexSize = layout.size;

	// Create layout
	try
	{
		auto inputDesc = new D3D11_INPUT_ELEMENT_DESC[layout.elements.size()];
		for (size_t i = 0; i < layout.elements.size(); ++i)
		{
			switch (layout.elements[i].format)
			{
				case VertexElementFormat::Float1: inputDesc[i].Format = DXGI_FORMAT_R32_FLOAT; break;
				case VertexElementFormat::Float2: inputDesc[i].Format = DXGI_FORMAT_R32G32_FLOAT; break;
				case VertexElementFormat::Float3: inputDesc[i].Format = DXGI_FORMAT_R32G32B32_FLOAT; break;
				case VertexElementFormat::Float4: inputDesc[i].Format = DXGI_FORMAT_R32G32B32A32_FLOAT; break;
				case VertexElementFormat::Int1: inputDesc[i].Format = DXGI_FORMAT_R32_SINT; break;
				case VertexElementFormat::Int2: inputDesc[i].Format = DXGI_FORMAT_R32G32_SINT; break;
				case VertexElementFormat::Int3: inputDesc[i].Format = DXGI_FORMAT_R32G32B32_SINT; break;
				case VertexElementFormat::Int4: inputDesc[i].Format = DXGI_FORMAT_R32G32B32A32_SINT; break;
				case VertexElementFormat::UInt1: inputDesc[i].Format = DXGI_FORMAT_R32_UINT; break;
				case VertexElementFormat::UInt2: inputDesc[i].Format = DXGI_FORMAT_R32G32_UINT; break;
				case VertexElementFormat::UInt3: inputDesc[i].Format = DXGI_FORMAT_R32G32B32_UINT; break;
				case VertexElementFormat::UInt4: inputDesc[i].Format = DXGI_FORMAT_R32G32B32A32_UINT; break;
				case VertexElementFormat::Invalid: throw std::runtime_error("Failed to create vertex buffer on D3D11Context:\Invalid vertex layout element format"); break;
				default: throw std::runtime_error("Failed to create vertex buffer on D3D11Context:\nUnsupported vertex layout element format"); break;
			}

			inputDesc[i].SemanticName = layout.elements[i].name.c_str();
			inputDesc[i].SemanticIndex = 0;
			inputDesc[i].InputSlot = 0;
			inputDesc[i].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			inputDesc[i].AlignedByteOffset = layout.elements[i].offset;
			inputDesc[i].InstanceDataStepRate = 0;
		}

		if (((ShaderProgram*)program)->shaders[(size_t)ShaderType::Vertex] == nullptr)
			throw std::runtime_error("Failed to create vertex buffer on D3D11Context:\nShader passed isn't a vertex shader");

		ID3D11InputLayout* inputLayout;
		hr = ((ID3D11Device*)m_device)->CreateInputLayout(inputDesc, layout.elements.size(), ((ShaderProgram*)program)->shaders[(size_t)ShaderType::Vertex]->blob->GetBufferPointer(), ((ShaderProgram*)program)->shaders[(size_t)ShaderType::Vertex]->blob->GetBufferSize(), &inputLayout);
		delete[] inputDesc;

		if (FAILED(hr))
		{
			std::stringstream ss;
			ss << "Failed to create vertex buffer on D3D11Context:\nCouldn't create buffer:\nError: " << hr;
			throw std::runtime_error(ss.str());
		}

		buffer->layout = inputLayout;
	}
	catch (...)
	{
		buffer->buffer->Release();
		delete buffer;
		throw;
	}

	return buffer;
}

void Magma::Framework::Graphics::D3D11Context::DestroyVertexBuffer(void * vertexBuffer)
{
	auto buffer = (VertexBuffer*)vertexBuffer;
	buffer->buffer->Release();
	buffer->layout->Release();
	delete buffer;
}

void Magma::Framework::Graphics::D3D11Context::BindVertexBuffer(void * vertexBuffer)
{
	m_boundVertexBuffer = vertexBuffer;
	if (vertexBuffer == nullptr)
		return;
	auto buffer = (VertexBuffer*)vertexBuffer;
	unsigned int offset = 0;
	((ID3D11DeviceContext*)m_deviceContext)->IASetVertexBuffers(0, 1, &buffer->buffer, &buffer->vertexSize, &offset);
	((ID3D11DeviceContext*)m_deviceContext)->IASetInputLayout(buffer->layout);
}

void Magma::Framework::Graphics::D3D11Context::Draw(size_t vertexCount, size_t offset, DrawMode mode)
{
	D3D11_PRIMITIVE_TOPOLOGY topology;
	switch (mode)
	{
		case DrawMode::Points: topology = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST; break;
		case DrawMode::Lines: topology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST; break;
		case DrawMode::LineStrip: topology = D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP; break;
		case DrawMode::Triangles: topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST; break;
		case DrawMode::TriangleStrip: topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP; break;
		case DrawMode::Invalid: throw std::runtime_error("Failed to draw on D3D11Context:\nInvalid draw mode"); break;
		default: throw std::runtime_error("Failed to draw on D3D11Context:\nUnsupported draw mode"); break;
	}

	((ID3D11DeviceContext*)m_deviceContext)->IASetPrimitiveTopology(topology);
	((ID3D11DeviceContext*)m_deviceContext)->Draw(vertexCount, offset);
}

void * Magma::Framework::Graphics::D3D11Context::CreateShader(ShaderType type, const std::string & src)
{
	HRESULT hr = 0;

	std::string compiledSrc;
	try
	{
		((MSL::HLSLCompiler*)m_compiler)->Load(src);
		((MSL::HLSLCompiler*)m_compiler)->Compile();
		compiledSrc = ((MSL::HLSLCompiler*)m_compiler)->GetOutput();
	}
	catch (std::runtime_error& e)
	{
		std::stringstream ss;
		ss << "Failed to create shader on D3D11Context:" << std::endl;
		ss << "Failed to compile from MSL to HLSL on D3D11Context:" << std::endl;
		ss << e.what();
		throw std::runtime_error(ss.str());
	}

	auto shader = new Shader();
	shader->type = type;
	switch (type)
	{
		case ShaderType::Vertex:
		{
			ID3DBlob* errorMessages;
			hr = D3DCompile(compiledSrc.c_str(), compiledSrc.size(), nullptr, nullptr, nullptr, "VS", "vs_4_0", D3D10_SHADER_PACK_MATRIX_COLUMN_MAJOR, 0, &shader->blob, &errorMessages);
			if (FAILED(hr))
			{
				std::stringstream ss;
				ss << "Failed to create shader on D3D11Context:\nFailed to compile vertex shader:\n";
				if (errorMessages)
					ss << (char*)errorMessages->GetBufferPointer();
				else
					ss << "Error: " << hr << " - no error messages";
				throw std::runtime_error(ss.str());
			}
			auto vertShader = (ID3D11VertexShader**)&shader->shader;
			hr = ((ID3D11Device*)m_device)->CreateVertexShader(shader->blob->GetBufferPointer(), shader->blob->GetBufferSize(), NULL, vertShader);
			if (FAILED(hr))
			{
				delete shader;
				std::stringstream ss;
				ss << "Failed to create shader on D3D11Context:\nFailed to create vertex shader:\n";
				ss << _com_error(hr).ErrorMessage();
				throw std::runtime_error(ss.str());
			}
		} break;
		case ShaderType::Pixel:
		{
			ID3DBlob* errorMessages;
			hr = D3DCompile(compiledSrc.c_str(), compiledSrc.size(), nullptr, nullptr, nullptr, "PS", "ps_4_0", D3D10_SHADER_PACK_MATRIX_COLUMN_MAJOR, 0, &shader->blob, &errorMessages);
			if (FAILED(hr))
			{
				std::stringstream ss;
				ss << "Failed to create shader on D3D11Context:\nFailed to compile pixel shader:\n";
				if (errorMessages)
					ss << (char*)errorMessages->GetBufferPointer();
				else
					ss << "Error (" << hr << ") - no error messages";
				throw std::runtime_error(ss.str());
			}
			auto pixelShader = (ID3D11PixelShader**)&shader->shader;
			hr = ((ID3D11Device*)m_device)->CreatePixelShader(shader->blob->GetBufferPointer(), shader->blob->GetBufferSize(), NULL, pixelShader);
			if (FAILED(hr))
			{
				delete shader;
				std::stringstream ss;
				ss << "Failed to create shader on D3D11Context:\nFailed to create pixel shader:\n";
				ss << _com_error(hr).ErrorMessage();
				throw std::runtime_error(ss.str());
			}
		} break;
		case ShaderType::Invalid:
			delete shader;
			throw std::runtime_error("Failed to create shader on D3D11Context:\nInvalid shader type");
			break;
		default:
			delete shader;
			throw std::runtime_error("Failed to create shader on D3D11Context:\nUnsupported shader type");
			break;
	}

	hr = D3DReflect(shader->blob->GetBufferPointer(), shader->blob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&shader->reflection);
	if (FAILED(hr))
	{
		switch (type)
		{
			case ShaderType::Vertex: ((ID3D11VertexShader*)shader->shader)->Release(); break;
			case ShaderType::Pixel: ((ID3D11PixelShader*)shader->shader)->Release(); break;
		}
		delete shader;
		std::stringstream ss;
		ss << "Failed to create shader on D3D11Context:\nFailed to create pixel shader reflection:\n";
		ss << _com_error(hr).ErrorMessage();
		throw std::runtime_error(ss.str());
	}

	return shader;
}

void Magma::Framework::Graphics::D3D11Context::DestroyShader(void * shader)
{
	auto shd = (Shader*)shader;

	switch (shd->type)
	{
		case ShaderType::Vertex:
			((ID3D11VertexShader*)shd->shader)->Release();
			break;
		case ShaderType::Pixel:
			((ID3D11PixelShader*)shd->shader)->Release();
			break;
		case ShaderType::Invalid:
			delete shd;
			throw std::runtime_error("Failed to destroy shader on D3D11Context:\Invalid shader type");
			break;
		default:
			delete shd;
			throw std::runtime_error("Failed to destroy shader on D3D11Context:\nUnsupported shader type");
			break;
	}

	shd->blob->Release();
	delete shd;
}

void * Magma::Framework::Graphics::D3D11Context::CreateProgram()
{
	auto program = new ShaderProgram();
	for (size_t i = 0; i < (size_t)ShaderType::Count; ++i)
		program->shaders[i] = nullptr;
	return program;
}

void Magma::Framework::Graphics::D3D11Context::DestroyProgram(void * program)
{
	auto prgm = (ShaderProgram*)program;
	delete prgm;
}

void Magma::Framework::Graphics::D3D11Context::AttachShader(void * program, void * shader)
{
	auto prgm = (ShaderProgram*)program;
	auto shd = (Shader*)shader;

	if (prgm->shaders[(size_t)shd->type] != nullptr)
		throw std::runtime_error("Failed to attach shader on D3D11Context:\nThere is already a shader of this type attached to the program");
	else
		prgm->shaders[(size_t)shd->type] = shd;
}

void Magma::Framework::Graphics::D3D11Context::DetachShader(void * program, void * shader)
{
	auto prgm = (ShaderProgram*)program;
	auto shd = (Shader*)shader;

	if (prgm->shaders[(size_t)shd->type] != shd)
		throw std::runtime_error("Failed to detach shader on D3D11Context:\nThe shader isn't attached to the program");
	prgm->shaders[(size_t)shd->type] = nullptr;
}

void Magma::Framework::Graphics::D3D11Context::LinkProgram(void * program)
{
	auto prgm = (ShaderProgram*)program;
	for (size_t i = 0; i < MAX_BINDING_POINTS; ++i)
	{
		for (size_t j = 0; j < (size_t)ShaderType::Count; ++j)
			prgm->bindingPoints[i].exists[j] = false;
		prgm->bindingPoints[i].index = 0;
		prgm->bindingPoints[i].name = "";
		prgm->bindingPoints[i].occupied = false;
	}
}

void Magma::Framework::Graphics::D3D11Context::BindProgram(void * program)
{
	m_boundProgram = program;
	if (program == nullptr)
		return;
	auto prgm = (ShaderProgram*)program;
	for (size_t i = 0; i < (size_t)ShaderType::Count; ++i)
		if (prgm->shaders[i] != nullptr)
			switch ((ShaderType)i)
			{
				case ShaderType::Vertex:
					((ID3D11DeviceContext*)m_deviceContext)->VSSetShader((ID3D11VertexShader*)prgm->shaders[i]->shader, nullptr, 0);
					break;
				case ShaderType::Pixel:
					((ID3D11DeviceContext*)m_deviceContext)->PSSetShader((ID3D11PixelShader*)prgm->shaders[i]->shader, nullptr, 0);
					break;
				case ShaderType::Invalid:
					throw std::runtime_error("Failed to bind program on D3D11Context:\nThe program has an attached shader with an invalid type");
					break;
				default:
					throw std::runtime_error("Failed to bind program on D3D11Context:\nThe program has an attached shader with an unsupported type");
					break;
			}
}

void * Magma::Framework::Graphics::D3D11Context::CreateIndexBuffer(void * data, size_t size, IndexFormat type)
{
	auto buffer = new IndexBuffer();

	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = size;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = data;
	HRESULT hr = ((ID3D11Device*)m_device)->CreateBuffer(&indexBufferDesc, &initData, &buffer->buffer);
	if (FAILED(hr))
	{
		std::stringstream ss;
		ss << "Failed to create index buffer on D3D11Context:\nD3D11 failed to create buffer:\n";
		ss << _com_error(hr).ErrorMessage();
		throw std::runtime_error(ss.str());
	}

	switch (type)
	{
		case IndexFormat::UInt8: buffer->format = DXGI_FORMAT_R8_UINT; break;
		case IndexFormat::UInt16: buffer->format = DXGI_FORMAT_R16_UINT; break;
		case IndexFormat::UInt32: buffer->format = DXGI_FORMAT_R32_UINT; break;
		case IndexFormat::Invalid: throw std::runtime_error("Failed to create index buffer on D3D11Context:\nInvalid index format"); break;
		default: throw std::runtime_error("Failed to create index buffer on D3D11Context:\nUnsupported index format"); break;
	}

	return buffer;
}

void Magma::Framework::Graphics::D3D11Context::DestroyIndexBuffer(void * indexBuffer)
{
	auto buffer = (IndexBuffer*)indexBuffer;
	buffer->buffer->Release();
	delete buffer;
}

void Magma::Framework::Graphics::D3D11Context::BindIndexBuffer(void * indexBuffer)
{
	m_boundIndexBuffer = indexBuffer;
	if (indexBuffer == nullptr)
		return;
	auto buffer = (IndexBuffer*)indexBuffer;
	((ID3D11DeviceContext*)m_deviceContext)->IASetIndexBuffer(buffer->buffer, buffer->format, 0);
}

void Magma::Framework::Graphics::D3D11Context::DrawIndexed(size_t indexedCount, size_t offset, DrawMode mode)
{
	D3D11_PRIMITIVE_TOPOLOGY topology;
	switch (mode)
	{
		case DrawMode::Points: topology = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST; break;
		case DrawMode::Lines: topology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST; break;
		case DrawMode::LineStrip: topology = D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP; break;
		case DrawMode::Triangles: topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST; break;
		case DrawMode::TriangleStrip: topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP; break;
		case DrawMode::Invalid: throw std::runtime_error("Failed to draw (indexed) on D3D11Context:\nInvalid draw mode"); break;
		default: throw std::runtime_error("Failed to draw (indexed) on D3D11Context:\nUnsupported draw mode"); break;
	}

	((ID3D11DeviceContext*)m_deviceContext)->IASetPrimitiveTopology(topology);

	((ID3D11DeviceContext*)m_deviceContext)->DrawIndexed(indexedCount, offset, 0);
}

void * Magma::Framework::Graphics::D3D11Context::CreateConstantBuffer(void * data, size_t size)
{
	auto buffer = new ConstantBuffer();

	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = size;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	if (data != nullptr)
	{
		D3D11_SUBRESOURCE_DATA initData;
		initData.pSysMem = data;

		HRESULT hr = ((ID3D11Device*)m_device)->CreateBuffer(&desc, &initData, &buffer->buffer);
		if (FAILED(hr))
		{
			delete buffer;
			std::stringstream ss;
			ss << "Failed to create constant buffer on D3D11Context:\nD3D11 failed to create buffer:\n";
			ss << _com_error(hr).ErrorMessage();
			throw std::runtime_error(ss.str());
		}
	}
	else
	{
		HRESULT hr = ((ID3D11Device*)m_device)->CreateBuffer(&desc, nullptr, &buffer->buffer);
		if (FAILED(hr))
		{
			delete buffer;
			std::stringstream ss;
			ss << "Failed to create constant buffer on D3D11Context:\nD3D11 failed to create buffer:\n";
			ss << _com_error(hr).ErrorMessage();
			throw std::runtime_error(ss.str());
		}
	}

	return buffer;
}

void Magma::Framework::Graphics::D3D11Context::DestroyConstantBuffer(void * constantBuffer)
{
	auto buffer = (ConstantBuffer*)constantBuffer;
	buffer->buffer->Release();
	delete buffer;
}

void Magma::Framework::Graphics::D3D11Context::UpdateConstantBuffer(void * constantBuffer, void * data)
{
	auto buffer = (ConstantBuffer*)constantBuffer;
	((ID3D11DeviceContext*)m_deviceContext)->UpdateSubresource(buffer->buffer, 0, NULL, data, 0, 0);
}

void Magma::Framework::Graphics::D3D11Context::BindConstantBuffer(void * constantBuffer, void * bindingPoint)
{
	auto buffer = (ConstantBuffer*)constantBuffer;
	auto bind = (BindingPoint*)bindingPoint;

	if (bind->exists[(size_t)ShaderType::Vertex])
		((ID3D11DeviceContext*)m_deviceContext)->VSSetConstantBuffers(bind->index, 1, &buffer->buffer);
	if (bind->exists[(size_t)ShaderType::Pixel])
		((ID3D11DeviceContext*)m_deviceContext)->PSSetConstantBuffers(bind->index, 1, &buffer->buffer);
}

void * Magma::Framework::Graphics::D3D11Context::GetConstantBindingPoint(void * program, const std::string & name)
{
	if (name == "")
		throw std::runtime_error("Failed to get constant buffer binding point from D3D11Context:\nName cannot be empty");

	auto prgm = (ShaderProgram*)program;

	BindingPoint* bind = nullptr;

	for (size_t i = 0; i < MAX_BINDING_POINTS; ++i)
	{
		if (!prgm->bindingPoints[i].occupied)
		{
			bind = &prgm->bindingPoints[i];
			continue;
		}

		if (prgm->bindingPoints[i].name == name)
			return &prgm->bindingPoints[i];
	}

	if (bind == nullptr)
		throw std::runtime_error("Failed to get constant buffer binding point from D3D11Context:\nBinding point count limit has been achieved");

	size_t index;
	bool success = false;
	for (size_t s = 0; s < (size_t)ShaderType::Count; ++s)
	{
		if (prgm->shaders[s] == nullptr)
			continue;

		D3D11_SHADER_INPUT_BIND_DESC desc;
		HRESULT hr = prgm->shaders[s]->reflection->GetResourceBindingDescByName(name.c_str(), &desc);

		bind->exists[s] = false;

		if (!FAILED(hr))
		{
			if (desc.Type != D3D_SHADER_INPUT_TYPE::D3D10_SIT_CBUFFER)
				throw std::runtime_error("Failed to get constant buffer binding point from D3D11Context:\nBinding point doesn't point to a constant buffer");

			bind->exists[s] = true;
			success = true;
		}
	}

	if (!success)
	{
		std::stringstream ss;
		ss << "Failed to get constant buffer binding point from D3D11Context:" << std::endl;
		ss << "No constant buffer binding point with name \"" << name << "\"" << std::endl;
		throw std::runtime_error(ss.str());
	}

	bind->occupied = true;
	bind->name = name;
	return bind;
}

void * Magma::Framework::Graphics::D3D11Context::CreateTexture2D(void * data, size_t width, size_t height, TextureFormat format)
{
	auto texture = new Texture2D();
	HRESULT hr;

	// Create texture
	try
	{
		// Texture description
		D3D11_TEXTURE2D_DESC desc;
		ZeroMemory(&desc, sizeof(desc));

		desc.Width = width;
		desc.Height = height;
		desc.MipLevels = desc.ArraySize = 1;
		desc.SampleDesc.Count = 1;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;

		// Initial texture data
		D3D11_SUBRESOURCE_DATA initData;
		ZeroMemory(&initData, sizeof(initData));
		initData.pSysMem = data;

		// Get format
		switch (format)
		{
			case TextureFormat::R8UInt: desc.Format = DXGI_FORMAT_R8_UNORM; initData.SysMemPitch = 1 * height; break;
			case TextureFormat::R16UInt: desc.Format = DXGI_FORMAT_R16_UNORM; initData.SysMemPitch = 2 * height; break;
			case TextureFormat::RG8UInt: desc.Format = DXGI_FORMAT_R8G8_UNORM; initData.SysMemPitch = 2 * height; break;
			case TextureFormat::RG16UInt: desc.Format = DXGI_FORMAT_R16G16_UNORM; initData.SysMemPitch = 4 * height; break;
			case TextureFormat::RGBA8UInt: desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; initData.SysMemPitch = 4 * height; break;
			case TextureFormat::RGBA16UInt: desc.Format = DXGI_FORMAT_R16G16B16A16_UNORM; initData.SysMemPitch = 8 * height; break;
			case TextureFormat::R32Float: desc.Format = DXGI_FORMAT_R32_FLOAT; initData.SysMemPitch = 4 * height; break;
			case TextureFormat::RG32Float: desc.Format = DXGI_FORMAT_R32G32_FLOAT; initData.SysMemPitch = 8 * height; break;
			case TextureFormat::RGB32Float: desc.Format = DXGI_FORMAT_R32G32B32_FLOAT; initData.SysMemPitch = 12 * height; break;
			case TextureFormat::RGBA32Float: desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT; initData.SysMemPitch = 16 * height; break;
			case TextureFormat::Invalid: throw std::runtime_error("Failed to create 2D texture on D3D11Context:\nInvalid format"); break;
			default: throw std::runtime_error("Failed to create 2D texture on D3D11Context:\nUnsupported format"); break;
		}

		if (((ID3D11Device*)m_device)->CreateTexture2D(&desc, &initData, NULL) != S_FALSE)
			throw std::runtime_error("Failed to create 2D texture on D3D11Context:\nD3D11 failed to create texture 2D, invalid description or initial data:\n");

		hr = ((ID3D11Device*)m_device)->CreateTexture2D(&desc, &initData, &texture->texture);
		if (FAILED(hr))
		{
			std::stringstream ss;
			ss << "Failed to create 2D texture on D3D11Context:\nD3D11 failed to create texture 2D:\n";
			ss << _com_error(hr).ErrorMessage();
			throw std::runtime_error(ss.str());
		}
	}
	catch (...)
	{
		delete texture;
		throw;
	}

	// Create resource view
	try
	{
		hr = ((ID3D11Device*)m_device)->CreateShaderResourceView(texture->texture, NULL, &texture->view);
		if (FAILED(hr))
		{
			std::stringstream ss;
			ss << "Failed to create 2D texture on D3D11Context:\nD3D11 failed to create shader resource view:\n";
			ss << _com_error(hr).ErrorMessage();
			throw std::runtime_error(ss.str());
		}

		// Generates mipmaps for this texture
		((ID3D11DeviceContext*)m_deviceContext)->GenerateMips(texture->view);
	}
	catch (...)
	{
		texture->texture->Release();
		delete texture;
		throw;
	}

	// Reserved for render textures
	texture->renderTargetView = nullptr;
	texture->depthStencilBuffer = nullptr;
	texture->depthStencilView = nullptr;

	return texture;
}

void Magma::Framework::Graphics::D3D11Context::DestroyTexture2D(void * texture)
{
	auto text = (Texture2D*)texture;
	text->view->Release();
	text->texture->Release();
	delete text;
}

void * Magma::Framework::Graphics::D3D11Context::GetTextureBindingPoint(void * program, const std::string & name)
{
	// TO DO

	if (name == "")
		throw std::runtime_error("Failed to get texture binding point from D3D11Context:\nName cannot be empty");

	auto prgm = (ShaderProgram*)program;

	BindingPoint* bind = nullptr;

	for (size_t i = 0; i < MAX_BINDING_POINTS; ++i)
	{
		if (!prgm->bindingPoints[i].occupied)
		{
			bind = &prgm->bindingPoints[i];
			continue;
		}

		if (prgm->bindingPoints[i].name == name)
			return &prgm->bindingPoints[i];
	}

	if (bind == nullptr)
		throw std::runtime_error("Failed to get texture binding point from D3D11Context:\nBinding point count limit has been achieved");

	size_t index;
	bool success = false;
	HRESULT hr;

	for (size_t s = 0; s < (size_t)ShaderType::Count; ++s)
	{
		if (prgm->shaders[s] == nullptr)
			continue;

		D3D11_SHADER_INPUT_BIND_DESC desc;
		hr = prgm->shaders[s]->reflection->GetResourceBindingDescByName(name.c_str(), &desc);

		bind->exists[s] = false;

		if (!FAILED(hr))
		{
			if (desc.Type != D3D_SHADER_INPUT_TYPE::D3D10_SIT_TEXTURE)
				throw std::runtime_error("Failed to get texture binding point from D3D11Context:\nBinding point doesn't point to a texture");
			bind->index = desc.BindPoint;

			hr = prgm->shaders[s]->reflection->GetResourceBindingDescByName((name + "_sampler").c_str(), &desc);
			if (FAILED(hr))
				throw std::runtime_error("Failed to get texture binding point from D3D11Context:\nCouldn't finding texture corresponding sampler (name_sampler)");
			if (desc.Type != D3D_SHADER_INPUT_TYPE::D3D10_SIT_SAMPLER)
				throw std::runtime_error("Failed to get texture binding point from D3D11Context:\nBinding point name_sampler doesn't point to a sampler");

			bind->samplerIndex = desc.BindPoint;
			bind->exists[s] = true;
			success = true;
		}
	}

	if (!success)
	{
		std::stringstream ss;
		ss << "Failed to get binding point from D3D11Context:" << std::endl;
		ss << "No binding point with name \"" << name << "\"" << std::endl;
		throw std::runtime_error(ss.str());
	}

	bind->occupied = true;
	bind->name = name;
	return bind;
}

void Magma::Framework::Graphics::D3D11Context::BindTexture2D(void * texture, void * bindPoint)
{
	auto text = (Texture2D*)texture;
	auto bind = (BindingPoint*)bindPoint;

	if (bind->exists[(size_t)ShaderType::Vertex])
		((ID3D11DeviceContext*)m_deviceContext)->VSSetShaderResources(bind->index, 1, &text->view);
	if (bind->exists[(size_t)ShaderType::Pixel])
		((ID3D11DeviceContext*)m_deviceContext)->PSSetShaderResources(bind->index, 1, &text->view);
}

void * Magma::Framework::Graphics::D3D11Context::CreateSampler(const SamplerSettings & settings)
{
	auto sampler = new Sampler();
	HRESULT hr;

	try
	{
		D3D11_SAMPLER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.MaxAnisotropy = 1;
		desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		desc.MipLODBias = 0;
		desc.MinLOD = 0;
		desc.MaxLOD = 0;
		desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

		// Set filter
		switch (settings.minFilter)
		{
			case TextureFilter::Nearest:
				switch (settings.magFilter)
				{
					case TextureFilter::Nearest: desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT; break;
					case TextureFilter::Linear: desc.Filter = D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT; break;
					case TextureFilter::Invalid: throw std::runtime_error("Failed to create sampler on D3D11Context:\nInvalid sampler settings:\Invalid magnifying filter"); break;
					default: throw std::runtime_error("Failed to create sampler on D3D11Context:\nInvalid sampler settings:\nUnsupported magnifying filter"); break;
				}
				break;
			case TextureFilter::Linear:
				switch (settings.magFilter)
				{
					case TextureFilter::Nearest: desc.Filter = D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR; break;
					case TextureFilter::Linear: desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR; break;
					case TextureFilter::Invalid: throw std::runtime_error("Failed to create sampler on D3D11Context:\nInvalid sampler settings:\Invalid magnifying filter"); break;
					default: throw std::runtime_error("Failed to create sampler on D3D11Context:\nInvalid sampler settings:\nUnsupported magnifying filter"); break;
				}
				break;
			case TextureFilter::Invalid: throw std::runtime_error("Failed to create sampler on D3D11Context:\nInvalid sampler settings:\Invalid mignifying filter"); break;
			default: throw std::runtime_error("Failed to create sampler on D3D11Context:\nInvalid sampler settings:\nUnsupported minifying filter"); break;
		}

		// Set adress mode for coordinate U
		switch (settings.adressU)
		{
			case TextureAdressMode::Wrap: desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP; break;
			case TextureAdressMode::Mirror: desc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR; break;
			case TextureAdressMode::Clamp: desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP; break;
			case TextureAdressMode::Border: desc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER; break;
			case TextureAdressMode::Invalid: throw std::runtime_error("Failed to create sampler on D3D11Context:\nInvalid sampler settings:\Invalid U coordinate texture adress mode"); break;
			default: throw std::runtime_error("Failed to create sampler on D3D11Context:\nInvalid sampler settings:\nUnsupported U coordinate texture adress mode"); break;
		}

		// Set adress mode for coordinate U
		switch (settings.adressV)
		{
			case TextureAdressMode::Wrap: desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP; break;
			case TextureAdressMode::Mirror: desc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR; break;
			case TextureAdressMode::Clamp: desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP; break;
			case TextureAdressMode::Border: desc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER; break;
			case TextureAdressMode::Invalid: throw std::runtime_error("Failed to create sampler on D3D11Context:\nInvalid sampler settings:\Invalid V coordinate texture adress mode"); break;
			default: throw std::runtime_error("Failed to create sampler on D3D11Context:\nInvalid texture sampler:\nUnsupported V coordinate texture adress mode"); break;
		}

		// Set border color
		desc.BorderColor[0] = settings.borderColor.r;
		desc.BorderColor[1] = settings.borderColor.g;
		desc.BorderColor[2] = settings.borderColor.b;
		desc.BorderColor[3] = settings.borderColor.a;

		// Create sampler state
		hr = ((ID3D11Device*)m_device)->CreateSamplerState(&desc, &sampler->samplerState);
		if (FAILED(hr))
		{
			std::stringstream ss;
			ss << "Failed to create sampler on D3D11Context:\nD3D11 failed to create sampler state:\n";
			ss << _com_error(hr).ErrorMessage();
			throw std::runtime_error(ss.str());
		}
	}
	catch (...)
	{
		delete sampler;
		throw;
	}

	return sampler;
}

void Magma::Framework::Graphics::D3D11Context::DestroySampler(void * sampler)
{
	auto smplr = (Sampler*)sampler;
	smplr->samplerState->Release();
	delete smplr;
}

void Magma::Framework::Graphics::D3D11Context::BindSampler(void * sampler, void * bindPoint)
{
	auto smplr = (Sampler*)sampler;
	auto bind = (BindingPoint*)bindPoint;

	if (bind->exists[(size_t)ShaderType::Vertex])
		((ID3D11DeviceContext*)m_deviceContext)->VSSetSamplers(bind->samplerIndex, 1, &smplr->samplerState);
	if (bind->exists[(size_t)ShaderType::Pixel])
		((ID3D11DeviceContext*)m_deviceContext)->PSSetSamplers(bind->samplerIndex, 1, &smplr->samplerState);
}

void * Magma::Framework::Graphics::D3D11Context::CreateRenderTexture(size_t width, size_t height, TextureFormat format, bool depthBuffer)
{
	auto texture = new Texture2D();
	HRESULT hr;

	DXGI_FORMAT dxFormat;

	// Create render texture
	try
	{
		// Texture description
		D3D11_TEXTURE2D_DESC desc;
		ZeroMemory(&desc, sizeof(desc));

		desc.Width = width;
		desc.Height = height;
		desc.MipLevels = desc.ArraySize = 1;
		desc.SampleDesc.Count = 1;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;

		// Get format
		switch (format)
		{
			case TextureFormat::R8UInt: dxFormat = desc.Format = DXGI_FORMAT_R8_UNORM; break;
			case TextureFormat::R16UInt: dxFormat = desc.Format = DXGI_FORMAT_R16_UNORM; break;
			case TextureFormat::RG8UInt: dxFormat = desc.Format = DXGI_FORMAT_R8G8_UNORM; break;
			case TextureFormat::RG16UInt: dxFormat = desc.Format = DXGI_FORMAT_R16G16_UNORM; break;
			case TextureFormat::RGBA8UInt: dxFormat = desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; break;
			case TextureFormat::RGBA16UInt: dxFormat = desc.Format = DXGI_FORMAT_R16G16B16A16_UNORM; break;
			case TextureFormat::R32Float: dxFormat = desc.Format = DXGI_FORMAT_R32_FLOAT; break;
			case TextureFormat::RG32Float: dxFormat = desc.Format = DXGI_FORMAT_R32G32_FLOAT; break;
			case TextureFormat::RGB32Float: dxFormat = desc.Format = DXGI_FORMAT_R32G32B32_FLOAT; break;
			case TextureFormat::RGBA32Float: dxFormat = desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT; break;
			case TextureFormat::Invalid: throw std::runtime_error("Failed to create render texture on D3D11Context:\nInvalid format"); break;
			default: throw std::runtime_error("Failed to create render texture on D3D11Context:\nUnsupported format"); break;
		}

		if (((ID3D11Device*)m_device)->CreateTexture2D(&desc, NULL, NULL) != S_FALSE)
			throw std::runtime_error("Failed to create render texture on D3D11Context:\nD3D11 failed to create texture 2D, invalid description\n");

		hr = ((ID3D11Device*)m_device)->CreateTexture2D(&desc, NULL, &texture->texture);
		if (FAILED(hr))
		{
			std::stringstream ss;
			ss << "Failed to create render texture on D3D11Context:\nD3D11 failed to create texture 2D:\n";
			ss << _com_error(hr).ErrorMessage();
			throw std::runtime_error(ss.str());
		}
	}
	catch (...)
	{
		delete texture;
		throw;
	}

	// Create render target view
	try
	{
		D3D11_RENDER_TARGET_VIEW_DESC desc;
		ZeroMemory(&desc, sizeof(desc));

		desc.Format = dxFormat;
		desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipSlice = 0;

		hr = ((ID3D11Device*)m_device)->CreateRenderTargetView(texture->texture, &desc, &texture->renderTargetView);
		if (FAILED(hr))
		{
			std::stringstream ss;
			ss << "Failed to create render texture on D3D11Context:\nD3D11 failed to create render target view:\n";
			ss << _com_error(hr).ErrorMessage();
			throw std::runtime_error(ss.str());
		}
	}
	catch (...)
	{
		texture->texture->Release();
		delete texture;
		throw;
	}

	// Create resource view
	try
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC desc;
		ZeroMemory(&desc, sizeof(desc));

		desc.Format = dxFormat;
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MostDetailedMip = 0;
		desc.Texture2D.MipLevels = 1;

		hr = ((ID3D11Device*)m_device)->CreateShaderResourceView(texture->texture, &desc, &texture->view);
		if (FAILED(hr))
		{
			std::stringstream ss;
			ss << "Failed to create render texture on D3D11Context:\nD3D11 failed to create shader resource view:\n";
			ss << _com_error(hr).ErrorMessage();
			throw std::runtime_error(ss.str());
		}
	}
	catch (...)
	{
		texture->renderTargetView->Release();
		texture->texture->Release();
		delete texture;
		throw;
	}

	if (depthBuffer)
	{
		// Create depth stencil buffer
		try
		{
			D3D11_TEXTURE2D_DESC desc;
			ZeroMemory(&desc, sizeof(desc));
			desc.Width = width;
			desc.Height = m_window->GetHeight();
			desc.MipLevels = 1;
			desc.ArraySize = 1;
			desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			desc.SampleDesc.Count = 1;
			desc.SampleDesc.Quality = 0;
			desc.Usage = D3D11_USAGE_DEFAULT;
			desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
			desc.CPUAccessFlags = 0;
			desc.MiscFlags = 0;

			hr = ((ID3D11Device*)m_device)->CreateTexture2D(&desc, NULL, &texture->depthStencilBuffer);
			if (FAILED(hr))
			{
				std::stringstream ss;
				ss << "Failed to create render texture on D3D11Context:\nFailed to create depth stencil texture:\n";
				ss << _com_error(hr).ErrorMessage();
				throw std::runtime_error(ss.str());
			}
		}
		catch (...)
		{
			texture->view->Release();
			texture->renderTargetView->Release();
			texture->texture->Release();
			delete texture;
			throw;
		}

		// Create depth stencil view
		try
		{
			hr = ((ID3D11Device*)m_device)->CreateDepthStencilView(texture->depthStencilBuffer, NULL, &texture->depthStencilView);
			if (FAILED(hr))
			{
				std::stringstream ss;
				ss << "Failed to create render texture on D3D11Context:\nFailed to create depth stencil view:\n";
				ss << _com_error(hr).ErrorMessage();
				throw std::runtime_error(ss.str());
			}
		}
		catch (...)
		{
			texture->depthStencilBuffer->Release();
			texture->view->Release();
			texture->renderTargetView->Release();
			texture->texture->Release();
			delete texture;
			throw;
		}
	}
	else
	{
		texture->depthStencilBuffer = nullptr;
		texture->depthStencilView = nullptr;
	}

	return texture;
}

void Magma::Framework::Graphics::D3D11Context::DestroyRenderTexture(void * renderTexture)
{
	auto text = (Texture2D*)renderTexture;
	if (text->depthStencilView != nullptr) text->depthStencilView->Release();
	if (text->depthStencilBuffer != nullptr) text->depthStencilBuffer->Release();
	text->view->Release();
	text->renderTargetView->Release();
	text->texture->Release();
	delete text;
}

void Magma::Framework::Graphics::D3D11Context::SetRenderTarget(void * renderTexture)
{
	auto text = (Texture2D*)renderTexture;
	if (text == nullptr)
	{
		m_renderTargetView = m_defaultRenderTargetView;
		m_depthStencilView = m_defaultDepthStencilView;
		((ID3D11DeviceContext*)m_deviceContext)->OMSetRenderTargets(1, (ID3D11RenderTargetView**)&m_defaultRenderTargetView, (ID3D11DepthStencilView*)m_defaultDepthStencilView);
	}
	else
	{
		m_renderTargetView = text->renderTargetView;
		m_depthStencilView = text->depthStencilView;
		((ID3D11DeviceContext*)m_deviceContext)->OMSetRenderTargets(1, &text->renderTargetView, (ID3D11DepthStencilView*)m_depthStencilView);
	}
}
#else
Magma::Framework::Graphics::D3D11Context::D3D11Context()
{
	throw std::runtime_error("Failed to construct D3D11Context: the project wasn't built for DirectX (MAGMA_FRAMEWORK_USE_DIRECTX must be defined)");
}

void Magma::Framework::Graphics::D3D11Context::Init(Input::Window * window, const ContextSettings& settings)
{
	throw std::runtime_error("Failed to init D3D11Context: the project wasn't built for DirectX (MAGMA_FRAMEWORK_USE_DIRECTX must be defined)");
}

void Magma::Framework::Graphics::D3D11Context::Terminate()
{
	throw std::runtime_error("Failed to terminate D3D11Context: the project wasn't built for DirectX (MAGMA_FRAMEWORK_USE_DIRECTX must be defined)");
}

void Magma::Framework::Graphics::D3D11Context::SetClearColor(glm::vec4 color)
{
	throw std::runtime_error("Failed to set clear color on D3D11Context: the project wasn't built for DirectX (MAGMA_FRAMEWORK_USE_DIRECTX must be defined)");
}

void Magma::Framework::Graphics::D3D11Context::Clear(BufferBit mask)
{
	throw std::runtime_error("Failed to clear D3D11Context: the project wasn't built for DirectX (MAGMA_FRAMEWORK_USE_DIRECTX must be defined)");
}

void Magma::Framework::Graphics::D3D11Context::SwapBuffers()
{
	throw std::runtime_error("Failed to swap buffers on D3D11Context: the project wasn't built for DirectX (MAGMA_FRAMEWORK_USE_DIRECTX must be defined)");
}

void * Magma::Framework::Graphics::D3D11Context::CreateVertexBuffer(void * data, size_t size, const VertexLayout & layout, void* vertexShader)
{
	throw std::runtime_error("Failed to create vertex buffer on D3D11Context: the project wasn't built for DirectX (MAGMA_FRAMEWORK_USE_DIRECTX must be defined)");
}

void Magma::Framework::Graphics::D3D11Context::DestroyVertexBuffer(void * vertexBuffer)
{
	throw std::runtime_error("Failed to destroy vertex buffer on D3D11Context: the project wasn't built for DirectX (MAGMA_FRAMEWORK_USE_DIRECTX must be defined)");
}

void Magma::Framework::Graphics::D3D11Context::BindVertexBuffer(void * vertexBuffer)
{
	throw std::runtime_error("Failed to bind vertex buffer on D3D11Context: the project wasn't built for DirectX (MAGMA_FRAMEWORK_USE_DIRECTX must be defined)");
}

void Magma::Framework::Graphics::D3D11Context::Draw(size_t vertexCount, size_t offset, DrawMode mode)
{
	throw std::runtime_error("Failed to draw on D3D11Context: the project wasn't built for DirectX (MAGMA_FRAMEWORK_USE_DIRECTX must be defined)");
}

void * Magma::Framework::Graphics::D3D11Context::CreateShader(ShaderType type, const std::string & src)
{
	throw std::runtime_error("Failed to create shader on D3D11Context: the project wasn't built for DirectX (MAGMA_FRAMEWORK_USE_DIRECTX must be defined)");
}

void Magma::Framework::Graphics::D3D11Context::DestroyShader(void * shader)
{
	throw std::runtime_error("Failed to destroy shader on D3D11Context: the project wasn't built for DirectX (MAGMA_FRAMEWORK_USE_DIRECTX must be defined)");
}

void * Magma::Framework::Graphics::D3D11Context::CreateProgram()
{
	throw std::runtime_error("Failed to create program on D3D11Context: the project wasn't built for DirectX (MAGMA_FRAMEWORK_USE_DIRECTX must be defined)");
}

void Magma::Framework::Graphics::D3D11Context::DestroyProgram(void * program)
{
	throw std::runtime_error("Failed to destroy program on D3D11Context: the project wasn't built for DirectX (MAGMA_FRAMEWORK_USE_DIRECTX must be defined)");
}

void Magma::Framework::Graphics::D3D11Context::AttachShader(void * program, void * shader)
{
	throw std::runtime_error("Failed to attach shader on D3D11Context: the project wasn't built for DirectX (MAGMA_FRAMEWORK_USE_DIRECTX must be defined)");
}

void Magma::Framework::Graphics::D3D11Context::DetachShader(void * program, void * shader)
{
	throw std::runtime_error("Failed to detach shader on D3D11Context: the project wasn't built for DirectX (MAGMA_FRAMEWORK_USE_DIRECTX must be defined)");
}

void Magma::Framework::Graphics::D3D11Context::LinkProgram(void * program)
{
	throw std::runtime_error("Failed to link program on D3D11Context: the project wasn't built for DirectX (MAGMA_FRAMEWORK_USE_DIRECTX must be defined)");
}

void Magma::Framework::Graphics::D3D11Context::BindProgram(void * program)
{
	throw std::runtime_error("Failed to bind program on D3D11Context: the project wasn't built for DirectX (MAGMA_FRAMEWORK_USE_DIRECTX must be defined)");
}

void * Magma::Framework::Graphics::D3D11Context::CreateIndexBuffer(void * data, size_t size, IndexFormat type)
{
	throw std::runtime_error("Failed to create index buffer on D3D11Context: the project wasn't built for DirectX (MAGMA_FRAMEWORK_USE_DIRECTX must be defined)");
}

void Magma::Framework::Graphics::D3D11Context::DestroyIndexBuffer(void * indexBuffer)
{
	throw std::runtime_error("Failed to destroy index buffer on D3D11Context: the project wasn't built for DirectX (MAGMA_FRAMEWORK_USE_DIRECTX must be defined)");
}

void Magma::Framework::Graphics::D3D11Context::BindIndexBuffer(void * indexBuffer)
{
	throw std::runtime_error("Failed to bind index buffer on D3D11Context: the project wasn't built for DirectX (MAGMA_FRAMEWORK_USE_DIRECTX must be defined)");
}

void Magma::Framework::Graphics::D3D11Context::DrawIndexed(size_t vertexCount, size_t offset, DrawMode mode)
{
	throw std::runtime_error("Failed to draw (indexed) on D3D11Context: the project wasn't built for DirectX (MAGMA_FRAMEWORK_USE_DIRECTX must be defined)");
}

void * Magma::Framework::Graphics::D3D11Context::CreateConstantBuffer(void * data, size_t size)
{
	throw std::runtime_error("Failed to create constant buffer on D3D11Context: the project wasn't built for DirectX (MAGMA_FRAMEWORK_USE_DIRECTX must be defined)");
}

void Magma::Framework::Graphics::D3D11Context::DestroyConstantBuffer(void * constantBuffer)
{
	throw std::runtime_error("Failed to destroy constant buffer on D3D11Context: the project wasn't built for DirectX (MAGMA_FRAMEWORK_USE_DIRECTX must be defined)");
}

void Magma::Framework::Graphics::D3D11Context::UpdateConstantBuffer(void * constantBuffer, void * data)
{
	throw std::runtime_error("Failed to update constant buffer on D3D11Context: the project wasn't built for DirectX (MAGMA_FRAMEWORK_USE_DIRECTX must be defined)");
}

void Magma::Framework::Graphics::D3D11Context::BindConstantBuffer(void * constantBuffer, void * bindingPoint)
{
	throw std::runtime_error("Failed to bind constant buffer on D3D11Context: the project wasn't built for DirectX (MAGMA_FRAMEWORK_USE_DIRECTX must be defined)");
}

void * Magma::Framework::Graphics::D3D11Context::GetConstantBindingPoint(void * program, const std::string & name)
{
	throw std::runtime_error("Failed to get constant binding point on D3D11Context: the project wasn't built for DirectX (MAGMA_FRAMEWORK_USE_DIRECTX must be defined)");
}

void * Magma::Framework::Graphics::D3D11Context::CreateTexture2D(void * data, size_t width, size_t height, TextureFormat format)
{
	throw std::runtime_error("Failed to create 2D texture on D3D11Context: the project wasn't built for DirectX (MAGMA_FRAMEWORK_USE_DIRECTX must be defined)");
}

void Magma::Framework::Graphics::D3D11Context::DestroyTexture2D(void * texture)
{
	throw std::runtime_error("Failed to destroy 2D texture on D3D11Context: the project wasn't built for DirectX (MAGMA_FRAMEWORK_USE_DIRECTX must be defined)");
}

void * Magma::Framework::Graphics::D3D11Context::GetTextureBindingPoint(void * program, const std::string & name)
{
	throw std::runtime_error("Failed to get texture binding point on D3D11Context: the project wasn't built for DirectX (MAGMA_FRAMEWORK_USE_DIRECTX must be defined)");
}

void Magma::Framework::Graphics::D3D11Context::BindTexture2D(void * texture, void * bindPoint)
{
	throw std::runtime_error("Failed to bind texture 2D on D3D11Context: the project wasn't built for DirectX (MAGMA_FRAMEWORK_USE_DIRECTX must be defined)");
}

void * Magma::Framework::Graphics::D3D11Context::CreateSampler(const SamplerSettings & settings)
{
	throw std::runtime_error("Failed to create sampler on D3D11Context: the project wasn't built for DirectX (MAGMA_FRAMEWORK_USE_DIRECTX must be defined)");
}

void Magma::Framework::Graphics::D3D11Context::DestroySampler(void * sampler)
{
	throw std::runtime_error("Failed to destroy sampler on D3D11Context: the project wasn't built for DirectX (MAGMA_FRAMEWORK_USE_DIRECTX must be defined)");
}

void Magma::Framework::Graphics::D3D11Context::BindSampler(void * sampler, void * bindPoint)
{
	throw std::runtime_error("Failed to bind sampler on D3D11Context: the project wasn't built for DirectX (MAGMA_FRAMEWORK_USE_DIRECTX must be defined)");
}

void * Magma::Framework::Graphics::D3D11Context::CreateRenderTexture(size_t width, size_t height, TextureFormat format)
{
	throw std::runtime_error("Failed to create render texture on D3D11Context: the project wasn't built for DirectX (MAGMA_FRAMEWORK_USE_DIRECTX must be defined)");
}

void Magma::Framework::Graphics::D3D11Context::DestroyRenderTexture(void * renderTexture)
{
	throw std::runtime_error("Failed to destroy render texture on D3D11Context: the project wasn't built for DirectX (MAGMA_FRAMEWORK_USE_DIRECTX must be defined)");
}

void Magma::Framework::Graphics::D3D11Context::SetRenderTarget(void * renderTexture)
{
	throw std::runtime_error("Failed to set render target on D3D11Context: the project wasn't built for DirectX (MAGMA_FRAMEWORK_USE_DIRECTX must be defined)");
}
#endif