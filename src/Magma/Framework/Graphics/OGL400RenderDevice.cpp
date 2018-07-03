#include "OGL400RenderDevice.hpp"
#include "OGL400Assembler.hpp"

using namespace Magma::Framework::Graphics;
using namespace Magma::Framework;

#include <Config.hpp>
#include <sstream>
#include <string>
#include <map>

#if defined(MAGMA_FRAMEWORK_USE_OPENGL)
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#if defined(MAGMA_FRAMEWORK_RENDER_DEVICE_DEBUG)
#define GL_CHECK_ERROR(title)\
{\
	GLenum err = glGetError();\
	if (err != GL_NO_ERROR)\
	{\
		std::stringstream ss;\
		ss << (title) << ":" << std::endl;\
		ss << "OpenGL error: " << err;\
		throw RenderDeviceError(ss.str());\
	}\
}
#else
#define GL_CHECK_ERROR(title)
#endif
#endif

class OGL400VertexShader final : public VertexShader
{
public:
	OGL400VertexShader(const char* source, const ShaderData& _data)
		: data(_data)
	{
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &source, NULL);
		glCompileShader(vertexShader);

		GLint success;
		GLchar infoLog[512];
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			std::stringstream ss;
			ss << "Failed to create OGL400VertexShader:" << std::endl;
			ss << "Compilation failed:" << std::endl;
			ss << infoLog;
			throw std::runtime_error(ss.str());
		}

		GL_CHECK_ERROR("Failed to create OGL400VertexShader");
	}

	virtual ~OGL400VertexShader() final
	{
		glDeleteShader(vertexShader);
	}

	GLuint vertexShader;
	ShaderData data;
};

class OGL400PixelShader final : public PixelShader
{
public:
	OGL400PixelShader(const char* source, const ShaderData& _data)
		: data(_data)
	{
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &source, NULL);
		glCompileShader(fragmentShader);

		GLint success;
		GLchar infoLog[512];
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
			std::stringstream ss;
			ss << "Failed to create OGL400PixelShader:" << std::endl;
			ss << "Compilation failed:" << std::endl;
			ss << infoLog;
			throw std::runtime_error(ss.str());
		}

		GL_CHECK_ERROR("Failed to create OGL400PixelShader");
	}

	virtual ~OGL400PixelShader() final
	{
		glDeleteShader(fragmentShader);
	}

	GLuint fragmentShader;
	ShaderData data;
};

class OGL400PipelineBindingPoint;

class OGL400Pipeline final : public Pipeline
{
public:
	OGL400Pipeline(OGL400VertexShader* vertexShader, OGL400PixelShader* pixelShader)
	{
		shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vertexShader->vertexShader);
		glAttachShader(shaderProgram, pixelShader->fragmentShader);
		glLinkProgram(shaderProgram);

		GLint success;
		GLchar infoLog[512];
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
			std::stringstream ss;
			ss << "Failed to create OGL400Pipeline:" << std::endl;
			ss << "Shader program linking failed:" << std::endl;
			ss << infoLog;
			throw std::runtime_error(ss.str());
		}

		GL_CHECK_ERROR("Failed to create OGL400Pipeline");
	}

	virtual ~OGL400Pipeline() final
	{
		glDeleteProgram(shaderProgram);
	}

	virtual PipelineBindingPoint * GetBindingPoint(const char * name) final;

	GLuint shaderProgram;

	std::unordered_map<std::string, OGL400PipelineBindingPoint*> bindingPoints;
};

class OGL400PipelineBindingPoint final : public PipelineBindingPoint
{
public:
	OGL400PipelineBindingPoint(OGL400Pipeline* _pipeline, GLint _location)
		: pipeline(_pipeline), location(_location)
	{
		// Empty
	}

	virtual ~OGL400PipelineBindingPoint() final
	{

	}

	virtual void Bind(Texture2D* texture) final
	{
		// TO DO
	}

	virtual void Bind(ConstantBuffer* buffer) final
	{
		// TO DO
	}

	OGL400Pipeline* pipeline;
	GLint location;
};


PipelineBindingPoint * OGL400Pipeline::GetBindingPoint(const char * name)
{
	// TO DO
	return nullptr;
}

class OGL400VertexBuffer final : public VertexBuffer
{
public:
	OGL400VertexBuffer(GLsizei size, const GLvoid* data, BufferUsage _usage)
	{
		GLenum usage;

		switch (_usage)
		{
			case BufferUsage::Default: usage = GL_STATIC_DRAW; break; // Default and static are the same in OGL400
			case BufferUsage::Static: usage = GL_STATIC_DRAW; break;
			case BufferUsage::Dynamic: usage = GL_DYNAMIC_DRAW; break;
			case BufferUsage::Invalid: throw RenderDeviceError("Failed to create OGL400VertexBuffer:\nInvalid buffer usage mode"); break;
			default: throw RenderDeviceError("Failed to create OGL400VertexBuffer:\nUnsupported buffer usage mode"); break;
		}

		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, size, data, usage);

