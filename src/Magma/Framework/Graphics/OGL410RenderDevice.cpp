#include "OGL410RenderDevice.hpp"
#include "OGL410Assembler.hpp"

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

class OGL410Texture2D final : public Texture2D
{
public:
	OGL410Texture2D(size_t width, size_t height, TextureFormat _format, const void* data)
	{
		switch (_format)
		{
			case TextureFormat::R8SNorm: internalFormat = GL_R8_SNORM; type = GL_BYTE; format = GL_RED; break;
			case TextureFormat::R16SNorm: internalFormat = GL_R16_SNORM; type = GL_SHORT; format = GL_RED; break;
			case TextureFormat::RG8SNorm: internalFormat = GL_RG8_SNORM; type = GL_BYTE; format = GL_RG; break;
			case TextureFormat::RG16SNorm: internalFormat = GL_RG16_SNORM; type = GL_SHORT; format = GL_RG; break;
			case TextureFormat::RGB8SNorm: internalFormat = GL_RGB8_SNORM; type = GL_BYTE; format = GL_RGB; break;
			case TextureFormat::RGB16SNorm: internalFormat = GL_RGB16_SNORM; type = GL_SHORT; format = GL_RGB; break;
			case TextureFormat::RGBA8SNorm: internalFormat = GL_RGBA8_SNORM; type = GL_BYTE; format = GL_RGBA; break;
			case TextureFormat::RGBA16SNorm: internalFormat = GL_RGBA16_SNORM; type = GL_SHORT; format = GL_RGBA; break;

			case TextureFormat::R8UNorm: internalFormat = GL_R8; type = GL_UNSIGNED_BYTE; format = GL_RED; break;
			case TextureFormat::R16UNorm: internalFormat = GL_R16; type = GL_UNSIGNED_SHORT; format = GL_RED; break;
			case TextureFormat::RG8UNorm: internalFormat = GL_RG8; type = GL_UNSIGNED_BYTE; format = GL_RG; break;
			case TextureFormat::RG16UNorm: internalFormat = GL_RG16; type = GL_UNSIGNED_SHORT; format = GL_RG; break;
			case TextureFormat::RGB8UNorm: internalFormat = GL_RGB8; type = GL_UNSIGNED_BYTE; format = GL_RGB; break;
			case TextureFormat::RGB16UNorm: internalFormat = GL_RGB16; type = GL_UNSIGNED_SHORT; format = GL_RGB; break;
			case TextureFormat::RGBA8UNorm: internalFormat = GL_RGBA8; type = GL_UNSIGNED_BYTE; format = GL_RGBA; break;
			case TextureFormat::RGBA16UNorm: internalFormat = GL_RGBA16; type = GL_UNSIGNED_SHORT; format = GL_RGBA; break;

			case TextureFormat::R8Int: internalFormat = GL_R8I; type = GL_BYTE; format = GL_RED; break;
			case TextureFormat::R16Int: internalFormat = GL_R16I; type = GL_SHORT; format = GL_RED; break;
			case TextureFormat::RG8Int: internalFormat = GL_RG8I; type = GL_BYTE; format = GL_RG; break;
			case TextureFormat::RG16Int: internalFormat = GL_RG16I; type = GL_SHORT; format = GL_RG; break;
			case TextureFormat::RGB8Int: internalFormat = GL_RGB8I; type = GL_BYTE; format = GL_RGB; break;
			case TextureFormat::RGB16Int: internalFormat = GL_RGB16I; type = GL_SHORT; format = GL_RGB; break;
			case TextureFormat::RGBA8Int: internalFormat = GL_RGBA8I; type = GL_BYTE; format = GL_RGBA; break;
			case TextureFormat::RGBA16Int: internalFormat = GL_RGBA16I; type = GL_SHORT; format = GL_RGBA; break;

			case TextureFormat::R8UInt: internalFormat = GL_R8UI; type = GL_UNSIGNED_BYTE; format = GL_RED; break;
			case TextureFormat::R16UInt: internalFormat = GL_R16UI; type = GL_UNSIGNED_SHORT; format = GL_RED; break;
			case TextureFormat::RG8UInt: internalFormat = GL_RG8UI; type = GL_UNSIGNED_BYTE; format = GL_RG; break;
			case TextureFormat::RG16UInt: internalFormat = GL_RG16UI; type = GL_UNSIGNED_SHORT; format = GL_RG; break;
			case TextureFormat::RGB8UInt: internalFormat = GL_RGB8UI; type = GL_UNSIGNED_BYTE; format = GL_RGB; break;
			case TextureFormat::RGB16UInt: internalFormat = GL_RGB16UI; type = GL_UNSIGNED_SHORT; format = GL_RGB; break;
			case TextureFormat::RGBA8UInt: internalFormat = GL_RGBA8UI; type = GL_UNSIGNED_BYTE; format = GL_RGBA; break;
			case TextureFormat::RGBA16UInt: internalFormat = GL_RGBA16UI; type = GL_UNSIGNED_SHORT; format = GL_RGBA; break;

			case TextureFormat::R32Float: internalFormat = GL_R32F; type = GL_FLOAT; format = GL_RED; break;
			case TextureFormat::RG32Float: internalFormat = GL_RG32F; type = GL_FLOAT; format = GL_RG; break;
			case TextureFormat::RGB32Float: internalFormat = GL_RGB32F; type = GL_FLOAT; format = GL_RGB; break;
			case TextureFormat::RGBA32Float: internalFormat = GL_RGBA32F; type = GL_FLOAT; format = GL_RGBA; break;

			case TextureFormat::Invalid: throw RenderDeviceError("Failed to create OGL410Texture2D:\nInvalid texture format"); break;
			default: throw RenderDeviceError("Failed to create OGL410Texture2D:\nUnsupported texture format"); break;
		}

		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, data);

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		GL_CHECK_ERROR("Failed to create OGL410Texture2D");
	}

	virtual ~OGL410Texture2D() final
	{
		 glDeleteTextures(1, &texture);
	}

	virtual void Update(size_t dstX, size_t dstY, size_t width, size_t height, void* data) final
	{
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexSubImage2D(GL_TEXTURE_2D, 0, dstX, dstY, width, height, format, type, data);
		GL_CHECK_ERROR("Failed to update OGL410Texture2D");
	}

	virtual void GenerateMipmaps()
	{
		glBindTexture(GL_TEXTURE_2D, texture);
		glGenerateMipmap(GL_TEXTURE_2D);
		GL_CHECK_ERROR("Failed to generate OGL410Texture2D mipmaps");
	}

	GLenum internalFormat;
	GLenum format;
	GLenum type;
	GLuint texture;
};

