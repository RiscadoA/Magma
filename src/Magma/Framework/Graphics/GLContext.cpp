#include "GLContext.hpp"

#include <Config.hpp>

#if defined(MAGMA_FRAMEWORK_USE_OPENGL)
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <sstream>

#if defined(MAGMA_FRAMEWORK_CONTEXT_DEBUG)
#define GL_CHECK_ERROR(title)\
{\
	GLenum err = glGetError();\
	if (err != GL_NO_ERROR)\
	{\
		std::stringstream ss;\
		ss << (title) << ":" << std::endl;\
		ss << "OpenGL error " << err << ":" << std::endl;\
		ss << (const char*)glewGetErrorString(err);\
		throw std::runtime_error(ss.str());\
	}\
}
#else
#define GL_CHECK_ERROR(title)
#endif

#include "MFXToGLSL.hpp"

#define MAX_BINDING_POINTS 16 

struct VertexBuffer
{
	GLuint vao;
	GLuint vbo;
};

struct IndexBuffer
{
	GLuint ibo;
	GLenum type;
};

struct BindingPoint
{
	GLuint index = 0;
	std::string name = "";
	bool exists = false;
};

struct ShaderProgram
{
	GLuint shaderProgram;
	BindingPoint bindingPoints[MAX_BINDING_POINTS];
};

struct Shader
{
	GLuint shader;
};

struct ConstantBuffer
{
	GLuint ubo;
	size_t size;
};

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

	if (settings.enableVsync)
		glfwSwapInterval(1);
	else
		glfwSwapInterval(0);

	GL_CHECK_ERROR("Failed to init GLContext");
}

void Magma::Framework::Graphics::GLContext::Terminate()
{
	
}

void Magma::Framework::Graphics::GLContext::SetClearColor(glm::vec4 color)
{
	glClearColor(color.r, color.g, color.b, color.a);
	GL_CHECK_ERROR("Failed to set clear color on GLContext");
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
	GL_CHECK_ERROR("Failed to clear on GLContext");
}

void * Magma::Framework::Graphics::GLContext::CreateVertexBuffer(void * data, size_t size, const VertexLayout & layout, void * program)
{
	VertexBuffer* vertexBuffer = new VertexBuffer();

	glCreateVertexArrays(1, &vertexBuffer->vao);
	glBindVertexArray(vertexBuffer->vao);

	GL_CHECK_ERROR("Failed to create vertex buffer on GLContext {1}");

	try { GL_CHECK_ERROR("Failed to create vertex buffer on GLContext {1}"); }
	catch (...)
	{
		glDeleteVertexArrays(1, &vertexBuffer->vao);
		delete vertexBuffer;
		throw;
	}

	glCreateBuffers(1, &vertexBuffer->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer->vbo);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

	try { GL_CHECK_ERROR("Failed to create vertex buffer on GLContext {2}"); }
	catch (...)
	{
		glDeleteVertexArrays(1, &vertexBuffer->vao);
		glDeleteBuffers(1, &vertexBuffer->vbo);
		delete vertexBuffer;
		throw;
	}

	for (auto& l : layout.elements)
	{
		void* offset = reinterpret_cast<void*>(l.offset);
		auto pos = glGetAttribLocation(((ShaderProgram*)program)->shaderProgram, l.name.c_str());
		if (pos == -1)
		{
			std::stringstream ss;
			ss << "Failed to create vertex buffer on GLContext:" << std::endl;
			ss << "Couldn't find attribute \"" << l.name << "\"";
			throw std::runtime_error(ss.str());
		}
		switch (l.format)
		{
			case VertexElementFormat::Float1: glVertexAttribPointer(pos, 1, GL_FLOAT, GL_FALSE, layout.size, offset); break;
			case VertexElementFormat::Float2: glVertexAttribPointer(pos, 2, GL_FLOAT, GL_FALSE, layout.size, offset); break;
			case VertexElementFormat::Float3: glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, layout.size, offset); break;
			case VertexElementFormat::Float4: glVertexAttribPointer(pos, 4, GL_FLOAT, GL_FALSE, layout.size, offset); break;
			case VertexElementFormat::Int1: glVertexAttribIPointer(pos, 1, GL_INT, layout.size, offset); break;
			case VertexElementFormat::Int2: glVertexAttribIPointer(pos, 2, GL_INT, layout.size, offset); break;
			case VertexElementFormat::Int3: glVertexAttribIPointer(pos, 3, GL_INT, layout.size, offset); break;
			case VertexElementFormat::Int4: glVertexAttribIPointer(pos, 4, GL_INT, layout.size, offset); break;
			case VertexElementFormat::UInt1: glVertexAttribIPointer(pos, 1, GL_UNSIGNED_INT, layout.size, offset); break;
			case VertexElementFormat::UInt2: glVertexAttribIPointer(pos, 2, GL_UNSIGNED_INT, layout.size, offset); break;
			case VertexElementFormat::UInt3: glVertexAttribIPointer(pos, 3, GL_UNSIGNED_INT, layout.size, offset); break;
			case VertexElementFormat::UInt4: glVertexAttribIPointer(pos, 4, GL_UNSIGNED_INT, layout.size, offset); break;
			default: delete vertexBuffer; throw std::runtime_error("Failed to create vertex buffer on GLContext:\nUnsupported vertex element format"); break;
		}
		glEnableVertexAttribArray(pos);
	}

	try { GL_CHECK_ERROR("Failed to create vertex buffer on GLContext {3}"); }
	catch (...)
	{
		glDeleteVertexArrays(1, &vertexBuffer->vao);
		glDeleteBuffers(1, &vertexBuffer->vbo);
		delete vertexBuffer;
		throw;
	}

	return vertexBuffer;
}

