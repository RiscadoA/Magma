#include "OGL4RenderDevice.h"
#include "OGL4Assembler.h"
#include "Config.h"
	
#include "../Memory/StackAllocator.h"
#include "../Memory/PoolAllocator.h"
#include "../String/StringStream.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef MAGMA_FRAMEWORK_USE_OPENGL

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define MFG_POOL_32_ELEMENT_COUNT 2048
#define MFG_POOL_64_ELEMENT_COUNT 2048
#define MFG_POOL_256_ELEMENT_COUNT 2048
#define MFG_POOL_512_ELEMENT_COUNT 2048

typedef struct mfgOGL4Shader mfgOGL4Shader;

typedef struct
{
	const mfgMetaDataBindingPoint* bp;
	GLint location;
	GLboolean active;
	mfgOGL4Shader* shader;
} mfgOGL4BindingPoint;

struct mfgOGL4Shader
{
	mfgRenderDeviceObject base;
	GLint program;
	const mfgMetaData* md;
	mfgOGL4BindingPoint bps[16];
};

typedef struct
{
	mfgRenderDeviceObject base;
	GLint pipeline;
} mfgOGL4Pipeline;

typedef struct
{
	mfgRenderDeviceObject base;
	GLint cb;
} mfgOGL4ConstantBuffer;

typedef struct
{
	mfgRenderDeviceObject base;

	mfmU8 packAligment;
	GLenum internalFormat;
	GLenum format;
	GLenum type;
	GLuint tex;
	GLuint width;
} mfgOGL4Texture1D;

typedef struct
{
	mfgRenderDeviceObject base;

	mfmU8 packAligment;
	GLenum internalFormat;
	GLenum format;
	GLenum type;
	GLuint tex;
	GLuint width;
	GLuint height;
} mfgOGL4Texture2D;

typedef struct
{
	mfgRenderDeviceObject base;

	mfmU8 packAligment;
	GLenum internalFormat;
	GLenum format;
	GLenum type;
	GLuint tex;
	GLuint width;
	GLuint height;
	GLuint depth;
} mfgOGL4Texture3D;

typedef struct
{
	mfgRenderDeviceObject base;
	GLuint sampler;
} mfgOGL4Sampler;

typedef struct
{
	mfgRenderDeviceObject base;
	GLint vb;
} mfgOGL4VertexBuffer;

typedef struct
{
	GLuint bufferIndex;
	GLuint index;
	GLuint size;
	GLenum type;
	GLboolean normalized;
	GLsizei stride;
	const GLvoid* offset;
	GLboolean isInteger;
} mfgOGL4VertexElement;

typedef struct
{
	mfgRenderDeviceObject base;
	mfgOGL4VertexElement elements[14];
	mfmU64 elementCount;
	mfgOGL4Shader* vs;
} mfgOGL4VertexLayout;

typedef struct
{
	mfgRenderDeviceObject base;
	GLint va;
} mfgOGL4VertexArray;

typedef struct
{
	mfgRenderDeviceObject base;
	GLint ib;
	GLenum indexType;
} mfgOGL4IndexBuffer;

typedef struct
{
	mfgRenderDeviceObject base;
	GLboolean cullEnabled;
	GLenum frontFace;
	GLenum cullFace;
	GLenum polygonMode;
} mfgOGL4RasterState;

typedef struct
{
	mfgRenderDeviceObject base;

	GLboolean depthEnabled;
	GLboolean depthWriteEnabled;
	GLfloat depthNear;
	GLfloat depthFar;
	GLenum depthFunc;

	GLuint stencilRef;
	GLboolean stencilEnabled;
	GLuint stencilReadMask;
	GLuint stencilWriteMask;

	GLenum frontStencilFunc;
	GLenum frontFaceStencilFail;
	GLenum frontFaceStencilPass;
	GLenum frontFaceDepthFail;

	GLenum backStencilFunc;
	GLenum backFaceStencilFail;
	GLenum backFaceStencilPass;
	GLenum backFaceDepthFail;
} mfgOGL4DepthStencilState;

typedef struct
{
	mfgRenderDeviceObject base;

	GLboolean blendEnabled;
	GLenum srcFactor;
	GLenum dstFactor;
	GLenum blendOp;
	GLenum srcAlphaFactor;
	GLenum dstAlphaFactor;
	GLenum alphaBlendOp;
} mfgOGL4BlendState;

typedef struct
{
	mfgRenderDevice base;

	mfiWindow* window;

	void* allocator;

	mfsUTF8CodeUnit errorString[256];
	mfmU64 errorStringSize;

	mfmPoolAllocator* pool32;
	mfmU8 pool32Memory[MFM_POOL_ALLOCATOR_SIZE(MFG_POOL_32_ELEMENT_COUNT, 32)];

	mfmPoolAllocator* pool64;
	mfmU8 pool64Memory[MFM_POOL_ALLOCATOR_SIZE(MFG_POOL_64_ELEMENT_COUNT, 64)];

	mfmPoolAllocator* pool256;
	mfmU8 pool256Memory[MFM_POOL_ALLOCATOR_SIZE(MFG_POOL_256_ELEMENT_COUNT, 256)];

	mfmPoolAllocator* pool512;
	mfmU8 pool512Memory[MFM_POOL_ALLOCATOR_SIZE(MFG_POOL_512_ELEMENT_COUNT, 512)];

	mfmStackAllocator* stack;
	mfmU8 stackMemory[MFM_STACK_ALLOCATOR_SIZE(2048)];

	mfgRasterState* defaultRasterState;
	mfgDepthStencilState* defaultDepthStencilState;
	mfgBlendState* defaultBlendState;

	mfgOGL4IndexBuffer* currentIndexBuffer;
} mfgOGL4RenderDevice;

#define MFG_RETURN_ERROR(code, msg) {\
	mfgOGL4RenderDevice* oglRD = (mfgOGL4RenderDevice*)rd;\
	oglRD->errorStringSize = snprintf(oglRD->errorString, sizeof(oglRD->errorString),\
									 msg) + 1;\
	if (oglRD->errorStringSize > sizeof(oglRD->errorString))\
	oglRD->errorStringSize = sizeof(oglRD->errorString);\
	return code; }

#ifdef MAGMA_FRAMEWORK_DEBUG
#define MFG_CHECK_GL_ERROR() do { GLenum err = glGetError();\
if (err != 0) {\
	MFG_RETURN_ERROR(MFG_ERROR_INTERNAL, u8"glGetError() returned non zero on " __FUNCTION__)\
}} while (0)
#else
#define MFG_CHECK_GL_ERROR()
#endif

void mfgOGL4DestroyVertexShader(void* vs)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (vs == NULL) abort();
#endif
	mfgOGL4Shader* oglVS = vs;
	glDeleteProgram(oglVS->program);
	if (mfmDeallocate(((mfgOGL4RenderDevice*)oglVS->base.renderDevice)->pool512, oglVS) != MFM_ERROR_OKAY)
		abort();
#ifdef MAGMA_FRAMEWORK_DEBUG
	GLenum err = glGetError();
	if (err != 0)
		abort();
#endif
}

mfgError mfgOGL4CreateVertexShader(mfgRenderDevice* rd, mfgVertexShader** vs, const mfmU8* bytecode, mfmU64 bytecodeSize, const mfgMetaData* metaData)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{ if (rd == NULL || vs == NULL || bytecode == NULL || metaData == NULL) return MFG_ERROR_INVALID_ARGUMENTS; }
#endif

	mfgOGL4RenderDevice* oglRD = (mfgOGL4RenderDevice*)rd;

	// Allocate vertex shader
	mfgOGL4Shader* oglVS = NULL;
	if (mfmAllocate(oglRD->pool512, &oglVS, sizeof(mfgOGL4Shader)) != MFM_ERROR_OKAY)
		MFG_RETURN_ERROR(MFG_ERROR_ALLOCATION_FAILED, u8"Failed to allocate vertex shader on pool");

	// Init object
	oglVS->base.object.destructorFunc = &mfgOGL4DestroyVertexShader;
	oglVS->base.object.referenceCount = 0;
	oglVS->base.renderDevice = rd;

	// Create string stream
	mfsStream* ss;
	GLchar buffer[4096];
	if (mfsCreateStringStream(&ss, buffer, sizeof(buffer), oglRD->stack) != MFM_ERROR_OKAY)
		MFG_RETURN_ERROR(MFG_ERROR_INTERNAL, u8"Failed to create string stream for mfgOGL4Assemble");
	mfgError err = mfgOGL4Assemble(bytecode, bytecodeSize, metaData, ss);
	if (err != MFG_ERROR_OKAY)
		MFG_RETURN_ERROR(MFG_ERROR_INTERNAL, u8"mfgOGL4Assemble failed");
	if (mfsPutByte(ss, '\0') != MFS_ERROR_OKAY)
	{
		mfsDestroyStringStream(ss);
		MFG_RETURN_ERROR(MFG_ERROR_INTERNAL, u8"mfsPutByte returned error");
	}

	mfsDestroyStringStream(ss);
	mfsPutString(mfsOutStream, buffer);

	// Create shader program
	oglVS->md = metaData;
	{
		const GLchar* bufferSrc = buffer;
		oglVS->program = glCreateShaderProgramv(GL_VERTEX_SHADER, 1, &bufferSrc);

		GLint success;
		glGetProgramiv(oglVS->program, GL_LINK_STATUS, &success);
		if (!success)
		{
			GLchar infoLog[512];
			glGetProgramInfoLog(oglVS->program, 512, NULL, infoLog);
			glDeleteProgram(oglVS->program);
			mfsPrintFormatUTF8(mfsOutStream, u8"Failed to compile/link vertex shader, source:\n");
			mfsPrintFormatUTF8(mfsOutStream, bufferSrc);
			mfsPrintFormatUTF8(mfsOutStream, u8"\n");
			mfsFlush(mfsOutStream);
			MFG_RETURN_ERROR(MFG_ERROR_INTERNAL, infoLog);
		}
	}

	// Init binding points
	for (mfmU16 i = 0; i < 16; ++i)
	{
		oglVS->bps[i].active = GL_FALSE;
		oglVS->bps[i].shader = oglVS;
	}

	{
		mfgMetaDataBindingPoint* bp = oglVS->md->firstBindingPoint;
		for (mfmU16 i = 0; i < 16 && bp != NULL; ++i)
		{
			if (bp->type == MFG_CONSTANT_BUFFER)
			{
				GLchar buf[256];

				if (mfsCreateStringStream(&ss, buf, sizeof(buf), oglRD->stack) != MFM_ERROR_OKAY)
					MFG_RETURN_ERROR(MFG_ERROR_INTERNAL, u8"Failed to create string stream for binding point name");
				
				if (mfsPutString(ss, u8"buf_") != MFS_ERROR_OKAY)
				{
					mfsDestroyStringStream(ss);
					MFG_RETURN_ERROR(MFG_ERROR_INTERNAL, u8"mfsPutByte returned error");
				}
				if (mfsWrite(ss, bp->name, sizeof(bp->name), NULL) != MFS_ERROR_OKAY)
				{
					mfsDestroyStringStream(ss);
					MFG_RETURN_ERROR(MFG_ERROR_INTERNAL, u8"mfsPutByte returned error");
				}
				if (mfsPutByte(ss, '\0') != MFS_ERROR_OKAY)
				{
					mfsDestroyStringStream(ss);
					MFG_RETURN_ERROR(MFG_ERROR_INTERNAL, u8"mfsPutByte returned error");
				}
				mfsDestroyStringStream(ss);

				oglVS->bps[i].bp = bp;
				oglVS->bps[i].location = glGetUniformBlockIndex(oglVS->program, buf);
				glUniformBlockBinding(oglVS->program, oglVS->bps[i].location, oglVS->bps[i].location);
				oglVS->bps[i].active = GL_TRUE;
			}
			else if (bp->type == MFG_TEXTURE_1D)
			{
				GLchar buf[256];

				if (mfsCreateStringStream(&ss, buf, sizeof(buf), oglRD->stack) != MFM_ERROR_OKAY)
					MFG_RETURN_ERROR(MFG_ERROR_INTERNAL, u8"Failed to create string stream for binding point name");

				if (mfsPutString(ss, u8"tex1d_") != MFS_ERROR_OKAY)
				{
					mfsDestroyStringStream(ss);
					MFG_RETURN_ERROR(MFG_ERROR_INTERNAL, u8"mfsPutString returned error");
				}	
				if (mfsPrintFormatUTF8(ss, u8"%d", ((mfgMetaDataTexture1D*)bp)->id) != MFS_ERROR_OKAY)
				{
					mfsDestroyStringStream(ss);
					MFG_RETURN_ERROR(MFG_ERROR_INTERNAL, u8"mfsPrintFormatUTF8 returned error");
				}
				if (mfsPutByte(ss, '\0') != MFS_ERROR_OKAY)
				{
					mfsDestroyStringStream(ss);
					MFG_RETURN_ERROR(MFG_ERROR_INTERNAL, u8"mfsPutByte returned error");
				}
				mfsDestroyStringStream(ss);

				oglVS->bps[i].bp = bp;
				oglVS->bps[i].location = glGetUniformLocation(oglVS->program, buf);
				oglVS->bps[i].active = GL_TRUE;
			}
			else if (bp->type == MFG_TEXTURE_2D)
			{
				GLchar buf[256];

				if (mfsCreateStringStream(&ss, buf, sizeof(buf), oglRD->stack) != MFM_ERROR_OKAY)
					MFG_RETURN_ERROR(MFG_ERROR_INTERNAL, u8"Failed to create string stream for binding point name");

				if (mfsPutString(ss, u8"tex2d_") != MFS_ERROR_OKAY)
				{
					mfsDestroyStringStream(ss);
					MFG_RETURN_ERROR(MFG_ERROR_INTERNAL, u8"mfsPutString returned error");
				}
				if (mfsPrintFormatUTF8(ss, u8"%d", ((mfgMetaDataTexture1D*)bp)->id) != MFS_ERROR_OKAY)
				{
					mfsDestroyStringStream(ss);
					MFG_RETURN_ERROR(MFG_ERROR_INTERNAL, u8"mfsPrintFormatUTF8 returned error");
				}
				if (mfsPutByte(ss, '\0') != MFS_ERROR_OKAY)
				{
					mfsDestroyStringStream(ss);
					MFG_RETURN_ERROR(MFG_ERROR_INTERNAL, u8"mfsPutByte returned error");
				}
				mfsDestroyStringStream(ss);

				oglVS->bps[i].bp = bp;
				oglVS->bps[i].location = glGetUniformLocation(oglVS->program, buf);
				oglVS->bps[i].active = GL_TRUE;
			}
			else if (bp->type == MFG_TEXTURE_3D)
			{
				GLchar buf[256];

				if (mfsCreateStringStream(&ss, buf, sizeof(buf), oglRD->stack) != MFM_ERROR_OKAY)
					MFG_RETURN_ERROR(MFG_ERROR_INTERNAL, u8"Failed to create string stream for binding point name");

				if (mfsPutString(ss, u8"tex3d_") != MFS_ERROR_OKAY)
				{
					mfsDestroyStringStream(ss);
					MFG_RETURN_ERROR(MFG_ERROR_INTERNAL, u8"mfsPutString returned error");
				}
				if (mfsPrintFormatUTF8(ss, u8"%d", ((mfgMetaDataTexture1D*)bp)->id) != MFS_ERROR_OKAY)
				{
					mfsDestroyStringStream(ss);
					MFG_RETURN_ERROR(MFG_ERROR_INTERNAL, u8"mfsPrintFormatUTF8 returned error");
				}
				if (mfsPutByte(ss, '\0') != MFS_ERROR_OKAY)
				{
					mfsDestroyStringStream(ss);
					MFG_RETURN_ERROR(MFG_ERROR_INTERNAL, u8"mfsPutByte returned error");
				}
				mfsDestroyStringStream(ss);

				oglVS->bps[i].bp = bp;
				oglVS->bps[i].location = glGetUniformLocation(oglVS->program, buf);
				oglVS->bps[i].active = GL_TRUE;
			}

			bp = bp->next;
		}
	}

	*vs = oglVS;

	MFG_CHECK_GL_ERROR();
	return MFG_ERROR_OKAY;
}