		GL_CHECK_ERROR("Failed to create OGL400VertexBuffer");
	}

	virtual ~OGL400VertexBuffer() final
	{
		glDeleteBuffers(1, &vbo);
	}
	
	GLuint vbo;
};

class OGL400VertexLayout final : public VertexLayout
{
public:
	struct OGL400VertexElement
	{
		GLuint bufferIndex;
		GLuint index;
		GLuint size;
		GLenum type;
		GLboolean normalized;
		GLsizei stride;
		const GLvoid* offset;
		bool isInteger;
	};

	OGL400VertexLayout(GLuint _vertexElementCount, const VertexElement* vertexElements, OGL400VertexShader* vertexShader)
		: vertexElementCount(_vertexElementCount)
	{
		oglVertexElements = new OGL400VertexElement[vertexElementCount];
		for (GLuint i = 0; i < vertexElementCount; ++i)
		{
			oglVertexElements[i].index = -1;
			for (auto& v : vertexShader->data.GetInputVariables())
				if (v.name == vertexElements[i].name)
				{
					oglVertexElements[i].index = v.index;
					break;	
				}
			if (oglVertexElements[i].index == -1)
			{
				std::stringstream ss;
				ss << "Failed to create OGL400VertexLayout:" << std::endl;
				ss << "Couldn't find any vertex shader input variable with the name \"" << vertexElements[i].name << "\"";
				throw RenderDeviceError(ss.str());
			}

			switch (vertexElements[i].type)
			{
				case VertexElementType::Byte: oglVertexElements[i].type = GL_BYTE; oglVertexElements[i].normalized = GL_FALSE; oglVertexElements[i].isInteger = true; break;
				case VertexElementType::Short: oglVertexElements[i].type = GL_SHORT; oglVertexElements[i].normalized = GL_FALSE; oglVertexElements[i].isInteger = true; break;
				case VertexElementType::Int: oglVertexElements[i].type = GL_INT; oglVertexElements[i].normalized = GL_FALSE; oglVertexElements[i].isInteger = true; break;
				case VertexElementType::UByte: oglVertexElements[i].type = GL_UNSIGNED_BYTE; oglVertexElements[i].normalized = GL_FALSE; oglVertexElements[i].isInteger = true; break;
				case VertexElementType::UShort: oglVertexElements[i].type = GL_UNSIGNED_SHORT; oglVertexElements[i].normalized = GL_FALSE; oglVertexElements[i].isInteger = true; break;
				case VertexElementType::UInt: oglVertexElements[i].type = GL_UNSIGNED_INT; oglVertexElements[i].normalized = GL_FALSE; oglVertexElements[i].isInteger = true; break;

				case VertexElementType::FByte: oglVertexElements[i].type = GL_BYTE; oglVertexElements[i].normalized = GL_FALSE; oglVertexElements[i].isInteger = false; break;
				case VertexElementType::FShort: oglVertexElements[i].type = GL_SHORT; oglVertexElements[i].normalized = GL_FALSE; oglVertexElements[i].isInteger = false; break;
				case VertexElementType::FInt: oglVertexElements[i].type = GL_INT; oglVertexElements[i].normalized = GL_FALSE; oglVertexElements[i].isInteger = false; break;
				case VertexElementType::FUByte: oglVertexElements[i].type = GL_UNSIGNED_BYTE; oglVertexElements[i].normalized = GL_FALSE; oglVertexElements[i].isInteger = false; break;
				case VertexElementType::FUShort: oglVertexElements[i].type = GL_UNSIGNED_SHORT; oglVertexElements[i].normalized = GL_FALSE; oglVertexElements[i].isInteger = false; break;
				case VertexElementType::FUInt: oglVertexElements[i].type = GL_UNSIGNED_INT; oglVertexElements[i].normalized = GL_FALSE; oglVertexElements[i].isInteger = false; break;

				case VertexElementType::NByte: oglVertexElements[i].type = GL_BYTE; oglVertexElements[i].normalized = GL_TRUE; oglVertexElements[i].isInteger = false; break;
				case VertexElementType::NShort: oglVertexElements[i].type = GL_SHORT; oglVertexElements[i].normalized = GL_TRUE; oglVertexElements[i].isInteger = false; break;
				case VertexElementType::NInt: oglVertexElements[i].type = GL_INT; oglVertexElements[i].normalized = GL_TRUE; oglVertexElements[i].isInteger = false; break;
				case VertexElementType::NUByte: oglVertexElements[i].type = GL_UNSIGNED_BYTE; oglVertexElements[i].normalized = GL_TRUE; oglVertexElements[i].isInteger = false; break;
				case VertexElementType::NUShort: oglVertexElements[i].type = GL_UNSIGNED_SHORT; oglVertexElements[i].normalized = GL_TRUE; oglVertexElements[i].isInteger = false; break;
				case VertexElementType::NUInt: oglVertexElements[i].type = GL_UNSIGNED_INT; oglVertexElements[i].normalized = GL_TRUE; oglVertexElements[i].isInteger = false; break;

				case VertexElementType::HalfFloat: oglVertexElements[i].type = GL_HALF_FLOAT; oglVertexElements[i].normalized = GL_FALSE; oglVertexElements[i].isInteger = false; break;
				case VertexElementType::Float: oglVertexElements[i].type = GL_FLOAT; oglVertexElements[i].normalized = GL_FALSE; oglVertexElements[i].isInteger = false; break;
				case VertexElementType::Double: oglVertexElements[i].type = GL_DOUBLE; oglVertexElements[i].normalized = GL_FALSE; oglVertexElements[i].isInteger = false; break;

				case VertexElementType::Invalid: throw RenderDeviceError("Failed to create OGL400VertexLayout:\nInvalid vertex element type"); break;
				default: throw RenderDeviceError("Failed to create OGL400VertexLayout:\nUnsupported vertex element type"); break;
			}

			oglVertexElements[i].bufferIndex = vertexElements[i].bufferIndex;
			oglVertexElements[i].size = vertexElements[i].size;
			oglVertexElements[i].stride = vertexElements[i].stride;
			oglVertexElements[i].offset = (char*)nullptr + vertexElements[i].offset;
		}
	}

