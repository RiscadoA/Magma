#include "OGL400RenderDevice.hpp"

using namespace Magma::Framework::Graphics;
using namespace Magma::Framework;

void Magma::Framework::Graphics::OGL400RenderDevice::Init(Input::Window * window, const RenderDeviceSettings & settings)
{
	
}

void Magma::Framework::Graphics::OGL400RenderDevice::Terminate()
{
}

VertexShader * Magma::Framework::Graphics::OGL400RenderDevice::CreateVertexShader(const char * code)
{
	return nullptr;
}

void Magma::Framework::Graphics::OGL400RenderDevice::DestroyVertexShader(VertexShader * vertexShader)
{
}

PixelShader * Magma::Framework::Graphics::OGL400RenderDevice::CreatePixelShader(const char * code)
{
	return nullptr;
}

void Magma::Framework::Graphics::OGL400RenderDevice::DestroyPixelShader(PixelShader * pixelShader)
{
}

Pipeline * Magma::Framework::Graphics::OGL400RenderDevice::CreatePipeline(VertexShader * vertexShader, PixelShader * pixelShader)
{
	return nullptr;
}

void Magma::Framework::Graphics::OGL400RenderDevice::DestroyPipeline(Pipeline * pipeline)
{
}

void Magma::Framework::Graphics::OGL400RenderDevice::SetPipeline(Pipeline * pipeline)
{
}

VertexBuffer * Magma::Framework::Graphics::OGL400RenderDevice::CreateVertexBuffer(size_t size, const void * data = nullptr, BufferUsage usage = BufferUsage::Default)
{
	return nullptr;
}

void Magma::Framework::Graphics::OGL400RenderDevice::DestroyVertexBuffer(VertexBuffer * vertexBuffer)
{
}

VertexLayout * Magma::Framework::Graphics::OGL400RenderDevice::CreateVertexLayout(size_t elementCount, const VertexElement * elements, VertexShader * vertexShader)
{
	return nullptr;
}

void Magma::Framework::Graphics::OGL400RenderDevice::DestroyVertexLayout(VertexLayout * vertexLayout)
{
}

VertexArray * Magma::Framework::Graphics::OGL400RenderDevice::CreateVertexArray(size_t bufferCount, VertexBuffer ** buffers, VertexLayout * layout)
{
	return nullptr;
}

void Magma::Framework::Graphics::OGL400RenderDevice::DestroyVertexArray(VertexArray * vertexArray)
{
}

void Magma::Framework::Graphics::OGL400RenderDevice::SetVertexArray(VertexArray * vertexArray)
{
}

IndexBuffer * Magma::Framework::Graphics::OGL400RenderDevice::CreateIndexBuffer(IndexType type, size_t size, const void * data = nullptr, BufferUsage usage = BufferUsage::Default)
{
	return nullptr;
}

void Magma::Framework::Graphics::OGL400RenderDevice::DestroyIndexBuffer(IndexBuffer * indexBuffer)
{
}

void Magma::Framework::Graphics::OGL400RenderDevice::SetIndexBuffer(IndexBuffer * indexBuffer)
{
}

Texture2D * Magma::Framework::Graphics::OGL400RenderDevice::CreateTexture2D(size_t width, size_t height, const void * data = nullptr, BufferUsage usage = BufferUsage::Default)
{
	return nullptr;
}

void Magma::Framework::Graphics::OGL400RenderDevice::DestroyTexture2D(Texture2D * texture)
{
}

RasterState * Magma::Framework::Graphics::OGL400RenderDevice::CreateRasterState(const RasterStateDesc & desc)
{
	return nullptr;
}

void Magma::Framework::Graphics::OGL400RenderDevice::DestroyRasterState(RasterState * rasterState)
{
}

void Magma::Framework::Graphics::OGL400RenderDevice::SetRasterState(RasterState * rasterState)
{
}

DepthStencilState * Magma::Framework::Graphics::OGL400RenderDevice::CreateDepthStencilState(const DepthStencilStateDesc & desc)
{
	return nullptr;
}

void Magma::Framework::Graphics::OGL400RenderDevice::DestroyDepthStencilState(DepthStencilState * depthStencilState)
{
}

void Magma::Framework::Graphics::OGL400RenderDevice::SetDepthStencilState(DepthStencilState * depthStencilState)
{
}

void Magma::Framework::Graphics::OGL400RenderDevice::Clear(glm::vec4 color, float depth = 1.0f, int stencil = 0)
{
}

void Magma::Framework::Graphics::OGL400RenderDevice::DrawTriangles(size_t offset, size_t count)
{
}

void Magma::Framework::Graphics::OGL400RenderDevice::DrawTrianglesIndexed(size_t offset, size_t count)
{
}

void Magma::Framework::Graphics::OGL400RenderDevice::SetRenderTargets(Texture2D ** textures, size_t count)
{
}