mfgError mfgOGL4GetVertexShaderBindingPoint(mfgRenderDevice* rd, mfgBindingPoint** bp, mfgVertexShader* vs, const mfsUTF8CodeUnit* name)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (rd == NULL || bp == NULL || vs == NULL || name == NULL) return MFG_ERROR_INVALID_ARGUMENTS;
#endif
	mfgOGL4Shader* oglVS = vs;
	
	for (mfmU64 i = 0; i < 16; ++i)
		if (oglVS->bps[i].active == GL_TRUE)
		{
			if (strcmp(name, oglVS->bps[i].bp->name) == 0)
			{
				*bp = &oglVS->bps[i];
				return MFG_ERROR_OKAY;
			}
		}

	return MFG_ERROR_NOT_FOUND;
}

void mfgOGL4DestroyPixelShader(void* ps)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (ps == NULL) abort();
#endif
	mfgOGL4Shader* oglPS = ps;
	glDeleteProgram(oglPS->program);
	if (mfmDeallocate(((mfgOGL4RenderDevice*)oglPS->base.renderDevice)->pool512, oglPS) != MFM_ERROR_OKAY)
		abort();
#ifdef MAGMA_FRAMEWORK_DEBUG
	GLenum err = glGetError();
	if (err != 0)
		abort();
#endif
}


mfgError mfgOGL4CreatePixelShader(mfgRenderDevice* rd, mfgPixelShader** ps, const mfmU8* bytecode, mfmU64 bytecodeSize, const mfgMetaData* metaData)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{ if (rd == NULL || ps == NULL || bytecode == NULL || metaData == NULL) return MFG_ERROR_INVALID_ARGUMENTS; }
#endif

	mfgOGL4RenderDevice* oglRD = (mfgOGL4RenderDevice*)rd;
	
	// Allocate pixel shader
	mfgOGL4Shader* oglPS = NULL;
	if (mfmAllocate(oglRD->pool512, &oglPS, sizeof(mfgOGL4Shader)) != MFM_ERROR_OKAY)
		MFG_RETURN_ERROR(MFG_ERROR_ALLOCATION_FAILED, u8"Failed to allocate pixel shader on pool");

	// Init object
	oglPS->base.object.destructorFunc = &mfgOGL4DestroyPixelShader;
	oglPS->base.object.referenceCount = 0;
	oglPS->base.renderDevice = rd;

	// Create string stream
	mfsStream* ss;
	GLchar buffer[4096];
	if (mfsCreateStringStream(&ss, buffer, sizeof(buffer), oglRD->stack) != MFM_ERROR_OKAY)
		MFG_RETURN_ERROR(MFG_ERROR_INTERNAL, u8"Failed to create string stream for mfgOGL4Assemble");
	mfgError err = mfgOGL4Assemble(bytecode, bytecodeSize, metaData, ss);
	if (err != MFG_ERROR_OKAY)
		MFG_RETURN_ERROR(MFG_ERROR_INTERNAL, u8"mfgOGL4Assemble failed");
	if (mfsPutByte(ss, '\0') != MFS_ERROR_OKAY)
	{
		mfsDestroyStringStream(ss);
		MFG_RETURN_ERROR(MFG_ERROR_INTERNAL, u8"mfsPutByte returned error");
	}
	mfsDestroyStringStream(ss);

	mfsPutString(mfsOutStream, buffer);

	// Create shader program
	oglPS->md = metaData;
	{
		const GLchar* bufferSrc = buffer;
		oglPS->program = glCreateShaderProgramv(GL_FRAGMENT_SHADER, 1, &bufferSrc);

		GLint success;
		glGetProgramiv(oglPS->program, GL_LINK_STATUS, &success);
		if (!success)
		{
			GLchar infoLog[512];
			glGetProgramInfoLog(oglPS->program, 512, NULL, infoLog);
			glDeleteProgram(oglPS->program);
			mfsPrintFormatUTF8(mfsOutStream, u8"Failed to compile/link pixel shader, source:\n");
			mfsPrintFormatUTF8(mfsOutStream, bufferSrc);
			mfsPrintFormatUTF8(mfsOutStream, u8"\n");
			mfsFlush(mfsOutStream);
			MFG_RETURN_ERROR(MFG_ERROR_INTERNAL, infoLog);
		}
	}

	// Init binding points
	for (mfmU16 i = 0; i < 16; ++i)
	{
		oglPS->bps[i].active = GL_FALSE;
		oglPS->bps[i].shader = oglPS;
	}

	{
		mfgMetaDataBindingPoint* bp = oglPS->md->firstBindingPoint;
		for (mfmU16 i = 0; i < 16 && bp != NULL; ++i)
		{
			if (bp->type == MFG_CONSTANT_BUFFER)
			{
				GLchar buf[256];

				if (mfsCreateStringStream(&ss, buf, sizeof(buf), oglRD->stack) != MFM_ERROR_OKAY)
					MFG_RETURN_ERROR(MFG_ERROR_INTERNAL, u8"Failed to create string stream for binding point name");

				if (mfsPutString(ss, u8"buf_") != MFS_ERROR_OKAY)
				{
					mfsDestroyStringStream(ss);
					MFG_RETURN_ERROR(MFG_ERROR_INTERNAL, u8"mfsPutByte returned error");
				}
				if (mfsWrite(ss, bp->name, sizeof(bp->name), NULL) != MFS_ERROR_OKAY)
				{
					mfsDestroyStringStream(ss);
					MFG_RETURN_ERROR(MFG_ERROR_INTERNAL, u8"mfsPutByte returned error");
				}
				if (mfsPutByte(ss, '\0') != MFS_ERROR_OKAY)
				{
					mfsDestroyStringStream(ss);
					MFG_RETURN_ERROR(MFG_ERROR_INTERNAL, u8"mfsPutByte returned error");
				}
				mfsDestroyStringStream(ss);

				oglPS->bps[i].bp = bp;
				oglPS->bps[i].location = glGetUniformBlockIndex(oglPS->program, buf);
				glUniformBlockBinding(oglPS->program, oglPS->bps[i].location, oglPS->bps[i].location);
				oglPS->bps[i].active = GL_TRUE;
			}
			else if (bp->type == MFG_TEXTURE_1D)
			{
				GLchar buf[256];

				if (mfsCreateStringStream(&ss, buf, sizeof(buf), oglRD->stack) != MFM_ERROR_OKAY)
					MFG_RETURN_ERROR(MFG_ERROR_INTERNAL, u8"Failed to create string stream for binding point name");

				if (mfsPutString(ss, u8"tex1d_") != MFS_ERROR_OKAY)
				{
					mfsDestroyStringStream(ss);
					MFG_RETURN_ERROR(MFG_ERROR_INTERNAL, u8"mfsPutString returned error");
				}
				if (mfsPrintFormatUTF8(ss, u8"%d", ((mfgMetaDataTexture1D*)bp)->id) != MFS_ERROR_OKAY)
				{
					mfsDestroyStringStream(ss);
					MFG_RETURN_ERROR(MFG_ERROR_INTERNAL, u8"mfsPrintFormatUTF8 returned error");
				}
				if (mfsPutByte(ss, '\0') != MFS_ERROR_OKAY)
				{
					mfsDestroyStringStream(ss);
					MFG_RETURN_ERROR(MFG_ERROR_INTERNAL, u8"mfsPutByte returned error");
				}
				mfsDestroyStringStream(ss);

				oglPS->bps[i].bp = bp;
				oglPS->bps[i].location = glGetUniformLocation(oglPS->program, buf);
				oglPS->bps[i].active = GL_TRUE;
			}
			else if (bp->type == MFG_TEXTURE_2D)
			{
				GLchar buf[256];

				if (mfsCreateStringStream(&ss, buf, sizeof(buf), oglRD->stack) != MFM_ERROR_OKAY)
					MFG_RETURN_ERROR(MFG_ERROR_INTERNAL, u8"Failed to create string stream for binding point name");

				if (mfsPutString(ss, u8"tex2d_") != MFS_ERROR_OKAY)
				{
					mfsDestroyStringStream(ss);
					MFG_RETURN_ERROR(MFG_ERROR_INTERNAL, u8"mfsPutString returned error");
				}
				if (mfsPrintFormatUTF8(ss, u8"%d", ((mfgMetaDataTexture1D*)bp)->id) != MFS_ERROR_OKAY)
				{
					mfsDestroyStringStream(ss);
					MFG_RETURN_ERROR(MFG_ERROR_INTERNAL, u8"mfsPrintFormatUTF8 returned error");
				}
				if (mfsPutByte(ss, '\0') != MFS_ERROR_OKAY)
				{
					mfsDestroyStringStream(ss);
					MFG_RETURN_ERROR(MFG_ERROR_INTERNAL, u8"mfsPutByte returned error");
				}
				mfsDestroyStringStream(ss);

				oglPS->bps[i].bp = bp;
				oglPS->bps[i].location = glGetUniformLocation(oglPS->program, buf);
				oglPS->bps[i].active = GL_TRUE;
			}
			else if (bp->type == MFG_TEXTURE_3D)
			{
				GLchar buf[256];

				if (mfsCreateStringStream(&ss, buf, sizeof(buf), oglRD->stack) != MFM_ERROR_OKAY)
					MFG_RETURN_ERROR(MFG_ERROR_INTERNAL, u8"Failed to create string stream for binding point name");

				if (mfsPutString(ss, u8"tex3d_") != MFS_ERROR_OKAY)
				{
					mfsDestroyStringStream(ss);
					MFG_RETURN_ERROR(MFG_ERROR_INTERNAL, u8"mfsPutString returned error");
				}
				if (mfsPrintFormatUTF8(ss, u8"%d", ((mfgMetaDataTexture1D*)bp)->id) != MFS_ERROR_OKAY)
				{
					mfsDestroyStringStream(ss);
					MFG_RETURN_ERROR(MFG_ERROR_INTERNAL, u8"mfsPrintFormatUTF8 returned error");
				}
				if (mfsPutByte(ss, '\0') != MFS_ERROR_OKAY)
				{
					mfsDestroyStringStream(ss);
					MFG_RETURN_ERROR(MFG_ERROR_INTERNAL, u8"mfsPutByte returned error");
				}
				mfsDestroyStringStream(ss);

				oglPS->bps[i].bp = bp;
				oglPS->bps[i].location = glGetUniformLocation(oglPS->program, buf);
				oglPS->bps[i].active = GL_TRUE;
			}

			bp = bp->next;
		}
	}

	*ps = oglPS;

	MFG_CHECK_GL_ERROR();
	return MFG_ERROR_OKAY;
}

mfgError mfgOGL4GetPixelShaderBindingPoint(mfgRenderDevice* rd, mfgBindingPoint** bp, mfgPixelShader* ps, const mfsUTF8CodeUnit* name)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (rd == NULL || bp == NULL || ps == NULL || name == NULL) return MFG_ERROR_INVALID_ARGUMENTS;
#endif
	mfgOGL4Shader* oglPS = ps;

	for (mfmU64 i = 0; i < 16; ++i)
		if (oglPS->bps[i].active == GL_TRUE)
		{
			if (strcmp(name, oglPS->bps[i].bp->name) == 0)
			{
				*bp = &oglPS->bps[i];
				return MFG_ERROR_OKAY;
			}
		}

	return MFG_ERROR_NOT_FOUND;
}

mfgError mfgOGL4BindConstantBuffer(mfgRenderDevice* rd, mfgBindingPoint* bp, mfgConstantBuffer* cb)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (rd == NULL || bp == NULL) return MFG_ERROR_INVALID_ARGUMENTS;
#endif
	mfgOGL4BindingPoint* oglBP = bp;
	mfgOGL4ConstantBuffer* oglCB = cb;

	if (oglCB == NULL)
		glBindBufferBase(GL_UNIFORM_BUFFER, oglBP->location, 0);
	else
		glBindBufferBase(GL_UNIFORM_BUFFER, oglBP->location, oglCB->cb);

	MFG_CHECK_GL_ERROR();
	return MFG_ERROR_OKAY;
}

mfgError mfgOGL4BindConstantBufferRange(mfgRenderDevice* rd, mfgBindingPoint* bp, mfgConstantBuffer* cb, mfmU64 offset, mfmU64 size)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (rd == NULL || bp == NULL || cb == NULL) return MFG_ERROR_INVALID_ARGUMENTS;
#endif
	mfgOGL4BindingPoint* oglBP = bp;
	mfgOGL4ConstantBuffer* oglCB = cb;

	glBindBufferRange(GL_UNIFORM_BUFFER, oglBP->location, oglCB->cb, offset, size);

	MFG_CHECK_GL_ERROR();
	return MFG_ERROR_OKAY;
}

mfgError mfgOGL4BindTexture1D(mfgRenderDevice* rd, mfgBindingPoint* bp, mfgTexture1D* tex)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (rd == NULL || bp == NULL) return MFG_ERROR_INVALID_ARGUMENTS;
#endif
	mfgOGL4BindingPoint* oglBP = bp;
	mfgOGL4Texture1D* oglT = tex;

	glActiveTexture(GL_TEXTURE0 + oglBP->location);
	if (oglT == NULL)
		glBindTexture(GL_TEXTURE_1D, 0);
	else
		glBindTexture(GL_TEXTURE_1D, oglT->tex);
	glProgramUniform1i(oglBP->shader->program, oglBP->location, oglBP->location);

	MFG_CHECK_GL_ERROR();
	return MFG_ERROR_OKAY;
}

mfgError mfgOGL4BindTexture2D(mfgRenderDevice* rd, mfgBindingPoint* bp, mfgTexture2D* tex)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (rd == NULL || bp == NULL) return MFG_ERROR_INVALID_ARGUMENTS;
#endif
	mfgOGL4BindingPoint* oglBP = bp;
	mfgOGL4Texture2D* oglT = tex;

	glActiveTexture(GL_TEXTURE0 + oglBP->location);
	if (oglT == NULL)
		glBindTexture(GL_TEXTURE_2D, 0);
	else
		glBindTexture(GL_TEXTURE_2D, oglT->tex);
	glProgramUniform1i(oglBP->shader->program, oglBP->location, oglBP->location);

	MFG_CHECK_GL_ERROR();
	return MFG_ERROR_OKAY;
}

mfgError mfgOGL4BindTexture3D(mfgRenderDevice* rd, mfgBindingPoint* bp, mfgTexture3D* tex)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (rd == NULL || bp == NULL) return MFG_ERROR_INVALID_ARGUMENTS;
#endif
	mfgOGL4BindingPoint* oglBP = bp;
	mfgOGL4Texture3D* oglT = tex;

	glActiveTexture(GL_TEXTURE0 + oglBP->location);
	if (oglT == NULL)
		glBindTexture(GL_TEXTURE_3D, 0);
	else
		glBindTexture(GL_TEXTURE_3D, oglT->tex);
	glProgramUniform1i(oglBP->shader->program, oglBP->location, oglBP->location);

	MFG_CHECK_GL_ERROR();
	return MFG_ERROR_OKAY;
}

mfgError mfgOGL4BindSampler(mfgRenderDevice* rd, mfgBindingPoint* bp, mfgSampler* sampler)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (rd == NULL || bp == NULL) return MFG_ERROR_INVALID_ARGUMENTS;
#endif
	mfgOGL4BindingPoint* oglBP = bp;
	mfgOGL4Sampler* oglS = sampler;

	if (oglS == NULL)
		glBindSampler(oglBP->location, 0);
	else
		glBindSampler(oglBP->location, oglS->sampler);

	MFG_CHECK_GL_ERROR();
	return MFG_ERROR_OKAY;
}

void mfgOGL4DestroyPipeline(void* pp)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (pp == NULL) abort();
#endif
	mfgOGL4Pipeline* oglPP = pp;
	glDeleteProgramPipelines(1, &oglPP->pipeline);
	if (mfmDeallocate(((mfgOGL4RenderDevice*)oglPP->base.renderDevice)->pool32, oglPP) != MFM_ERROR_OKAY)
		abort();
#ifdef MAGMA_FRAMEWORK_DEBUG
	GLenum err = glGetError();
	if (err != 0)
		abort();
#endif
}