class OGL410Sampler2D : public Sampler2D
{
public:
	OGL410Sampler2D(const Sampler2DDesc& desc)
	{
		GLenum minFilter, magFilter;
		GLenum wrapS, wrapT;

		switch (desc.adressU)
		{
			case TextureAdressMode::Repeat: wrapS = GL_REPEAT; break;
			case TextureAdressMode::Mirror: wrapS = GL_MIRRORED_REPEAT; break;
			case TextureAdressMode::Clamp: wrapS = GL_CLAMP; break;
			case TextureAdressMode::Border: wrapS = GL_CLAMP_TO_BORDER; break;
			case TextureAdressMode::Invalid: throw RenderDeviceError("Failed to create OGL410Sampler2D:\nInvalid texture coordinate U adress mode"); break;
			default: throw RenderDeviceError("Failed to create OGL410Sampler2D:\nUnsupported texture coordinate U adress mode"); break;
		}

		switch (desc.adressV)
		{
			case TextureAdressMode::Repeat: wrapT = GL_REPEAT; break;
			case TextureAdressMode::Mirror: wrapT = GL_MIRRORED_REPEAT; break;
			case TextureAdressMode::Clamp: wrapT = GL_CLAMP; break;
			case TextureAdressMode::Border: wrapT = GL_CLAMP_TO_BORDER; break;
			case TextureAdressMode::Invalid: throw RenderDeviceError("Failed to create OGL410Sampler2D:\nInvalid texture coordinate V adress mode"); break;
			default: throw RenderDeviceError("Failed to create OGL410Sampler2D:\nUnsupported texture coordinate V adress mode"); break;
		}

		switch (desc.minFilter)
		{
			case TextureFilter::Nearest:
				if (desc.mipmapFilter == TextureFilter::Invalid)
					minFilter = GL_NEAREST;
				else if (desc.mipmapFilter == TextureFilter::Nearest)
					minFilter = GL_NEAREST_MIPMAP_NEAREST;
				else if (desc.mipmapFilter == TextureFilter::Linear)
					minFilter = GL_NEAREST_MIPMAP_LINEAR;
				else throw RenderDeviceError("Failed to create OGL410Sampler2D:\nUnsupported mipmap filter"); break;
				break;
			case TextureFilter::Linear:
				if (desc.mipmapFilter == TextureFilter::Invalid)
					minFilter = GL_LINEAR;
				else if (desc.mipmapFilter == TextureFilter::Nearest)
					minFilter = GL_LINEAR_MIPMAP_NEAREST;
				else if (desc.mipmapFilter == TextureFilter::Linear)
					minFilter = GL_LINEAR_MIPMAP_LINEAR;
				else throw RenderDeviceError("Failed to create OGL410Sampler2D:\nUnsupported mipmap filter"); break;
				break;
			case TextureFilter::Invalid: throw RenderDeviceError("Failed to create OGL410Sampler2D:\nInvalid minifying filter"); break;
			default: throw RenderDeviceError("Failed to create OGL410Sampler2D:\nUnsupported minifying filter"); break;
		}

		switch (desc.magFilter)
		{
			case TextureFilter::Nearest: magFilter = GL_NEAREST; break;
			case TextureFilter::Linear: magFilter = GL_LINEAR; break;
			case TextureFilter::Invalid: throw RenderDeviceError("Failed to create OGL410Sampler2D:\nInvalid magnifying filter"); break;
			default: throw RenderDeviceError("Failed to create OGL410Sampler2D:\nUnsupported magnifying filter"); break;
		}

		glGenSamplers(1, &sampler);

		GL_CHECK_ERROR("Failed to create OGL410Sampler2D {1}");

		glSamplerParameteri(sampler, GL_TEXTURE_MIN_FILTER, minFilter);
		glSamplerParameteri(sampler, GL_TEXTURE_MAG_FILTER, magFilter);
		glSamplerParameteri(sampler, GL_TEXTURE_WRAP_S, wrapS);
		glSamplerParameteri(sampler, GL_TEXTURE_WRAP_T, wrapT);

		GL_CHECK_ERROR("Failed to create OGL410Sampler2D {2}");
	}

	virtual ~OGL410Sampler2D() final
	{
		glDeleteSamplers(1, &sampler);
	}

	GLuint sampler;
};

class OGL410VertexBindingPoint;

class OGL410VertexShader final : public VertexShader
{
public:
	OGL410VertexShader(const char* source, const ShaderData& _data)
		: data(_data)
	{
		vertexShader = glCreateShaderProgramv(GL_VERTEX_SHADER, 1, &source);

		GLint success;
		GLchar infoLog[512];
		glGetProgramiv(vertexShader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(vertexShader, 512, NULL, infoLog);
			glDeleteProgram(vertexShader);
			std::stringstream ss;
			ss << "Failed to create OGL410VertexShader:" << std::endl;
			ss << infoLog << std::endl;
			ss << "Source:" << std::endl;
			ss << source;
			throw RenderDeviceError(ss.str());
		}

		GL_CHECK_ERROR("Failed to create OGL410VertexShader");
	}

	virtual ~OGL410VertexShader() final
	{
		for (auto& bp : bindingPoints)
			delete bp.second;
		glDeleteProgram(vertexShader);
	}

	virtual VertexBindingPoint* GetBindingPoint(const char * name) final;

