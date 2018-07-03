#include "OGL400RenderDevice.hpp"

using namespace Magma::Framework::Graphics;
using namespace Magma::Framework;

#include <Config.hpp>
#include <sstream>

#if defined(MAGMA_FRAMEWORK_USE_OPENGL)
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#if defined(MAGMA_FRAMEWORK_CONTEXT_DEBUG)
#define GL_CHECK_ERROR(title)\
{\
	GLenum err = glGetError();\
	if (err != GL_NO_ERROR)\
	{\
		std::stringstream ss;\
		ss << (title) << ":" << std::endl;\
		ss << "OpenGL error: " << err;\
		throw std::runtime_error(ss.str());\
	}\
}
#else
#define GL_CHECK_ERROR(title)
#endif
#endif

void Magma::Framework::Graphics::OGL400RenderDevice::Init(Input::Window * window, const RenderDeviceSettings & settings)
{
#if defined(MAGMA_FRAMEWORK_USE_OPENGL)
	m_window = dynamic_cast<Input::GLWindow*>(window);
	if (m_window == nullptr)
		throw RenderDeviceError("Failed to init OGL400RenderDevice:\nCouldn't cast from Magma::Framework::Input::Window* to Magma::Framework::Input::GLWindow*");

	// Init glew
	window->MakeCurrent();
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		std::stringstream ss;
		ss << "Failed to init OGL400RenderDevice:" << std::endl << "GLEW failed to init:" << std::endl;
		ss << (const char*)glewGetErrorString(err);
		throw RenderDeviceError(ss.str());
	}

	// Enable or disable VSync
	if (settings.enableVSync)
		glfwSwapInterval(1);
	else
		glfwSwapInterval(0);
#else
	throw RenderDeviceError("Failed to call OGL400RenderDevice function:\nMAGMA_FRAMEWORK_USE_OPENGL must be defined to use this render device");
#endif
}

void Magma::Framework::Graphics::OGL400RenderDevice::Terminate()
{
#if defined(MAGMA_FRAMEWORK_USE_OPENGL)
	// Oonly terminated when the GLWindow closes
#else
	throw RenderDeviceError("Failed to call OGL400RenderDevice function:\nMAGMA_FRAMEWORK_USE_OPENGL must be defined to use this render device");
#endif
}

VertexShader * Magma::Framework::Graphics::OGL400RenderDevice::CreateVertexShader(const ShaderData& data)
{
	// TO DO
	return nullptr;
}

void Magma::Framework::Graphics::OGL400RenderDevice::DestroyVertexShader(VertexShader * vertexShader)
{
	// TO DO
}

PixelShader * Magma::Framework::Graphics::OGL400RenderDevice::CreatePixelShader(const ShaderData& data)
{
	// TO DO
	return nullptr;
}

void Magma::Framework::Graphics::OGL400RenderDevice::DestroyPixelShader(PixelShader * pixelShader)
{
	// TO DO
}

Pipeline * Magma::Framework::Graphics::OGL400RenderDevice::CreatePipeline(VertexShader * vertexShader, PixelShader * pixelShader)
{
	// TO DO
	return nullptr;
}

void Magma::Framework::Graphics::OGL400RenderDevice::DestroyPipeline(Pipeline * pipeline)
{
	// TO DO
}

void Magma::Framework::Graphics::OGL400RenderDevice::SetPipeline(Pipeline * pipeline)
{
	// TO DO
}

VertexBuffer * Magma::Framework::Graphics::OGL400RenderDevice::CreateVertexBuffer(size_t size, const void * data, BufferUsage usage)
{
	// TO DO
	return nullptr;
}

void Magma::Framework::Graphics::OGL400RenderDevice::DestroyVertexBuffer(VertexBuffer * vertexBuffer)
{
	// TO DO
}

VertexLayout * Magma::Framework::Graphics::OGL400RenderDevice::CreateVertexLayout(size_t elementCount, const VertexElement * elements, VertexShader * vertexShader)
{
	// TO DO
	return nullptr;
}

