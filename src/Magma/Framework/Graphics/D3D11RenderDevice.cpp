#include "D3D11RenderDevice.hpp"
#include "D3D11Assembler.hpp"

using namespace Magma::Framework::Graphics;
using namespace Magma::Framework;

#include <Config.hpp>
#include <sstream>
#include <string>
#include <map>

#if defined(MAGMA_FRAMEWORK_USE_DIRECTX)
#include <windows.h>
#include <windowsx.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <comdef.h>

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")
#pragma comment (lib, "dxguid.lib")
#endif

class D3D11Texture2D final : public Texture2D
{
public:
	D3D11Texture2D(D3D11RenderDevice* device, size_t width, size_t height, TextureFormat _format, const void* data)
	{
		
	}

	virtual ~D3D11Texture2D() final
	{
		
	}

	virtual void Update(size_t dstX, size_t dstY, size_t width, size_t height, void* data) final
	{
		
	}

	virtual void GenerateMipmaps()
	{
		
	}
};

class D3D11Sampler2D : public Sampler2D
{
public:
	D3D11Sampler2D(D3D11RenderDevice* device, const Sampler2DDesc& desc)
	{
		
	}

	virtual ~D3D11Sampler2D() final
	{
		
	}
};

class D3D11ConstantBuffer final : public ConstantBuffer
{
public:
	D3D11ConstantBuffer(D3D11RenderDevice* device, size_t size, const void* data, BufferUsage _usage)
	{
		
	}

	virtual ~D3D11ConstantBuffer() final
	{
		
	}

	virtual void* Map() final
	{
		return nullptr;
	}

	virtual void Unmap() final
	{
		
	}
};

class D3D11VertexBindingPoint;
class D3D11VertexShader : public VertexShader
{
public:
	D3D11VertexShader(D3D11RenderDevice* device, const ShaderData& data)
	{
		this->device = device;
	}

	virtual ~D3D11VertexShader() final
	{

	}

	virtual VertexBindingPoint* GetBindingPoint(const char * name) final;

	D3D11RenderDevice* device;
	ID3D11VertexShader* vs;
};

class D3D11VertexBindingPoint final : public VertexBindingPoint
{
public:
	D3D11VertexBindingPoint(D3D11VertexShader* _shader, int _location)
		: shader(_shader), location(_location)
	{
		// Empty
	}

	virtual ~D3D11VertexBindingPoint() final
	{

	}

	virtual void Bind(Texture2D* texture) final
	{
		
	}

	virtual void Bind(Sampler2D* sampler) final
	{
		
	}

	virtual void Bind(ConstantBuffer* buffer) final
	{
		
	}

	D3D11VertexShader* shader;
	int location;
};

VertexBindingPoint * D3D11VertexShader::GetBindingPoint(const char * name)
{
	return nullptr;
}

class D3D11PixelBindingPoint;
class D3D11PixelShader : public PixelShader
{
public:
	D3D11PixelShader(D3D11RenderDevice* device, const ShaderData& data)
	{
		this->device = device;
	}

	virtual ~D3D11PixelShader() final
	{

	}

	virtual PixelBindingPoint * GetBindingPoint(const char * name) final;

	D3D11RenderDevice* device;
	ID3D11PixelShader* ps;
};

class D3D11PixelBindingPoint final : public PixelBindingPoint
{
public:
	D3D11PixelBindingPoint(D3D11PixelShader* _shader, int _location)
		: shader(_shader), location(_location)
	{
		// Empty
	}

	virtual ~D3D11PixelBindingPoint() final
	{

	}

	virtual void Bind(Texture2D* texture) final
	{

	}

	virtual void Bind(Sampler2D* sampler) final
	{

	}

	virtual void Bind(ConstantBuffer* buffer) final
	{

	}

	D3D11PixelShader* shader;
	int location;
};

PixelBindingPoint * D3D11PixelShader::GetBindingPoint(const char * name)
{
	return nullptr;
}