	std::unordered_map<std::string, OGL410VertexBindingPoint*> bindingPoints;

	GLuint vertexShader;
	ShaderData data;
};

class OGL410VertexBindingPoint final : public VertexBindingPoint
{
public:
	OGL410VertexBindingPoint(OGL410VertexShader* _shader, GLint _location)
		: shader(_shader), location(_location)
	{
		// Empty
	}

	virtual ~OGL410VertexBindingPoint() final
	{

	}

	virtual void Bind(Texture2D* texture) final
	{
		glActiveTexture(GL_TEXTURE0 + location);
		glBindTexture(GL_TEXTURE_2D, static_cast<OGL410Texture2D*>(texture)->texture);
		glProgramUniform1i(shader->vertexShader, location, location);

		GL_CHECK_ERROR("Failed to bind OGL410Texture2D to OGL410VertexBindingPoint");
	}

	virtual void Bind(Sampler2D* sampler) final
	{
		glBindSampler(GL_TEXTURE0 + location, static_cast<OGL410Sampler2D*>(sampler)->sampler);
	}

	virtual void Bind(ConstantBuffer* buffer) final
	{
		// TO DO
	}

	OGL410VertexShader* shader;
	GLint location;
};

VertexBindingPoint * OGL410VertexShader::GetBindingPoint(const char * name)
{
	for (auto& bp : bindingPoints)
		if (bp.first.c_str() == name)
			return bp.second;

	for (auto& cv : data.GetShaderConstantBuffers())
		if (cv.name == name)
		{
			auto index = glGetUniformBlockIndex(vertexShader, name);
			if (index == -1)
			{
				std::stringstream ss;
				ss << "Failed to get OGL410VertexShader binding point:" << std::endl;
				ss << "Couldn't get constant buffer \"" << cv.name << "\" index (returned -1)";
				throw RenderDeviceError(ss.str());
			}
			glUniformBlockBinding(vertexShader, index, cv.index);
			auto bp = new OGL410VertexBindingPoint(this, cv.index);
			bindingPoints[name] = bp;
			GL_CHECK_ERROR("Failed to get OGL410VertexShader binding point");
			return bp;
		}

	for (auto& t : data.GetTexture2DVariables())
		if (t.name == name)
		{
			auto loc = glGetUniformLocation(vertexShader, ("tex_" + std::to_string(t.index)).c_str());
			if (loc == -1)
			{
				std::stringstream ss;
				ss << "Failed to get OGL410VertexShader binding point:" << std::endl;
				ss << "Couldn't get texture \"" << t.name << "\" location (returned -1)";
				throw RenderDeviceError(ss.str());
			}
			auto bp = new OGL410VertexBindingPoint(this, loc);
			bindingPoints[name] = bp;
			GL_CHECK_ERROR("Failed to get OGL410VertexShader binding point");
			return bp;
		}

	return nullptr;
}

class OGL410PixelBindingPoint;

class OGL410PixelShader final : public PixelShader
{
public:
	OGL410PixelShader(const char* source, const ShaderData& _data)
		: data(_data)
	{
		fragmentShader = glCreateShaderProgramv(GL_FRAGMENT_SHADER, 1, &source);

		GLint success;
		GLchar infoLog[512];
		glGetProgramiv(fragmentShader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(fragmentShader, 512, NULL, infoLog);
			glDeleteProgram(fragmentShader);
			std::stringstream ss;
			ss << "Failed to create OGL410PixelShader:" << std::endl;
			ss << infoLog << std::endl;
			ss << "Source:" << std::endl;
			ss << source;
			throw RenderDeviceError(ss.str());
		}

		GL_CHECK_ERROR("Failed to create OGL410PixelShader");
	}

	virtual ~OGL410PixelShader() final
	{
		for (auto& bp : bindingPoints)
			delete bp.second;
		glDeleteProgram(fragmentShader);
	}

	virtual PixelBindingPoint * GetBindingPoint(const char * name) final;

	std::unordered_map<std::string, OGL410PixelBindingPoint*> bindingPoints;

	GLuint fragmentShader;
	ShaderData data;
};

class OGL410PixelBindingPoint final : public PixelBindingPoint
{
public:
	OGL410PixelBindingPoint(OGL410PixelShader* _shader, GLint _location)
		: shader(_shader), location(_location)
	{
		// Empty
	}

	virtual ~OGL410PixelBindingPoint() final
	{
		// Empty
	}

	virtual void Bind(Texture2D* texture) final
	{
		glActiveTexture(GL_TEXTURE0 + location);
		glBindTexture(GL_TEXTURE_2D, static_cast<OGL410Texture2D*>(texture)->texture);
		glProgramUniform1i(shader->fragmentShader, location, location);

		GL_CHECK_ERROR("Failed to bind OGL410Texture2D to OGL410PixelBindingPoint");
	}

	virtual void Bind(Sampler2D* sampler) final
	{
		glBindSampler(location, static_cast<OGL410Sampler2D*>(sampler)->sampler);

		GL_CHECK_ERROR("Failed to bind OGL410Sampler2D to OGL410PixelBindingPoint");
	}

	virtual void Bind(ConstantBuffer* buffer) final
	{
		// TO DO
	}

	OGL410PixelShader* shader;
	GLint location;
};