mfgError mfgOGL4CreatePipeline(mfgRenderDevice* rd, mfgPipeline** pp, mfgVertexShader* vs, mfgPixelShader* ps)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{ if (rd == NULL || pp == NULL || vs == NULL || ps == NULL) return MFG_ERROR_INVALID_ARGUMENTS; }
#endif

	mfgOGL4RenderDevice* oglRD = (mfgOGL4RenderDevice*)rd;

	// Allocate pipeline
	mfgOGL4Pipeline* oglPP = NULL;
	if (mfmAllocate(oglRD->pool32, &oglPP, sizeof(mfgOGL4Pipeline)) != MFM_ERROR_OKAY)
		MFG_RETURN_ERROR(MFG_ERROR_ALLOCATION_FAILED, u8"Failed to allocate pipeline on pool");

	// Init object
	oglPP->base.object.destructorFunc = &mfgOGL4DestroyPipeline;
	oglPP->base.object.referenceCount = 0;
	oglPP->base.renderDevice = rd;

	// Create shader pipeline
	glGenProgramPipelines(1, &oglPP->pipeline);
	glUseProgramStages(oglPP->pipeline, GL_VERTEX_SHADER_BIT, ((mfgOGL4Shader*)vs)->program);
	glUseProgramStages(oglPP->pipeline, GL_FRAGMENT_SHADER_BIT, ((mfgOGL4Shader*)ps)->program);

	*pp = oglPP;

	MFG_CHECK_GL_ERROR();
	return MFG_ERROR_OKAY;
}

mfgError mfgOGL4SetPipeline(mfgRenderDevice* rd, mfgPipeline* pp)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{ if (rd == NULL) return MFG_ERROR_INVALID_ARGUMENTS; }
#endif

	mfgOGL4RenderDevice* oglRD = (mfgOGL4RenderDevice*)rd;

	// Set pipeline
	mfgOGL4Pipeline* oglPP = pp;
	if (oglPP == NULL)
		glBindProgramPipeline(0);
	else
		glBindProgramPipeline(oglPP->pipeline);

	MFG_CHECK_GL_ERROR();
	return MFG_ERROR_OKAY;
}

void mfgOGL4DestroyConstantBuffer(void* buffer)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (buffer == NULL) abort();
#endif
	mfgOGL4ConstantBuffer* oglCB = buffer;
	glDeleteBuffers(1, &oglCB->cb);
	if (mfmDeallocate(((mfgOGL4RenderDevice*)oglCB->base.renderDevice)->pool32, oglCB) != MFM_ERROR_OKAY)
		abort();
#ifdef MAGMA_FRAMEWORK_DEBUG
	GLenum err = glGetError();
	if (err != 0)
		abort();
#endif
}

mfgError mfgOGL4CreateConstantBuffer(mfgRenderDevice* rd, mfgConstantBuffer** cb, mfmU64 size, const void* data, mfgEnum usage)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{ if (rd == NULL || cb == NULL) return MFG_ERROR_INVALID_ARGUMENTS; }
#endif

	mfgOGL4RenderDevice* oglRD = (mfgOGL4RenderDevice*)rd;

	// Allocate constant buffer
	mfgOGL4ConstantBuffer* oglCB = NULL;
	if (mfmAllocate(oglRD->pool32, &oglCB, sizeof(mfgOGL4ConstantBuffer)) != MFM_ERROR_OKAY)
		MFG_RETURN_ERROR(MFG_ERROR_ALLOCATION_FAILED, u8"Failed to allocate constant buffer on pool");

	// Init object
	oglCB->base.object.destructorFunc = &mfgOGL4DestroyConstantBuffer;
	oglCB->base.object.referenceCount = 0;
	oglCB->base.renderDevice = rd;

	// Create constant buffer
	GLenum gl_usage;

	switch (usage)
	{
		case MFG_USAGE_DEFAULT: gl_usage = GL_STATIC_DRAW; break;
		case MFG_USAGE_STATIC: gl_usage = GL_STATIC_DRAW; break;
		case MFG_USAGE_DYNAMIC: gl_usage = GL_DYNAMIC_DRAW; break;
		default: MFG_RETURN_ERROR(MFG_ERROR_INVALID_ARGUMENTS, "Unsupported usage mode");
	}

	glGenBuffers(1, &oglCB->cb);
	glBindBuffer(GL_UNIFORM_BUFFER, oglCB->cb);
	glBufferData(GL_UNIFORM_BUFFER, size, data, gl_usage);

	*cb = oglCB;

	MFG_CHECK_GL_ERROR();
	return MFG_ERROR_OKAY;
}

mfgError mfgOGL4MapConstantBuffer(mfgRenderDevice* rd, mfgConstantBuffer* cb, void** memory)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{ if (rd == NULL || cb == NULL || memory == NULL) return MFG_ERROR_INVALID_ARGUMENTS; }
#endif

	mfgOGL4RenderDevice* oglRD = (mfgOGL4RenderDevice*)rd;

	// Map vertex buffer
	mfgOGL4ConstantBuffer* oglCB = cb;
	
	glBindBuffer(GL_UNIFORM_BUFFER, oglCB->cb);
	*memory = glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);

	MFG_CHECK_GL_ERROR();
	return MFG_ERROR_OKAY;
}

mfgError mfgOGL4UnmapConstantBuffer(mfgRenderDevice* rd, mfgConstantBuffer* cb)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{ if (rd == NULL || cb == NULL) return MFG_ERROR_INVALID_ARGUMENTS; }
#endif

	mfgOGL4RenderDevice* oglRD = (mfgOGL4RenderDevice*)rd;

	// Unmap vertex buffer
	mfgOGL4ConstantBuffer* oglCB = cb;

	glBindBuffer(GL_UNIFORM_BUFFER, oglCB->cb);
	glUnmapBuffer(GL_UNIFORM_BUFFER);

	MFG_CHECK_GL_ERROR();
	return MFG_ERROR_OKAY;
}


void mfgOGL4DestroyVertexBuffer(void* buffer)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (buffer == NULL) abort();
#endif
	mfgOGL4VertexBuffer* oglVB = buffer;
	glDeleteBuffers(1, &oglVB->vb);
	if (mfmDeallocate(((mfgOGL4RenderDevice*)oglVB->base.renderDevice)->pool32, oglVB) != MFM_ERROR_OKAY)
		abort();
#ifdef MAGMA_FRAMEWORK_DEBUG
	GLenum err = glGetError();
	if (err != 0)
		abort();
#endif
}

mfgError mfgOGL4CreateVertexBuffer(mfgRenderDevice* rd, mfgVertexBuffer** vb, mfmU64 size, const void* data, mfgEnum usage)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{
		if (rd == NULL || vb == NULL) return MFG_ERROR_INVALID_ARGUMENTS;
	}
#endif

	mfgOGL4RenderDevice* oglRD = (mfgOGL4RenderDevice*)rd;

	// Allocate vertex buffer
	mfgOGL4VertexBuffer* oglVB = NULL;
	if (mfmAllocate(oglRD->pool32, &oglVB, sizeof(mfgOGL4VertexBuffer)) != MFM_ERROR_OKAY)
		MFG_RETURN_ERROR(MFG_ERROR_ALLOCATION_FAILED, u8"Failed to allocate vertex buffer on pool");

	// Init object
	oglVB->base.object.destructorFunc = &mfgOGL4DestroyVertexBuffer;
	oglVB->base.object.referenceCount = 0;
	oglVB->base.renderDevice = rd;

	// Create vertex buffer
	GLenum gl_usage;

	switch (usage)
	{
		case MFG_USAGE_DEFAULT: gl_usage = GL_STATIC_DRAW; break;
		case MFG_USAGE_STATIC: gl_usage = GL_STATIC_DRAW; break;
		case MFG_USAGE_DYNAMIC: gl_usage = GL_DYNAMIC_DRAW; break;
		default: MFG_RETURN_ERROR(MFG_ERROR_INVALID_ARGUMENTS, "Unsupported usage mode");
	}

	glGenBuffers(1, &oglVB->vb);
	glBindBuffer(GL_ARRAY_BUFFER, oglVB->vb);
	glBufferData(GL_ARRAY_BUFFER, size, data, gl_usage);

	*vb = oglVB;

	MFG_CHECK_GL_ERROR();
	return MFG_ERROR_OKAY;
}


mfgError mfgOGL4MapVertexBuffer(mfgRenderDevice* rd, mfgVertexBuffer* vb, void** memory)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{
		if (rd == NULL || vb == NULL || memory == NULL) return MFG_ERROR_INVALID_ARGUMENTS;
	}
#endif

	mfgOGL4RenderDevice* oglRD = (mfgOGL4RenderDevice*)rd;

	// Map vertex buffer
	mfgOGL4VertexBuffer* oglVB = vb;

	glBindBuffer(GL_ARRAY_BUFFER, oglVB->vb);
	*memory = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

	MFG_CHECK_GL_ERROR();
	return MFG_ERROR_OKAY;
}

mfgError mfgOGL4UnmapVertexBuffer(mfgRenderDevice* rd, mfgVertexBuffer* vb)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{
		if (rd == NULL || vb == NULL) return MFG_ERROR_INVALID_ARGUMENTS;
	}
#endif

	mfgOGL4RenderDevice* oglRD = (mfgOGL4RenderDevice*)rd;

	// Unmap vertex buffer
	mfgOGL4VertexBuffer* oglVB = vb;

	glBindBuffer(GL_ARRAY_BUFFER, oglVB->vb);
	glUnmapBuffer(GL_ARRAY_BUFFER);

	MFG_CHECK_GL_ERROR();
	return MFG_ERROR_OKAY;
}

void mfgOGL4DestroyIndexBuffer(void* buffer)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (buffer == NULL) abort();
#endif
	mfgOGL4IndexBuffer* oglIB = buffer;
	glDeleteBuffers(1, &oglIB->ib);
	if (mfmDeallocate(((mfgOGL4RenderDevice*)oglIB->base.renderDevice)->pool32, oglIB) != MFM_ERROR_OKAY)
		abort();
#ifdef MAGMA_FRAMEWORK_DEBUG
	GLenum err = glGetError();
	if (err != 0)
		abort();
#endif
}

mfgError mfgOGL4CreateIndexBuffer(mfgRenderDevice* rd, mfgIndexBuffer** ib, mfmU64 size, const void* data, mfgEnum format, mfgEnum usage)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{
		if (rd == NULL || ib == NULL) return MFG_ERROR_INVALID_ARGUMENTS;
	}
#endif

	mfgOGL4RenderDevice* oglRD = (mfgOGL4RenderDevice*)rd;

	// Allocate index buffer
	mfgOGL4IndexBuffer* oglIB = NULL;
	if (mfmAllocate(oglRD->pool32, &oglIB, sizeof(mfgOGL4IndexBuffer)) != MFM_ERROR_OKAY)
		MFG_RETURN_ERROR(MFG_ERROR_ALLOCATION_FAILED, u8"Failed to allocate index buffer on pool");

	// Init object
	oglIB->base.object.destructorFunc = &mfgOGL4DestroyIndexBuffer;
	oglIB->base.object.referenceCount = 0;
	oglIB->base.renderDevice = rd;

	// Create index buffer
	GLenum gl_usage;

	switch (format)
	{
		case MFG_UBYTE: oglIB->indexType = GL_UNSIGNED_BYTE; break;
		case MFG_USHORT: oglIB->indexType = GL_UNSIGNED_SHORT; break;
		case MFG_UINT: oglIB->indexType = GL_UNSIGNED_INT; break;
		default: MFG_RETURN_ERROR(MFG_ERROR_INVALID_ARGUMENTS, "Unsupported index format");
	}

	switch (usage)
	{
		case MFG_USAGE_DEFAULT: gl_usage = GL_STATIC_DRAW; break;
		case MFG_USAGE_STATIC: gl_usage = GL_STATIC_DRAW; break;
		case MFG_USAGE_DYNAMIC: gl_usage = GL_DYNAMIC_DRAW; break;
		default: MFG_RETURN_ERROR(MFG_ERROR_INVALID_ARGUMENTS, "Unsupported usage mode");
	}

	glGenBuffers(1, &oglIB->ib);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, oglIB->ib);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, gl_usage);

	*ib = oglIB;

	MFG_CHECK_GL_ERROR();
	return MFG_ERROR_OKAY;
}


mfgError mfgOGL4MapIndexBuffer(mfgRenderDevice* rd, mfgIndexBuffer* ib, void** memory)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{
		if (rd == NULL || ib == NULL || memory == NULL) return MFG_ERROR_INVALID_ARGUMENTS;
	}
#endif

	mfgOGL4RenderDevice* oglRD = (mfgOGL4RenderDevice*)rd;

	// Map index buffer
	mfgOGL4IndexBuffer* oglIB = ib;

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, oglIB->ib);
	*memory = glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);

	MFG_CHECK_GL_ERROR();
	return MFG_ERROR_OKAY;
}

mfgError mfgOGL4UnmapIndexBuffer(mfgRenderDevice* rd, mfgIndexBuffer* ib)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{
		if (rd == NULL || ib == NULL) return MFG_ERROR_INVALID_ARGUMENTS;
	}
#endif

	mfgOGL4RenderDevice* oglRD = (mfgOGL4RenderDevice*)rd;

	// Unmap index buffer
	mfgOGL4IndexBuffer* oglIB = ib;

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, oglIB->ib);
	glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);

	MFG_CHECK_GL_ERROR();
	return MFG_ERROR_OKAY;
}

mfgError mfgOGL4SetIndexBuffer(mfgRenderDevice* rd, mfgIndexBuffer* ib)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{
		if (rd == NULL) return MFG_ERROR_INVALID_ARGUMENTS;
	}
#endif

	mfgOGL4RenderDevice* oglRD = (mfgOGL4RenderDevice*)rd;

	// Set index buffer
	mfgOGL4IndexBuffer* oglIB = ib;
	if (oglIB == NULL)
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	else
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, oglIB->ib);

	oglRD->currentIndexBuffer = oglIB;

	MFG_CHECK_GL_ERROR();
	return MFG_ERROR_OKAY;
}

void mfgOGL4DestroyVertexLayout(void* vl)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (vl == NULL) abort();
#endif
	mfgOGL4VertexLayout* oglVL = vl;
	if (mfmDeallocate(((mfgOGL4RenderDevice*)oglVL->base.renderDevice)->pool512, oglVL) != MFM_ERROR_OKAY)
		abort();
#ifdef MAGMA_FRAMEWORK_DEBUG
	GLenum err = glGetError();
	if (err != 0)
		abort();
#endif
}