void Magma::Framework::Graphics::GLContext::DestroyVertexBuffer(void * vertexBuffer)
{
	auto buffer = (VertexBuffer*)vertexBuffer;
	glDeleteVertexArrays(1, &buffer->vao);
	glDeleteBuffers(1, &buffer->vbo);
	delete buffer;

	GL_CHECK_ERROR("Failed to destroy vertex buffer on GLContext");
}

void Magma::Framework::Graphics::GLContext::BindVertexBuffer(void * vertexBuffer)
{
	if (vertexBuffer == nullptr)
	{
		glBindVertexArray(0);
		GL_CHECK_ERROR("Failed to bind vertex buffer on GLContext {1}");
		return;
	}
	auto buffer = (VertexBuffer*)vertexBuffer;
	glBindVertexArray(buffer->vao);
	GL_CHECK_ERROR("Failed to bind vertex buffer on GLContext {2}");
}

void Magma::Framework::Graphics::GLContext::Draw(size_t vertexCount, size_t offset, DrawMode mode)
{
	GLenum gl_mode;
	switch (mode)
	{
		case DrawMode::Points: gl_mode = GL_POINTS; break;
		case DrawMode::Lines: gl_mode = GL_LINES; break;
		case DrawMode::LineStrip: gl_mode = GL_LINE_STRIP; break;
		case DrawMode::Triangles: gl_mode = GL_TRIANGLES; break;
		case DrawMode::TriangleStrip: gl_mode = GL_TRIANGLE_STRIP; break;
		default: throw std::runtime_error("Failed to draw on GLContext:\nUnsupported draw mode"); break;
	}
	glDrawArrays(gl_mode, offset, vertexCount);
	GL_CHECK_ERROR("Failed to draw on GLContext");
}