PixelBindingPoint * OGL410PixelShader::GetBindingPoint(const char * name)
{
	for (auto& bp : bindingPoints)
		if (bp.first.c_str() == name)
			return bp.second;

	for (auto& cv : data.GetShaderConstantBuffers())
		if (cv.name == name)
		{
			auto index = glGetUniformBlockIndex(fragmentShader, name);
			if (index == -1)
			{
				std::stringstream ss;
				ss << "Failed to get OGL410PixelShader binding point:" << std::endl;
				ss << "Couldn't get constant buffer \"" << cv.name << "\" index (returned -1)";
				throw RenderDeviceError(ss.str());
			}
			glUniformBlockBinding(fragmentShader, index, cv.index);
			auto bp = new OGL410PixelBindingPoint(this, cv.index);
			bindingPoints[name] = bp;
			GL_CHECK_ERROR("Failed to get OGL410PixelShader binding point");
			return bp;
		}

	for (auto& t : data.GetTexture2DVariables())
		if (t.name == name)
		{
			auto loc = glGetUniformLocation(fragmentShader, ("tex_" + std::to_string(t.index)).c_str());
			if (loc == -1)
			{
				std::stringstream ss;
				ss << "Failed to get OGL410PixelShader binding point:" << std::endl;
				ss << "Couldn't get texture \"" << t.name << "\" location (returned -1)";
				throw RenderDeviceError(ss.str());
			}
			auto bp = new OGL410PixelBindingPoint(this, loc);
			bindingPoints[name] = bp;
			GL_CHECK_ERROR("Failed to get OGL410PixelShader binding point");
			return bp;
		}

	return nullptr;
}

class OGL410Pipeline final : public Pipeline
{
public:
	OGL410Pipeline(OGL410VertexShader* vertexShader, OGL410PixelShader* pixelShader)
	{
		glGenProgramPipelines(1, &shaderPipeline);
		glUseProgramStages(shaderPipeline, GL_VERTEX_SHADER_BIT, vertexShader->vertexShader);
		glUseProgramStages(shaderPipeline, GL_FRAGMENT_SHADER_BIT, pixelShader->fragmentShader);

		GL_CHECK_ERROR("Failed to create OGL410Pipeline");
	}

	virtual ~OGL410Pipeline() final
	{
		glDeleteProgramPipelines(1, &shaderPipeline);
	}

	GLuint shaderPipeline;
};

class OGL410VertexBuffer final : public VertexBuffer
{
public:
	OGL410VertexBuffer(GLsizei size, const GLvoid* data, BufferUsage _usage)
	{
		GLenum usage;

		switch (_usage)
		{
			case BufferUsage::Default: usage = GL_STATIC_DRAW; break; // Default and static are the same in OGL410
			case BufferUsage::Static: usage = GL_STATIC_DRAW; break;
			case BufferUsage::Dynamic: usage = GL_DYNAMIC_DRAW; break;
			case BufferUsage::Invalid: throw RenderDeviceError("Failed to create OGL410VertexBuffer:\nInvalid buffer usage mode"); break;
			default: throw RenderDeviceError("Failed to create OGL410VertexBuffer:\nUnsupported buffer usage mode"); break;
		}

		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, size, data, usage);

		GL_CHECK_ERROR("Failed to create OGL410VertexBuffer");
	}

	virtual ~OGL410VertexBuffer() final
	{
		glDeleteBuffers(1, &vbo);
	}
	
	virtual void* Map() final
	{
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		return glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	}

	virtual void Unmap() final
	{
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glUnmapBuffer(GL_ARRAY_BUFFER);
	}

	GLuint vbo;
};

class OGL410VertexLayout final : public VertexLayout
{
public:
	struct OGL410VertexElement
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

	OGL410VertexLayout(GLuint _vertexElementCount, const VertexElement* vertexElements, OGL410VertexShader* vertexShader)
		: vertexElementCount(_vertexElementCount)
	{
		oglVertexElements = new OGL410VertexElement[vertexElementCount];
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
				ss << "Failed to create OGL410VertexLayout:" << std::endl;
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

				case VertexElementType::Invalid: throw RenderDeviceError("Failed to create OGL410VertexLayout:\nInvalid vertex element type"); break;
				default: throw RenderDeviceError("Failed to create OGL410VertexLayout:\nUnsupported vertex element type"); break;
			}

			oglVertexElements[i].bufferIndex = vertexElements[i].bufferIndex;
			oglVertexElements[i].size = vertexElements[i].size;
			oglVertexElements[i].stride = vertexElements[i].stride;
			oglVertexElements[i].offset = (char*)nullptr + vertexElements[i].offset;
		}
	}

	virtual ~OGL410VertexLayout() final
	{
		delete[] oglVertexElements;
	}

	GLuint vertexElementCount;
	OGL410VertexElement* oglVertexElements = nullptr;
};

class OGL410VertexArray final : public VertexArray
{
public:
	OGL410VertexArray(GLuint bufferCount, VertexBuffer** buffers, VertexLayout* layout)
	{
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		auto oglLayout = static_cast<OGL410VertexLayout*>(layout);

		for (GLuint i = 0; i < bufferCount; ++i)
		{
			auto buffer = static_cast<OGL410VertexBuffer*>(buffers[i]);
			
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

		GL_CHECK_ERROR("Failed to create OGL410VertexBuffer");
	}

	virtual ~OGL410VertexArray() final
	{
		glDeleteVertexArrays(1, &vao);
	}

	GLuint vao;
};

class OGL410IndexBuffer final : public IndexBuffer
{
public:
	OGL410IndexBuffer(IndexType _type, GLsizei size, const void* data, BufferUsage _usage)
	{
		GLenum usage;

		switch (_usage)
		{
			case BufferUsage::Default: usage = GL_STATIC_DRAW; break; // Default and static are the same in OGL410
			case BufferUsage::Static: usage = GL_STATIC_DRAW; break;
			case BufferUsage::Dynamic: usage = GL_DYNAMIC_DRAW; break;
			case BufferUsage::Invalid: throw RenderDeviceError("Failed to create OGL410IndexBuffer:\nInvalid buffer usage mode"); break;
			default: throw RenderDeviceError("Failed to create OGL410IndexBuffer:\nUnsupported buffer usage mode"); break;
		}

		switch (_type)
		{
			// TO DO
			case IndexType::UByte: type = GL_UNSIGNED_BYTE; break;
			case IndexType::UShort: type = GL_UNSIGNED_SHORT; break;
			case IndexType::UInt: type = GL_UNSIGNED_INT; break;
			case IndexType::Invalid: throw RenderDeviceError("Failed to create OGL410IndexBuffer:\nInvalid index type"); break;
			default: throw RenderDeviceError("Failed to create OGL410IndexBuffer:\nUnsupported index type"); break;
		}

		glGenBuffers(1, &ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, usage);

		GL_CHECK_ERROR("Failed to create OGL410IndexBuffer");
	}