void Magma::Framework::Graphics::OGL400RenderDevice::DestroyVertexLayout(VertexLayout * vertexLayout)
{
	// TO DO
}

VertexArray * Magma::Framework::Graphics::OGL400RenderDevice::CreateVertexArray(size_t bufferCount, VertexBuffer ** buffers, VertexLayout * layout)
{
	// TO DO
	return nullptr;
}

void Magma::Framework::Graphics::OGL400RenderDevice::DestroyVertexArray(VertexArray * vertexArray)
{
	// TO DO
}

void Magma::Framework::Graphics::OGL400RenderDevice::SetVertexArray(VertexArray * vertexArray)
{
	// TO DO
}

IndexBuffer * Magma::Framework::Graphics::OGL400RenderDevice::CreateIndexBuffer(IndexType type, size_t size, const void * data, BufferUsage usage)
{
	// TO DO
	return nullptr;
}

void Magma::Framework::Graphics::OGL400RenderDevice::DestroyIndexBuffer(IndexBuffer * indexBuffer)
{
	// TO DO
}

void Magma::Framework::Graphics::OGL400RenderDevice::SetIndexBuffer(IndexBuffer * indexBuffer)
{
	// TO DO
}

Texture2D * Magma::Framework::Graphics::OGL400RenderDevice::CreateTexture2D(size_t width, size_t height, const void * data, BufferUsage usage)
{
	// TO DO
	return nullptr;
}

void Magma::Framework::Graphics::OGL400RenderDevice::DestroyTexture2D(Texture2D * texture)
{
	// TO DO
}

RasterState * Magma::Framework::Graphics::OGL400RenderDevice::CreateRasterState(const RasterStateDesc & desc)
{
	// TO DO
	return nullptr;
}

void Magma::Framework::Graphics::OGL400RenderDevice::DestroyRasterState(RasterState * rasterState)
{
	// TO DO
}

void Magma::Framework::Graphics::OGL400RenderDevice::SetRasterState(RasterState * rasterState)
{
	// TO DO
}

DepthStencilState * Magma::Framework::Graphics::OGL400RenderDevice::CreateDepthStencilState(const DepthStencilStateDesc & desc)
{
	// TO DO
	return nullptr;
}

void Magma::Framework::Graphics::OGL400RenderDevice::DestroyDepthStencilState(DepthStencilState * depthStencilState)
{
	// TO DO
}

void Magma::Framework::Graphics::OGL400RenderDevice::SetDepthStencilState(DepthStencilState * depthStencilState)
{
	// TO DO
}

void Magma::Framework::Graphics::OGL400RenderDevice::Clear(glm::vec4 color, float depth, int stencil)
{
#if defined(MAGMA_FRAMEWORK_USE_OPENGL)
	glClearColor(color.r, color.g, color.b, color.a);
	glClearDepth(depth);
	glClearStencil(stencil);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
#else
	throw RenderDeviceError("Failed to call OGL400RenderDevice function:\nMAGMA_FRAMEWORK_USE_OPENGL must be defined to use this render device");
#endif
}

void Magma::Framework::Graphics::OGL400RenderDevice::DrawTriangles(size_t offset, size_t count)
{
	// TO DO
}

void Magma::Framework::Graphics::OGL400RenderDevice::DrawTrianglesIndexed(size_t offset, size_t count)
{
	// TO DO
}

void Magma::Framework::Graphics::OGL400RenderDevice::SetRenderTargets(Texture2D ** textures, size_t count)
{
	// TO DO
}

void Magma::Framework::Graphics::OGL400RenderDevice::SwapBuffers()
{
#if defined(MAGMA_FRAMEWORK_USE_OPENGL)
	m_window->SwapBuffers();
#else
	throw RenderDeviceError("Failed to call OGL400RenderDevice function:\nMAGMA_FRAMEWORK_USE_OPENGL must be defined to use this render device");
#endif
}