	virtual ~OGL400VertexLayout() final
	{
		delete[] oglVertexElements;
	}

	GLuint vertexElementCount;
	OGL400VertexElement* oglVertexElements = nullptr;
};

class OGL400VertexArray final : public VertexArray
{
public:
	OGL400VertexArray(GLuint bufferCount, VertexBuffer** buffers, VertexLayout* layout)
	{
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		auto oglLayout = static_cast<OGL400VertexLayout*>(layout);

		for (GLuint i = 0; i < bufferCount; ++i)
		{
			auto buffer = static_cast<OGL400VertexBuffer*>(buffers[i]);
			
			glBindBuffer(GL_ARRAY_BUFFER, buffer->vbo);

			for (GLuint j = 0; j < oglLayout->vertexElementCount; ++j)
			{
				if (oglLayout->oglVertexElements[j].bufferIndex != i)
					continue;
				glEnableVertexAttribArray(oglLayout->oglVertexElements[j].index);
				if (oglLayout->oglVertexElements[j].isInteger)
					glVertexAttribIPointer(oglLayout->oglVertexElements[j].index, oglLayout->oglVertexElements[j].size, oglLayout->oglVertexElements[j].type, oglLayout->oglVertexElements[j].stride, oglLayout->oglVertexElements[j].offset);
				else
					glVertexAttribPointer(oglLayout->oglVertexElements[j].index, oglLayout->oglVertexElements[j].size, oglLayout->oglVertexElements[j].type, oglLayout->oglVertexElements[j].normalized, oglLayout->oglVertexElements[j].stride, oglLayout->oglVertexElements[j].offset);
			}
		}

		GL_CHECK_ERROR("Failed to create OGL400VertexBuffer");
	}

	virtual ~OGL400VertexArray() final
	{
		glDeleteVertexArrays(1, &vao);
	}

	GLuint vao;
};

class OGL400IndexBuffer final : public IndexBuffer
{
public:
	OGL400IndexBuffer(IndexType _type, GLsizei size, const void* data, BufferUsage _usage)
	{
		GLenum usage;

		switch (_usage)
		{
			case BufferUsage::Default: usage = GL_STATIC_DRAW; break; // Default and static are the same in OGL400
			case BufferUsage::Static: usage = GL_STATIC_DRAW; break;
			case BufferUsage::Dynamic: usage = GL_DYNAMIC_DRAW; break;
			case BufferUsage::Invalid: throw RenderDeviceError("Failed to create OGL400IndexBuffer:\nInvalid buffer usage mode"); break;
			default: throw RenderDeviceError("Failed to create OGL400IndexBuffer:\nUnsupported buffer usage mode"); break;
		}

		switch (_type)
		{
			// TO DO
			case IndexType::UByte: type = GL_UNSIGNED_BYTE; break;
			case IndexType::UShort: type = GL_UNSIGNED_SHORT; break;
			case IndexType::UInt: type = GL_UNSIGNED_INT; break;
			case IndexType::Invalid: throw RenderDeviceError("Failed to create OGL400IndexBuffer:\nInvalid index type"); break;
			default: throw RenderDeviceError("Failed to create OGL400IndexBuffer:\nUnsupported index type"); break;
		}

		glGenBuffers(1, &ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, usage);

		GL_CHECK_ERROR("Failed to create OGL400IndexBuffer");
	}

	virtual ~OGL400IndexBuffer() final
	{
		glDeleteBuffers(1, &ibo);
	}

	GLuint ibo;
	GLenum type;
};