mfgError mfgOGL4CreateVertexLayout(mfgRenderDevice* rd, mfgVertexLayout** vl, mfmU64 elementCount, const mfgVertexElement* elements, mfgVertexShader* vs)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{ if (rd == NULL || vl == NULL || elementCount == 0 || elementCount > 14 || vs == NULL || elements == NULL) return MFG_ERROR_INVALID_ARGUMENTS; }
#endif

	mfgOGL4RenderDevice* oglRD = (mfgOGL4RenderDevice*)rd;

	// Allocate vertex layout
	mfgOGL4VertexLayout* oglVL = NULL;
	if (mfmAllocate(oglRD->pool512, &oglVL, sizeof(mfgOGL4VertexLayout)) != MFM_ERROR_OKAY)
		MFG_RETURN_ERROR(MFG_ERROR_ALLOCATION_FAILED, u8"Failed to allocate vertex layout on pool");

	// Init object
	oglVL->base.object.destructorFunc = &mfgOGL4DestroyVertexLayout;
	oglVL->base.object.referenceCount = 0;
	oglVL->base.renderDevice = rd;

	// Create vertex layout
	oglVL->vs = vs;
	oglVL->elementCount = elementCount;
	for (mfmU64 i = 0; i < elementCount; ++i)
	{
		oglVL->elements[i].index = UINT32_MAX;

		// Search for input variable with element name
		{
			mfgMetaDataInputVariable* var = ((mfgOGL4Shader*)vs)->md->firstInputVar;
			while (var != NULL)
			{
				mfmBool isVar = MFM_TRUE;
				for (mfmU64 j = 0; j < 16; ++j)
				{
					if (var->name[j] != elements[i].name[j])
					{
						isVar = MFM_FALSE;
						break;
					}
					else if (var->name[j] == '\0')
						break;
				}
				
				if (isVar == MFM_TRUE)
				{
					oglVL->elements[i].index = var->id;
					break;
				}

				var = var->next;
			}
		}

		if (oglVL->elements[i].index == UINT32_MAX)
			MFG_RETURN_ERROR(MFG_ERROR_INVALID_ARGUMENTS, "Couldn't find shader input with vertex element name");

		oglVL->elements[i].bufferIndex = elements[i].bufferIndex;
		oglVL->elements[i].size = elements[i].size;
		oglVL->elements[i].offset = (char*)NULL + elements[i].offset;
		oglVL->elements[i].stride = elements[i].stride;


		switch (elements[i].type)
		{
			case MFG_BYTE: oglVL->elements[i].type = GL_BYTE; oglVL->elements[i].normalized = GL_FALSE; oglVL->elements[i].isInteger = GL_TRUE; break;
			case MFG_SHORT: oglVL->elements[i].type = GL_SHORT; oglVL->elements[i].normalized = GL_FALSE; oglVL->elements[i].isInteger = GL_TRUE; break;
			case MFG_INT: oglVL->elements[i].type = GL_INT; oglVL->elements[i].normalized = GL_FALSE; oglVL->elements[i].isInteger = GL_TRUE; break;
			case MFG_UBYTE: oglVL->elements[i].type = GL_UNSIGNED_BYTE; oglVL->elements[i].normalized = GL_FALSE; oglVL->elements[i].isInteger = GL_TRUE; break;
			case MFG_USHORT: oglVL->elements[i].type = GL_UNSIGNED_SHORT; oglVL->elements[i].normalized = GL_FALSE; oglVL->elements[i].isInteger = GL_TRUE; break;
			case MFG_UINT: oglVL->elements[i].type = GL_UNSIGNED_INT; oglVL->elements[i].normalized = GL_FALSE; oglVL->elements[i].isInteger = GL_TRUE; break;
			case MFG_NBYTE: oglVL->elements[i].type = GL_BYTE; oglVL->elements[i].normalized = GL_TRUE; oglVL->elements[i].isInteger = GL_FALSE; break;
			case MFG_NSHORT: oglVL->elements[i].type = GL_SHORT; oglVL->elements[i].normalized = GL_TRUE; oglVL->elements[i].isInteger = GL_FALSE; break;
			case MFG_NUBYTE: oglVL->elements[i].type = GL_UNSIGNED_BYTE; oglVL->elements[i].normalized = GL_TRUE; oglVL->elements[i].isInteger = GL_FALSE; break;
			case MFG_NUSHORT: oglVL->elements[i].type = GL_UNSIGNED_SHORT; oglVL->elements[i].normalized = GL_TRUE; oglVL->elements[i].isInteger = GL_FALSE; break;
			case MFG_FLOAT: oglVL->elements[i].type = GL_FLOAT; oglVL->elements[i].normalized = GL_FALSE; oglVL->elements[i].isInteger = GL_FALSE; break;
			default: MFG_RETURN_ERROR(MFG_ERROR_INVALID_ARGUMENTS, "Unsupported vertex element type");
		}
	}

	*vl = oglVL;

	MFG_CHECK_GL_ERROR();
	return MFG_ERROR_OKAY;
}

void mfgOGL4DestroyVertexArray(void* va)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (va == NULL) abort();
#endif
	mfgOGL4VertexArray* oglVA = va;
	glDeleteVertexArrays(1, &oglVA->va);
	if (mfmDeallocate(((mfgOGL4RenderDevice*)oglVA->base.renderDevice)->pool32, oglVA) != MFM_ERROR_OKAY)
		abort();
#ifdef MAGMA_FRAMEWORK_DEBUG
	GLenum err = glGetError();
	if (err != 0)
		abort();
#endif
}

mfgError mfgOGL4CreateVertexArray(mfgRenderDevice* rd, mfgVertexArray** va, mfmU64 bufferCount, mfgVertexBuffer** buffers, mfgVertexLayout* vl)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{ if (rd == NULL || va == NULL || bufferCount == 0 || vl == NULL || buffers == NULL) return MFG_ERROR_INVALID_ARGUMENTS; }
#endif

	mfgOGL4RenderDevice* oglRD = (mfgOGL4RenderDevice*)rd;

	// Allocate vertex array
	mfgOGL4VertexArray* oglVA = NULL;
	if (mfmAllocate(oglRD->pool32, &oglVA, sizeof(mfgOGL4VertexArray)) != MFM_ERROR_OKAY)
		MFG_RETURN_ERROR(MFG_ERROR_ALLOCATION_FAILED, u8"Failed to allocate vertex array on pool");
	mfgOGL4VertexLayout* oglVL = vl;

	// Init object
	oglVA->base.object.destructorFunc = &mfgOGL4DestroyVertexArray;
	oglVA->base.object.referenceCount = 0;
	oglVA->base.renderDevice = rd;

	// Create vertex array
	glGenVertexArrays(1, &oglVA->va);
	glBindVertexArray(oglVA->va);

	for (mfmU64 i = 0; i < bufferCount; ++i)
	{
		mfgOGL4VertexBuffer* buffer = buffers[i];

		glBindBuffer(GL_ARRAY_BUFFER, buffer->vb);

		for (mfmU64 j = 0; j < oglVL->elementCount; ++j)
		{
			if (oglVL->elements[j].bufferIndex != i)
				continue;
			glEnableVertexAttribArray(oglVL->elements[j].index);
			if (oglVL->elements[j].isInteger)
				glVertexAttribIPointer(oglVL->elements[j].index, oglVL->elements[j].size, oglVL->elements[j].type, oglVL->elements[j].stride, oglVL->elements[j].offset);
			else
				glVertexAttribPointer(oglVL->elements[j].index, oglVL->elements[j].size, oglVL->elements[j].type, oglVL->elements[j].normalized, oglVL->elements[j].stride, oglVL->elements[j].offset);
		}
	}

	*va = oglVA;

	MFG_CHECK_GL_ERROR();
	return MFG_ERROR_OKAY;
}

mfgError mfgOGL4SetVertexArray(mfgRenderDevice* rd, mfgVertexArray* va)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{ if (rd == NULL || va == NULL) return MFG_ERROR_INVALID_ARGUMENTS; }
#endif

	mfgOGL4RenderDevice* oglRD = (mfgOGL4RenderDevice*)rd;

	// Set vertex array as active
	mfgOGL4VertexArray* oglVA = va;
	glBindVertexArray(oglVA->va);

	MFG_CHECK_GL_ERROR();
	return MFG_ERROR_OKAY;
}


void mfgOGL4DestroyTexture1D(void* tex)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (tex == NULL) abort();
#endif
	mfgOGL4Texture1D* oglTex = tex;
	glDeleteTextures(1, &oglTex->tex);
	if (mfmDeallocate(((mfgOGL4RenderDevice*)oglTex->base.renderDevice)->pool64, oglTex) != MFM_ERROR_OKAY)
		abort();
#ifdef MAGMA_FRAMEWORK_DEBUG
	GLenum err = glGetError();
	if (err != 0)
		abort();
#endif
}

mfgError mfgOGL4CreateTexture1D(mfgRenderDevice* rd, mfgTexture1D** tex, mfmU64 width, mfgEnum format, const void* data, mfgEnum usage)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{ if (rd == NULL || tex == NULL) return MFG_ERROR_INVALID_ARGUMENTS; }
#endif

	mfgOGL4RenderDevice* oglRD = (mfgOGL4RenderDevice*)rd;

	// Allocate texture
	mfgOGL4Texture1D* oglTex = NULL;
	if (mfmAllocate(oglRD->pool64, &oglTex, sizeof(mfgOGL4Texture1D)) != MFM_ERROR_OKAY)
		MFG_RETURN_ERROR(MFG_ERROR_ALLOCATION_FAILED, u8"Failed to allocate texture 1D on pool");

	// Init object
	oglTex->base.object.destructorFunc = &mfgOGL4DestroyTexture1D;
	oglTex->base.object.referenceCount = 0;
	oglTex->base.renderDevice = rd;

	// Create texture
	GLenum gl_usage;

	switch (format)
	{
		case MFG_R8SNORM: oglTex->internalFormat = GL_R8_SNORM; oglTex->type = GL_BYTE; oglTex->format = GL_RED; oglTex->packAligment = 1; break;
		case MFG_R16SNORM: oglTex->internalFormat = GL_R16_SNORM; oglTex->type = GL_SHORT; oglTex->format = GL_RED; oglTex->packAligment = 2; break;
		case MFG_RG8SNORM: oglTex->internalFormat = GL_RG8_SNORM; oglTex->type = GL_BYTE; oglTex->format = GL_RG; oglTex->packAligment = 2; break;
		case MFG_RG16SNORM: oglTex->internalFormat = GL_RG16_SNORM; oglTex->type = GL_SHORT; oglTex->format = GL_RG; oglTex->packAligment = 4; break;
		case MFG_RGBA8SNORM: oglTex->internalFormat = GL_RGBA8_SNORM; oglTex->type = GL_BYTE; oglTex->format = GL_RGBA; oglTex->packAligment = 4; break;
		case MFG_RGBA16SNORM: oglTex->internalFormat = GL_RGBA16_SNORM; oglTex->type = GL_SHORT; oglTex->format = GL_RGBA; oglTex->packAligment = 4; break;

		case MFG_R8UNORM: oglTex->internalFormat = GL_R8; oglTex->type = GL_UNSIGNED_BYTE; oglTex->format = GL_RED; oglTex->packAligment = 1; break;
		case MFG_R16UNORM: oglTex->internalFormat = GL_R16; oglTex->type = GL_UNSIGNED_SHORT; oglTex->format = GL_RED; oglTex->packAligment = 2; break;
		case MFG_RG8UNORM: oglTex->internalFormat = GL_RG8; oglTex->type = GL_UNSIGNED_BYTE; oglTex->format = GL_RG; oglTex->packAligment = 2; break;
		case MFG_RG16UNORM: oglTex->internalFormat = GL_RG16; oglTex->type = GL_UNSIGNED_SHORT; oglTex->format = GL_RG; oglTex->packAligment = 4; break;
		case MFG_RGBA8UNORM: oglTex->internalFormat = GL_RGBA8; oglTex->type = GL_UNSIGNED_BYTE; oglTex->format = GL_RGBA; oglTex->packAligment = 4; break;
		case MFG_RGBA16UNORM: oglTex->internalFormat = GL_RGBA16; oglTex->type = GL_UNSIGNED_SHORT; oglTex->format = GL_RGBA; oglTex->packAligment = 4; break;

		case MFG_R8SINT: oglTex->internalFormat = GL_R8I; oglTex->type = GL_BYTE; oglTex->format = GL_RED; oglTex->packAligment = 1; break;
		case MFG_R16SINT: oglTex->internalFormat = GL_R16I; oglTex->type = GL_SHORT; oglTex->format = GL_RED; oglTex->packAligment = 2; break;
		case MFG_RG8SINT: oglTex->internalFormat = GL_RG8I; oglTex->type = GL_BYTE; oglTex->format = GL_RG; oglTex->packAligment = 2; break;
		case MFG_RG16SINT: oglTex->internalFormat = GL_RG16I; oglTex->type = GL_SHORT; oglTex->format = GL_RG; oglTex->packAligment = 4; break;
		case MFG_RGBA8SINT: oglTex->internalFormat = GL_RGBA8I; oglTex->type = GL_BYTE; oglTex->format = GL_RGBA; oglTex->packAligment = 4; break;
		case MFG_RGBA16SINT: oglTex->internalFormat = GL_RGBA16I; oglTex->type = GL_SHORT; oglTex->format = GL_RGBA; oglTex->packAligment = 4; break;

		case MFG_R8UINT: oglTex->internalFormat = GL_R8UI; oglTex->type = GL_UNSIGNED_BYTE; oglTex->format = GL_RED; oglTex->packAligment = 1; break;
		case MFG_R16UINT: oglTex->internalFormat = GL_R16UI; oglTex->type = GL_UNSIGNED_SHORT; oglTex->format = GL_RED; oglTex->packAligment = 2; break;
		case MFG_RG8UINT: oglTex->internalFormat = GL_RG8UI; oglTex->type = GL_UNSIGNED_BYTE; oglTex->format = GL_RG; oglTex->packAligment = 2; break;
		case MFG_RG16UINT: oglTex->internalFormat = GL_RG16UI; oglTex->type = GL_UNSIGNED_SHORT; oglTex->format = GL_RG; oglTex->packAligment = 4; break;
		case MFG_RGBA8UINT: oglTex->internalFormat = GL_RGBA8UI; oglTex->type = GL_UNSIGNED_BYTE; oglTex->format = GL_RGBA; oglTex->packAligment = 4; break;
		case MFG_RGBA16UINT: oglTex->internalFormat = GL_RGBA16UI; oglTex->type = GL_UNSIGNED_SHORT; oglTex->format = GL_RGBA; oglTex->packAligment = 4; break;

		case MFG_R32FLOAT: oglTex->internalFormat = GL_R32F; oglTex->type = GL_FLOAT; oglTex->format = GL_RED; oglTex->packAligment = 4; break;
		case MFG_RG32FLOAT: oglTex->internalFormat = GL_RG32F; oglTex->type = GL_FLOAT; oglTex->format = GL_RG; oglTex->packAligment = 4; break;
		case MFG_RGB32FLOAT: oglTex->internalFormat = GL_RGB32F; oglTex->type = GL_FLOAT; oglTex->format = GL_RGB; oglTex->packAligment = 4; break;
		case MFG_RGBA32FLOAT: oglTex->internalFormat = GL_RGBA32F; oglTex->type = GL_FLOAT; oglTex->format = GL_RGBA; oglTex->packAligment = 4; break;

		default: MFG_RETURN_ERROR(MFG_ERROR_INVALID_ARGUMENTS, u8"Unsupported texture format");
	}

	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &oglTex->tex);
	glBindTexture(GL_TEXTURE_1D, oglTex->tex);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 1.0f);

	glPixelStorei(GL_PACK_ALIGNMENT, oglTex->packAligment);
	glPixelStorei(GL_UNPACK_ALIGNMENT, oglTex->packAligment);
	if (data == NULL)
		glTexStorage1D(GL_TEXTURE_1D, 1, oglTex->internalFormat, width);
	else
		glTexImage1D(GL_TEXTURE_1D, 0, oglTex->internalFormat, width, 0, oglTex->format, oglTex->type, data);

	*tex = oglTex;

	MFG_CHECK_GL_ERROR();
	return MFG_ERROR_OKAY;
}

mfgError mfgOGL4UpdateTexture1D(mfgRenderDevice* rd, mfgTexture1D* tex, mfmU64 dstX, mfmU64 width, const void* data)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{ if (rd == NULL || tex == NULL || data == NULL) return MFG_ERROR_INVALID_ARGUMENTS; }
#endif

	mfgOGL4RenderDevice* oglRD = (mfgOGL4RenderDevice*)rd;

	// Update texture 1D
	mfgOGL4Texture1D* oglTex = tex;
	glBindTexture(GL_TEXTURE_1D, oglTex->tex);
	glPixelStorei(GL_PACK_ALIGNMENT, oglTex->packAligment);
	glPixelStorei(GL_UNPACK_ALIGNMENT, oglTex->packAligment);
	glTexSubImage1D(GL_TEXTURE_1D, 0, dstX, width, oglTex->format, oglTex->type, data);

	MFG_CHECK_GL_ERROR();
	return MFG_ERROR_OKAY;
}

mfgError mfgOGL4GenerateTexture1DMipmaps(mfgRenderDevice* rd, mfgTexture1D* tex)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{ if (rd == NULL || tex == NULL) return MFG_ERROR_INVALID_ARGUMENTS; }
#endif

	mfgOGL4RenderDevice* oglRD = (mfgOGL4RenderDevice*)rd;

	// Generate texture 1D mipmaps
	mfgOGL4Texture1D* oglTex = tex;
	glBindTexture(GL_TEXTURE_1D, oglTex->tex);
	glGenerateMipmap(GL_TEXTURE_1D);

	MFG_CHECK_GL_ERROR();
	return MFG_ERROR_OKAY;
}

void mfgOGL4DestroyTexture2D(void* tex)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (tex == NULL) abort();
#endif
	mfgOGL4Texture2D* oglTex = tex;
	glDeleteTextures(1, &oglTex->tex);
	if (mfmDeallocate(((mfgOGL4RenderDevice*)oglTex->base.renderDevice)->pool64, oglTex) != MFM_ERROR_OKAY)
		abort();
#ifdef MAGMA_FRAMEWORK_DEBUG
	GLenum err = glGetError();
	if (err != 0)
		abort();
#endif
}