class D3D11Pipeline final : public Pipeline
{
public:
	D3D11Pipeline(D3D11VertexShader* vertexShader, D3D11PixelShader* pixelShader)
	{
		
	}

	virtual ~D3D11Pipeline() final
	{
		
	}
};

class D3D11VertexBuffer final : public VertexBuffer
{
public:
	D3D11VertexBuffer(D3D11RenderDevice* device, size_t size, const void* data, BufferUsage _usage)
	{
		
	}

	virtual ~D3D11VertexBuffer() final
	{
		
	}

	virtual void* Map() final
	{
		return nullptr;
	}

	virtual void Unmap() final
	{
		
	}
};

class D3D11VertexLayout final : public VertexLayout
{
public:
	D3D11VertexLayout(D3D11RenderDevice* device, size_t vertexElementCount, const VertexElement* vertexElements, D3D11VertexShader* vertexShader)
	{
		
	}

	virtual ~D3D11VertexLayout() final
	{
		
	}
};

class D3D11VertexArray final : public VertexArray
{
public:
	D3D11VertexArray(D3D11RenderDevice* device, size_t bufferCount, D3D11VertexBuffer** buffers, D3D11VertexLayout* layout)
	{
		
	}

	virtual ~D3D11VertexArray() final
	{
		
	}
};

class D3D11IndexBuffer final : public IndexBuffer
{
public:
	D3D11IndexBuffer(D3D11RenderDevice* device, IndexType _type, size_t size, const void* data, BufferUsage _usage)
	{
		
	}

	virtual ~D3D11IndexBuffer() final
	{
		
	}
};

class D3D11RasterState final : public RasterState
{
public:
	D3D11RasterState(D3D11RenderDevice* device, const RasterStateDesc& desc)
	{

	}

	virtual ~D3D11RasterState() final
	{

	}
};

class D3D11DepthStencilState final : public DepthStencilState
{
public:
	D3D11DepthStencilState(D3D11RenderDevice* device, const DepthStencilStateDesc& desc)
	{
		
	}

	virtual ~D3D11DepthStencilState() final
	{

	}
};

class D3D11BlendState final : public BlendState
{
public:
	D3D11BlendState(D3D11RenderDevice* device, const BlendStateDesc& desc)
	{

	}

	virtual ~D3D11BlendState() final
	{

	}
};

