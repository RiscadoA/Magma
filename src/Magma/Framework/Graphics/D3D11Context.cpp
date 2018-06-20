#include "D3D11Context.hpp"

#include <Config.hpp>

#if defined(MAGMA_FRAMEWORK_USE_DIRECTX)
#include <windows.h>
#include <windowsx.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

#include <glm/gtc/type_ptr.hpp>
#include <sstream>

using namespace Magma::Framework::Graphics;

struct Shader
{
	ID3D10Blob* blob;
	void* shader;
	ShaderType type;
};

struct ShaderProgram
{
	Shader* shaders[(size_t)ShaderType::Count];
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

Magma::Framework::Graphics::D3D11Context::D3D11Context()
{
	m_swapChain = nullptr;
	m_device = nullptr;
	m_deviceContext = nullptr;
	m_renderTargetView = nullptr;
	m_depthStencilBuffer = nullptr;
	m_depthStencilState = nullptr;
	m_depthStencilView = nullptr;
	m_rasterState = nullptr;
}

void Magma::Framework::Graphics::D3D11Context::Init(Input::Window * window, const ContextSettings& settings)
{
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

	D3D11CreateDeviceAndSwapChain(
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

	m_swapChain = swapChain;
	m_device = device;
	m_deviceContext = deviceContext;

	// Create back buffer 
	ID3D11Texture2D* backBuffer;
	swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);

	// Create render target view
	ID3D11RenderTargetView* renderTargetView;
	device->CreateRenderTargetView(backBuffer, NULL, &renderTargetView);
	m_renderTargetView = renderTargetView;
	backBuffer->Release();

	// Create depth stencil buffer and depth stencil view
	ID3D11DepthStencilView* depthStencilView;
	ID3D11Texture2D* depthStencilBuffer;

	D3D11_TEXTURE2D_DESC depthStencilDesc;
	depthStencilDesc.Width = m_window->GetWidth();
	depthStencilDesc.Height = m_window->GetHeight();
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	device->CreateTexture2D(&depthStencilDesc, NULL, &depthStencilBuffer);
	device->CreateDepthStencilView(depthStencilBuffer, NULL, &depthStencilView);

	m_depthStencilBuffer = depthStencilBuffer;
	m_depthStencilView = depthStencilView;

	// Set render target
	deviceContext->OMSetRenderTargets(1, &renderTargetView, /*depthStencilView*/0);

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
}

void Magma::Framework::Graphics::D3D11Context::Terminate()
{
	((ID3D11Texture2D*)m_depthStencilBuffer)->Release();
	((ID3D11DepthStencilView*)m_depthStencilView)->Release();
	((ID3D11RenderTargetView*)m_renderTargetView)->Release();
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
		((ID3D11DeviceContext*)m_deviceContext)->ClearDepthStencilView((ID3D11DepthStencilView*)m_depthStencilView, depthStencilFlags, 1.0f, 0);
}

void Magma::Framework::Graphics::D3D11Context::SwapBuffers()
{
	((IDXGISwapChain*)m_swapChain)->Present(0, 0);
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
		std::stringstream ss;
		ss << "Failed to create vertex buffer on D3D11Context:\nCouldn't create buffer:\nError: " << hr;
		throw std::runtime_error(ss.str());
	}
	buffer->vertexSize = layout.size;

	// Create layout
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
			default: throw std::runtime_error("Failed to create vertex buffer on D3D11Context:\nUnsupported vertex layout element format");
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
		default: throw std::runtime_error("Failed to draw on D3D11Context:\nUnsupported draw mode"); break;
	}

	((ID3D11DeviceContext*)m_deviceContext)->IASetPrimitiveTopology(topology);
	((ID3D11DeviceContext*)m_deviceContext)->Draw(vertexCount, offset);
}

void * Magma::Framework::Graphics::D3D11Context::CreateShader(ShaderType type, const std::string & src)
{
	HRESULT hr = 0;

	auto shader = new Shader();
	shader->type = type;
	switch (type)
	{
	case ShaderType::Vertex:
	{
		ID3DBlob* errorMessages;
		hr = D3DCompile(src.c_str(), src.size(), nullptr, nullptr, nullptr, "VS", "vs_4_0", 0, 0, &shader->blob, &errorMessages);
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
			std::stringstream ss;
			ss << "Failed to create shader on D3D11Context:\nFailed to create vertex shader:\n";
			ss << "Error: " << hr;
			throw std::runtime_error(ss.str());
		}
	} break;
	case ShaderType::Pixel:
	{
		ID3DBlob* errorMessages;
		hr = D3DCompile(src.c_str(), src.size(), nullptr, nullptr, nullptr, "PS", "ps_4_0", 0, 0, &shader->blob, &errorMessages);
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
			std::stringstream ss;
			ss << "Failed to create shader on D3D11Context:\nFailed to create pixel shader:\n";
			ss << "Error: " << hr;
			throw std::runtime_error(ss.str());
		}
	} break;
	default:
		delete shader;
		throw std::runtime_error("Failed to create shader on D3D11Context:\nUnsupported shader type");
		break;
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
	// Do nothing, this step isn't necessary on D3D11
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
		ss << "Failed to create index buffer on D3D11Context:\nFailed to create buffer:\n";
		ss << "Error: " << hr;
		throw std::runtime_error(ss.str());
	}

	switch (type)
	{
		case IndexFormat::UInt8: buffer->format = DXGI_FORMAT_R8_UINT; break;
		case IndexFormat::UInt16: buffer->format = DXGI_FORMAT_R16_UINT; break;
		case IndexFormat::UInt32: buffer->format = DXGI_FORMAT_R32_UINT; break;
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
		default: throw std::runtime_error("Failed to draw on D3D11Context:\nUnsupported draw mode"); break;
	}

	((ID3D11DeviceContext*)m_deviceContext)->IASetPrimitiveTopology(topology);

	((ID3D11DeviceContext*)m_deviceContext)->DrawIndexed(indexedCount, offset, 0);
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

#endif