mfgError mfgOGL4CreateTexture2D(mfgRenderDevice* rd, mfgTexture2D** tex, mfmU64 width, mfmU64 height, mfgEnum format, const void* data, mfgEnum usage)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{ if (rd == NULL || tex == NULL) return MFG_ERROR_INVALID_ARGUMENTS; }
#endif

	mfgOGL4RenderDevice* oglRD = (mfgOGL4RenderDevice*)rd;

	// Allocate texture
	mfgOGL4Texture2D* oglTex = NULL;
	if (mfmAllocate(oglRD->pool64, &oglTex, sizeof(mfgOGL4Texture2D)) != MFM_ERROR_OKAY)
		MFG_RETURN_ERROR(MFG_ERROR_ALLOCATION_FAILED, u8"Failed to allocate texture 2D on pool");

	// Init object
	oglTex->base.object.destructorFunc = &mfgOGL4DestroyTexture2D;
	oglTex->base.object.referenceCount = 0;
	oglTex->base.renderDevice = rd;

	// Create texture
	GLenum gl_usage;

	switch (format)
	{
		case MFG_R8SNORM: oglTex->internalFormat = GL_R8_SNORM; oglTex->type = GL_BYTE; oglTex->format = GL_RED; oglTex->packAligment = 1; break;
		case MFG_R16SNORM: oglTex->internalFormat = GL_R16_SNORM; oglTex->type = GL_SHORT; oglTex->format = GL_RED; oglTex->packAligment = 2; break;
		case MFG_RG8SNORM: oglTex->internalFormat = GL_RG8_SNORM; oglTex->type = GL_BYTE; oglTex->format = GL_RG; oglTex->packAligment = 2; break;
		case MFG_RG16SNORM: oglTex->internalFormat = GL_RG16_SNORM; oglTex->type = GL_SHORT; oglTex->format = GL_RG; oglTex->packAligment = 4; break;
		case MFG_RGBA8SNORM: oglTex->internalFormat = GL_RGBA8_SNORM; oglTex->type = GL_BYTE; oglTex->format = GL_RGBA; oglTex->packAligment = 4; break;
		case MFG_RGBA16SNORM: oglTex->internalFormat = GL_RGBA16_SNORM; oglTex->type = GL_SHORT; oglTex->format = GL_RGBA; oglTex->packAligment = 4; break;

		case MFG_R8UNORM: oglTex->internalFormat = GL_R8; oglTex->type = GL_UNSIGNED_BYTE; oglTex->format = GL_RED; oglTex->packAligment = 1; break;
		case MFG_R16UNORM: oglTex->internalFormat = GL_R16; oglTex->type = GL_UNSIGNED_SHORT; oglTex->format = GL_RED; oglTex->packAligment = 2; break;
		case MFG_RG8UNORM: oglTex->internalFormat = GL_RG8; oglTex->type = GL_UNSIGNED_BYTE; oglTex->format = GL_RG; oglTex->packAligment = 2; break;
		case MFG_RG16UNORM: oglTex->internalFormat = GL_RG16; oglTex->type = GL_UNSIGNED_SHORT; oglTex->format = GL_RG; oglTex->packAligment = 4; break;
		case MFG_RGBA8UNORM: oglTex->internalFormat = GL_RGBA8; oglTex->type = GL_UNSIGNED_BYTE; oglTex->format = GL_RGBA; oglTex->packAligment = 4; break;
		case MFG_RGBA16UNORM: oglTex->internalFormat = GL_RGBA16; oglTex->type = GL_UNSIGNED_SHORT; oglTex->format = GL_RGBA; oglTex->packAligment = 4; break;

		case MFG_R8SINT: oglTex->internalFormat = GL_R8I; oglTex->type = GL_BYTE; oglTex->format = GL_RED; oglTex->packAligment = 1; break;
		case MFG_R16SINT: oglTex->internalFormat = GL_R16I; oglTex->type = GL_SHORT; oglTex->format = GL_RED; oglTex->packAligment = 2; break;
		case MFG_RG8SINT: oglTex->internalFormat = GL_RG8I; oglTex->type = GL_BYTE; oglTex->format = GL_RG; oglTex->packAligment = 2; break;
		case MFG_RG16SINT: oglTex->internalFormat = GL_RG16I; oglTex->type = GL_SHORT; oglTex->format = GL_RG; oglTex->packAligment = 4; break;
		case MFG_RGBA8SINT: oglTex->internalFormat = GL_RGBA8I; oglTex->type = GL_BYTE; oglTex->format = GL_RGBA; oglTex->packAligment = 4; break;
		case MFG_RGBA16SINT: oglTex->internalFormat = GL_RGBA16I; oglTex->type = GL_SHORT; oglTex->format = GL_RGBA; oglTex->packAligment = 4; break;

		case MFG_R8UINT: oglTex->internalFormat = GL_R8UI; oglTex->type = GL_UNSIGNED_BYTE; oglTex->format = GL_RED; oglTex->packAligment = 1; break;
		case MFG_R16UINT: oglTex->internalFormat = GL_R16UI; oglTex->type = GL_UNSIGNED_SHORT; oglTex->format = GL_RED; oglTex->packAligment = 2; break;
		case MFG_RG8UINT: oglTex->internalFormat = GL_RG8UI; oglTex->type = GL_UNSIGNED_BYTE; oglTex->format = GL_RG; oglTex->packAligment = 2; break;
		case MFG_RG16UINT: oglTex->internalFormat = GL_RG16UI; oglTex->type = GL_UNSIGNED_SHORT; oglTex->format = GL_RG; oglTex->packAligment = 4; break;
		case MFG_RGBA8UINT: oglTex->internalFormat = GL_RGBA8UI; oglTex->type = GL_UNSIGNED_BYTE; oglTex->format = GL_RGBA; oglTex->packAligment = 4; break;
		case MFG_RGBA16UINT: oglTex->internalFormat = GL_RGBA16UI; oglTex->type = GL_UNSIGNED_SHORT; oglTex->format = GL_RGBA; oglTex->packAligment = 4; break;

		case MFG_R32FLOAT: oglTex->internalFormat = GL_R32F; oglTex->type = GL_FLOAT; oglTex->format = GL_RED; oglTex->packAligment = 4; break;
		case MFG_RG32FLOAT: oglTex->internalFormat = GL_RG32F; oglTex->type = GL_FLOAT; oglTex->format = GL_RG; oglTex->packAligment = 4; break;
		case MFG_RGB32FLOAT: oglTex->internalFormat = GL_RGB32F; oglTex->type = GL_FLOAT; oglTex->format = GL_RGB; oglTex->packAligment = 4; break;
		case MFG_RGBA32FLOAT: oglTex->internalFormat = GL_RGBA32F; oglTex->type = GL_FLOAT; oglTex->format = GL_RGBA; oglTex->packAligment = 4; break;
	
		default: MFG_RETURN_ERROR(MFG_ERROR_INVALID_ARGUMENTS, u8"Unsupported texture format");
	}

	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &oglTex->tex);
	glBindTexture(GL_TEXTURE_2D, oglTex->tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 1.0f);

	glPixelStorei(GL_PACK_ALIGNMENT, oglTex->packAligment);
	glPixelStorei(GL_UNPACK_ALIGNMENT, oglTex->packAligment);
	if (data == NULL)
		glTexStorage2D(GL_TEXTURE_2D, 1, oglTex->internalFormat, width, height);
	else
		glTexImage2D(GL_TEXTURE_2D, 0, oglTex->internalFormat, width, height, 0, oglTex->format, oglTex->type, data);

	*tex = oglTex;

	MFG_CHECK_GL_ERROR();
	return MFG_ERROR_OKAY;
}

mfgError mfgOGL4UpdateTexture2D(mfgRenderDevice* rd, mfgTexture2D* tex, mfmU64 dstX, mfmU64 dstY, mfmU64 width, mfmU64 height, const void* data)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{ if (rd == NULL || tex == NULL || data == NULL) return MFG_ERROR_INVALID_ARGUMENTS; }
#endif

	mfgOGL4RenderDevice* oglRD = (mfgOGL4RenderDevice*)rd;

	// Update texture 2D
	mfgOGL4Texture2D* oglTex = tex;
	glBindTexture(GL_TEXTURE_2D, oglTex->tex);
	glPixelStorei(GL_PACK_ALIGNMENT, oglTex->packAligment);
	glPixelStorei(GL_UNPACK_ALIGNMENT, oglTex->packAligment);
	glTexSubImage2D(GL_TEXTURE_2D, 0, dstX, dstY, width, height, oglTex->format, oglTex->type, data);

	MFG_CHECK_GL_ERROR();
	return MFG_ERROR_OKAY;
}

mfgError mfgOGL4GenerateTexture2DMipmaps(mfgRenderDevice* rd, mfgTexture2D* tex)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{ if (rd == NULL || tex == NULL) return MFG_ERROR_INVALID_ARGUMENTS; }
#endif

	mfgOGL4RenderDevice* oglRD = (mfgOGL4RenderDevice*)rd;

	// Generate texture 2D mipmaps
	mfgOGL4Texture2D* oglTex = tex;
	glBindTexture(GL_TEXTURE_2D, oglTex->tex);
	glGenerateMipmap(GL_TEXTURE_2D);

	MFG_CHECK_GL_ERROR();
	return MFG_ERROR_OKAY;
}

void mfgOGL4DestroyTexture3D(void* tex)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (tex == NULL) abort();
#endif
	mfgOGL4Texture3D* oglTex = tex;
	glDeleteTextures(1, &oglTex->tex);
	if (mfmDeallocate(((mfgOGL4RenderDevice*)oglTex->base.renderDevice)->pool64, oglTex) != MFM_ERROR_OKAY)
		abort();
#ifdef MAGMA_FRAMEWORK_DEBUG
	GLenum err = glGetError();
	if (err != 0)
		abort();
#endif
}

mfgError mfgOGL4CreateTexture3D(mfgRenderDevice* rd, mfgTexture3D** tex, mfmU64 width, mfmU64 height, mfmU64 depth, mfgEnum format, const void* data, mfgEnum usage)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{ if (rd == NULL || tex == NULL) return MFG_ERROR_INVALID_ARGUMENTS; }
#endif

	mfgOGL4RenderDevice* oglRD = (mfgOGL4RenderDevice*)rd;

	// Allocate texture
	mfgOGL4Texture3D* oglTex = NULL;
	if (mfmAllocate(oglRD->pool64, &oglTex, sizeof(mfgOGL4Texture3D)) != MFM_ERROR_OKAY)
		MFG_RETURN_ERROR(MFG_ERROR_ALLOCATION_FAILED, u8"Failed to allocate texture 3D on pool");

	// Init object
	oglTex->base.object.destructorFunc = &mfgOGL4DestroyTexture3D;
	oglTex->base.object.referenceCount = 0;
	oglTex->base.renderDevice = rd;

	// Create texture
	GLenum gl_usage;

	switch (format)
	{
		case MFG_R8SNORM: oglTex->internalFormat = GL_R8_SNORM; oglTex->type = GL_BYTE; oglTex->format = GL_RED; oglTex->packAligment = 1; break;
		case MFG_R16SNORM: oglTex->internalFormat = GL_R16_SNORM; oglTex->type = GL_SHORT; oglTex->format = GL_RED; oglTex->packAligment = 2; break;
		case MFG_RG8SNORM: oglTex->internalFormat = GL_RG8_SNORM; oglTex->type = GL_BYTE; oglTex->format = GL_RG; oglTex->packAligment = 2; break;
		case MFG_RG16SNORM: oglTex->internalFormat = GL_RG16_SNORM; oglTex->type = GL_SHORT; oglTex->format = GL_RG; oglTex->packAligment = 4; break;
		case MFG_RGBA8SNORM: oglTex->internalFormat = GL_RGBA8_SNORM; oglTex->type = GL_BYTE; oglTex->format = GL_RGBA; oglTex->packAligment = 4; break;
		case MFG_RGBA16SNORM: oglTex->internalFormat = GL_RGBA16_SNORM; oglTex->type = GL_SHORT; oglTex->format = GL_RGBA; oglTex->packAligment = 4; break;

		case MFG_R8UNORM: oglTex->internalFormat = GL_R8; oglTex->type = GL_UNSIGNED_BYTE; oglTex->format = GL_RED; oglTex->packAligment = 1; break;
		case MFG_R16UNORM: oglTex->internalFormat = GL_R16; oglTex->type = GL_UNSIGNED_SHORT; oglTex->format = GL_RED; oglTex->packAligment = 2; break;
		case MFG_RG8UNORM: oglTex->internalFormat = GL_RG8; oglTex->type = GL_UNSIGNED_BYTE; oglTex->format = GL_RG; oglTex->packAligment = 2; break;
		case MFG_RG16UNORM: oglTex->internalFormat = GL_RG16; oglTex->type = GL_UNSIGNED_SHORT; oglTex->format = GL_RG; oglTex->packAligment = 4; break;
		case MFG_RGBA8UNORM: oglTex->internalFormat = GL_RGBA8; oglTex->type = GL_UNSIGNED_BYTE; oglTex->format = GL_RGBA; oglTex->packAligment = 4; break;
		case MFG_RGBA16UNORM: oglTex->internalFormat = GL_RGBA16; oglTex->type = GL_UNSIGNED_SHORT; oglTex->format = GL_RGBA; oglTex->packAligment = 4; break;

		case MFG_R8SINT: oglTex->internalFormat = GL_R8I; oglTex->type = GL_BYTE; oglTex->format = GL_RED; oglTex->packAligment = 1; break;
		case MFG_R16SINT: oglTex->internalFormat = GL_R16I; oglTex->type = GL_SHORT; oglTex->format = GL_RED; oglTex->packAligment = 2; break;
		case MFG_RG8SINT: oglTex->internalFormat = GL_RG8I; oglTex->type = GL_BYTE; oglTex->format = GL_RG; oglTex->packAligment = 2; break;
		case MFG_RG16SINT: oglTex->internalFormat = GL_RG16I; oglTex->type = GL_SHORT; oglTex->format = GL_RG; oglTex->packAligment = 4; break;
		case MFG_RGBA8SINT: oglTex->internalFormat = GL_RGBA8I; oglTex->type = GL_BYTE; oglTex->format = GL_RGBA; oglTex->packAligment = 4; break;
		case MFG_RGBA16SINT: oglTex->internalFormat = GL_RGBA16I; oglTex->type = GL_SHORT; oglTex->format = GL_RGBA; oglTex->packAligment = 4; break;

		case MFG_R8UINT: oglTex->internalFormat = GL_R8UI; oglTex->type = GL_UNSIGNED_BYTE; oglTex->format = GL_RED; oglTex->packAligment = 1; break;
		case MFG_R16UINT: oglTex->internalFormat = GL_R16UI; oglTex->type = GL_UNSIGNED_SHORT; oglTex->format = GL_RED; oglTex->packAligment = 2; break;
		case MFG_RG8UINT: oglTex->internalFormat = GL_RG8UI; oglTex->type = GL_UNSIGNED_BYTE; oglTex->format = GL_RG; oglTex->packAligment = 2; break;
		case MFG_RG16UINT: oglTex->internalFormat = GL_RG16UI; oglTex->type = GL_UNSIGNED_SHORT; oglTex->format = GL_RG; oglTex->packAligment = 4; break;
		case MFG_RGBA8UINT: oglTex->internalFormat = GL_RGBA8UI; oglTex->type = GL_UNSIGNED_BYTE; oglTex->format = GL_RGBA; oglTex->packAligment = 4; break;
		case MFG_RGBA16UINT: oglTex->internalFormat = GL_RGBA16UI; oglTex->type = GL_UNSIGNED_SHORT; oglTex->format = GL_RGBA; oglTex->packAligment = 4; break;

		case MFG_R32FLOAT: oglTex->internalFormat = GL_R32F; oglTex->type = GL_FLOAT; oglTex->format = GL_RED; oglTex->packAligment = 4; break;
		case MFG_RG32FLOAT: oglTex->internalFormat = GL_RG32F; oglTex->type = GL_FLOAT; oglTex->format = GL_RG; oglTex->packAligment = 4; break;
		case MFG_RGB32FLOAT: oglTex->internalFormat = GL_RGB32F; oglTex->type = GL_FLOAT; oglTex->format = GL_RGB; oglTex->packAligment = 4; break;
		case MFG_RGBA32FLOAT: oglTex->internalFormat = GL_RGBA32F; oglTex->type = GL_FLOAT; oglTex->format = GL_RGBA; oglTex->packAligment = 4; break;

		default: MFG_RETURN_ERROR(MFG_ERROR_INVALID_ARGUMENTS, u8"Unsupported texture format");
	}

	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &oglTex->tex);
	glBindTexture(GL_TEXTURE_3D, oglTex->tex);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 1.0f);

	glPixelStorei(GL_PACK_ALIGNMENT, oglTex->packAligment);
	glPixelStorei(GL_UNPACK_ALIGNMENT, oglTex->packAligment);
	if (data == NULL)
		glTexStorage3D(GL_TEXTURE_3D, 1, oglTex->internalFormat, width, height, depth);
	else
		glTexImage3D(GL_TEXTURE_3D, 0, oglTex->internalFormat, width, height, depth, 0, oglTex->format, oglTex->type, data);

	*tex = oglTex;

	MFG_CHECK_GL_ERROR();
	return MFG_ERROR_OKAY;
}