void * Magma::Framework::Graphics::GLContext::CreateShader(ShaderType type, const std::string & src)
{
	auto shader = new Shader();

	GLenum gl_type = 0;
	switch (type)
	{
		case ShaderType::Vertex: gl_type = GL_VERTEX_SHADER; break;
		case ShaderType::Pixel: gl_type = GL_FRAGMENT_SHADER; break;
		default: delete shader; throw std::runtime_error("Failed to create shader on GLContext:\nUnsupported shader type"); break;
	}

	std::string compiledSrc;
	try
	{
		Compile(src, compiledSrc, type);
	}
	catch (std::runtime_error& e)
	{
		delete shader;

		std::stringstream ss;
		ss << "Failed to create shader on GLContext:" << std::endl;
		ss << "Failed to compile from MFX to GLSL on GLContext:" << std::endl;
		ss << e.what();
		throw std::runtime_error(ss.str());
	}

	shader->shader = glCreateShader(gl_type);
	const char* source = compiledSrc.c_str();
	glShaderSource(shader->shader, 1, &source, NULL);
	glCompileShader(shader->shader);
	try { GL_CHECK_ERROR("Failed to create shader on GLContext"); }
	catch (...)
	{
		glDeleteShader(shader->shader);
		delete shader;
		throw;
	}

	GLint compileStatus;
	glGetShaderiv(shader->shader, GL_COMPILE_STATUS, &compileStatus);
	if (compileStatus == GL_FALSE)
	{
		GLint infoLogLength;
		glGetShaderiv(shader->shader, GL_INFO_LOG_LENGTH, &infoLogLength);

		auto infoLog = new GLchar[infoLogLength];
		glGetShaderInfoLog(shader->shader, infoLogLength, &infoLogLength, infoLog);

		glDeleteShader(shader->shader);
		delete shader;

		std::stringstream ss;
		ss << "Failed to create shader on GLContext:" << std::endl << "Shader compilation failed:" << std::endl;
		ss << infoLog;
		delete[] infoLog;
	
		throw std::runtime_error(ss.str());
	}

	return shader;
}

void Magma::Framework::Graphics::GLContext::DestroyShader(void * shader)
{
	auto shd = (Shader*)shader;
	glDeleteShader(shd->shader);
	GL_CHECK_ERROR("Failed to destroy shader on GLContext");
}

void * Magma::Framework::Graphics::GLContext::CreateProgram()
{
	auto program = new ShaderProgram();
	program->shaderProgram = glCreateProgram();
	try { GL_CHECK_ERROR("Failed to create program on GLContext"); }
	catch (...)
	{
		delete program;
		throw;
	}
	return program;
}

void Magma::Framework::Graphics::GLContext::DestroyProgram(void * program)
{
	auto prgm = (ShaderProgram*)program;
	glDeleteProgram(prgm->shaderProgram);
	GL_CHECK_ERROR("Failed to destroy program on GLContext");
}

void Magma::Framework::Graphics::GLContext::AttachShader(void * program, void * shader)
{
	auto prgm = (ShaderProgram*)program;
	auto shd = (Shader*)shader;
	glAttachShader(prgm->shaderProgram, shd->shader);
	GL_CHECK_ERROR("Failed to attach shader on GLContext");
}

void Magma::Framework::Graphics::GLContext::DetachShader(void * program, void * shader)
{
	auto prgm = (ShaderProgram*)program;
	auto shd = (Shader*)shader;
	glDetachShader(prgm->shaderProgram, shd->shader);
	GL_CHECK_ERROR("Failed to detach shader on GLContext");
}

void Magma::Framework::Graphics::GLContext::LinkProgram(void * program)
{
	auto prgm = (ShaderProgram*)program;
	glLinkProgram(prgm->shaderProgram);
	GL_CHECK_ERROR("Failed to link program on GLContext");
	GLint linkStatus;
	glGetProgramiv(prgm->shaderProgram, GL_LINK_STATUS, &linkStatus);
	if (linkStatus == GL_FALSE)
	{
		GLint infoLogLength;
		glGetProgramiv(prgm->shaderProgram, GL_INFO_LOG_LENGTH, &infoLogLength);

		auto infoLog = new GLchar[infoLogLength];
		glGetProgramInfoLog(prgm->shaderProgram, infoLogLength, &infoLogLength, infoLog);

		std::stringstream ss;
		ss << "Failed to link program on GLContext:\nShader program link failed:\n";
		ss << infoLog;
		delete[] infoLog;
		throw std::runtime_error(ss.str());
	}

	for (size_t i = 0; i < MAX_BINDING_POINTS; ++i)
	{
		prgm->bindingPoints[i].exists = false;
		prgm->bindingPoints[i].index = 0;
		prgm->bindingPoints[i].name = "";
	}
}