class OGL400RasterState final : public RasterState
{
public:
	OGL400RasterState(const RasterStateDesc& desc)
	{
		cullEnabled = desc.cullEnabled;

		switch (desc.frontFace)
		{
			case Winding::CW: frontFace = GL_CW; break;
			case Winding::CCW: frontFace = GL_CCW; break;
			case Winding::Invalid: throw RenderDeviceError("Failed to create OGL400RasterState:\nInvalid front face"); break;
			default: throw RenderDeviceError("Failed to create OGL400RasterState:\nUnsupported front face"); break;
		}

		switch (desc.cullFace)
		{
			case Face::Front: cullFace = GL_FRONT; break;
			case Face::Back: cullFace = GL_BACK; break;
			case Face::FrontAndBack: cullFace = GL_FRONT_AND_BACK; break;
			case Face::Invalid: throw RenderDeviceError("Failed to create OGL400RasterState:\nInvalid cull face"); break;
			default: throw RenderDeviceError("Failed to create OGL400RasterState:\nUnsupported cull face"); break;
		}

		switch (desc.rasterMode)
		{
			case RasterMode::Point: polygonMode = GL_POINT; break;
			case RasterMode::Line: polygonMode = GL_LINE; break;
			case RasterMode::Fill: polygonMode = GL_FILL; break;
			case RasterMode::Invalid: throw RenderDeviceError("Failed to create OGL400RasterState:\nInvalid raster mode"); break;
			default: throw RenderDeviceError("Failed to create OGL400RasterState:\nUnsupported raster mode"); break;
		}
	}

	virtual ~OGL400RasterState() final
	{

	}

	GLboolean cullEnabled;
	GLenum frontFace;
	GLenum cullFace;
	GLenum polygonMode;
};

class OGL400DepthStencilState final : public DepthStencilState
{
public:
	OGL400DepthStencilState(const DepthStencilStateDesc& desc)
	{
		const static auto GetCompare = [](Compare compare) -> GLenum
		{
			switch (compare)
			{
				case Compare::Never: return GL_NEVER; break;
				case Compare::Less: return GL_LESS; break;
				case Compare::LEqual: return GL_LEQUAL; break;
				case Compare::Greater: return GL_GREATER; break;
				case Compare::GEqual: return GL_GEQUAL; break;
				case Compare::Equal: return GL_EQUAL; break;
				case Compare::NotEqual: return GL_NOTEQUAL; break;
				case Compare::Always: return GL_ALWAYS; break;
				case Compare::Invalid: throw RenderDeviceError("Failed to create OGL400DepthStencilState:\nInvalid compare mode"); break;
				default: throw RenderDeviceError("Failed to create OGL400DepthStencilState:\nUnsupported compare mode"); break;
			}
		};

		const static auto GetAction = [](StencilAction action) ->  GLenum
		{
			switch (action)
			{
				case StencilAction::Keep: return GL_KEEP; break;
				case StencilAction::Zero: return GL_ZERO; break;
				case StencilAction::Replace: return GL_REPLACE; break;
				case StencilAction::Increment: return GL_INCR; break;
				case StencilAction::IncrementWrap: return GL_INCR_WRAP; break;
				case StencilAction::Decrement: return GL_DECR; break;
				case StencilAction::DecrementWrap: return GL_DECR_WRAP; break;
				case StencilAction::Invert: return GL_INVERT; break;
				case StencilAction::Invalid: throw RenderDeviceError("Failed to create OGL400DepthStencilState:\nInvalid stencil action"); break;
				default: throw RenderDeviceError("Failed to create OGL400DepthStencilState:\nUnsupported stencil action"); break;
			}
		};

		// Depth
		depthEnabled = desc.depthEnabled;
		depthWriteEnabled = desc.depthWriteEnabled;
		depthNear = desc.depthNear;
		depthFar = desc.depthFar;
		depthFunc = GetCompare(desc.depthCompare);

		// Front stencil
		frontFaceStencilEnabled = desc.frontFaceStencilEnabled;
		frontStencilFunc = GetCompare(desc.frontFaceStencilCompare);
		frontFaceStencilFail = GetAction(desc.frontFaceStencilFail);
		frontFaceStencilPass = GetAction(desc.frontFaceStencilPass);
		frontFaceDepthFail = GetAction(desc.frontFaceDepthFail);
		frontFaceRef = desc.frontFaceRef;
		frontFaceReadMask = desc.frontFaceReadMask;
		frontFaceWriteMask = desc.frontFaceWriteMask;

		// Back stencil
		backFaceStencilEnabled = desc.backFaceStencilEnabled;
		backStencilFunc = GetCompare(desc.backFaceStencilCompare);
		backFaceStencilFail = GetAction(desc.backFaceStencilFail);
		backFaceStencilPass = GetAction(desc.backFaceStencilPass);
		backFaceDepthFail = GetAction(desc.backFaceDepthFail);
		backFaceRef = desc.backFaceRef;
		backFaceReadMask = desc.backFaceReadMask;
		backFaceWriteMask = desc.backFaceWriteMask;
	}