mfgError mfgOGL4UpdateTexture3D(mfgRenderDevice* rd, mfgTexture3D* tex, mfmU64 dstX, mfmU64 dstY, mfmU64 dstZ, mfmU64 width, mfmU64 height, mfmU64 depth, const void* data)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{ if (rd == NULL || tex == NULL || data == NULL) return MFG_ERROR_INVALID_ARGUMENTS; }
#endif

	mfgOGL4RenderDevice* oglRD = (mfgOGL4RenderDevice*)rd;

	// Update texture 3D
	mfgOGL4Texture3D* oglTex = tex;
	glBindTexture(GL_TEXTURE_3D, oglTex->tex);
	glPixelStorei(GL_PACK_ALIGNMENT, oglTex->packAligment);
	glPixelStorei(GL_UNPACK_ALIGNMENT, oglTex->packAligment);
	glTexSubImage3D(GL_TEXTURE_3D, 0, dstX, dstY, dstZ, width, height, depth, oglTex->format, oglTex->type, data);

	MFG_CHECK_GL_ERROR();
	return MFG_ERROR_OKAY;
}

mfgError mfgOGL4GenerateTexture3DMipmaps(mfgRenderDevice* rd, mfgTexture3D* tex)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{ if (rd == NULL || tex == NULL) return MFG_ERROR_INVALID_ARGUMENTS; }
#endif

	mfgOGL4RenderDevice* oglRD = (mfgOGL4RenderDevice*)rd;

	// Generate texture 3D mipmaps
	mfgOGL4Texture3D* oglTex = tex;
	glBindTexture(GL_TEXTURE_3D, oglTex->tex);
	glGenerateMipmap(GL_TEXTURE_3D);

	MFG_CHECK_GL_ERROR();
	return MFG_ERROR_OKAY;
}

void mfgOGL4DestroySampler(void* sampler)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (sampler == NULL) abort();
#endif
	mfgOGL4Sampler* oglS = sampler;
	glDeleteSamplers(1, &oglS->sampler);
	if (mfmDeallocate(((mfgOGL4RenderDevice*)oglS->base.renderDevice)->pool32, oglS) != MFM_ERROR_OKAY)
		abort();
#ifdef MAGMA_FRAMEWORK_DEBUG
	GLenum err = glGetError();
	if (err != 0)
		abort();
#endif
}

mfgError mfgOGL4CreateSampler(mfgRenderDevice* rd, mfgSampler** sampler, const mfgSamplerDesc* desc)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{ if (rd == NULL || sampler == NULL || desc == NULL) return MFG_ERROR_INVALID_ARGUMENTS; }
#endif

	mfgOGL4RenderDevice* oglRD = (mfgOGL4RenderDevice*)rd;

	// Allocate sampler
	mfgOGL4Sampler* oglS = NULL;
	if (mfmAllocate(oglRD->pool32, &oglS, sizeof(mfgOGL4Sampler)) != MFM_ERROR_OKAY)
		MFG_RETURN_ERROR(MFG_ERROR_ALLOCATION_FAILED, u8"Failed to allocate sampler on pool");

	// Init object
	oglS->base.object.destructorFunc = &mfgOGL4DestroySampler;
	oglS->base.object.referenceCount = 0;
	oglS->base.renderDevice = rd;

	// Create sampler
	GLenum minFilter, magFilter;
	GLenum wrapS, wrapT, wrapR;

	switch (desc->adressU)
	{
		case MFG_REPEAT: wrapS = GL_REPEAT; break;
		case MFG_MIRROR: wrapS = GL_MIRRORED_REPEAT; break;
		case MFG_CLAMP: wrapS = GL_CLAMP_TO_EDGE; break;
		case MFG_BORDER: wrapS = GL_CLAMP_TO_BORDER; break;
		default: MFG_RETURN_ERROR(MFG_ERROR_INVALID_ARGUMENTS, u8"Unsupported texture U adress mode");
	}

	switch (desc->adressV)
	{
		case MFG_REPEAT: wrapT = GL_REPEAT; break;
		case MFG_MIRROR: wrapT = GL_MIRRORED_REPEAT; break;
		case MFG_CLAMP: wrapT = GL_CLAMP_TO_EDGE; break;
		case MFG_BORDER: wrapT = GL_CLAMP_TO_BORDER; break;
		default: MFG_RETURN_ERROR(MFG_ERROR_INVALID_ARGUMENTS, u8"Unsupported texture V adress mode");
	}

	switch (desc->adressW)
	{
		case MFG_REPEAT: wrapR = GL_REPEAT; break;
		case MFG_MIRROR: wrapR = GL_MIRRORED_REPEAT; break;
		case MFG_CLAMP: wrapR = GL_CLAMP_TO_EDGE; break;
		case MFG_BORDER: wrapR = GL_CLAMP_TO_BORDER; break;
		default: MFG_RETURN_ERROR(MFG_ERROR_INVALID_ARGUMENTS, u8"Unsupported texture W adress mode");
	}

	switch (desc->minFilter)
	{
		case MFG_NEAREST:
			if (desc->mipmapFilter == MFG_NONE)
				minFilter = GL_NEAREST;
			else if (desc->mipmapFilter == MFG_NEAREST)
				minFilter = GL_NEAREST_MIPMAP_NEAREST;
			else if (desc->mipmapFilter == MFG_LINEAR)
				minFilter = GL_NEAREST_MIPMAP_LINEAR;
			else
				MFG_RETURN_ERROR(MFG_ERROR_INVALID_ARGUMENTS, u8"Unsupported texture mipmap filter");
			break;

		case MFG_LINEAR:
			if (desc->mipmapFilter == MFG_NONE)
				minFilter = GL_LINEAR;
			else if (desc->mipmapFilter == MFG_NEAREST)
				minFilter = GL_LINEAR_MIPMAP_NEAREST;
			else if (desc->mipmapFilter == MFG_LINEAR)
				minFilter = GL_LINEAR_MIPMAP_LINEAR;
			else
				MFG_RETURN_ERROR(MFG_ERROR_INVALID_ARGUMENTS, u8"Unsupported texture mipmap filter");
			break;

		default: 
			MFG_RETURN_ERROR(MFG_ERROR_INVALID_ARGUMENTS, u8"Unsupported texture minifying filter");
			break;
	}
	
	switch (desc->magFilter)
	{
		case MFG_NEAREST:
			magFilter = GL_NEAREST;
			break;

		case MFG_LINEAR:
			magFilter = GL_LINEAR;
			break;

		default:
			MFG_RETURN_ERROR(MFG_ERROR_INVALID_ARGUMENTS, u8"Unsupported texture magnifying filter");
			break;
	}

	glGenSamplers(1, &oglS->sampler);
	glSamplerParameteri(oglS->sampler, GL_TEXTURE_MIN_FILTER, minFilter);
	glSamplerParameteri(oglS->sampler, GL_TEXTURE_MAG_FILTER, magFilter);
	glSamplerParameteri(oglS->sampler, GL_TEXTURE_WRAP_S, wrapS);
	glSamplerParameteri(oglS->sampler, GL_TEXTURE_WRAP_T, wrapT);
	glSamplerParameteri(oglS->sampler, GL_TEXTURE_WRAP_R, wrapR);
	glSamplerParameterf(oglS->sampler, GL_TEXTURE_MAX_ANISOTROPY_EXT, desc->maxAnisotropy);

	*sampler = oglS;

	MFG_CHECK_GL_ERROR();
	return MFG_ERROR_OKAY;
}

mfgError mfgOGL4ClearColor(mfgRenderDevice* rd, mfmF32 r, mfmF32 g, mfmF32 b, mfmF32 a)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{ if (rd == NULL) return MFG_ERROR_INVALID_ARGUMENTS; }
#endif
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT);
	MFG_CHECK_GL_ERROR();
	return MFG_ERROR_OKAY;
}

mfgError mfgOGL4ClearDepth(mfgRenderDevice* rd, mfmF32 depth)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{ if (rd == NULL) return MFG_ERROR_INVALID_ARGUMENTS; }
#endif
	glClearDepth(depth);
	glClear(GL_DEPTH_BUFFER_BIT);
	MFG_CHECK_GL_ERROR();
	return MFG_ERROR_OKAY;
}

mfgError mfgOGL4ClearStencil(mfgRenderDevice* rd, mfmI32 stencil)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{ if (rd == NULL) return MFG_ERROR_INVALID_ARGUMENTS; }
#endif
	glClearStencil(stencil);
	glClear(GL_STENCIL_BUFFER_BIT);
	MFG_CHECK_GL_ERROR();
	return MFG_ERROR_OKAY;
}

mfgError mfgOGL4SwapBuffers(mfgRenderDevice* rd)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{ if (rd == NULL) return MFG_ERROR_INVALID_ARGUMENTS; }
#endif
	glfwSwapBuffers((GLFWwindow*)mfiGetGLWindowGLFWHandle(((mfgOGL4RenderDevice*)rd)->window));
	MFG_CHECK_GL_ERROR();
	return MFG_ERROR_OKAY;
}

void mfgOGL4DestroyRasterState(void* state)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (state == NULL) abort();
#endif
	mfgOGL4RasterState* oglState = state;
	if (mfmDeallocate(((mfgOGL4RenderDevice*)oglState->base.renderDevice)->pool64, oglState) != MFM_ERROR_OKAY)
		abort();
#ifdef MAGMA_FRAMEWORK_DEBUG
	GLenum err = glGetError();
	if (err != 0)
		abort();
#endif
}

mfgError mfgOGL4CreateRasterState(mfgRenderDevice* rd, mfgRasterState** state, const mfgRasterStateDesc* desc)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{ if (rd == NULL || state == NULL || desc == NULL) return MFG_ERROR_INVALID_ARGUMENTS; }
#endif

	mfgOGL4RenderDevice* oglRD = (mfgOGL4RenderDevice*)rd;

	// Allocate state
	mfgOGL4RasterState* oglState = NULL;
	if (mfmAllocate(oglRD->pool64, &oglState, sizeof(mfgOGL4RasterState)) != MFM_ERROR_OKAY)
		MFG_RETURN_ERROR(MFG_ERROR_ALLOCATION_FAILED, u8"Failed to allocate raster state on pool");

	// Init object
	oglState->base.object.destructorFunc = &mfgOGL4DestroyRasterState;
	oglState->base.object.referenceCount = 0;
	oglState->base.renderDevice = rd;

	*state = oglState;

	// Set properties
	if (desc->cullEnabled == MFM_FALSE)
		oglState->cullEnabled = GL_FALSE;
	else
		oglState->cullEnabled = GL_TRUE;

	switch (desc->frontFace)
	{
		case MFG_CW: oglState->frontFace = GL_CW; break;
		case MFG_CCW: oglState->frontFace = GL_CCW; break;
		default: MFG_RETURN_ERROR(MFG_ERROR_INVALID_ARGUMENTS, "Unsupported front face");
	}

	switch (desc->cullFace)
	{
		case MFG_FRONT: oglState->cullFace = GL_FRONT; break;
		case MFG_BACK: oglState->cullFace = GL_BACK; break;
		case MFG_FRONT_AND_BACK: oglState->cullFace = GL_FRONT_AND_BACK; break;
		default: MFG_RETURN_ERROR(MFG_ERROR_INVALID_ARGUMENTS, "Unsupported cull face");
	}

	switch (desc->rasterMode)
	{
		case MFG_WIREFRAME: oglState->polygonMode = GL_LINE; break;
		case MFG_FILL: oglState->polygonMode = GL_FILL; break;
		default: MFG_RETURN_ERROR(MFG_ERROR_INVALID_ARGUMENTS, "Unsupported raster mode");
	}

	MFG_CHECK_GL_ERROR();
	return MFG_ERROR_OKAY;
}

mfgError mfgOGL4SetRasterState(mfgRenderDevice* rd, mfgRasterState* state)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{ if (rd == NULL) return MFG_ERROR_INVALID_ARGUMENTS; }
#endif
	if (state == NULL)
		return mfgOGL4SetRasterState(rd, ((mfgOGL4RenderDevice*)rd)->defaultRasterState);

	mfgOGL4RasterState* oglRS = (mfgOGL4RasterState*)state;

	if (oglRS->cullEnabled)
		glEnable(GL_CULL_FACE);
	else
		glDisable(GL_CULL_FACE);
	glFrontFace(oglRS->frontFace);
	glCullFace(oglRS->cullFace);
	glPolygonMode(GL_FRONT_AND_BACK, oglRS->polygonMode);

	MFG_CHECK_GL_ERROR();

	return MFG_ERROR_OKAY;
}

void mfgOGL4DestroyDepthStencilState(void* state)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (state == NULL) abort();
#endif
	mfgOGL4DepthStencilState* oglState = state;
	if (mfmDeallocate(((mfgOGL4RenderDevice*)oglState->base.renderDevice)->pool256, oglState) != MFM_ERROR_OKAY)
		abort();
#ifdef MAGMA_FRAMEWORK_DEBUG
	GLenum err = glGetError();
	if (err != 0)
		abort();
#endif
}