	virtual ~OGL410IndexBuffer() final
	{
		glDeleteBuffers(1, &ibo);
	}

	GLuint ibo;
	GLenum type;
};

class OGL410RasterState final : public RasterState
{
public:
	OGL410RasterState(const RasterStateDesc& desc)
	{
		cullEnabled = desc.cullEnabled;

		switch (desc.frontFace)
		{
			case Winding::CW: frontFace = GL_CW; break;
			case Winding::CCW: frontFace = GL_CCW; break;
			case Winding::Invalid: throw RenderDeviceError("Failed to create OGL410RasterState:\nInvalid front face"); break;
			default: throw RenderDeviceError("Failed to create OGL410RasterState:\nUnsupported front face"); break;
		}

		switch (desc.cullFace)
		{
			case Face::Front: cullFace = GL_FRONT; break;
			case Face::Back: cullFace = GL_BACK; break;
			case Face::FrontAndBack: cullFace = GL_FRONT_AND_BACK; break;
			case Face::Invalid: throw RenderDeviceError("Failed to create OGL410RasterState:\nInvalid cull face"); break;
			default: throw RenderDeviceError("Failed to create OGL410RasterState:\nUnsupported cull face"); break;
		}

		switch (desc.rasterMode)
		{
			case RasterMode::Point: polygonMode = GL_POINT; break;
			case RasterMode::Line: polygonMode = GL_LINE; break;
			case RasterMode::Fill: polygonMode = GL_FILL; break;
			case RasterMode::Invalid: throw RenderDeviceError("Failed to create OGL410RasterState:\nInvalid raster mode"); break;
			default: throw RenderDeviceError("Failed to create OGL410RasterState:\nUnsupported raster mode"); break;
		}
	}

	virtual ~OGL410RasterState() final
	{

	}

	GLboolean cullEnabled;
	GLenum frontFace;
	GLenum cullFace;
	GLenum polygonMode;
};

class OGL410DepthStencilState final : public DepthStencilState
{
public:
	OGL410DepthStencilState(const DepthStencilStateDesc& desc)
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
				case Compare::Invalid: throw RenderDeviceError("Failed to create OGL410DepthStencilState:\nInvalid compare mode"); break;
				default: throw RenderDeviceError("Failed to create OGL410DepthStencilState:\nUnsupported compare mode"); break;
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
				case StencilAction::Invalid: throw RenderDeviceError("Failed to create OGL410DepthStencilState:\nInvalid stencil action"); break;
				default: throw RenderDeviceError("Failed to create OGL410DepthStencilState:\nUnsupported stencil action"); break;
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

	virtual ~OGL410DepthStencilState() final
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

void Magma::Framework::Graphics::OGL410RenderDevice::Init(Input::Window * window, const RenderDeviceSettings & settings)
{
#if defined(MAGMA_FRAMEWORK_USE_OPENGL)
	m_window = dynamic_cast<Input::GLWindow*>(window);
	if (m_window == nullptr)
		throw RenderDeviceError("Failed to init OGL410RenderDevice:\nCouldn't cast from Magma::Framework::Input::Window* to Magma::Framework::Input::GLWindow*");

	// Init glew
	window->MakeCurrent();
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		std::stringstream ss;
		ss << "Failed to init OGL410RenderDevice:" << std::endl << "GLEW failed to init:" << std::endl;
		ss << (const char*)glewGetErrorString(err);
		throw RenderDeviceError(ss.str());
	}

