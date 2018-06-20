#include "GLContext.hpp"

#include <Config.hpp>

#if defined(MAGMA_FRAMEWORK_USE_OPENGL)
#define GLEW_STATIC
#include <GL/glew.h>
#include <sstream>

void Magma::Framework::Graphics::GLContext::Init(Input::Window * window, const ContextSettings& settings)
{
	m_window = dynamic_cast<Input::GLWindow*>(window);
	if (m_window == nullptr)
		throw std::runtime_error("Failed to init GLContext:\nCouldn't cast from Magma::Framework::Input::Window* to Magma::Framework::Input::GLWindow*");

	window->MakeCurrent();
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		std::stringstream ss;
		ss << "Failed to init GLContext:" << std::endl << "GLEW failed to init:" << std::endl;
		ss << (const char*)glewGetErrorString(err);
		throw std::runtime_error(ss.str());
	}
}

void Magma::Framework::Graphics::GLContext::Terminate()
{
	
}

void Magma::Framework::Graphics::GLContext::SetClearColor(glm::vec4 color)
{
	glClearColor(color.r, color.g, color.b, color.a);
}

void Magma::Framework::Graphics::GLContext::Clear(BufferBit mask)
{
	GLbitfield gl_mask = 0;
	if ((mask & BufferBit::Color) != BufferBit::None)
		gl_mask |= GL_COLOR_BUFFER_BIT;
	if ((mask & BufferBit::Depth) != BufferBit::None)
		gl_mask |= GL_DEPTH_BUFFER_BIT;
	if ((mask & BufferBit::Stencil) != BufferBit::None)
		gl_mask |= GL_STENCIL_BUFFER_BIT;
	glClear(gl_mask);
}

void * Magma::Framework::Graphics::GLContext::CreateVertexBuffer(void * data, size_t size, const VertexLayout & layout, void * vertexShader)
{
	return nullptr;
}

void Magma::Framework::Graphics::GLContext::DestroyVertexBuffer(void * vertexBuffer)
{
}

void Magma::Framework::Graphics::GLContext::BindVertexBuffer(void * vertexBuffer)
{
}

void Magma::Framework::Graphics::GLContext::Draw(size_t vertexCount, size_t offset, DrawMode mode)
{
}

void * Magma::Framework::Graphics::GLContext::CreateShader(ShaderType type, const std::string & src)
{
	return nullptr;
}

void Magma::Framework::Graphics::GLContext::DestroyShader(void * shader)
{
}

void * Magma::Framework::Graphics::GLContext::CreateProgram()
{
	return nullptr;
}

void Magma::Framework::Graphics::GLContext::DestroyProgram(void * program)
{
}

void Magma::Framework::Graphics::GLContext::AttachShader(void * program, void * shader)
{
}

void Magma::Framework::Graphics::GLContext::DetachShader(void * program, void * shader)
{
}

void Magma::Framework::Graphics::GLContext::LinkProgram(void * program)
{
}

void Magma::Framework::Graphics::GLContext::BindProgram(void * program)
{
}

void * Magma::Framework::Graphics::GLContext::CreateIndexBuffer(void * data, size_t size, IndexFormat type)
{
	return nullptr;
}

void Magma::Framework::Graphics::GLContext::DestroyIndexBuffer(void * indexBuffer)
{
}

void Magma::Framework::Graphics::GLContext::BindIndexBuffer(void * indexBuffer)
{
}

void Magma::Framework::Graphics::GLContext::DrawIndexed(size_t indexCount, size_t offset, DrawMode mode)
{
}

void Magma::Framework::Graphics::GLContext::SwapBuffers()
{
	m_window->SwapBuffers();
}
#else
void Magma::Framework::Graphics::GLContext::Init(Input::Window * window, const ContextSettings& settings)
{
	throw std::runtime_error("Failed to init GLContext:\nThe project wasn't built for OpenGL (MAGMA_FRAMEWORK_USE_OPENGL must be defined)");
}

void Magma::Framework::Graphics::GLContext::Terminate()
{
	throw std::runtime_error("Failed to terminate GLContext:\nThe project wasn't built for OpenGL (MAGMA_FRAMEWORK_USE_OPENGL must be defined)");
}

void Magma::Framework::Graphics::GLContext::SetClearColor(glm::vec4 color)
{
	throw std::runtime_error("Failed to set clear color on GLContext:\nThe project wasn't built for OpenGL (MAGMA_FRAMEWORK_USE_OPENGL must be defined)");
}

void Magma::Framework::Graphics::GLContext::Clear(BufferBit mask)
{
	throw std::runtime_error("Failed to clear GLContext:\nThe project wasn't built for OpenGL (MAGMA_FRAMEWORK_USE_OPENGL must be defined)");
}