mfgError mfgOGL4CreateDepthStencilState(mfgRenderDevice* rd, mfgDepthStencilState** state, const mfgDepthStencilStateDesc* desc)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{ if (rd == NULL || state == NULL || desc == NULL) return MFG_ERROR_INVALID_ARGUMENTS; }
#endif

	mfgOGL4RenderDevice* oglRD = (mfgOGL4RenderDevice*)rd;

	// Allocate state
	mfgOGL4DepthStencilState* oglState = NULL;
	if (mfmAllocate(oglRD->pool256, &oglState, sizeof(mfgOGL4DepthStencilState)) != MFM_ERROR_OKAY)
		MFG_RETURN_ERROR(MFG_ERROR_ALLOCATION_FAILED, u8"Failed to allocate depth stencil state on pool");

	// Init object
	oglState->base.object.destructorFunc = &mfgOGL4DestroyDepthStencilState;
	oglState->base.object.referenceCount = 0;
	oglState->base.renderDevice = rd;

	// Set properties
	oglState->depthEnabled = desc->depthEnabled;
	oglState->depthWriteEnabled = desc->depthWriteEnabled;
	oglState->depthNear = desc->depthNear;
	oglState->depthFar = desc->depthFar;

	switch (desc->depthCompare)
	{
		case MFG_NEVER: oglState->depthFunc = GL_NEVER; break;
		case MFG_LESS: oglState->depthFunc = GL_LESS; break;
		case MFG_LEQUAL: oglState->depthFunc = GL_LEQUAL; break;
		case MFG_GREATER: oglState->depthFunc = GL_GREATER; break;
		case MFG_GEQUAL: oglState->depthFunc = GL_GEQUAL; break;
		case MFG_EQUAL: oglState->depthFunc = GL_EQUAL; break;
		case MFG_NEQUAL: oglState->depthFunc = GL_NOTEQUAL; break;
		case MFG_ALWAYS: oglState->depthFunc = GL_ALWAYS; break;
		default: MFG_RETURN_ERROR(MFG_ERROR_INVALID_ARGUMENTS, "Unsupported depth compare func");
	}

	oglState->stencilEnabled = desc->stencilEnabled;
	oglState->stencilRef = desc->stencilRef;
	oglState->stencilReadMask = desc->stencilReadMask;
	oglState->stencilWriteMask = desc->stencilWriteMask;

	switch (desc->frontFaceStencilCompare)
	{
		case MFG_NEVER: oglState->frontStencilFunc = GL_NEVER; break;
		case MFG_LESS: oglState->frontStencilFunc = GL_LESS; break;
		case MFG_LEQUAL: oglState->frontStencilFunc = GL_LEQUAL; break;
		case MFG_GREATER: oglState->frontStencilFunc = GL_GREATER; break;
		case MFG_GEQUAL: oglState->frontStencilFunc = GL_GEQUAL; break;
		case MFG_EQUAL: oglState->frontStencilFunc = GL_EQUAL; break;
		case MFG_NEQUAL: oglState->frontStencilFunc = GL_NOTEQUAL; break;
		case MFG_ALWAYS: oglState->frontStencilFunc = GL_ALWAYS; break;
		default: MFG_RETURN_ERROR(MFG_ERROR_INVALID_ARGUMENTS, "Unsupported front face stencil compare func");
	}

	switch (desc->frontFaceStencilFail)
	{
		case MFG_KEEP: oglState->frontFaceStencilFail = GL_KEEP; break;
		case MFG_ZERO: oglState->frontFaceStencilFail = GL_ZERO; break;
		case MFG_REPLACE: oglState->frontFaceStencilFail = GL_REPLACE; break;
		case MFG_INCREMENT: oglState->frontFaceStencilFail = GL_INCR; break;
		case MFG_INCREMENT_WRAP: oglState->frontFaceStencilFail = GL_INCR_WRAP; break;
		case MFG_DECREMENT: oglState->frontFaceStencilFail = GL_DECR; break;
		case MFG_DECREMENT_WRAP: oglState->frontFaceStencilFail = GL_DECR_WRAP; break;
		case MFG_INVERT: oglState->frontFaceStencilFail = GL_INVERT; break;
		default: MFG_RETURN_ERROR(MFG_ERROR_INVALID_ARGUMENTS, "Unsupported front face stencil fail action");
	}

	switch (desc->frontFaceStencilPass)
	{
		case MFG_KEEP: oglState->frontFaceStencilPass = GL_KEEP; break;
		case MFG_ZERO: oglState->frontFaceStencilPass = GL_ZERO; break;
		case MFG_REPLACE: oglState->frontFaceStencilPass = GL_REPLACE; break;
		case MFG_INCREMENT: oglState->frontFaceStencilPass = GL_INCR; break;
		case MFG_INCREMENT_WRAP: oglState->frontFaceStencilPass = GL_INCR_WRAP; break;
		case MFG_DECREMENT: oglState->frontFaceStencilPass = GL_DECR; break;
		case MFG_DECREMENT_WRAP: oglState->frontFaceStencilPass = GL_DECR_WRAP; break;
		case MFG_INVERT: oglState->frontFaceStencilPass = GL_INVERT; break;
		default: MFG_RETURN_ERROR(MFG_ERROR_INVALID_ARGUMENTS, "Unsupported front face stencil pass action");
	}

	switch (desc->frontFaceDepthFail)
	{
		case MFG_KEEP: oglState->frontFaceDepthFail = GL_KEEP; break;
		case MFG_ZERO: oglState->frontFaceDepthFail = GL_ZERO; break;
		case MFG_REPLACE: oglState->frontFaceDepthFail = GL_REPLACE; break;
		case MFG_INCREMENT: oglState->frontFaceDepthFail = GL_INCR; break;
		case MFG_INCREMENT_WRAP: oglState->frontFaceDepthFail = GL_INCR_WRAP; break;
		case MFG_DECREMENT: oglState->frontFaceDepthFail = GL_DECR; break;
		case MFG_DECREMENT_WRAP: oglState->frontFaceDepthFail = GL_DECR_WRAP; break;
		case MFG_INVERT: oglState->frontFaceDepthFail = GL_INVERT; break;
		default: MFG_RETURN_ERROR(MFG_ERROR_INVALID_ARGUMENTS, "Unsupported front face depth fail action");
	}

	switch (desc->backFaceStencilCompare)
	{
		case MFG_NEVER: oglState->backStencilFunc = GL_NEVER; break;
		case MFG_LESS: oglState->backStencilFunc = GL_LESS; break;
		case MFG_LEQUAL: oglState->backStencilFunc = GL_LEQUAL; break;
		case MFG_GREATER: oglState->backStencilFunc = GL_GREATER; break;
		case MFG_GEQUAL: oglState->backStencilFunc = GL_GEQUAL; break;
		case MFG_EQUAL: oglState->backStencilFunc = GL_EQUAL; break;
		case MFG_NEQUAL: oglState->backStencilFunc = GL_NOTEQUAL; break;
		case MFG_ALWAYS: oglState->backStencilFunc = GL_ALWAYS; break;
		default: MFG_RETURN_ERROR(MFG_ERROR_INVALID_ARGUMENTS, "Unsupported back face stencil compare func");
	}

	switch (desc->backFaceStencilFail)
	{
		case MFG_KEEP: oglState->backFaceStencilFail = GL_KEEP; break;
		case MFG_ZERO: oglState->backFaceStencilFail = GL_ZERO; break;
		case MFG_REPLACE: oglState->backFaceStencilFail = GL_REPLACE; break;
		case MFG_INCREMENT: oglState->backFaceStencilFail = GL_INCR; break;
		case MFG_INCREMENT_WRAP: oglState->backFaceStencilFail = GL_INCR_WRAP; break;
		case MFG_DECREMENT: oglState->backFaceStencilFail = GL_DECR; break;
		case MFG_DECREMENT_WRAP: oglState->backFaceStencilFail = GL_DECR_WRAP; break;
		case MFG_INVERT: oglState->backFaceStencilFail = GL_INVERT; break;
		default: MFG_RETURN_ERROR(MFG_ERROR_INVALID_ARGUMENTS, "Unsupported back face stencil fail action");
	}

	switch (desc->backFaceStencilPass)
	{
		case MFG_KEEP: oglState->backFaceStencilPass = GL_KEEP; break;
		case MFG_ZERO: oglState->backFaceStencilPass = GL_ZERO; break;
		case MFG_REPLACE: oglState->backFaceStencilPass = GL_REPLACE; break;
		case MFG_INCREMENT: oglState->backFaceStencilPass = GL_INCR; break;
		case MFG_INCREMENT_WRAP: oglState->backFaceStencilPass = GL_INCR_WRAP; break;
		case MFG_DECREMENT: oglState->backFaceStencilPass = GL_DECR; break;
		case MFG_DECREMENT_WRAP: oglState->backFaceStencilPass = GL_DECR_WRAP; break;
		case MFG_INVERT: oglState->backFaceStencilPass = GL_INVERT; break;
		default: MFG_RETURN_ERROR(MFG_ERROR_INVALID_ARGUMENTS, "Unsupported back face stencil pass action");
	}

	switch (desc->backFaceDepthFail)
	{
		case MFG_KEEP: oglState->backFaceDepthFail = GL_KEEP; break;
		case MFG_ZERO: oglState->backFaceDepthFail = GL_ZERO; break;
		case MFG_REPLACE: oglState->backFaceDepthFail = GL_REPLACE; break;
		case MFG_INCREMENT: oglState->backFaceDepthFail = GL_INCR; break;
		case MFG_INCREMENT_WRAP: oglState->backFaceDepthFail = GL_INCR_WRAP; break;
		case MFG_DECREMENT: oglState->backFaceDepthFail = GL_DECR; break;
		case MFG_DECREMENT_WRAP: oglState->backFaceDepthFail = GL_DECR_WRAP; break;
		case MFG_INVERT: oglState->backFaceDepthFail = GL_INVERT; break;
		default: MFG_RETURN_ERROR(MFG_ERROR_INVALID_ARGUMENTS, "Unsupported back face depth fail action");
	}

	*state = oglState;

	MFG_CHECK_GL_ERROR();
	return MFG_ERROR_OKAY;
}

mfgError mfgOGL4SetDepthStencilState(mfgRenderDevice* rd, mfgDepthStencilState* state)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{ if (rd == NULL) return MFG_ERROR_INVALID_ARGUMENTS; }
#endif
	if (state == NULL)
		return mfgOGL4SetDepthStencilState(rd, ((mfgOGL4RenderDevice*)rd)->defaultDepthStencilState);

	mfgOGL4DepthStencilState* oglDSS = (mfgOGL4DepthStencilState*)state;

	if (oglDSS->depthEnabled)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);
	
	glDepthFunc(oglDSS->depthFunc);
	glDepthMask(oglDSS->depthWriteEnabled ? GL_TRUE : GL_FALSE);
	glDepthRange(oglDSS->depthNear, oglDSS->depthFar);

	if (oglDSS->stencilEnabled)
		glEnable(GL_STENCIL_TEST);
	else
		glDisable(GL_STENCIL_TEST);

	glStencilFuncSeparate(GL_FRONT, oglDSS->frontStencilFunc, oglDSS->stencilRef, oglDSS->stencilReadMask);
	glStencilMaskSeparate(GL_FRONT, oglDSS->stencilWriteMask);
	glStencilOpSeparate(GL_FRONT, oglDSS->frontFaceStencilFail, oglDSS->frontFaceDepthFail, oglDSS->frontFaceStencilPass);

	glStencilFuncSeparate(GL_BACK, oglDSS->backStencilFunc, oglDSS->stencilRef, oglDSS->stencilReadMask);
	glStencilMaskSeparate(GL_BACK, oglDSS->stencilWriteMask);
	glStencilOpSeparate(GL_BACK, oglDSS->backFaceStencilFail, oglDSS->backFaceDepthFail, oglDSS->backFaceStencilPass);

	MFG_CHECK_GL_ERROR();

	return MFG_ERROR_OKAY;
}

void mfgOGL4DestroyBlendState(void* state)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (state == NULL) abort();
#endif
	mfgOGL4BlendState* oglState = state;
	if (mfmDeallocate(((mfgOGL4RenderDevice*)oglState->base.renderDevice)->pool64, oglState) != MFM_ERROR_OKAY)
		abort();
#ifdef MAGMA_FRAMEWORK_DEBUG
	GLenum err = glGetError();
	if (err != 0)
		abort();
#endif
}

mfgError mfgOGL4CreateBlendState(mfgRenderDevice* rd, mfgBlendState** state, const mfgBlendStateDesc* desc)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{ if (rd == NULL || state == NULL || desc == NULL) return MFG_ERROR_INVALID_ARGUMENTS; }
#endif

	mfgOGL4RenderDevice* oglRD = (mfgOGL4RenderDevice*)rd;

	// Allocate state
	mfgOGL4BlendState* oglState = NULL;
	if (mfmAllocate(oglRD->pool64, &oglState, sizeof(mfgOGL4BlendState)) != MFM_ERROR_OKAY)
		MFG_RETURN_ERROR(MFG_ERROR_ALLOCATION_FAILED, u8"Failed to allocate raster state on pool");

	// Init object
	oglState->base.object.destructorFunc = &mfgOGL4DestroyRasterState;
	oglState->base.object.referenceCount = 0;
	oglState->base.renderDevice = rd;

	// Set properties
	oglState->blendEnabled = desc->blendEnabled;

	switch (desc->sourceFactor)
	{
		case MFG_ZERO: oglState->srcFactor = GL_ZERO; break;
		case MFG_ONE: oglState->srcFactor = GL_ONE; break;
		case MFG_SRC_COLOR: oglState->srcFactor = GL_SRC_COLOR; break;
		case MFG_INV_SRC_COLOR: oglState->srcFactor = GL_ONE_MINUS_SRC_COLOR; break;
		case MFG_DST_COLOR: oglState->srcFactor = GL_DST_COLOR; break;
		case MFG_INV_DST_COLOR: oglState->srcFactor = GL_ONE_MINUS_DST_COLOR; break;
		case MFG_SRC_ALPHA: oglState->srcFactor = GL_SRC_ALPHA; break;
		case MFG_INV_SRC_ALPHA: oglState->srcFactor = GL_ONE_MINUS_SRC_ALPHA; break;
		case MFG_DST_ALPHA: oglState->srcFactor = GL_DST_ALPHA; break;
		case MFG_INV_DST_ALPHA: oglState->srcFactor = GL_ONE_MINUS_DST_ALPHA; break;

		default: MFG_RETURN_ERROR(MFG_ERROR_INVALID_ARGUMENTS, "Unsupported blend source factor");
	}

	switch (desc->destinationFactor)
	{
		case MFG_ZERO: oglState->dstFactor = GL_ZERO; break;
		case MFG_ONE: oglState->dstFactor = GL_ONE; break;
		case MFG_SRC_COLOR: oglState->dstFactor = GL_SRC_COLOR; break;
		case MFG_INV_SRC_COLOR: oglState->dstFactor = GL_ONE_MINUS_SRC_COLOR; break;
		case MFG_DST_COLOR: oglState->dstFactor = GL_DST_COLOR; break;
		case MFG_INV_DST_COLOR: oglState->dstFactor = GL_ONE_MINUS_DST_COLOR; break;
		case MFG_SRC_ALPHA: oglState->dstFactor = GL_SRC_ALPHA; break;
		case MFG_INV_SRC_ALPHA: oglState->dstFactor = GL_ONE_MINUS_SRC_ALPHA; break;
		case MFG_DST_ALPHA: oglState->dstFactor = GL_DST_ALPHA; break;
		case MFG_INV_DST_ALPHA: oglState->dstFactor = GL_ONE_MINUS_DST_ALPHA; break;

		default: MFG_RETURN_ERROR(MFG_ERROR_INVALID_ARGUMENTS, "Unsupported blend destination factor");
	}

	switch (desc->blendOperation)
	{
		case MFG_ADD: oglState->blendOp = GL_FUNC_ADD; break;
		case MFG_SUBTRACT: oglState->blendOp = GL_FUNC_SUBTRACT; break;
		case MFG_REV_SUBTRACT: oglState->blendOp = GL_FUNC_REVERSE_SUBTRACT; break;
		case MFG_MIN: oglState->blendOp = GL_MIN; break;
		case MFG_MAX: oglState->blendOp = GL_MAX; break;

		default: MFG_RETURN_ERROR(MFG_ERROR_INVALID_ARGUMENTS, "Unsupported blend operation");
	}

	switch (desc->sourceAlphaFactor)
	{
		case MFG_ZERO: oglState->srcAlphaFactor = GL_ZERO; break;
		case MFG_ONE: oglState->srcAlphaFactor = GL_ONE; break;
		case MFG_SRC_COLOR: oglState->srcAlphaFactor = GL_SRC_COLOR; break;
		case MFG_INV_SRC_COLOR: oglState->srcAlphaFactor = GL_ONE_MINUS_SRC_COLOR; break;
		case MFG_DST_COLOR: oglState->srcAlphaFactor = GL_DST_COLOR; break;
		case MFG_INV_DST_COLOR: oglState->srcAlphaFactor = GL_ONE_MINUS_DST_COLOR; break;
		case MFG_SRC_ALPHA: oglState->srcAlphaFactor = GL_SRC_ALPHA; break;
		case MFG_INV_SRC_ALPHA: oglState->srcAlphaFactor = GL_ONE_MINUS_SRC_ALPHA; break;
		case MFG_DST_ALPHA: oglState->srcAlphaFactor = GL_DST_ALPHA; break;
		case MFG_INV_DST_ALPHA: oglState->srcAlphaFactor = GL_ONE_MINUS_DST_ALPHA; break;

		default: MFG_RETURN_ERROR(MFG_ERROR_INVALID_ARGUMENTS, "Unsupported blend source alpha factor");
	}

	switch (desc->destinationAlphaFactor)
	{
		case MFG_ZERO: oglState->dstAlphaFactor = GL_ZERO; break;
		case MFG_ONE: oglState->dstAlphaFactor = GL_ONE; break;
		case MFG_SRC_COLOR: oglState->dstAlphaFactor = GL_SRC_COLOR; break;
		case MFG_INV_SRC_COLOR: oglState->dstAlphaFactor = GL_ONE_MINUS_SRC_COLOR; break;
		case MFG_DST_COLOR: oglState->dstAlphaFactor = GL_DST_COLOR; break;
		case MFG_INV_DST_COLOR: oglState->dstAlphaFactor = GL_ONE_MINUS_DST_COLOR; break;
		case MFG_SRC_ALPHA: oglState->dstAlphaFactor = GL_SRC_ALPHA; break;
		case MFG_INV_SRC_ALPHA: oglState->dstAlphaFactor = GL_ONE_MINUS_SRC_ALPHA; break;
		case MFG_DST_ALPHA: oglState->dstAlphaFactor = GL_DST_ALPHA; break;
		case MFG_INV_DST_ALPHA: oglState->dstAlphaFactor = GL_ONE_MINUS_DST_ALPHA; break;

		default: MFG_RETURN_ERROR(MFG_ERROR_INVALID_ARGUMENTS, "Unsupported blend destination alpha factor");
	}

	switch (desc->blendAlphaOperation)
	{
		case MFG_ADD: oglState->alphaBlendOp = GL_FUNC_ADD; break;
		case MFG_SUBTRACT: oglState->alphaBlendOp = GL_FUNC_SUBTRACT; break;
		case MFG_REV_SUBTRACT: oglState->alphaBlendOp = GL_FUNC_REVERSE_SUBTRACT; break;
		case MFG_MIN: oglState->alphaBlendOp = GL_MIN; break;
		case MFG_MAX: oglState->alphaBlendOp = GL_MAX; break;

		default: MFG_RETURN_ERROR(MFG_ERROR_INVALID_ARGUMENTS, "Unsupported blend alpha operation");
	}

	*state = oglState;

	MFG_CHECK_GL_ERROR();
	return MFG_ERROR_OKAY;
}