	virtual ~OGL400DepthStencilState() final
	{

	}

	GLboolean depthEnabled;
	GLboolean depthWriteEnabled;
	GLfloat depthNear;
	GLfloat depthFar;
	GLenum depthFunc;

	GLboolean frontFaceStencilEnabled;
	GLenum	frontStencilFunc;
	GLenum frontFaceStencilFail;
	GLenum frontFaceStencilPass;
	GLenum frontFaceDepthFail;
	GLint frontFaceRef;
	GLuint frontFaceReadMask;
	GLuint frontFaceWriteMask;

	GLboolean backFaceStencilEnabled;
	GLenum backStencilFunc;
	GLenum backFaceStencilFail;
	GLenum backFaceStencilPass;
	GLenum backFaceDepthFail;
	GLint backFaceRef;
	GLuint backFaceReadMask;
	GLuint backFaceWriteMask;
};

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

	// Get and set the default raster state
	{
		RasterStateDesc desc; // Default description
		m_defaultRasterState = this->CreateRasterState(desc);
		this->SetRasterState(m_defaultRasterState);
	}

	// Get and set the default depth stencil state
	{
		DepthStencilStateDesc desc; // Default description
		m_defaultDepthStencilState = this->CreateDepthStencilState(desc);
		this->SetDepthStencilState(m_defaultDepthStencilState);
	}
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
#if defined(MAGMA_FRAMEWORK_USE_OPENGL)
	std::string source;
	OGL400Assembler::Assemble(data, source);
	return new OGL400VertexShader(source.c_str(), data);
#else
	throw RenderDeviceError("Failed to call OGL400RenderDevice function:\nMAGMA_FRAMEWORK_USE_OPENGL must be defined to use this render device");
	return nullptr;
#endif
}

void Magma::Framework::Graphics::OGL400RenderDevice::DestroyVertexShader(VertexShader * vertexShader)
{
#if defined(MAGMA_FRAMEWORK_USE_OPENGL)
	delete vertexShader;
#else
	throw RenderDeviceError("Failed to call OGL400RenderDevice function:\nMAGMA_FRAMEWORK_USE_OPENGL must be defined to use this render device");
#endif
}

PixelShader * Magma::Framework::Graphics::OGL400RenderDevice::CreatePixelShader(const ShaderData& data)
{
#if defined(MAGMA_FRAMEWORK_USE_OPENGL)
	std::string source;
	OGL400Assembler::Assemble(data, source);
	return new OGL400PixelShader(source.c_str(), data);
#else
	throw RenderDeviceError("Failed to call OGL400RenderDevice function:\nMAGMA_FRAMEWORK_USE_OPENGL must be defined to use this render device");
	return nullptr;
#endif
}

void Magma::Framework::Graphics::OGL400RenderDevice::DestroyPixelShader(PixelShader * pixelShader)
{
#if defined(MAGMA_FRAMEWORK_USE_OPENGL)
	delete pixelShader;
#else
	throw RenderDeviceError("Failed to call OGL400RenderDevice function:\nMAGMA_FRAMEWORK_USE_OPENGL must be defined to use this render device");
#endif
}

Pipeline * Magma::Framework::Graphics::OGL400RenderDevice::CreatePipeline(VertexShader * vertexShader, PixelShader * pixelShader)
{
#if defined(MAGMA_FRAMEWORK_USE_OPENGL)
	return new OGL400Pipeline(static_cast<OGL400VertexShader*>(vertexShader), static_cast<OGL400PixelShader*>(pixelShader));
#else
	throw RenderDeviceError("Failed to call OGL400RenderDevice function:\nMAGMA_FRAMEWORK_USE_OPENGL must be defined to use this render device");
	return nullptr;
#endif
}

void Magma::Framework::Graphics::OGL400RenderDevice::DestroyPipeline(Pipeline * pipeline)
{
#if defined(MAGMA_FRAMEWORK_USE_OPENGL)
	delete pipeline;
#else
	throw RenderDeviceError("Failed to call OGL400RenderDevice function:\nMAGMA_FRAMEWORK_USE_OPENGL must be defined to use this render device");
#endif	
}

void Magma::Framework::Graphics::OGL400RenderDevice::SetPipeline(Pipeline * pipeline)
{
#if defined(MAGMA_FRAMEWORK_USE_OPENGL)
	if (pipeline == nullptr)
		glUseProgram(0);
	else
		glUseProgram(static_cast<OGL400Pipeline*>(pipeline)->shaderProgram);
	GL_CHECK_ERROR("Failed to set pipeline on OGL400RenderDevice");
#else
	throw RenderDeviceError("Failed to call OGL400RenderDevice function:\nMAGMA_FRAMEWORK_USE_OPENGL must be defined to use this render device");
#endif
}