void Magma::Framework::Graphics::D3D11RenderDevice::Init(Input::Window * window, const RenderDeviceSettings & settings)
{
#if defined(MAGMA_FRAMEWORK_USE_DIRECTX)
	m_settings = settings;
	m_window = dynamic_cast<Input::D3DWindow*>(window);
	if (m_window == nullptr)
		throw std::runtime_error("Failed to init D3D11RenderDevice:\nCouldn't cast from Magma::Framework::Input::Window* to Magma::Framework::Input::D3DWindow*");

	HRESULT hr;
	IDXGISwapChain* swapChain = nullptr;
	ID3D11Device* device = nullptr;
	ID3D11DeviceContext* deviceContext = nullptr;

	// Create device and swap chain
	{
		DXGI_SWAP_CHAIN_DESC scd;
		ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

		scd.BufferCount = 1;
		scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		scd.OutputWindow = (HWND)m_window->GetHWND();
		scd.SampleDesc.Count = 1;
		scd.SampleDesc.Quality = 0;
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
			ss << "Failed to init D3D11RenderDevice:\nCouldn't create device and swap chain:\nError: " << _com_error(hr).ErrorMessage();
			throw std::runtime_error(ss.str());
		}
	}
	
	// Get back buffer
	ID3D11Texture2D* backBuffer;
	{
		hr = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
		if (FAILED(hr))
		{
			swapChain->Release();
			device->Release();
			deviceContext->Release();
			std::stringstream ss;
			ss << "Failed to init D3D11RenderDevice:\nFailed to get back buffer:\nError: " << _com_error(hr).ErrorMessage();
			throw std::runtime_error(ss.str());
		}
	}

	// Create render target view
	ID3D11RenderTargetView* renderTargetView;
	{
		hr = device->CreateRenderTargetView(backBuffer, NULL, &renderTargetView);
		backBuffer->Release();
		if (FAILED(hr))
		{
			swapChain->Release();
			device->Release();
			deviceContext->Release();
			std::stringstream ss;
			ss << "Failed to init D3D11RenderDevice:\nFailed to create render target view:\nError: " << _com_error(hr).ErrorMessage();
			throw std::runtime_error(ss.str());
		}
	}

	// Create depth stencil buffer
	ID3D11Texture2D* depthStencilBuffer;
	{
		D3D11_TEXTURE2D_DESC depthStencilDesc;
		ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
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

		hr = device->CreateTexture2D(&depthStencilDesc, NULL, &depthStencilBuffer);
		if (FAILED(hr))
		{
			swapChain->Release();
			device->Release();
			deviceContext->Release();
			renderTargetView->Release();
			std::stringstream ss;
			ss << "Failed to init D3D11Context:\nFailed to create depth stencil texture:\nError: " << _com_error(hr).ErrorMessage();
			throw std::runtime_error(ss.str());
		}
	}

	// Create depth stencil view
	ID3D11DepthStencilView* depthStencilView;
	{
		hr = device->CreateDepthStencilView(depthStencilBuffer, NULL, &depthStencilView);
		if (FAILED(hr))
		{
			depthStencilBuffer->Release();
			swapChain->Release();
			device->Release();
			deviceContext->Release();
			renderTargetView->Release();
			std::stringstream ss;
			ss << "Failed to init D3D11Context:\nFailed to create depth stencil view:\nError: " << _com_error(hr).ErrorMessage();
			throw std::runtime_error(ss.str());
		}
	}

	// Set default render target
	deviceContext->OMSetRenderTargets(1, &renderTargetView, depthStencilView);

	// Set viewport
	{
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

	// Set rasterizer state
	ID3D11RasterizerState* rasterState;
	{
		D3D11_RASTERIZER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));

		desc.FillMode = D3D11_FILL_SOLID;
		desc.CullMode = D3D11_CULL_BACK;
		desc.FrontCounterClockwise = TRUE;
		desc.MultisampleEnable = TRUE;
		desc.AntialiasedLineEnable = TRUE;

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
	}

	// Store Direct3D stuff on void pointers
	m_swapChain = swapChain;
	m_device = device;
	m_deviceContext = deviceContext;
	m_defaultRenderTargetView = renderTargetView;
	m_renderTargetView = renderTargetView;
	m_depthStencilBuffer = depthStencilBuffer;
	m_defaultDepthStencilView = depthStencilView;
	m_depthStencilView = depthStencilView;
	m_rasterState = rasterState;

#else
	throw RenderDeviceError("Failed to call function on D3D11RenderDevice:\nMAGMA_FRAMEWORK_USE_DIRECTX must be defined");
#endif
}

void Magma::Framework::Graphics::D3D11RenderDevice::Terminate()
{
#if defined(MAGMA_FRAMEWORK_USE_DIRECTX)
	
#else
	throw RenderDeviceError("Failed to call function on D3D11RenderDevice:\nMAGMA_FRAMEWORK_USE_DIRECTX must be defined");
#endif
}

VertexShader * Magma::Framework::Graphics::D3D11RenderDevice::CreateVertexShader(const ShaderData & data)
{
#if defined(MAGMA_FRAMEWORK_USE_DIRECTX)
	return new D3D11VertexShader(this, data);
#else
	throw RenderDeviceError("Failed to call function on D3D11RenderDevice:\nMAGMA_FRAMEWORK_USE_DIRECTX must be defined");
#endif
}