	// Check extensions
	if (!GLEW_EXT_texture_filter_anisotropic)
	{
		std::stringstream ss;
		ss << "Failed to init OGL410RenderDevice:" << std::endl << "GLEW coudln't get extension:" << std::endl;
		ss << "EXT_texture_filter_anisotropic";
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
	throw RenderDeviceError("Failed to call OGL410RenderDevice function:\nMAGMA_FRAMEWORK_USE_OPENGL must be defined to use this render device");
#endif
}

void Magma::Framework::Graphics::OGL410RenderDevice::Terminate()
{
#if defined(MAGMA_FRAMEWORK_USE_OPENGL)
	// Oonly terminated when the GLWindow closes
#else
	throw RenderDeviceError("Failed to call OGL410RenderDevice function:\nMAGMA_FRAMEWORK_USE_OPENGL must be defined to use this render device");
#endif
}

VertexShader * Magma::Framework::Graphics::OGL410RenderDevice::CreateVertexShader(const ShaderData& data)
{
#if defined(MAGMA_FRAMEWORK_USE_OPENGL)
	std::string source;
	OGL410Assembler::Assemble(data, source);
	return new OGL410VertexShader(source.c_str(), data);
#else
	throw RenderDeviceError("Failed to call OGL410RenderDevice function:\nMAGMA_FRAMEWORK_USE_OPENGL must be defined to use this render device");
	return nullptr;
#endif
}

void Magma::Framework::Graphics::OGL410RenderDevice::DestroyVertexShader(VertexShader * vertexShader)
{
#if defined(MAGMA_FRAMEWORK_USE_OPENGL)
	delete vertexShader;
#else
	throw RenderDeviceError("Failed to call OGL410RenderDevice function:\nMAGMA_FRAMEWORK_USE_OPENGL must be defined to use this render device");
#endif
}

PixelShader * Magma::Framework::Graphics::OGL410RenderDevice::CreatePixelShader(const ShaderData& data)
{
#if defined(MAGMA_FRAMEWORK_USE_OPENGL)
	std::string source;
	OGL410Assembler::Assemble(data, source);
	return new OGL410PixelShader(source.c_str(), data);
#else
	throw RenderDeviceError("Failed to call OGL410RenderDevice function:\nMAGMA_FRAMEWORK_USE_OPENGL must be defined to use this render device");
	return nullptr;
#endif
}

void Magma::Framework::Graphics::OGL410RenderDevice::DestroyPixelShader(PixelShader * pixelShader)
{
#if defined(MAGMA_FRAMEWORK_USE_OPENGL)
	delete pixelShader;
#else
	throw RenderDeviceError("Failed to call OGL410RenderDevice function:\nMAGMA_FRAMEWORK_USE_OPENGL must be defined to use this render device");
#endif
}

Pipeline * Magma::Framework::Graphics::OGL410RenderDevice::CreatePipeline(VertexShader * vertexShader, PixelShader * pixelShader)
{
#if defined(MAGMA_FRAMEWORK_USE_OPENGL)
	return new OGL410Pipeline(static_cast<OGL410VertexShader*>(vertexShader), static_cast<OGL410PixelShader*>(pixelShader));
#else
	throw RenderDeviceError("Failed to call OGL410RenderDevice function:\nMAGMA_FRAMEWORK_USE_OPENGL must be defined to use this render device");
	return nullptr;
#endif
}

void Magma::Framework::Graphics::OGL410RenderDevice::DestroyPipeline(Pipeline * pipeline)
{
#if defined(MAGMA_FRAMEWORK_USE_OPENGL)
	delete pipeline;
#else
	throw RenderDeviceError("Failed to call OGL410RenderDevice function:\nMAGMA_FRAMEWORK_USE_OPENGL must be defined to use this render device");
#endif	
}

void Magma::Framework::Graphics::OGL410RenderDevice::SetPipeline(Pipeline * pipeline)
{
#if defined(MAGMA_FRAMEWORK_USE_OPENGL)
	if (pipeline == nullptr)
		glBindProgramPipeline(0);
		//glUseProgram(0);
	else
		glBindProgramPipeline(static_cast<OGL410Pipeline*>(pipeline)->shaderPipeline);
		//glUseProgram(static_cast<OGL410Pipeline*>(pipeline)->shaderProgram);
	GL_CHECK_ERROR("Failed to set pipeline on OGL410RenderDevice");
#else
	throw RenderDeviceError("Failed to call OGL410RenderDevice function:\nMAGMA_FRAMEWORK_USE_OPENGL must be defined to use this render device");
#endif
}

VertexBuffer * Magma::Framework::Graphics::OGL410RenderDevice::CreateVertexBuffer(size_t size, const void * data, BufferUsage usage)
{
#if defined(MAGMA_FRAMEWORK_USE_OPENGL)
	return new OGL410VertexBuffer(size, data, usage);
#else
	throw RenderDeviceError("Failed to call OGL410RenderDevice function:\nMAGMA_FRAMEWORK_USE_OPENGL must be defined to use this render device");
	return nullptr;
#endif
}

void Magma::Framework::Graphics::OGL410RenderDevice::DestroyVertexBuffer(VertexBuffer * vertexBuffer)
{
#if defined(MAGMA_FRAMEWORK_USE_OPENGL)
	delete vertexBuffer;
#else
	throw RenderDeviceError("Failed to call OGL410RenderDevice function:\nMAGMA_FRAMEWORK_USE_OPENGL must be defined to use this render device");
#endif
}

VertexLayout * Magma::Framework::Graphics::OGL410RenderDevice::CreateVertexLayout(size_t elementCount, const VertexElement * elements, VertexShader * vertexShader)
{
#if defined(MAGMA_FRAMEWORK_USE_OPENGL)
	return new OGL410VertexLayout(elementCount, elements, static_cast<OGL410VertexShader*>(vertexShader));
#else
	throw RenderDeviceError("Failed to call OGL410RenderDevice function:\nMAGMA_FRAMEWORK_USE_OPENGL must be defined to use this render device");
	return nullptr;
#endif
}

void Magma::Framework::Graphics::OGL410RenderDevice::DestroyVertexLayout(VertexLayout * vertexLayout)
{
#if defined(MAGMA_FRAMEWORK_USE_OPENGL)
	delete vertexLayout;
#else
	throw RenderDeviceError("Failed to call OGL410RenderDevice function:\nMAGMA_FRAMEWORK_USE_OPENGL must be defined to use this render device");
#endif
}

VertexArray * Magma::Framework::Graphics::OGL410RenderDevice::CreateVertexArray(size_t bufferCount, VertexBuffer ** buffers, VertexLayout * layout)
{
#if defined(MAGMA_FRAMEWORK_USE_OPENGL)
	return new OGL410VertexArray(bufferCount, buffers, layout);
#else
	throw RenderDeviceError("Failed to call OGL410RenderDevice function:\nMAGMA_FRAMEWORK_USE_OPENGL must be defined to use this render device");
	return nullptr;
#endif	
}

void Magma::Framework::Graphics::OGL410RenderDevice::DestroyVertexArray(VertexArray * vertexArray)
{
#if defined(MAGMA_FRAMEWORK_USE_OPENGL)
	delete vertexArray;
#else
	throw RenderDeviceError("Failed to call OGL410RenderDevice function:\nMAGMA_FRAMEWORK_USE_OPENGL must be defined to use this render device");
#endif	
}

void Magma::Framework::Graphics::OGL410RenderDevice::SetVertexArray(VertexArray * vertexArray)
{
#if defined(MAGMA_FRAMEWORK_USE_OPENGL)
	if (vertexArray == nullptr)
		glBindVertexArray(0);
	else
		glBindVertexArray(static_cast<OGL410VertexArray*>(vertexArray)->vao);
	GL_CHECK_ERROR("Failed to set vertex array on OGL410RenderDevice");
#else
	throw RenderDeviceError("Failed to call OGL410RenderDevice function:\nMAGMA_FRAMEWORK_USE_OPENGL must be defined to use this render device");
#endif	
}

IndexBuffer * Magma::Framework::Graphics::OGL410RenderDevice::CreateIndexBuffer(IndexType type, size_t size, const void * data, BufferUsage usage)
{
#if defined(MAGMA_FRAMEWORK_USE_OPENGL)
	return new OGL410IndexBuffer(type, size, data, usage);
#else
	throw RenderDeviceError("Failed to call OGL410RenderDevice function:\nMAGMA_FRAMEWORK_USE_OPENGL must be defined to use this render device");
	return nullptr;
#endif
}

void Magma::Framework::Graphics::OGL410RenderDevice::DestroyIndexBuffer(IndexBuffer * indexBuffer)
{
#if defined(MAGMA_FRAMEWORK_USE_OPENGL)
	delete indexBuffer;
#else
	throw RenderDeviceError("Failed to call OGL410RenderDevice function:\nMAGMA_FRAMEWORK_USE_OPENGL must be defined to use this render device");
#endif
}

void Magma::Framework::Graphics::OGL410RenderDevice::SetIndexBuffer(IndexBuffer * indexBuffer)
{
#if defined(MAGMA_FRAMEWORK_USE_OPENGL)
	m_currentIndexBuffer = indexBuffer;
	if (indexBuffer == nullptr)
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	else
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, static_cast<OGL410IndexBuffer*>(indexBuffer)->ibo);
	GL_CHECK_ERROR("Failed to set index buffer on OGL410RenderDevice");
#else
	throw RenderDeviceError("Failed to call OGL410RenderDevice function:\nMAGMA_FRAMEWORK_USE_OPENGL must be defined to use this render device");
#endif	
}

Texture2D * Magma::Framework::Graphics::OGL410RenderDevice::CreateTexture2D(size_t width, size_t height, TextureFormat format, const void * data)
{
#if defined(MAGMA_FRAMEWORK_USE_OPENGL)
	return new OGL410Texture2D(width, height, format, data);
#else
	throw RenderDeviceError("Failed to call OGL410RenderDevice function:\nMAGMA_FRAMEWORK_USE_OPENGL must be defined to use this render device");
	return nullptr;
#endif
}

void Magma::Framework::Graphics::OGL410RenderDevice::DestroyTexture2D(Texture2D * texture)
{
#if defined(MAGMA_FRAMEWORK_USE_OPENGL)
	delete texture;
#else
	throw RenderDeviceError("Failed to call OGL410RenderDevice function:\nMAGMA_FRAMEWORK_USE_OPENGL must be defined to use this render device");
#endif
}

Sampler2D * Magma::Framework::Graphics::OGL410RenderDevice::CreateSampler2D(const Sampler2DDesc & desc)
{
#if defined(MAGMA_FRAMEWORK_USE_OPENGL)
	return new OGL410Sampler2D(desc);
#else
	throw RenderDeviceError("Failed to call OGL410RenderDevice function:\nMAGMA_FRAMEWORK_USE_OPENGL must be defined to use this render device");
#endif
}

void Magma::Framework::Graphics::OGL410RenderDevice::DestroySampler2D(Sampler2D* sampler)
{
#if defined(MAGMA_FRAMEWORK_USE_OPENGL)
	delete sampler;
#else
	throw RenderDeviceError("Failed to call OGL410RenderDevice function:\nMAGMA_FRAMEWORK_USE_OPENGL must be defined to use this render device");
#endif
}

RasterState * Magma::Framework::Graphics::OGL410RenderDevice::CreateRasterState(const RasterStateDesc & desc)
{
#if defined(MAGMA_FRAMEWORK_USE_OPENGL)
	return new OGL410RasterState(desc);
#else
	throw RenderDeviceError("Failed to call OGL410RenderDevice function:\nMAGMA_FRAMEWORK_USE_OPENGL must be defined to use this render device");
	return nullptr;
#endif	
}

void Magma::Framework::Graphics::OGL410RenderDevice::DestroyRasterState(RasterState * rasterState)
{
#if defined(MAGMA_FRAMEWORK_USE_OPENGL)
	delete rasterState;
#else
	throw RenderDeviceError("Failed to call OGL410RenderDevice function:\nMAGMA_FRAMEWORK_USE_OPENGL must be defined to use this render device");
#endif	
}

void Magma::Framework::Graphics::OGL410RenderDevice::SetRasterState(RasterState * rasterState)
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
		if (static_cast<OGL410RasterState*>(m_currentRasterState)->cullEnabled)
			glEnable(GL_CULL_FACE);
		else
			glDisable(GL_CULL_FACE);
		glFrontFace(static_cast<OGL410RasterState*>(m_currentRasterState)->frontFace);
		glCullFace(static_cast<OGL410RasterState*>(m_currentRasterState)->cullFace);
		glPolygonMode(GL_FRONT_AND_BACK, static_cast<OGL410RasterState*>(m_currentRasterState)->polygonMode);
	}

	GL_CHECK_ERROR("Failed to set raster state on OGL410RenderDevice");