VertexBuffer * Magma::Framework::Graphics::OGL400RenderDevice::CreateVertexBuffer(size_t size, const void * data, BufferUsage usage)
{
#if defined(MAGMA_FRAMEWORK_USE_OPENGL)
	return new OGL400VertexBuffer(size, data, usage);
#else
	throw RenderDeviceError("Failed to call OGL400RenderDevice function:\nMAGMA_FRAMEWORK_USE_OPENGL must be defined to use this render device");
	return nullptr;
#endif
}

void Magma::Framework::Graphics::OGL400RenderDevice::DestroyVertexBuffer(VertexBuffer * vertexBuffer)
{
#if defined(MAGMA_FRAMEWORK_USE_OPENGL)
	delete vertexBuffer;
#else
	throw RenderDeviceError("Failed to call OGL400RenderDevice function:\nMAGMA_FRAMEWORK_USE_OPENGL must be defined to use this render device");
#endif
}

VertexLayout * Magma::Framework::Graphics::OGL400RenderDevice::CreateVertexLayout(size_t elementCount, const VertexElement * elements, VertexShader * vertexShader)
{
#if defined(MAGMA_FRAMEWORK_USE_OPENGL)
	return new OGL400VertexLayout(elementCount, elements, static_cast<OGL400VertexShader*>(vertexShader));
#else
	throw RenderDeviceError("Failed to call OGL400RenderDevice function:\nMAGMA_FRAMEWORK_USE_OPENGL must be defined to use this render device");
	return nullptr;
#endif
}

void Magma::Framework::Graphics::OGL400RenderDevice::DestroyVertexLayout(VertexLayout * vertexLayout)
{
#if defined(MAGMA_FRAMEWORK_USE_OPENGL)
	delete vertexLayout;
#else
	throw RenderDeviceError("Failed to call OGL400RenderDevice function:\nMAGMA_FRAMEWORK_USE_OPENGL must be defined to use this render device");
#endif
}

VertexArray * Magma::Framework::Graphics::OGL400RenderDevice::CreateVertexArray(size_t bufferCount, VertexBuffer ** buffers, VertexLayout * layout)
{
#if defined(MAGMA_FRAMEWORK_USE_OPENGL)
	return new OGL400VertexArray(bufferCount, buffers, layout);
#else
	throw RenderDeviceError("Failed to call OGL400RenderDevice function:\nMAGMA_FRAMEWORK_USE_OPENGL must be defined to use this render device");
	return nullptr;
#endif	
}

void Magma::Framework::Graphics::OGL400RenderDevice::DestroyVertexArray(VertexArray * vertexArray)
{
#if defined(MAGMA_FRAMEWORK_USE_OPENGL)
	delete vertexArray;
#else
	throw RenderDeviceError("Failed to call OGL400RenderDevice function:\nMAGMA_FRAMEWORK_USE_OPENGL must be defined to use this render device");
#endif	
}

void Magma::Framework::Graphics::OGL400RenderDevice::SetVertexArray(VertexArray * vertexArray)
{
#if defined(MAGMA_FRAMEWORK_USE_OPENGL)
	if (vertexArray == nullptr)
		glBindVertexArray(0);
	else
		glBindVertexArray(static_cast<OGL400VertexArray*>(vertexArray)->vao);
	GL_CHECK_ERROR("Failed to set vertex array on OGL400RenderDevice");
#else
	throw RenderDeviceError("Failed to call OGL400RenderDevice function:\nMAGMA_FRAMEWORK_USE_OPENGL must be defined to use this render device");
#endif	
}

IndexBuffer * Magma::Framework::Graphics::OGL400RenderDevice::CreateIndexBuffer(IndexType type, size_t size, const void * data, BufferUsage usage)
{
#if defined(MAGMA_FRAMEWORK_USE_OPENGL)
	return new OGL400IndexBuffer(type, size, data, usage);
#else
	throw RenderDeviceError("Failed to call OGL400RenderDevice function:\nMAGMA_FRAMEWORK_USE_OPENGL must be defined to use this render device");
	return nullptr;
#endif
}

void Magma::Framework::Graphics::OGL400RenderDevice::DestroyIndexBuffer(IndexBuffer * indexBuffer)
{
#if defined(MAGMA_FRAMEWORK_USE_OPENGL)
	delete indexBuffer;
#else
	throw RenderDeviceError("Failed to call OGL400RenderDevice function:\nMAGMA_FRAMEWORK_USE_OPENGL must be defined to use this render device");
#endif
}

void Magma::Framework::Graphics::OGL400RenderDevice::SetIndexBuffer(IndexBuffer * indexBuffer)
{
#if defined(MAGMA_FRAMEWORK_USE_OPENGL)
	m_currentIndexBuffer = indexBuffer;
	if (indexBuffer == nullptr)
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	else
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, static_cast<OGL400IndexBuffer*>(indexBuffer)->ibo);
	GL_CHECK_ERROR("Failed to set index buffer on OGL400RenderDevice");
#else
	throw RenderDeviceError("Failed to call OGL400RenderDevice function:\nMAGMA_FRAMEWORK_USE_OPENGL must be defined to use this render device");