void Magma::Framework::Graphics::D3D11RenderDevice::DestroyVertexShader(VertexShader * vertexShader)
{
#if defined(MAGMA_FRAMEWORK_USE_DIRECTX)
	delete vertexShader;
#else
	throw RenderDeviceError("Failed to call function on D3D11RenderDevice:\nMAGMA_FRAMEWORK_USE_DIRECTX must be defined");
#endif
}

PixelShader * Magma::Framework::Graphics::D3D11RenderDevice::CreatePixelShader(const ShaderData & data)
{
#if defined(MAGMA_FRAMEWORK_USE_DIRECTX)
	return new D3D11PixelShader(this, data);
#else
	throw RenderDeviceError("Failed to call function on D3D11RenderDevice:\nMAGMA_FRAMEWORK_USE_DIRECTX must be defined");
#endif
}

void Magma::Framework::Graphics::D3D11RenderDevice::DestroyPixelShader(PixelShader * pixelShader)
{
#if defined(MAGMA_FRAMEWORK_USE_DIRECTX)
	delete pixelShader;
#else
	throw RenderDeviceError("Failed to call function on D3D11RenderDevice:\nMAGMA_FRAMEWORK_USE_DIRECTX must be defined");
#endif
}

Pipeline * Magma::Framework::Graphics::D3D11RenderDevice::CreatePipeline(VertexShader * vertexShader, PixelShader * pixelShader)
{
#if defined(MAGMA_FRAMEWORK_USE_DIRECTX)
	return new D3D11Pipeline((D3D11VertexShader*)vertexShader, (D3D11PixelShader*)pixelShader);
#else
	throw RenderDeviceError("Failed to call function on D3D11RenderDevice:\nMAGMA_FRAMEWORK_USE_DIRECTX must be defined");
#endif
}

void Magma::Framework::Graphics::D3D11RenderDevice::DestroyPipeline(Pipeline * pipeline)
{
#if defined(MAGMA_FRAMEWORK_USE_DIRECTX)
	delete pipeline;
#else
	throw RenderDeviceError("Failed to call function on D3D11RenderDevice:\nMAGMA_FRAMEWORK_USE_DIRECTX must be defined");
#endif
}

void Magma::Framework::Graphics::D3D11RenderDevice::SetPipeline(Pipeline * pipeline)
{
#if defined(MAGMA_FRAMEWORK_USE_DIRECTX)
	// TO DO
#else
	throw RenderDeviceError("Failed to call function on D3D11RenderDevice:\nMAGMA_FRAMEWORK_USE_DIRECTX must be defined");
#endif
}

VertexBuffer * Magma::Framework::Graphics::D3D11RenderDevice::CreateVertexBuffer(size_t size, const void * data, BufferUsage usage)
{
#if defined(MAGMA_FRAMEWORK_USE_DIRECTX)
	return new D3D11VertexBuffer(this, size, data, usage);
#else
	throw RenderDeviceError("Failed to call function on D3D11RenderDevice:\nMAGMA_FRAMEWORK_USE_DIRECTX must be defined");
#endif
}

void Magma::Framework::Graphics::D3D11RenderDevice::DestroyVertexBuffer(VertexBuffer * vertexBuffer)
{
#if defined(MAGMA_FRAMEWORK_USE_DIRECTX)
	delete vertexBuffer;
#else
	throw RenderDeviceError("Failed to call function on D3D11RenderDevice:\nMAGMA_FRAMEWORK_USE_DIRECTX must be defined");
#endif
}

VertexLayout * Magma::Framework::Graphics::D3D11RenderDevice::CreateVertexLayout(size_t elementCount, const VertexElement * elements, VertexShader * vertexShader)
{
#if defined(MAGMA_FRAMEWORK_USE_DIRECTX)
	return new D3D11VertexLayout(this, elementCount, elements, (D3D11VertexShader*)vertexShader);
#else
	throw RenderDeviceError("Failed to call function on D3D11RenderDevice:\nMAGMA_FRAMEWORK_USE_DIRECTX must be defined");
#endif
}