#else
	throw RenderDeviceError("Failed to call OGL410RenderDevice function:\nMAGMA_FRAMEWORK_USE_OPENGL must be defined to use this render device");
#endif
}

DepthStencilState * Magma::Framework::Graphics::OGL410RenderDevice::CreateDepthStencilState(const DepthStencilStateDesc & desc)
{
#if defined(MAGMA_FRAMEWORK_USE_OPENGL)
	return new OGL410DepthStencilState(desc);
#else
	throw RenderDeviceError("Failed to call OGL410RenderDevice function:\nMAGMA_FRAMEWORK_USE_OPENGL must be defined to use this render device");
	return nullptr;
#endif	
}

void Magma::Framework::Graphics::OGL410RenderDevice::DestroyDepthStencilState(DepthStencilState * depthStencilState)
{
#if defined(MAGMA_FRAMEWORK_USE_OPENGL)
	delete depthStencilState;
#else
	throw RenderDeviceError("Failed to call OGL410RenderDevice function:\nMAGMA_FRAMEWORK_USE_OPENGL must be defined to use this render device");
#endif	
}

void Magma::Framework::Graphics::OGL410RenderDevice::SetDepthStencilState(DepthStencilState * depthStencilState)
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
		if (static_cast<OGL410DepthStencilState*>(m_currentDepthStencilState)->depthEnabled)
			glEnable(GL_DEPTH_TEST);
		else
			glDisable(GL_DEPTH_TEST);

		glDepthFunc(static_cast<OGL410DepthStencilState*>(m_currentDepthStencilState)->depthFunc);
		glDepthMask(static_cast<OGL410DepthStencilState*>(m_currentDepthStencilState)->depthWriteEnabled ? GL_TRUE : GL_FALSE);
		glDepthRange(static_cast<OGL410DepthStencilState*>(m_currentDepthStencilState)->depthNear, static_cast<OGL410DepthStencilState*>(m_currentDepthStencilState)->depthFar);

		// Stencil
		if (static_cast<OGL410DepthStencilState*>(m_currentDepthStencilState)->frontFaceStencilEnabled || static_cast<OGL410DepthStencilState*>(m_currentDepthStencilState)->backFaceStencilEnabled)
			glEnable(GL_STENCIL_TEST);
		else
			glDisable(GL_STENCIL_TEST);

		// Front face
		glStencilFuncSeparate(GL_FRONT, static_cast<OGL410DepthStencilState*>(m_currentDepthStencilState)->frontStencilFunc, static_cast<OGL410DepthStencilState*>(m_currentDepthStencilState)->frontFaceRef, static_cast<OGL410DepthStencilState*>(m_currentDepthStencilState)->frontFaceReadMask);
		glStencilMaskSeparate(GL_FRONT, static_cast<OGL410DepthStencilState*>(m_currentDepthStencilState)->frontFaceWriteMask);
		glStencilOpSeparate(GL_FRONT, static_cast<OGL410DepthStencilState*>(m_currentDepthStencilState)->frontFaceStencilFail, static_cast<OGL410DepthStencilState*>(m_currentDepthStencilState)->frontFaceDepthFail, static_cast<OGL410DepthStencilState*>(m_currentDepthStencilState)->frontFaceStencilPass);
		
		// Back face
		glStencilFuncSeparate(GL_BACK, static_cast<OGL410DepthStencilState*>(m_currentDepthStencilState)->backStencilFunc, static_cast<OGL410DepthStencilState*>(m_currentDepthStencilState)->backFaceRef, static_cast<OGL410DepthStencilState*>(m_currentDepthStencilState)->backFaceReadMask);
		glStencilMaskSeparate(GL_BACK, static_cast<OGL410DepthStencilState*>(m_currentDepthStencilState)->backFaceWriteMask);
		glStencilOpSeparate(GL_BACK, static_cast<OGL410DepthStencilState*>(m_currentDepthStencilState)->backFaceStencilFail, static_cast<OGL410DepthStencilState*>(m_currentDepthStencilState)->backFaceDepthFail, static_cast<OGL410DepthStencilState*>(m_currentDepthStencilState)->backFaceStencilPass);
	}

	GL_CHECK_ERROR("Failed to set depth stencil state on OGL410RenderDevice");