void Magma::Framework::Graphics::GLContext::BindProgram(void * program)
{
	if (program == nullptr)
	{
		glUseProgram(0);
		GL_CHECK_ERROR("Failed to bind program on GLContext {1}");
		return;
	}

	auto prgm = (ShaderProgram*)program;
	glUseProgram(prgm->shaderProgram);
	GL_CHECK_ERROR("Failed to bind program on GLContext {2}");
}

void * Magma::Framework::Graphics::GLContext::CreateIndexBuffer(void * data, size_t size, IndexFormat type)
{
	auto buffer = new IndexBuffer();

	glCreateBuffers(1, &buffer->ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer->ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

	try
	{
		GL_CHECK_ERROR("Failed to create index buffer on GLContext");

		switch (type)
		{
			case IndexFormat::UInt8: buffer->type = GL_UNSIGNED_BYTE; break;
			case IndexFormat::UInt16: buffer->type = GL_UNSIGNED_SHORT; break;
			case IndexFormat::UInt32: buffer->type = GL_UNSIGNED_INT; break;
			case IndexFormat::Invalid: throw std::runtime_error("Failed to create index buffer on GLContext:\nPassed invalid index format"); break;
			default: throw std::runtime_error("Failed to create index buffer on GLContext:\nUnsupported index format"); break;
		}
	}
	catch (...)
	{
		glDeleteBuffers(1, &buffer->ibo);
		delete buffer;
		throw;
	}

	return buffer;
}

void Magma::Framework::Graphics::GLContext::DestroyIndexBuffer(void * indexBuffer)
{
	auto buffer = (IndexBuffer*)indexBuffer;
	glDeleteBuffers(1, &buffer->ibo);
	delete buffer;
	GL_CHECK_ERROR("Failed to delete index buffer on GLContext");
}

void Magma::Framework::Graphics::GLContext::BindIndexBuffer(void * indexBuffer)
{
	m_boundIndexBuffer = indexBuffer;
	if (m_boundIndexBuffer == nullptr)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		return;
	}
	auto buffer = (IndexBuffer*)indexBuffer;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer->ibo);
}

void Magma::Framework::Graphics::GLContext::DrawIndexed(size_t indexCount, size_t offset, DrawMode mode)
{
	GLenum gl_mode;
	switch (mode)
	{
	case DrawMode::Points: gl_mode = GL_POINTS; break;
	case DrawMode::Lines: gl_mode = GL_LINES; break;
	case DrawMode::LineStrip: gl_mode = GL_LINE_STRIP; break;
	case DrawMode::Triangles: gl_mode = GL_TRIANGLES; break;
	case DrawMode::TriangleStrip: gl_mode = GL_TRIANGLE_STRIP; break;
	default: throw std::runtime_error("Failed to draw (indexed) on GLContext:\nUnsupported draw mode"); break;
	}
	glDrawElements(gl_mode, indexCount, ((IndexBuffer*)m_boundIndexBuffer)->type, reinterpret_cast<void*>(offset));
	GL_CHECK_ERROR("Failed to draw (indexed) on GLContext");
}

void * Magma::Framework::Graphics::GLContext::CreateConstantBuffer(void * data, size_t size)
{
	auto constantBuffer = new ConstantBuffer();
	constantBuffer->size = size;

	try
	{
		glGenBuffers(1, &constantBuffer->ubo);
		GL_CHECK_ERROR("Failed to create constant buffer {1}");

		try
		{
			glBindBuffer(GL_UNIFORM_BUFFER, constantBuffer->ubo);
			glBufferData(GL_UNIFORM_BUFFER, size, data, GL_STATIC_DRAW);
			GL_CHECK_ERROR("Failed to create constant buffer {2}");
		}
		catch (...)
		{
			glDeleteBuffers(1, &constantBuffer->ubo);
			throw;
		}
	}
	catch (...)
	{
		delete constantBuffer;
		throw;
	}

	return constantBuffer;
}