void Magma::Framework::Graphics::D3D11RenderDevice::DestroyVertexLayout(VertexLayout * vertexLayout)
{
#if defined(MAGMA_FRAMEWORK_USE_DIRECTX)
	delete vertexLayout;
#else
	throw RenderDeviceError("Failed to call function on D3D11RenderDevice:\nMAGMA_FRAMEWORK_USE_DIRECTX must be defined");
#endif
}

VertexArray * Magma::Framework::Graphics::D3D11RenderDevice::CreateVertexArray(size_t bufferCount, VertexBuffer ** buffers, VertexLayout * layout)
{
#if defined(MAGMA_FRAMEWORK_USE_DIRECTX)
	return new D3D11VertexArray(this, bufferCount, (D3D11VertexBuffer**)buffers, (D3D11VertexLayout*)layout);
#else
	throw RenderDeviceError("Failed to call function on D3D11RenderDevice:\nMAGMA_FRAMEWORK_USE_DIRECTX must be defined");
#endif
}

void Magma::Framework::Graphics::D3D11RenderDevice::DestroyVertexArray(VertexArray * vertexArray)
{
#if defined(MAGMA_FRAMEWORK_USE_DIRECTX)
	delete vertexArray;
#else
	throw RenderDeviceError("Failed to call function on D3D11RenderDevice:\nMAGMA_FRAMEWORK_USE_DIRECTX must be defined");
#endif
}

void Magma::Framework::Graphics::D3D11RenderDevice::SetVertexArray(VertexArray * vertexArray)
{
#if defined(MAGMA_FRAMEWORK_USE_DIRECTX)
	// TO DO
#else
	throw RenderDeviceError("Failed to call function on D3D11RenderDevice:\nMAGMA_FRAMEWORK_USE_DIRECTX must be defined");
#endif
}

IndexBuffer * Magma::Framework::Graphics::D3D11RenderDevice::CreateIndexBuffer(IndexType type, size_t size, const void * data, BufferUsage usage)
{
#if defined(MAGMA_FRAMEWORK_USE_DIRECTX)
	return new D3D11IndexBuffer(this, type, size, data, usage);
#else
	throw RenderDeviceError("Failed to call function on D3D11RenderDevice:\nMAGMA_FRAMEWORK_USE_DIRECTX must be defined");
#endif
}

void Magma::Framework::Graphics::D3D11RenderDevice::DestroyIndexBuffer(IndexBuffer * indexBuffer)
{
#if defined(MAGMA_FRAMEWORK_USE_DIRECTX)
	delete indexBuffer;
#else
	throw RenderDeviceError("Failed to call function on D3D11RenderDevice:\nMAGMA_FRAMEWORK_USE_DIRECTX must be defined");
#endif
}

void Magma::Framework::Graphics::D3D11RenderDevice::SetIndexBuffer(IndexBuffer * indexBuffer)
{
#if defined(MAGMA_FRAMEWORK_USE_DIRECTX)
	// TO DO
#else
	throw RenderDeviceError("Failed to call function on D3D11RenderDevice:\nMAGMA_FRAMEWORK_USE_DIRECTX must be defined");
#endif
}

Texture2D * Magma::Framework::Graphics::D3D11RenderDevice::CreateTexture2D(size_t width, size_t height, TextureFormat format, const void * data)
{
#if defined(MAGMA_FRAMEWORK_USE_DIRECTX)
	return new D3D11Texture2D(this, width, height, format, data);
#else
	throw RenderDeviceError("Failed to call function on D3D11RenderDevice:\nMAGMA_FRAMEWORK_USE_DIRECTX must be defined");
#endif
}

void Magma::Framework::Graphics::D3D11RenderDevice::DestroyTexture2D(Texture2D * texture)
{
#if defined(MAGMA_FRAMEWORK_USE_DIRECTX)
	delete texture;
#else
	throw RenderDeviceError("Failed to call function on D3D11RenderDevice:\nMAGMA_FRAMEWORK_USE_DIRECTX must be defined");
#endif
}