#endif	
}

Texture2D * Magma::Framework::Graphics::OGL400RenderDevice::CreateTexture2D(size_t width, size_t height, const void * data, BufferUsage usage)
{
#if defined(MAGMA_FRAMEWORK_USE_OPENGL)
	// TO DO
	return nullptr;
#else
	throw RenderDeviceError("Failed to call OGL400RenderDevice function:\nMAGMA_FRAMEWORK_USE_OPENGL must be defined to use this render device");
	return nullptr;
#endif
}

void Magma::Framework::Graphics::OGL400RenderDevice::DestroyTexture2D(Texture2D * texture)
{
#if defined(MAGMA_FRAMEWORK_USE_OPENGL)
	// TO DO
#else
	throw RenderDeviceError("Failed to call OGL400RenderDevice function:\nMAGMA_FRAMEWORK_USE_OPENGL must be defined to use this render device");
#endif
}

RasterState * Magma::Framework::Graphics::OGL400RenderDevice::CreateRasterState(const RasterStateDesc & desc)
{
#if defined(MAGMA_FRAMEWORK_USE_OPENGL)
	return new OGL400RasterState(desc);
#else
	throw RenderDeviceError("Failed to call OGL400RenderDevice function:\nMAGMA_FRAMEWORK_USE_OPENGL must be defined to use this render device");
	return nullptr;
#endif	
}

void Magma::Framework::Graphics::OGL400RenderDevice::DestroyRasterState(RasterState * rasterState)
{
#if defined(MAGMA_FRAMEWORK_USE_OPENGL)
	delete rasterState;
#else
	throw RenderDeviceError("Failed to call OGL400RenderDevice function:\nMAGMA_FRAMEWORK_USE_OPENGL must be defined to use this render device");
#endif	
}

void Magma::Framework::Graphics::OGL400RenderDevice::SetRasterState(RasterState * rasterState)
{
#if defined(MAGMA_FRAMEWORK_USE_OPENGL)
	auto old = m_currentRasterState;
	if (rasterState == nullptr)
		m_currentRasterState = m_defaultRasterState;
	else
		m_currentRasterState = rasterState;

	// If the raster state changed
	if (old != m_currentRasterState)
	{
		if (static_cast<OGL400RasterState*>(m_currentRasterState)->cullEnabled)
			glEnable(GL_CULL_FACE);
		else
			glDisable(GL_CULL_FACE);
		glFrontFace(static_cast<OGL400RasterState*>(m_currentRasterState)->frontFace);
		glCullFace(static_cast<OGL400RasterState*>(m_currentRasterState)->cullFace);
		glPolygonMode(GL_FRONT_AND_BACK, static_cast<OGL400RasterState*>(m_currentRasterState)->polygonMode);
	}

	GL_CHECK_ERROR("Failed to set raster state on OGL400RenderDevice");
#else
	throw RenderDeviceError("Failed to call OGL400RenderDevice function:\nMAGMA_FRAMEWORK_USE_OPENGL must be defined to use this render device");
#endif
}

DepthStencilState * Magma::Framework::Graphics::OGL400RenderDevice::CreateDepthStencilState(const DepthStencilStateDesc & desc)
{
#if defined(MAGMA_FRAMEWORK_USE_OPENGL)
	return new OGL400DepthStencilState(desc);
#else
	throw RenderDeviceError("Failed to call OGL400RenderDevice function:\nMAGMA_FRAMEWORK_USE_OPENGL must be defined to use this render device");
	return nullptr;
#endif	
}

void Magma::Framework::Graphics::OGL400RenderDevice::DestroyDepthStencilState(DepthStencilState * depthStencilState)
{
#if defined(MAGMA_FRAMEWORK_USE_OPENGL)
	delete depthStencilState;
#else
	throw RenderDeviceError("Failed to call OGL400RenderDevice function:\nMAGMA_FRAMEWORK_USE_OPENGL must be defined to use this render device");
#endif	
}