void * Magma::Framework::Graphics::GLContext::CreateVertexBuffer(void * data, size_t size, const VertexLayout & layout, void * vertexShader)
{
	throw std::runtime_error("Failed to create vertex buffer on GLContext:\nThe project wasn't built for OpenGL (MAGMA_FRAMEWORK_USE_OPENGL must be defined)");
}

void Magma::Framework::Graphics::GLContext::DestroyVertexBuffer(void * vertexBuffer)
{
	throw std::runtime_error("Failed to destroy vertex buffer on GLContext:\nThe project wasn't built for OpenGL (MAGMA_FRAMEWORK_USE_OPENGL must be defined)");
}

void Magma::Framework::Graphics::GLContext::BindVertexBuffer(void * vertexBuffer)
{
	throw std::runtime_error("Failed to bind vertex buffer on GLContext:\nThe project wasn't built for OpenGL (MAGMA_FRAMEWORK_USE_OPENGL must be defined)");
}

void Magma::Framework::Graphics::GLContext::Draw(size_t vertexCount, size_t offset, DrawMode mode)
{
	throw std::runtime_error("Failed to draw on GLContext:\nThe project wasn't built for OpenGL (MAGMA_FRAMEWORK_USE_OPENGL must be defined)");
}

void * Magma::Framework::Graphics::GLContext::CreateShader(ShaderType type, const std::string & src)
{
	throw std::runtime_error("Failed to create shader on GLContext:\nThe project wasn't built for OpenGL (MAGMA_FRAMEWORK_USE_OPENGL must be defined)");
}

void Magma::Framework::Graphics::GLContext::DestroyShader(void * shader)
{
	throw std::runtime_error("Failed to destroy shader GLContext:\nThe project wasn't built for OpenGL (MAGMA_FRAMEWORK_USE_OPENGL must be defined)");
}

void * Magma::Framework::Graphics::GLContext::CreateProgram()
{
	throw std::runtime_error("Failed to create program on GLContext:\nThe project wasn't built for OpenGL (MAGMA_FRAMEWORK_USE_OPENGL must be defined)");
}

void Magma::Framework::Graphics::GLContext::DestroyProgram(void * program)
{
	throw std::runtime_error("Failed to destroy program GLContext:\nThe project wasn't built for OpenGL (MAGMA_FRAMEWORK_USE_OPENGL must be defined)");
}

void Magma::Framework::Graphics::GLContext::AttachShader(void * program, void * shader)
{
	throw std::runtime_error("Failed to attach shader on GLContext:\nThe project wasn't built for OpenGL (MAGMA_FRAMEWORK_USE_OPENGL must be defined)");
}

void Magma::Framework::Graphics::GLContext::DetachShader(void * program, void * shader)
{
	throw std::runtime_error("Failed to detach shader on GLContext:\nThe project wasn't built for OpenGL (MAGMA_FRAMEWORK_USE_OPENGL must be defined)");
}

void Magma::Framework::Graphics::GLContext::LinkProgram(void * program)
{
	throw std::runtime_error("Failed to link program on GLContext:\nThe project wasn't built for OpenGL (MAGMA_FRAMEWORK_USE_OPENGL must be defined)");
}

void Magma::Framework::Graphics::GLContext::BindProgram(void * program)
{
	throw std::runtime_error("Failed to bind program on GLContext:\nThe project wasn't built for OpenGL (MAGMA_FRAMEWORK_USE_OPENGL must be defined)");
}

void * Magma::Framework::Graphics::GLContext::CreateIndexBuffer(void * data, size_t size, IndexFormat type)
{
	throw std::runtime_error("Failed to create index bufer on GLContext:\nThe project wasn't built for OpenGL (MAGMA_FRAMEWORK_USE_OPENGL must be defined)");
}

void Magma::Framework::Graphics::GLContext::DestroyIndexBuffer(void * indexBuffer)
{
	throw std::runtime_error("Failed to destroy index buffer on GLContext:\nThe project wasn't built for OpenGL (MAGMA_FRAMEWORK_USE_OPENGL must be defined)");
}

void Magma::Framework::Graphics::GLContext::BindIndexBuffer(void * indexBuffer)
{
	throw std::runtime_error("Failed to bind index buffer on GLContext:\nThe project wasn't built for OpenGL (MAGMA_FRAMEWORK_USE_OPENGL must be defined)");
}

void Magma::Framework::Graphics::GLContext::DrawIndexed(size_t indexCount, size_t offset, DrawMode mode)
{
	throw std::runtime_error("Failed to draw (indexed) on GLContext:\nThe project wasn't built for OpenGL (MAGMA_FRAMEWORK_USE_OPENGL must be defined)");
}

void Magma::Framework::Graphics::GLContext::SwapBuffers()
{
	throw std::runtime_error("Failed to swap buffers on GLContext:\nThe project wasn't built for OpenGL (MAGMA_FRAMEWORK_USE_OPENGL must be defined)");
}
#endif