Sampler2D * Magma::Framework::Graphics::D3D11RenderDevice::CreateSampler2D(const Sampler2DDesc & desc)
{
#if defined(MAGMA_FRAMEWORK_USE_DIRECTX)
	return new D3D11Sampler2D(this, desc);
#else
	throw RenderDeviceError("Failed to call function on D3D11RenderDevice:\nMAGMA_FRAMEWORK_USE_DIRECTX must be defined");
#endif
}

void Magma::Framework::Graphics::D3D11RenderDevice::DestroySampler2D(Sampler2D * sampler)
{
#if defined(MAGMA_FRAMEWORK_USE_DIRECTX)
	delete sampler;
#else
	throw RenderDeviceError("Failed to call function on D3D11RenderDevice:\nMAGMA_FRAMEWORK_USE_DIRECTX must be defined");
#endif
}

RasterState * Magma::Framework::Graphics::D3D11RenderDevice::CreateRasterState(const RasterStateDesc & desc)
{
#if defined(MAGMA_FRAMEWORK_USE_DIRECTX)
	return new D3D11RasterState(this, desc);
#else
	throw RenderDeviceError("Failed to call function on D3D11RenderDevice:\nMAGMA_FRAMEWORK_USE_DIRECTX must be defined");
#endif
}

void Magma::Framework::Graphics::D3D11RenderDevice::DestroyRasterState(RasterState * rasterState)
{
#if defined(MAGMA_FRAMEWORK_USE_DIRECTX)
	delete rasterState;
#else
	throw RenderDeviceError("Failed to call function on D3D11RenderDevice:\nMAGMA_FRAMEWORK_USE_DIRECTX must be defined");
#endif
}

void Magma::Framework::Graphics::D3D11RenderDevice::SetRasterState(RasterState * rasterState)
{
#if defined(MAGMA_FRAMEWORK_USE_DIRECTX)
	// TO DO
#else
	throw RenderDeviceError("Failed to call function on D3D11RenderDevice:\nMAGMA_FRAMEWORK_USE_DIRECTX must be defined");
#endif
}

DepthStencilState * Magma::Framework::Graphics::D3D11RenderDevice::CreateDepthStencilState(const DepthStencilStateDesc & desc)
{
#if defined(MAGMA_FRAMEWORK_USE_DIRECTX)
	return new D3D11DepthStencilState(this, desc);
#else
	throw RenderDeviceError("Failed to call function on D3D11RenderDevice:\nMAGMA_FRAMEWORK_USE_DIRECTX must be defined");
#endif
}

void Magma::Framework::Graphics::D3D11RenderDevice::DestroyDepthStencilState(DepthStencilState * depthStencilState)
{
#if defined(MAGMA_FRAMEWORK_USE_DIRECTX)
	delete depthStencilState;
#else
	throw RenderDeviceError("Failed to call function on D3D11RenderDevice:\nMAGMA_FRAMEWORK_USE_DIRECTX must be defined");
#endif
}

void Magma::Framework::Graphics::D3D11RenderDevice::SetDepthStencilState(DepthStencilState * depthStencilState)
{
#if defined(MAGMA_FRAMEWORK_USE_DIRECTX)
	// TO DO
#else
	throw RenderDeviceError("Failed to call function on D3D11RenderDevice:\nMAGMA_FRAMEWORK_USE_DIRECTX must be defined");
#endif
}

void Magma::Framework::Graphics::D3D11RenderDevice::Clear(glm::vec4 color, float depth, int stencil)
{
#if defined(MAGMA_FRAMEWORK_USE_DIRECTX)
	((ID3D11DeviceContext*)m_deviceContext)->ClearRenderTargetView((ID3D11RenderTargetView*)m_renderTargetView, &color[0]);
#else
	throw RenderDeviceError("Failed to call function on D3D11RenderDevice:\nMAGMA_FRAMEWORK_USE_DIRECTX must be defined");
#endif
}