mfgError mfgOGL4SetBlendState(mfgRenderDevice* rd, mfgBlendState* state)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{ if (rd == NULL) return MFG_ERROR_INVALID_ARGUMENTS; }
#endif
	if (state == NULL)
		return mfgOGL4SetBlendState(rd, ((mfgOGL4RenderDevice*)rd)->defaultBlendState);

	mfgOGL4BlendState* oglBS = (mfgOGL4BlendState*)state;
	
	if (oglBS->blendEnabled)
		glEnable(GL_BLEND);
	else
		glDisable(GL_BLEND);

	glBlendFuncSeparate(oglBS->srcFactor,
						oglBS->dstFactor,
						oglBS->srcAlphaFactor,
						oglBS->dstAlphaFactor);

	glBlendEquationSeparate(oglBS->blendOp,
							oglBS->alphaBlendOp);

	MFG_CHECK_GL_ERROR();

	return MFG_ERROR_OKAY;
}

mfmBool mfgOGL4GetErrorString(mfgRenderDevice* rd, mfsUTF8CodeUnit* str, mfmU64 maxSize)
{
	if (rd == NULL || str == NULL || maxSize == 0)
		abort();

	mfgOGL4RenderDevice* oglRD = (mfgOGL4RenderDevice*)rd;

	if (oglRD->errorStringSize == 0)
	{
		str[0] = '\0';
		return MFM_FALSE;
	}

	if (maxSize >= oglRD->errorStringSize)
	{
		memcpy(str, oglRD->errorString, oglRD->errorStringSize);
		str[oglRD->errorStringSize] = '\0';
	}
	else
	{
		memcpy(str, oglRD->errorString, maxSize - 1);
		str[maxSize] = '\0';
	}

	return MFM_TRUE;
}

mfgError mfgOGL4DrawTriangles(mfgRenderDevice* rd, mfmU64 offset, mfmU64 count)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{ if (rd == NULL) return MFG_ERROR_INVALID_ARGUMENTS; }
#endif
	glDrawArrays(GL_TRIANGLES, offset, count);
	MFG_CHECK_GL_ERROR();
	return MFG_ERROR_OKAY;
}

mfgError mfgOGL4DrawTrianglesIndexed(mfgRenderDevice* rd, mfmU64 offset, mfmU64 count)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{
		if (rd == NULL ||
			((mfgOGL4RenderDevice*)rd)->currentIndexBuffer == NULL) return MFG_ERROR_INVALID_ARGUMENTS;
	}
#endif
	glDrawElements(GL_TRIANGLES, count, ((mfgOGL4RenderDevice*)rd)->currentIndexBuffer->indexType, (char*)NULL + offset);
	MFG_CHECK_GL_ERROR();
	return MFG_ERROR_OKAY;
}

mfgError mfgCreateOGL4RenderDevice(mfgRenderDevice ** renderDevice, mfiWindow* window, const mfgRenderDeviceDesc * desc, void * allocator)
{
	// Check if params are valid
	if (renderDevice == NULL || window == NULL || desc == NULL || window->type != MFI_OGLWINDOW)
		return MFG_ERROR_INVALID_ARGUMENTS;
	
	// Allocate render device
	mfgOGL4RenderDevice* rd;
	if (mfmAllocate(allocator, &rd, sizeof(mfgOGL4RenderDevice)) != MFM_ERROR_OKAY)
		return MFG_ERROR_ALLOCATION_FAILED;

	// Create 32 bytes pool
	{
		mfmPoolAllocatorDesc desc;
		desc.expandable = MFM_FALSE;
		desc.slotCount = MFG_POOL_32_ELEMENT_COUNT;
		desc.slotSize = 32;
		mfmError err = mfmCreatePoolAllocatorOnMemory(&rd->pool32, &desc, rd->pool32Memory, sizeof(rd->pool32Memory));
		if (err != MFM_ERROR_OKAY)
			return MFG_ERROR_ALLOCATION_FAILED;
	}

	// Create 64 bytes pool
	{
		mfmPoolAllocatorDesc desc;
		desc.expandable = MFM_FALSE;
		desc.slotCount = MFG_POOL_64_ELEMENT_COUNT;
		desc.slotSize = 64;
		mfmError err = mfmCreatePoolAllocatorOnMemory(&rd->pool64, &desc, rd->pool64Memory, sizeof(rd->pool64Memory));
		if (err != MFM_ERROR_OKAY)
			return MFG_ERROR_ALLOCATION_FAILED;
	}

	// Create 256 bytes pool
	{
		mfmPoolAllocatorDesc desc;
		desc.expandable = MFM_FALSE;
		desc.slotCount = MFG_POOL_256_ELEMENT_COUNT;
		desc.slotSize = 256;
		mfmError err = mfmCreatePoolAllocatorOnMemory(&rd->pool256, &desc, rd->pool256Memory, sizeof(rd->pool256Memory));
		if (err != MFM_ERROR_OKAY)
			return MFG_ERROR_ALLOCATION_FAILED;
	}

	// Create 512 bytes pool
	{
		mfmPoolAllocatorDesc desc;
		desc.expandable = MFM_FALSE;
		desc.slotCount = MFG_POOL_512_ELEMENT_COUNT;
		desc.slotSize = 512;
		mfmError err = mfmCreatePoolAllocatorOnMemory(&rd->pool512, &desc, rd->pool512Memory, sizeof(rd->pool512Memory));
		if (err != MFM_ERROR_OKAY)
			return MFG_ERROR_ALLOCATION_FAILED;
	}

	// Create stack
	{
		mfmError err = mfmCreateStackAllocatorOnMemory(&rd->stack, 2048, rd->stackMemory, sizeof(rd->stackMemory));
		if (err != MFM_ERROR_OKAY)
			return MFG_ERROR_ALLOCATION_FAILED;
	}

	// Initialize some properties
	rd->base.object.destructorFunc = &mfgDestroyOGL4RenderDevice;
	rd->base.object.referenceCount = 0;
	rd->window = window;
	rd->allocator = allocator;
	memset(rd->errorString, '\0', sizeof(rd->errorString));
	rd->errorStringSize = 0;

	rd->currentIndexBuffer = NULL;

	// Init context
	glfwMakeContextCurrent((GLFWwindow*)mfiGetGLWindowGLFWHandle(((mfgOGL4RenderDevice*)rd)->window));
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		mfsPrintFormatUTF8(mfsErrStream, u8"Failed to create OGL4RenderDevice:\nFailed to init GLEW:\n%s", (const mfsUTF8CodeUnit*)glewGetErrorString(err));
		return MFS_ERROR_INTERNAL;
	}

	// Check extensions
	if (!GLEW_EXT_texture_filter_anisotropic)
		return MFG_ERROR_NO_EXTENSION;

	// Enable or disable VSync
	if (desc->vsyncEnabled == MFM_TRUE)
		glfwSwapInterval(1);
	else
		glfwSwapInterval(0);

	// Set functions
	rd->base.createVertexShader = &mfgOGL4CreateVertexShader;
	rd->base.destroyVertexShader = &mfgOGL4DestroyVertexShader;
	rd->base.createPixelShader = &mfgOGL4CreatePixelShader;
	rd->base.destroyPixelShader = &mfgOGL4DestroyPixelShader;
	rd->base.createPipeline = &mfgOGL4CreatePipeline;
	rd->base.destroyPipeline = &mfgOGL4DestroyPipeline;
	rd->base.setPipeline = &mfgOGL4SetPipeline;

	rd->base.getVertexShaderBindingPoint = &mfgOGL4GetVertexShaderBindingPoint;
	rd->base.getPixelShaderBindingPoint = &mfgOGL4GetPixelShaderBindingPoint;
	rd->base.bindConstantBuffer = &mfgOGL4BindConstantBuffer;
	rd->base.bindConstantBufferRange = &mfgOGL4BindConstantBufferRange;
	rd->base.bindTexture1D = &mfgOGL4BindTexture1D;
	rd->base.bindTexture2D = &mfgOGL4BindTexture2D;
	rd->base.bindTexture3D = &mfgOGL4BindTexture3D;
	rd->base.bindSampler = &mfgOGL4BindSampler;

	rd->base.createConstantBuffer = &mfgOGL4CreateConstantBuffer;
	rd->base.destroyConstantBuffer = &mfgOGL4DestroyConstantBuffer;
	rd->base.mapConstantBuffer = &mfgOGL4MapConstantBuffer;
	rd->base.unmapConstantBuffer = &mfgOGL4UnmapConstantBuffer;

	rd->base.createVertexBuffer = &mfgOGL4CreateVertexBuffer;
	rd->base.destroyVertexBuffer = &mfgOGL4DestroyVertexBuffer;
	rd->base.mapVertexBuffer = &mfgOGL4MapVertexBuffer;
	rd->base.unmapVertexBuffer = &mfgOGL4UnmapVertexBuffer;
	rd->base.createVertexLayout = &mfgOGL4CreateVertexLayout;
	rd->base.destroyVertexLayout = &mfgOGL4DestroyVertexLayout;
	rd->base.createVertexArray = &mfgOGL4CreateVertexArray;
	rd->base.destroyVertexArray = &mfgOGL4DestroyVertexArray;
	rd->base.setVertexArray = &mfgOGL4SetVertexArray;
	rd->base.createIndexBuffer = &mfgOGL4CreateIndexBuffer;
	rd->base.destroyIndexBuffer = &mfgOGL4DestroyIndexBuffer;
	rd->base.setIndexBuffer = &mfgOGL4SetIndexBuffer;
	rd->base.mapIndexBuffer = &mfgOGL4MapIndexBuffer;
	rd->base.unmapIndexBuffer = &mfgOGL4UnmapIndexBuffer;

	rd->base.createTexture1D = &mfgOGL4CreateTexture1D;
	rd->base.destroyTexture1D = &mfgOGL4DestroyTexture1D;
	rd->base.updateTexture1D = &mfgOGL4UpdateTexture1D;
	rd->base.generateTexture1DMipmaps = &mfgOGL4GenerateTexture1DMipmaps;

	rd->base.createTexture2D = &mfgOGL4CreateTexture2D;
	rd->base.destroyTexture2D = &mfgOGL4DestroyTexture2D;
	rd->base.updateTexture2D = &mfgOGL4UpdateTexture2D;
	rd->base.generateTexture2DMipmaps = &mfgOGL4GenerateTexture2DMipmaps;

	rd->base.createTexture3D = &mfgOGL4CreateTexture3D;
	rd->base.destroyTexture3D = &mfgOGL4DestroyTexture3D;
	rd->base.updateTexture3D = &mfgOGL4UpdateTexture3D;
	rd->base.generateTexture3DMipmaps = &mfgOGL4GenerateTexture3DMipmaps;

	rd->base.createSampler = &mfgOGL4CreateSampler;
	rd->base.destroySampler = &mfgOGL4DestroySampler;

	rd->base.createRasterState = &mfgOGL4CreateRasterState;
	rd->base.destroyRasterState = &mfgOGL4DestroyRasterState;
	rd->base.setRasterState = &mfgOGL4SetRasterState;
	rd->base.createDepthStencilState = &mfgOGL4CreateDepthStencilState;
	rd->base.destroyDepthStencilState = &mfgOGL4DestroyDepthStencilState;
	rd->base.setDepthStencilState = &mfgOGL4SetDepthStencilState;
	rd->base.createBlendState = &mfgOGL4CreateBlendState;
	rd->base.destroyBlendState = &mfgOGL4DestroyBlendState;
	rd->base.setBlendState = &mfgOGL4SetBlendState;

	rd->base.createRenderTexture = NULL;
	rd->base.destroyRenderTexture = NULL;
	rd->base.createDepthStencilTexture = NULL;
	rd->base.destroyDepthStencilTexture = NULL;
	rd->base.createFramebuffer = NULL;
	rd->base.destroyFramebuffer = NULL;
	rd->base.setFramebuffer = NULL;

	rd->base.clearColor = &mfgOGL4ClearColor;
	rd->base.clearDepth = &mfgOGL4ClearDepth;
	rd->base.clearStencil = &mfgOGL4ClearStencil;
	rd->base.drawTriangles = &mfgOGL4DrawTriangles;
	rd->base.drawTrianglesIndexed = &mfgOGL4DrawTrianglesIndexed;
	rd->base.swapBuffers = &mfgOGL4SwapBuffers;

	rd->base.getPropertyI = NULL;
	rd->base.getPropertyF = NULL;

	rd->base.getErrorString = &mfgOGL4GetErrorString;

	// Get and set the default raster state
	{
		mfgRasterStateDesc desc;
		mfgDefaultRasterStateDesc(&desc);
		mfgError err = mfgCreateRasterState(rd, &rd->defaultRasterState, &desc);
		if (err != MFG_ERROR_OKAY)
			return err;
		err = mfgSetRasterState(rd, rd->defaultRasterState);
		if (err != MFG_ERROR_OKAY)
			return err;
	}

	// Get and set the default depth stencil state
	{
		mfgDepthStencilStateDesc desc;
		mfgDefaultDepthStencilStateDesc(&desc);
		mfgError err = mfgCreateDepthStencilState(rd, &rd->defaultDepthStencilState, &desc);
		if (err != MFG_ERROR_OKAY)
			return err;
		err = mfgSetDepthStencilState(rd, rd->defaultDepthStencilState);
		if (err != MFG_ERROR_OKAY)
			return err;
	}

	// Get and set the default blend state
	{
		mfgBlendStateDesc desc;
		mfgDefaultBlendStateDesc(&desc);
		mfgError err = mfgCreateBlendState(rd, &rd->defaultBlendState, &desc);
		if (err != MFG_ERROR_OKAY)
			return err;
		err = mfgSetBlendState(rd, rd->defaultBlendState);
		if (err != MFG_ERROR_OKAY)
			return err;
	}

	// Successfully inited render device
	*renderDevice = rd;
	return MFG_ERROR_OKAY;
}

void mfgDestroyOGL4RenderDevice(void * renderDevice)
{
	if (renderDevice == NULL)
		abort();

	mfgOGL4RenderDevice* rd = (mfgOGL4RenderDevice*)renderDevice;

	// Destroy default states
	mfgDestroyRasterState(rd->defaultRasterState);
	mfgDestroyDepthStencilState(rd->defaultDepthStencilState);
	mfgDestroyBlendState(rd->defaultBlendState);

	// Destroy pools
	mfmDestroyStackAllocator(rd->stack);
	mfmDestroyPoolAllocator(rd->pool512);
	mfmDestroyPoolAllocator(rd->pool256);
	mfmDestroyPoolAllocator(rd->pool64);
	mfmDestroyPoolAllocator(rd->pool32);

	// Deallocate render device
	if (mfmDeallocate(rd->allocator, rd) != MFM_ERROR_OKAY)
		abort();
}

#else

mfgError mfgCreateOGL4RenderDevice(mfgRenderDevice ** renderDevice, mfiWindow* window, const mfgRenderDeviceDesc * desc, void * allocator)
{
	return MFG_ERROR_NOT_SUPPORTED;
}

void mfgDestroyOGL4RenderDevice(void * renderDevice)
{
	abort();
}

#endif