void Magma::Framework::Graphics::OGL400RenderDevice::SetDepthStencilState(DepthStencilState * depthStencilState)
{
#if defined(MAGMA_FRAMEWORK_USE_OPENGL)
	auto old = m_currentDepthStencilState;
	if (depthStencilState == nullptr)
		m_currentDepthStencilState = m_defaultDepthStencilState;
	else
		m_currentDepthStencilState = depthStencilState;

	// If the depth stencil state changed
	if (old != m_currentDepthStencilState)
	{
		// Depth
		if (static_cast<OGL400DepthStencilState*>(m_currentDepthStencilState)->depthEnabled)
			glEnable(GL_DEPTH_TEST);
		else
			glDisable(GL_DEPTH_TEST);

		glDepthFunc(static_cast<OGL400DepthStencilState*>(m_currentDepthStencilState)->depthFunc);
		glDepthMask(static_cast<OGL400DepthStencilState*>(m_currentDepthStencilState)->depthWriteEnabled ? GL_TRUE : GL_FALSE);
		glDepthRange(static_cast<OGL400DepthStencilState*>(m_currentDepthStencilState)->depthNear, static_cast<OGL400DepthStencilState*>(m_currentDepthStencilState)->depthFar);

		// Stencil
		if (static_cast<OGL400DepthStencilState*>(m_currentDepthStencilState)->frontFaceStencilEnabled || static_cast<OGL400DepthStencilState*>(m_currentDepthStencilState)->backFaceStencilEnabled)
			glEnable(GL_STENCIL_TEST);
		else
			glDisable(GL_STENCIL_TEST);

		// Front face
		glStencilFuncSeparate(GL_FRONT, static_cast<OGL400DepthStencilState*>(m_currentDepthStencilState)->frontStencilFunc, static_cast<OGL400DepthStencilState*>(m_currentDepthStencilState)->frontFaceRef, static_cast<OGL400DepthStencilState*>(m_currentDepthStencilState)->frontFaceReadMask);
		glStencilMaskSeparate(GL_FRONT, static_cast<OGL400DepthStencilState*>(m_currentDepthStencilState)->frontFaceWriteMask);
		glStencilOpSeparate(GL_FRONT, static_cast<OGL400DepthStencilState*>(m_currentDepthStencilState)->frontFaceStencilFail, static_cast<OGL400DepthStencilState*>(m_currentDepthStencilState)->frontFaceDepthFail, static_cast<OGL400DepthStencilState*>(m_currentDepthStencilState)->frontFaceStencilPass);
		
		// Back face
		glStencilFuncSeparate(GL_BACK, static_cast<OGL400DepthStencilState*>(m_currentDepthStencilState)->backStencilFunc, static_cast<OGL400DepthStencilState*>(m_currentDepthStencilState)->backFaceRef, static_cast<OGL400DepthStencilState*>(m_currentDepthStencilState)->backFaceReadMask);
		glStencilMaskSeparate(GL_BACK, static_cast<OGL400DepthStencilState*>(m_currentDepthStencilState)->backFaceWriteMask);
		glStencilOpSeparate(GL_BACK, static_cast<OGL400DepthStencilState*>(m_currentDepthStencilState)->backFaceStencilFail, static_cast<OGL400DepthStencilState*>(m_currentDepthStencilState)->backFaceDepthFail, static_cast<OGL400DepthStencilState*>(m_currentDepthStencilState)->backFaceStencilPass);
	}

	GL_CHECK_ERROR("Failed to set depth stencil state on OGL400RenderDevice");
#else
	throw RenderDeviceError("Failed to call OGL400RenderDevice function:\nMAGMA_FRAMEWORK_USE_OPENGL must be defined to use this render device");
#endif
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
#if defined(MAGMA_FRAMEWORK_USE_OPENGL)
	glDrawArrays(GL_TRIANGLES, offset, count);
	GL_CHECK_ERROR("Failed to draw triangles on OGL400RenderDevice");
#else
	throw RenderDeviceError("Failed to call OGL400RenderDevice function:\nMAGMA_FRAMEWORK_USE_OPENGL must be defined to use this render device");
#endif
}

void Magma::Framework::Graphics::OGL400RenderDevice::DrawTrianglesIndexed(size_t offset, size_t count)
{
#if defined(MAGMA_FRAMEWORK_USE_OPENGL)
#ifdef MAGMA_FRAMEWORK_RENDER_DEVICE_DEBUG
	if (m_currentIndexBuffer == nullptr)
		throw RenderDeviceError("Failed to draw indexed triangles on OGL400RenderDevice:\nNo index buffer currently bound");
#endif
	glDrawElements(GL_TRIANGLES, count, static_cast<OGL400IndexBuffer*>(m_currentIndexBuffer)->type, reinterpret_cast<const void*>(offset));
	GL_CHECK_ERROR("Failed to draw indexed triangles on OGL400RenderDevice");
#else
	throw RenderDeviceError("Failed to call OGL400RenderDevice function:\nMAGMA_FRAMEWORK_USE_OPENGL must be defined to use this render device");
#endif
}

void Magma::Framework::Graphics::OGL400RenderDevice::SetRenderTargets(Texture2D ** textures, size_t count)
{
#if defined(MAGMA_FRAMEWORK_USE_OPENGL)
	
#else
	throw RenderDeviceError("Failed to call OGL400RenderDevice function:\nMAGMA_FRAMEWORK_USE_OPENGL must be defined to use this render device");
#endif
}

void Magma::Framework::Graphics::OGL400RenderDevice::SwapBuffers()
{
#if defined(MAGMA_FRAMEWORK_USE_OPENGL)
	m_window->SwapBuffers();
#else
	throw RenderDeviceError("Failed to call OGL400RenderDevice function:\nMAGMA_FRAMEWORK_USE_OPENGL must be defined to use this render device");
#endif
}