void Magma::Framework::Graphics::GLContext::DestroyConstantBuffer(void * constantBuffer)
{
	auto buffer = (ConstantBuffer*)constantBuffer;
	glDeleteBuffers(1, &buffer->ubo);
	delete buffer;
	GL_CHECK_ERROR("Failed to destroy constant buffer");
}

void Magma::Framework::Graphics::GLContext::UpdateConstantBuffer(void * constantBuffer, void * data)
{
	auto buffer = (ConstantBuffer*)constantBuffer;
	glBindBuffer(GL_UNIFORM_BUFFER, buffer->ubo);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, buffer->size, data);
	GL_CHECK_ERROR("Failed to update constant buffer");
}

void Magma::Framework::Graphics::GLContext::BindConstantBuffer(void * constantBuffer, void * bindingPoint)
{
	auto buffer = (ConstantBuffer*)constantBuffer;
	auto bind = (BindingPoint*)bindingPoint;

	if (buffer == nullptr)
	{
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
		glBindBufferBase(GL_UNIFORM_BUFFER, bind->index, 0);
		GL_CHECK_ERROR("Failed to bind constant buffer {1}");
	}
	else
	{
		glBindBuffer(GL_UNIFORM_BUFFER, buffer->ubo);
		glBindBufferBase(GL_UNIFORM_BUFFER, bind->index, buffer->ubo);
		GL_CHECK_ERROR("Failed to bind constant buffer {2}");
	}
}

void * Magma::Framework::Graphics::GLContext::GetBindingPoint(void * program, const std::string & name)
{
	if (name == "")
		throw std::runtime_error("Failed to get binding point from GLContext:\nName cannot be empty");

	auto prgm = (ShaderProgram*)program;

	BindingPoint* bind = nullptr;
	for (size_t i = 0; i < MAX_BINDING_POINTS; ++i)
	{
		if (!prgm->bindingPoints[i].exists)
		{
			bind = &prgm->bindingPoints[i];
			continue;
		}

		if (prgm->bindingPoints[i].name == name)
			return &prgm->bindingPoints[i];
	}

	if (bind == nullptr)
		throw std::runtime_error("Failed to get binding point from GLContext:\nBinding point count limit has been achieved");
	
	bind->index = glGetUniformBlockIndex(prgm->shaderProgram, name.c_str());
	if (bind->index == -1)
	{
		std::stringstream ss;
		ss << "Failed to get binding point from GLContext:" << std::endl;
		ss << "No binding point with name \"" << name << "\"" << std::endl;
		throw std::runtime_error(ss.str());
	}

	glUniformBlockBinding(prgm->shaderProgram, bind->index, bind->index);

	bind->name = name;
	bind->exists = true;
	GL_CHECK_ERROR("Failed to get binding point from GLContext");
	return bind;
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

void * Magma::Framework::Graphics::GLContext::CreateConstantBuffer(void * data, size_t size)
{
	throw std::runtime_error("Failed to create constant buffer on GLContext:\nThe project wasn't built for OpenGL (MAGMA_FRAMEWORK_USE_OPENGL must be defined)");
}

void Magma::Framework::Graphics::GLContext::DestroyConstantBuffer(void * constantBuffer)
{
	throw std::runtime_error("Failed to destroy constant buffer on GLContext:\nThe project wasn't built for OpenGL (MAGMA_FRAMEWORK_USE_OPENGL must be defined)");
}

void Magma::Framework::Graphics::GLContext::UpdateConstantBuffer(void * constantBuffer, void * data)
{
	throw std::runtime_error("Failed to update constant buffer on GLContext:\nThe project wasn't built for OpenGL (MAGMA_FRAMEWORK_USE_OPENGL must be defined)");
}

void Magma::Framework::Graphics::GLContext::BindConstantBuffer(void * constantBuffer, const std::string & name)
{
	throw std::runtime_error("Failed to bind constant buffer on GLContext:\nThe project wasn't built for OpenGL (MAGMA_FRAMEWORK_USE_OPENGL must be defined)");
}
#endif