void Magma::Framework::Graphics::D3D11RenderDevice::DrawTriangles(size_t offset, size_t count)
{
#if defined(MAGMA_FRAMEWORK_USE_DIRECTX)
	// TO DO
#else
	throw RenderDeviceError("Failed to call function on D3D11RenderDevice:\nMAGMA_FRAMEWORK_USE_DIRECTX must be defined");
#endif
}

void Magma::Framework::Graphics::D3D11RenderDevice::DrawTrianglesIndexed(size_t offset, size_t count)
{
#if defined(MAGMA_FRAMEWORK_USE_DIRECTX)
	// TO DO
#else
	throw RenderDeviceError("Failed to call function on D3D11RenderDevice:\nMAGMA_FRAMEWORK_USE_DIRECTX must be defined");
#endif
}

void Magma::Framework::Graphics::D3D11RenderDevice::SetRenderTargets(Texture2D ** textures, size_t count)
{
#if defined(MAGMA_FRAMEWORK_USE_DIRECTX)
	// TO DO
#else
	throw RenderDeviceError("Failed to call function on D3D11RenderDevice:\nMAGMA_FRAMEWORK_USE_DIRECTX must be defined");
#endif
}

void Magma::Framework::Graphics::D3D11RenderDevice::SwapBuffers()
{
#if defined(MAGMA_FRAMEWORK_USE_DIRECTX)
	((IDXGISwapChain*)m_swapChain)->Present(m_settings.enableVSync ? 1 : 0, 0);
#else
	throw RenderDeviceError("Failed to call function on D3D11RenderDevice:\nMAGMA_FRAMEWORK_USE_DIRECTX must be defined");
#endif
}

ConstantBuffer * Magma::Framework::Graphics::D3D11RenderDevice::CreateConstantBuffer(size_t size, const void * data, BufferUsage usage)
{
#if defined(MAGMA_FRAMEWORK_USE_DIRECTX)
	return new D3D11ConstantBuffer(this, size, data, usage);
#else
	throw RenderDeviceError("Failed to call function on D3D11RenderDevice:\nMAGMA_FRAMEWORK_USE_DIRECTX must be defined");
#endif
}

void Magma::Framework::Graphics::D3D11RenderDevice::DestroyConstantBuffer(ConstantBuffer * constantBuffer)
{
#if defined(MAGMA_FRAMEWORK_USE_DIRECTX)
	delete constantBuffer;
#else
	throw RenderDeviceError("Failed to call function on D3D11RenderDevice:\nMAGMA_FRAMEWORK_USE_DIRECTX must be defined");
#endif
}

BlendState * Magma::Framework::Graphics::D3D11RenderDevice::CreateBlendState(const BlendStateDesc & desc)
{
#if defined(MAGMA_FRAMEWORK_USE_DIRECTX)
	return new D3D11BlendState(this, desc);
#else
	throw RenderDeviceError("Failed to call function on D3D11RenderDevice:\nMAGMA_FRAMEWORK_USE_DIRECTX must be defined");
#endif
}

void Magma::Framework::Graphics::D3D11RenderDevice::DestroyBlendState(BlendState * blendState)
{
#if defined(MAGMA_FRAMEWORK_USE_DIRECTX)
	delete blendState;
#else
	throw RenderDeviceError("Failed to call function on D3D11RenderDevice:\nMAGMA_FRAMEWORK_USE_DIRECTX must be defined");
#endif
}

void Magma::Framework::Graphics::D3D11RenderDevice::SetBlendState(BlendState * blendState)
{
#if defined(MAGMA_FRAMEWORK_USE_DIRECTX)
	// TO DO
#else
	throw RenderDeviceError("Failed to call function on D3D11RenderDevice:\nMAGMA_FRAMEWORK_USE_DIRECTX must be defined");
#endif
}