#else
	throw RenderDeviceError("Failed to call OGL410RenderDevice function:\nMAGMA_FRAMEWORK_USE_OPENGL must be defined to use this render device");
#endif
}

void Magma::Framework::Graphics::OGL410RenderDevice::Clear(glm::vec4 color, float depth, int stencil)
{
#if defined(MAGMA_FRAMEWORK_USE_OPENGL)
	glClearColor(color.r, color.g, color.b, color.a);
	glClearDepth(depth);
	glClearStencil(stencil);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
#else
	throw RenderDeviceError("Failed to call OGL410RenderDevice function:\nMAGMA_FRAMEWORK_USE_OPENGL must be defined to use this render device");
#endif
}

void Magma::Framework::Graphics::OGL410RenderDevice::DrawTriangles(size_t offset, size_t count)
{
#if defined(MAGMA_FRAMEWORK_USE_OPENGL)
	glDrawArrays(GL_TRIANGLES, offset, count);
	GL_CHECK_ERROR("Failed to draw triangles on OGL410RenderDevice");
#else
	throw RenderDeviceError("Failed to call OGL410RenderDevice function:\nMAGMA_FRAMEWORK_USE_OPENGL must be defined to use this render device");
#endif
}

void Magma::Framework::Graphics::OGL410RenderDevice::DrawTrianglesIndexed(size_t offset, size_t count)
{
#if defined(MAGMA_FRAMEWORK_USE_OPENGL)
#ifdef MAGMA_FRAMEWORK_RENDER_DEVICE_DEBUG
	if (m_currentIndexBuffer == nullptr)
		throw RenderDeviceError("Failed to draw indexed triangles on OGL410RenderDevice:\nNo index buffer currently bound");
#endif
	glDrawElements(GL_TRIANGLES, count, static_cast<OGL410IndexBuffer*>(m_currentIndexBuffer)->type, reinterpret_cast<const void*>(offset));
	GL_CHECK_ERROR("Failed to draw indexed triangles on OGL410RenderDevice");
#else
	throw RenderDeviceError("Failed to call OGL410RenderDevice function:\nMAGMA_FRAMEWORK_USE_OPENGL must be defined to use this render device");
#endif
}

void Magma::Framework::Graphics::OGL410RenderDevice::SetRenderTargets(Texture2D ** textures, size_t count)
{
#if defined(MAGMA_FRAMEWORK_USE_OPENGL)
	
#else
	throw RenderDeviceError("Failed to call OGL410RenderDevice function:\nMAGMA_FRAMEWORK_USE_OPENGL must be defined to use this render device");
#endif
}

void Magma::Framework::Graphics::OGL410RenderDevice::SwapBuffers()
{
#if defined(MAGMA_FRAMEWORK_USE_OPENGL)
	m_window->SwapBuffers();
#else
	throw RenderDeviceError("Failed to call OGL410RenderDevice function:\nMAGMA_FRAMEWORK_USE_OPENGL must be defined to use this render device");
#endif
}
