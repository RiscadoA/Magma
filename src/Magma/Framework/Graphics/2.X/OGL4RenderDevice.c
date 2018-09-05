#include "OGL4RenderDevice.h"
#include "OGL4Assembler.h"
#include "Config.h"
	
#include "../../Memory/StackAllocator.h"
#include "../../Memory/PoolAllocator.h"
#include "../../String/StringStream.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef MAGMA_FRAMEWORK_USE_OPENGL

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define MFG_POOL_48_ELEMENT_COUNT 2048
#define MFG_POOL_64_ELEMENT_COUNT 2048
#define MFG_POOL_256_ELEMENT_COUNT 2048
#define MFG_POOL_512_ELEMENT_COUNT 2048

#define MFG_OGL4_SHADER_MAX_BP_COUNT 8
#define MFG_OGL4_SHADER_MAX_ELEMENT_COUNT 8

typedef struct mfgOGL4Shader mfgOGL4Shader;

typedef struct
{
	const mfgMetaDataBindingPoint* bp;
	GLint location;
	GLboolean active;
	mfgOGL4Shader* shader;
	mfmObject* boundObject;
} mfgOGL4BindingPoint;

struct mfgOGL4Shader
{
	mfgV2XRenderDeviceObject base;
	GLint program;
	const mfgMetaData* md;
	mfgOGL4BindingPoint bps[MFG_OGL4_SHADER_MAX_BP_COUNT];
};

typedef struct
{
	mfgV2XRenderDeviceObject base;
	GLint pipeline;
	mfgOGL4Shader* vs;
	mfgOGL4Shader* ps;
} mfgOGL4Pipeline;

typedef struct
{
	mfgV2XRenderDeviceObject base;
	GLint cb;
} mfgOGL4ConstantBuffer;

typedef struct
{
	mfgV2XRenderDeviceObject base;

	mfmU8 packAligment;
	GLenum internalFormat;
	GLenum format;
	GLenum type;
	GLuint tex;
	GLuint width;
} mfgOGL4Texture1D;

typedef struct
{
	mfgV2XRenderDeviceObject base;

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
	mfgV2XRenderDeviceObject base;

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
	mfgV2XRenderDeviceObject base;
	GLuint sampler;
} mfgOGL4Sampler;

typedef struct
{
	mfgV2XRenderDeviceObject base;

	GLuint tex;
	GLuint width;
	GLuint height;

	mfmU8 packAligment;
	GLenum internalFormat;
	GLenum format;
	GLenum type;
} mfgOGL4RenderTexture;

typedef struct
{
	mfgV2XRenderDeviceObject base;

	GLuint rbo;
	GLuint width;
	GLuint height;
} mfgOGL4DepthStencilTexture;

typedef struct
{
	mfgV2XRenderDeviceObject base;
	GLuint fbo;
	mfgOGL4RenderTexture* textures[MFG_OGL4_SHADER_MAX_BP_COUNT];
	mfgOGL4DepthStencilTexture* depthStencilTexture;
} mfgOGL4Framebuffer;

typedef struct
{
	mfgV2XRenderDeviceObject base;
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
	mfgV2XRenderDeviceObject base;
	mfgOGL4VertexElement elements[MFG_OGL4_SHADER_MAX_ELEMENT_COUNT];
	mfmU64 elementCount;
	mfgOGL4Shader* vs;
} mfgOGL4VertexLayout;

typedef struct
{
	mfgV2XRenderDeviceObject base;
	GLint va;
	mfgOGL4VertexBuffer* vb[16];
	mfgOGL4VertexLayout* vl;
} mfgOGL4VertexArray;

typedef struct
{
	mfgV2XRenderDeviceObject base;
	GLint ib;
	GLenum indexType;
} mfgOGL4IndexBuffer;

typedef struct
{
	mfgV2XRenderDeviceObject base;
	GLboolean cullEnabled;
	GLenum frontFace;
	GLenum cullFace;
	GLenum polygonMode;
} mfgOGL4RasterState;

typedef struct
{
	mfgV2XRenderDeviceObject base;

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
	mfgV2XRenderDeviceObject base;

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
	mfgV2XRenderDevice base;

	mfiWindow* window;

	void* allocator;

	mfsUTF8CodeUnit errorString[256];
	mfmU64 errorStringSize;

	mfmPoolAllocator* pool48;
	mfmU8 pool48Memory[MFM_POOL_ALLOCATOR_SIZE(MFG_POOL_48_ELEMENT_COUNT, 48)];

	mfmPoolAllocator* pool64;
	mfmU8 pool64Memory[MFM_POOL_ALLOCATOR_SIZE(MFG_POOL_64_ELEMENT_COUNT, 64)];

	mfmPoolAllocator* pool256;
	mfmU8 pool256Memory[MFM_POOL_ALLOCATOR_SIZE(MFG_POOL_256_ELEMENT_COUNT, 256)];

	mfmPoolAllocator* pool512;
	mfmU8 pool512Memory[MFM_POOL_ALLOCATOR_SIZE(MFG_POOL_512_ELEMENT_COUNT, 512)];

	mfmStackAllocator* stack;
	mfmU8 stackMemory[MFM_STACK_ALLOCATOR_SIZE(2048)];

	mfgV2XRasterState* defaultRasterState;
	mfgV2XDepthStencilState* defaultDepthStencilState;
	mfgV2XBlendState* defaultBlendState;

	mfgOGL4Pipeline* currentPipeline;
	mfgOGL4Framebuffer* currentFramebuffer;
	mfgOGL4VertexArray* currentVertexArray;
	mfgOGL4IndexBuffer* currentIndexBuffer;

	mfgV2XRasterState* currentRasterState;
	mfgV2XDepthStencilState* currentDepthStencilState;
	mfgV2XBlendState* currentBlendState;
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
	if (mfmDecObjectRef(oglVS->base.renderDevice) != MF_ERROR_OKAY)
		abort();
	if (mfmDecObjectRef(oglVS->md) != MF_ERROR_OKAY)
		abort();
	if (mfmDestroyObject(&oglVS->base.object) != MF_ERROR_OKAY)
		abort();
	if (mfmDeallocate(((mfgOGL4RenderDevice*)oglVS->base.renderDevice)->pool512, oglVS) != MF_ERROR_OKAY)
		abort();
#ifdef MAGMA_FRAMEWORK_DEBUG
	GLenum err = glGetError();
	if (err != 0)
		abort();
#endif
}

mfError mfgOGL4CreateVertexShader(mfgV2XRenderDevice* rd, mfgV2XVertexShader** vs, const mfmU8* bytecode, mfmU64 bytecodeSize, const mfgMetaData* metaData)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{ if (rd == NULL || vs == NULL || bytecode == NULL || metaData == NULL) return MFG_ERROR_INVALID_ARGUMENTS; }
#endif

	mfgOGL4RenderDevice* oglRD = (mfgOGL4RenderDevice*)rd;

	// Allocate vertex shader
	mfgOGL4Shader* oglVS = NULL;
	if (mfmAllocate(oglRD->pool512, &oglVS, sizeof(mfgOGL4Shader)) != MF_ERROR_OKAY)
		MFG_RETURN_ERROR(MFG_ERROR_ALLOCATION_FAILED, u8"Failed to allocate vertex shader on pool");

	// Init object
	{
		mfError err = mfmInitObject(&oglVS->base.object);
		if (err != MF_ERROR_OKAY)
			return err;
	}
	oglVS->base.object.destructorFunc = &mfgOGL4DestroyVertexShader;
	oglVS->base.renderDevice = rd;

	// Create string stream
	mfsStream* ss;
	GLchar buffer[4096];
	if (mfsCreateStringStream(&ss, buffer, sizeof(buffer), oglRD->stack) != MF_ERROR_OKAY)
		MFG_RETURN_ERROR(MFG_ERROR_INTERNAL, u8"Failed to create string stream for mfgOGL4Assemble");
	mfError err = mfgV2XOGL4Assemble(bytecode, bytecodeSize, metaData, ss);
	if (err != MF_ERROR_OKAY)
		MFG_RETURN_ERROR(MFG_ERROR_INTERNAL, u8"mfgOGL4Assemble failed");
	if (mfsPutByte(ss, '\0') != MF_ERROR_OKAY)
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
			mfsPrintFormat(mfsOutStream, u8"Failed to compile/link vertex shader, source:\n");
			mfsPrintFormat(mfsOutStream, bufferSrc);
			mfsPrintFormat(mfsOutStream, u8"\n");
			mfsFlush(mfsOutStream);
			MFG_RETURN_ERROR(MFG_ERROR_INTERNAL, infoLog);
		}
	}

	// Init binding points
	for (mfmU16 i = 0; i < MFG_OGL4_SHADER_MAX_BP_COUNT; ++i)
	{
		oglVS->bps[i].active = GL_FALSE;
		oglVS->bps[i].shader = oglVS;
		oglVS->bps[i].boundObject = NULL;
	}

	{
		mfgMetaDataBindingPoint* bp = oglVS->md->firstBindingPoint;
		for (mfmU16 i = 0; i < MFG_OGL4_SHADER_MAX_BP_COUNT && bp != NULL; ++i)
		{
			if (bp->type == MFG_CONSTANT_BUFFER)
			{
				GLchar buf[256];

				if (mfsCreateStringStream(&ss, buf, sizeof(buf), oglRD->stack) != MF_ERROR_OKAY)
					MFG_RETURN_ERROR(MFG_ERROR_INTERNAL, u8"Failed to create string stream for binding point name");
				
				if (mfsPutString(ss, u8"buf_") != MF_ERROR_OKAY)
				{
					mfsDestroyStringStream(ss);
					MFG_RETURN_ERROR(MFG_ERROR_INTERNAL, u8"mfsPutByte returned error");
				}
				if (mfsWrite(ss, bp->name, sizeof(bp->name), NULL) != MF_ERROR_OKAY)
				{
					mfsDestroyStringStream(ss);
					MFG_RETURN_ERROR(MFG_ERROR_INTERNAL, u8"mfsPutByte returned error");
				}
				if (mfsPutByte(ss, '\0') != MF_ERROR_OKAY)
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

				if (mfsCreateStringStream(&ss, buf, sizeof(buf), oglRD->stack) != MF_ERROR_OKAY)
					MFG_RETURN_ERROR(MFG_ERROR_INTERNAL, u8"Failed to create string stream for binding point name");

				if (mfsPutString(ss, u8"tex1d_") != MF_ERROR_OKAY)
				{
					mfsDestroyStringStream(ss);
					MFG_RETURN_ERROR(MFG_ERROR_INTERNAL, u8"mfsPutString returned error");
				}	
				if (mfsPrintFormat(ss, u8"%d", bp->id) != MF_ERROR_OKAY)
				{
					mfsDestroyStringStream(ss);
					MFG_RETURN_ERROR(MFG_ERROR_INTERNAL, u8"mfsPrintFormatUTF8 returned error");
				}
				if (mfsPutByte(ss, '\0') != MF_ERROR_OKAY)
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

				if (mfsCreateStringStream(&ss, buf, sizeof(buf), oglRD->stack) != MF_ERROR_OKAY)
					MFG_RETURN_ERROR(MFG_ERROR_INTERNAL, u8"Failed to create string stream for binding point name");

				if (mfsPutString(ss, u8"tex2d_") != MF_ERROR_OKAY)
				{
					mfsDestroyStringStream(ss);
					MFG_RETURN_ERROR(MFG_ERROR_INTERNAL, u8"mfsPutString returned error");
				}
				if (mfsPrintFormat(ss, u8"%d", bp->id) != MF_ERROR_OKAY)
				{
					mfsDestroyStringStream(ss);
					MFG_RETURN_ERROR(MFG_ERROR_INTERNAL, u8"mfsPrintFormatUTF8 returned error");
				}
				if (mfsPutByte(ss, '\0') != MF_ERROR_OKAY)
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

				if (mfsCreateStringStream(&ss, buf, sizeof(buf), oglRD->stack) != MF_ERROR_OKAY)
					MFG_RETURN_ERROR(MFG_ERROR_INTERNAL, u8"Failed to create string stream for binding point name");

				if (mfsPutString(ss, u8"tex3d_") != MF_ERROR_OKAY)
				{
					mfsDestroyStringStream(ss);
					MFG_RETURN_ERROR(MFG_ERROR_INTERNAL, u8"mfsPutString returned error");
				}
				if (mfsPrintFormat(ss, u8"%d", bp->id) != MF_ERROR_OKAY)
				{
					mfsDestroyStringStream(ss);
					MFG_RETURN_ERROR(MFG_ERROR_INTERNAL, u8"mfsPrintFormatUTF8 returned error");
				}
				if (mfsPutByte(ss, '\0') != MF_ERROR_OKAY)
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

	err = mfmIncObjectRef(metaData);
	if (err != MF_ERROR_OKAY)
		MFG_RETURN_ERROR(err, u8"mfmIncObjectRef failed on meta data");

	*vs = oglVS;
	err = mfmIncObjectRef(rd);
	if (err != MF_ERROR_OKAY)
		MFG_RETURN_ERROR(err, u8"mfmIncObjectRef failed on render device");

	MFG_CHECK_GL_ERROR();
	return MF_ERROR_OKAY;
}

mfError mfgOGL4GetVertexShaderBindingPoint(mfgV2XRenderDevice* rd, mfgV2XBindingPoint** bp, mfgV2XVertexShader* vs, const mfsUTF8CodeUnit* name)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (rd == NULL || bp == NULL || vs == NULL || name == NULL) return MFG_ERROR_INVALID_ARGUMENTS;
#endif
	mfgOGL4Shader* oglVS = vs;
	
	for (mfmU64 i = 0; i < MFG_OGL4_SHADER_MAX_BP_COUNT; ++i)
		if (oglVS->bps[i].active == GL_TRUE)
		{
			if (strcmp(name, oglVS->bps[i].bp->name) == 0)
			{
				*bp = &oglVS->bps[i];
				return MF_ERROR_OKAY;
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
	if (mfmDecObjectRef(oglPS->base.renderDevice) != MF_ERROR_OKAY)
		abort();
	if (mfmDecObjectRef(oglPS->md) != MF_ERROR_OKAY)
		abort();
	if (mfmDestroyObject(&oglPS->base.object) != MF_ERROR_OKAY)
		abort();
	if (mfmDeallocate(((mfgOGL4RenderDevice*)oglPS->base.renderDevice)->pool512, oglPS) != MF_ERROR_OKAY)
		abort();
#ifdef MAGMA_FRAMEWORK_DEBUG
	GLenum err = glGetError();
	if (err != 0)
		abort();
#endif
}

mfError mfgOGL4CreatePixelShader(mfgV2XRenderDevice* rd, mfgV2XPixelShader** ps, const mfmU8* bytecode, mfmU64 bytecodeSize, const mfgMetaData* metaData)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{ if (rd == NULL || ps == NULL || bytecode == NULL || metaData == NULL) return MFG_ERROR_INVALID_ARGUMENTS; }
#endif

	mfgOGL4RenderDevice* oglRD = (mfgOGL4RenderDevice*)rd;
	
	// Allocate pixel shader
	mfgOGL4Shader* oglPS = NULL;
	if (mfmAllocate(oglRD->pool512, &oglPS, sizeof(mfgOGL4Shader)) != MF_ERROR_OKAY)
		MFG_RETURN_ERROR(MFG_ERROR_ALLOCATION_FAILED, u8"Failed to allocate pixel shader on pool");

	// Init object
	{
		mfError err = mfmInitObject(&oglPS->base.object);
		if (err != MF_ERROR_OKAY)
			return err;
	}
	oglPS->base.object.destructorFunc = &mfgOGL4DestroyPixelShader;
	oglPS->base.renderDevice = rd;

	// Create string stream
	mfsStream* ss;
	GLchar buffer[4096];
	if (mfsCreateStringStream(&ss, buffer, sizeof(buffer), oglRD->stack) != MF_ERROR_OKAY)
		MFG_RETURN_ERROR(MFG_ERROR_INTERNAL, u8"Failed to create string stream for mfgOGL4Assemble");
	mfError err = mfgV2XOGL4Assemble(bytecode, bytecodeSize, metaData, ss);
	if (err != MF_ERROR_OKAY)
		MFG_RETURN_ERROR(MFG_ERROR_INTERNAL, u8"mfgOGL4Assemble failed");
	if (mfsPutByte(ss, '\0') != MF_ERROR_OKAY)
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
			mfsPrintFormat(mfsOutStream, u8"Failed to compile/link pixel shader, source:\n");
			mfsPrintFormat(mfsOutStream, bufferSrc);
			mfsPrintFormat(mfsOutStream, u8"\n");
			mfsFlush(mfsOutStream);
			MFG_RETURN_ERROR(MFG_ERROR_INTERNAL, infoLog);
		}
	}

	// Init binding points
	for (mfmU16 i = 0; i < MFG_OGL4_SHADER_MAX_BP_COUNT; ++i)
	{
		oglPS->bps[i].active = GL_FALSE;
		oglPS->bps[i].shader = oglPS;
		oglPS->bps[i].boundObject = NULL;
	}

	{
		mfgMetaDataBindingPoint* bp = oglPS->md->firstBindingPoint;
		for (mfmU16 i = 0; i < MFG_OGL4_SHADER_MAX_BP_COUNT && bp != NULL; ++i)
		{
			if (bp->type == MFG_CONSTANT_BUFFER)
			{
				GLchar buf[256];

				if (mfsCreateStringStream(&ss, buf, sizeof(buf), oglRD->stack) != MF_ERROR_OKAY)
					MFG_RETURN_ERROR(MFG_ERROR_INTERNAL, u8"Failed to create string stream for binding point name");

				if (mfsPutString(ss, u8"buf_") != MF_ERROR_OKAY)
				{
					mfsDestroyStringStream(ss);
					MFG_RETURN_ERROR(MFG_ERROR_INTERNAL, u8"mfsPutByte returned error");
				}
				if (mfsWrite(ss, bp->name, sizeof(bp->name), NULL) != MF_ERROR_OKAY)
				{
					mfsDestroyStringStream(ss);
					MFG_RETURN_ERROR(MFG_ERROR_INTERNAL, u8"mfsPutByte returned error");
				}
				if (mfsPutByte(ss, '\0') != MF_ERROR_OKAY)
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

				if (mfsCreateStringStream(&ss, buf, sizeof(buf), oglRD->stack) != MF_ERROR_OKAY)
					MFG_RETURN_ERROR(MFG_ERROR_INTERNAL, u8"Failed to create string stream for binding point name");

				if (mfsPutString(ss, u8"tex1d_") != MF_ERROR_OKAY)
				{
					mfsDestroyStringStream(ss);
					MFG_RETURN_ERROR(MFG_ERROR_INTERNAL, u8"mfsPutString returned error");
				}
				if (mfsPrintFormat(ss, u8"%d", bp->id) != MF_ERROR_OKAY)
				{
					mfsDestroyStringStream(ss);
					MFG_RETURN_ERROR(MFG_ERROR_INTERNAL, u8"mfsPrintFormatUTF8 returned error");
				}
				if (mfsPutByte(ss, '\0') != MF_ERROR_OKAY)
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

				if (mfsCreateStringStream(&ss, buf, sizeof(buf), oglRD->stack) != MF_ERROR_OKAY)
					MFG_RETURN_ERROR(MFG_ERROR_INTERNAL, u8"Failed to create string stream for binding point name");

				if (mfsPutString(ss, u8"tex2d_") != MF_ERROR_OKAY)
				{
					mfsDestroyStringStream(ss);
					MFG_RETURN_ERROR(MFG_ERROR_INTERNAL, u8"mfsPutString returned error");
				}
				if (mfsPrintFormat(ss, u8"%d", bp->id) != MF_ERROR_OKAY)
				{
					mfsDestroyStringStream(ss);
					MFG_RETURN_ERROR(MFG_ERROR_INTERNAL, u8"mfsPrintFormatUTF8 returned error");
				}
				if (mfsPutByte(ss, '\0') != MF_ERROR_OKAY)
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

				if (mfsCreateStringStream(&ss, buf, sizeof(buf), oglRD->stack) != MF_ERROR_OKAY)
					MFG_RETURN_ERROR(MFG_ERROR_INTERNAL, u8"Failed to create string stream for binding point name");

				if (mfsPutString(ss, u8"tex3d_") != MF_ERROR_OKAY)
				{
					mfsDestroyStringStream(ss);
					MFG_RETURN_ERROR(MFG_ERROR_INTERNAL, u8"mfsPutString returned error");
				}
				if (mfsPrintFormat(ss, u8"%d", bp->id) != MF_ERROR_OKAY)
				{
					mfsDestroyStringStream(ss);
					MFG_RETURN_ERROR(MFG_ERROR_INTERNAL, u8"mfsPrintFormatUTF8 returned error");
				}
				if (mfsPutByte(ss, '\0') != MF_ERROR_OKAY)
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

	err = mfmIncObjectRef(metaData);
	if (err != MF_ERROR_OKAY)
		MFG_RETURN_ERROR(err, u8"mfmIncObjectRef failed on meta data");

	*ps = oglPS;
	err = mfmIncObjectRef(rd);
	if (err != MF_ERROR_OKAY)
		MFG_RETURN_ERROR(err, u8"mfmIncObjectRef failed on render device");

	MFG_CHECK_GL_ERROR();
	return MF_ERROR_OKAY;
}

mfError mfgOGL4GetPixelShaderBindingPoint(mfgV2XRenderDevice* rd, mfgV2XBindingPoint** bp, mfgV2XPixelShader* ps, const mfsUTF8CodeUnit* name)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (rd == NULL || bp == NULL || ps == NULL || name == NULL) return MFG_ERROR_INVALID_ARGUMENTS;
#endif
	mfgOGL4Shader* oglPS = ps;

	for (mfmU64 i = 0; i < MFG_OGL4_SHADER_MAX_BP_COUNT; ++i)
		if (oglPS->bps[i].active == GL_TRUE)
		{
			if (strcmp(name, oglPS->bps[i].bp->name) == 0)
			{
				*bp = &oglPS->bps[i];
				return MF_ERROR_OKAY;
			}
		}

	return MFG_ERROR_NOT_FOUND;
}

mfError mfgOGL4BindConstantBuffer(mfgV2XRenderDevice* rd, mfgV2XBindingPoint* bp, mfgV2XConstantBuffer* cb)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (rd == NULL || bp == NULL) return MFG_ERROR_INVALID_ARGUMENTS;
#endif
	mfgOGL4BindingPoint* oglBP = bp;
	mfgOGL4ConstantBuffer* oglCB = cb;

	mfError err;
	if (oglBP->boundObject != NULL)
	{
		err = mfmDecObjectRef(oglBP->boundObject);
		if (err != MF_ERROR_OKAY)
			return err;
	}
	oglBP->boundObject = oglCB;

	if (oglCB == NULL)
		glBindBufferBase(GL_UNIFORM_BUFFER, oglBP->location, 0);
	else
	{
		err = mfmIncObjectRef(oglBP->boundObject);
		if (err != MF_ERROR_OKAY)
			return err;
		glBindBufferBase(GL_UNIFORM_BUFFER, oglBP->location, oglCB->cb);
	}

	MFG_CHECK_GL_ERROR();
	return MF_ERROR_OKAY;
}

mfError mfgOGL4BindConstantBufferRange(mfgV2XRenderDevice* rd, mfgV2XBindingPoint* bp, mfgV2XConstantBuffer* cb, mfmU64 offset, mfmU64 size)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (rd == NULL || bp == NULL || cb == NULL) return MFG_ERROR_INVALID_ARGUMENTS;
#endif
	mfgOGL4BindingPoint* oglBP = bp;
	mfgOGL4ConstantBuffer* oglCB = cb;

	mfError err;
	if (oglBP->boundObject != NULL)
	{
		err = mfmDecObjectRef(oglBP->boundObject);
		if (err != MF_ERROR_OKAY)
			return err;
	}
	oglBP->boundObject = oglCB;
	err = mfmIncObjectRef(oglBP->boundObject);
	if (err != MF_ERROR_OKAY)
		return err;

	glBindBufferRange(GL_UNIFORM_BUFFER, oglBP->location, oglCB->cb, offset * 16, size * 16);

	MFG_CHECK_GL_ERROR();
	return MF_ERROR_OKAY;
}

mfError mfgOGL4BindTexture1D(mfgV2XRenderDevice* rd, mfgV2XBindingPoint* bp, mfgV2XTexture1D* tex)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (rd == NULL || bp == NULL) return MFG_ERROR_INVALID_ARGUMENTS;
#endif
	mfgOGL4BindingPoint* oglBP = bp;
	mfgOGL4Texture1D* oglT = tex;

	mfError err;
	if (oglBP->boundObject != NULL)
	{
		err = mfmDecObjectRef(oglBP->boundObject);
		if (err != MF_ERROR_OKAY)
			return err;
	}
	oglBP->boundObject = oglT;

	glActiveTexture(GL_TEXTURE0 + oglBP->location);
	if (oglT == NULL)
		glBindTexture(GL_TEXTURE_1D, 0);
	else
	{
		glBindTexture(GL_TEXTURE_1D, oglT->tex);
		err = mfmIncObjectRef(oglBP->boundObject);
		if (err != MF_ERROR_OKAY)
			return err;
	}
	glProgramUniform1i(oglBP->shader->program, oglBP->location, oglBP->location);

	MFG_CHECK_GL_ERROR();
	return MF_ERROR_OKAY;
}

mfError mfgOGL4BindTexture2D(mfgV2XRenderDevice* rd, mfgV2XBindingPoint* bp, mfgV2XTexture2D* tex)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (rd == NULL || bp == NULL) return MFG_ERROR_INVALID_ARGUMENTS;
#endif
	mfgOGL4BindingPoint* oglBP = bp;
	mfgOGL4Texture2D* oglT = tex;

	mfError err;
	if (oglBP->boundObject != NULL)
	{
		err = mfmDecObjectRef(oglBP->boundObject);
		if (err != MF_ERROR_OKAY)
			return err;
	}
	oglBP->boundObject = oglT;

	glActiveTexture(GL_TEXTURE0 + oglBP->location);
	if (oglT == NULL)
		glBindTexture(GL_TEXTURE_2D, 0);
	else
	{
		err = mfmIncObjectRef(oglBP->boundObject);
		if (err != MF_ERROR_OKAY)
			return err;
		glBindTexture(GL_TEXTURE_2D, oglT->tex);
	}
	glProgramUniform1i(oglBP->shader->program, oglBP->location, oglBP->location);

	MFG_CHECK_GL_ERROR();
	return MF_ERROR_OKAY;
}

mfError mfgOGL4BindTexture3D(mfgV2XRenderDevice* rd, mfgV2XBindingPoint* bp, mfgV2XTexture3D* tex)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (rd == NULL || bp == NULL) return MFG_ERROR_INVALID_ARGUMENTS;
#endif
	mfgOGL4BindingPoint* oglBP = bp;
	mfgOGL4Texture3D* oglT = tex;

	mfError err;
	if (oglBP->boundObject != NULL)
	{
		err = mfmDecObjectRef(oglBP->boundObject);
		if (err != MF_ERROR_OKAY)
			return err;
	}
	oglBP->boundObject = oglT;

	glActiveTexture(GL_TEXTURE0 + oglBP->location);
	if (oglT == NULL)
		glBindTexture(GL_TEXTURE_3D, 0);
	else
	{
		err = mfmIncObjectRef(oglBP->boundObject);
		if (err != MF_ERROR_OKAY)
			return err;
		glBindTexture(GL_TEXTURE_3D, oglT->tex);
	}
	glProgramUniform1i(oglBP->shader->program, oglBP->location, oglBP->location);

	MFG_CHECK_GL_ERROR();
	return MF_ERROR_OKAY;
}

mfError mfgOGL4BindRenderTexture(mfgV2XRenderDevice* rd, mfgV2XBindingPoint* bp, mfgV2XRenderTexture* tex)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (rd == NULL || bp == NULL) return MFG_ERROR_INVALID_ARGUMENTS;
#endif
	mfgOGL4BindingPoint* oglBP = bp;
	mfgOGL4RenderTexture* oglT = tex;

	mfError err;
	if (oglBP->boundObject != NULL)
	{
		err = mfmDecObjectRef(oglBP->boundObject);
		if (err != MF_ERROR_OKAY)
			return err;
	}
	oglBP->boundObject = oglT;

	glActiveTexture(GL_TEXTURE0 + oglBP->location);
	if (oglT == NULL)
		glBindTexture(GL_TEXTURE_2D, 0);
	else
	{
		glBindTexture(GL_TEXTURE_2D, oglT->tex);
		err = mfmIncObjectRef(oglBP->boundObject);
		if (err != MF_ERROR_OKAY)
			return err;
	}

	glProgramUniform1i(oglBP->shader->program, oglBP->location, oglBP->location);

	MFG_CHECK_GL_ERROR();
	return MF_ERROR_OKAY;
}

mfError mfgOGL4BindSampler(mfgV2XRenderDevice* rd, mfgV2XBindingPoint* bp, mfgV2XSampler* sampler)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (rd == NULL || bp == NULL) return MFG_ERROR_INVALID_ARGUMENTS;
#endif
	mfgOGL4BindingPoint* oglBP = bp;
	mfgOGL4Sampler* oglS = sampler;

	mfError err;
	if (oglBP->boundObject != NULL)
	{
		err = mfmDecObjectRef(oglBP->boundObject);
		if (err != MF_ERROR_OKAY)
			return err;
	}
	oglBP->boundObject = oglS;
	
	if (oglS == NULL)
		glBindSampler(oglBP->location, 0);
	else
	{
		err = mfmIncObjectRef(oglBP->boundObject);
		if (err != MF_ERROR_OKAY)
			return err;
		glBindSampler(oglBP->location, oglS->sampler);
	}

	MFG_CHECK_GL_ERROR();
	return MF_ERROR_OKAY;
}

void mfgOGL4DestroyPipeline(void* pp)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (pp == NULL) abort();
#endif
	mfgOGL4Pipeline* oglPP = pp;
	glDeleteProgramPipelines(1, &oglPP->pipeline);

	if (mfmDecObjectRef(oglPP->vs) != MF_ERROR_OKAY)
		abort();
	if (mfmDecObjectRef(oglPP->ps) != MF_ERROR_OKAY)
		abort();
	if (mfmDecObjectRef(oglPP->base.renderDevice) != MF_ERROR_OKAY)
		abort();
	if (mfmDestroyObject(&oglPP->base.object) != MF_ERROR_OKAY)
		abort();
	if (mfmDeallocate(((mfgOGL4RenderDevice*)oglPP->base.renderDevice)->pool48, oglPP) != MF_ERROR_OKAY)
		abort();
#ifdef MAGMA_FRAMEWORK_DEBUG
	GLenum err = glGetError();
	if (err != 0)
		abort();
#endif
}

mfError mfgOGL4CreatePipeline(mfgV2XRenderDevice* rd, mfgV2XPipeline** pp, mfgV2XVertexShader* vs, mfgV2XPixelShader* ps)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{ if (rd == NULL || pp == NULL || vs == NULL || ps == NULL) return MFG_ERROR_INVALID_ARGUMENTS; }
#endif

	mfgOGL4RenderDevice* oglRD = (mfgOGL4RenderDevice*)rd;

	// Allocate pipeline
	mfgOGL4Pipeline* oglPP = NULL;
	if (mfmAllocate(oglRD->pool48, &oglPP, sizeof(mfgOGL4Pipeline)) != MF_ERROR_OKAY)
		MFG_RETURN_ERROR(MFG_ERROR_ALLOCATION_FAILED, u8"Failed to allocate pipeline on pool");

	// Init object
	{
		mfError err = mfmInitObject(&oglPP->base.object);
		if (err != MF_ERROR_OKAY)
			return err;
	}
	oglPP->base.object.destructorFunc = &mfgOGL4DestroyPipeline;
	oglPP->base.renderDevice = rd;

	// Create shader pipeline
	glGenProgramPipelines(1, &oglPP->pipeline);
	glUseProgramStages(oglPP->pipeline, GL_VERTEX_SHADER_BIT, ((mfgOGL4Shader*)vs)->program);
	glUseProgramStages(oglPP->pipeline, GL_FRAGMENT_SHADER_BIT, ((mfgOGL4Shader*)ps)->program);

	oglPP->vs = vs;
	oglPP->ps = ps;
	{
		mfError err = mfmIncObjectRef(vs);
		if (err != MF_ERROR_OKAY)
			return err;
		err = mfmIncObjectRef(ps);
		if (err != MF_ERROR_OKAY)
			return err;
	}

	*pp = oglPP;
	mfError err = mfmIncObjectRef(rd);
	if (err != MF_ERROR_OKAY)
		MFG_RETURN_ERROR(err, u8"mfmIncObjectRef failed on render device");

	MFG_CHECK_GL_ERROR();
	return MF_ERROR_OKAY;
}

mfError mfgOGL4SetPipeline(mfgV2XRenderDevice* rd, mfgV2XPipeline* pp)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{ if (rd == NULL) return MFG_ERROR_INVALID_ARGUMENTS; }
#endif

	mfgOGL4RenderDevice* oglRD = (mfgOGL4RenderDevice*)rd;

	mfError err;
	if (oglRD->currentPipeline != NULL)
	{
		err = mfmDecObjectRef(oglRD->currentPipeline);
		if (err != MF_ERROR_OKAY)
			return err;
	}
	oglRD->currentPipeline = pp;

	// Set pipeline
	mfgOGL4Pipeline* oglPP = pp;
	if (oglPP == NULL)
		glBindProgramPipeline(0);
	else
	{
		glBindProgramPipeline(oglPP->pipeline);
		err = mfmIncObjectRef(pp);
		if (err != MF_ERROR_OKAY)
			return err;
	}

	MFG_CHECK_GL_ERROR();
	return MF_ERROR_OKAY;
}

void mfgOGL4DestroyConstantBuffer(void* buffer)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (buffer == NULL) abort();
#endif
	mfgOGL4ConstantBuffer* oglCB = buffer;
	glDeleteBuffers(1, &oglCB->cb);
	if (mfmDecObjectRef(oglCB->base.renderDevice) != MF_ERROR_OKAY)
		abort();
	if (mfmDestroyObject(&oglCB->base.object) != MF_ERROR_OKAY)
		abort();
	if (mfmDeallocate(((mfgOGL4RenderDevice*)oglCB->base.renderDevice)->pool48, oglCB) != MF_ERROR_OKAY)
		abort();
#ifdef MAGMA_FRAMEWORK_DEBUG
	GLenum err = glGetError();
	if (err != 0)
		abort();
#endif
}

mfError mfgOGL4CreateConstantBuffer(mfgV2XRenderDevice* rd, mfgV2XConstantBuffer** cb, mfmU64 size, const void* data, mfgEnum usage)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{ if (rd == NULL || cb == NULL) return MFG_ERROR_INVALID_ARGUMENTS; }
#endif

	mfgOGL4RenderDevice* oglRD = (mfgOGL4RenderDevice*)rd;

	// Allocate constant buffer
	mfgOGL4ConstantBuffer* oglCB = NULL;
	if (mfmAllocate(oglRD->pool48, &oglCB, sizeof(mfgOGL4ConstantBuffer)) != MF_ERROR_OKAY)
		MFG_RETURN_ERROR(MFG_ERROR_ALLOCATION_FAILED, u8"Failed to allocate constant buffer on pool");

	// Init object
	{
		mfError err = mfmInitObject(&oglCB->base.object);
		if (err != MF_ERROR_OKAY)
			return err;
	}
	oglCB->base.object.destructorFunc = &mfgOGL4DestroyConstantBuffer;
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
	mfError err = mfmIncObjectRef(rd);
	if (err != MF_ERROR_OKAY)
		MFG_RETURN_ERROR(err, u8"mfmIncObjectRef failed on render device");

	MFG_CHECK_GL_ERROR();
	return MF_ERROR_OKAY;
}

mfError mfgOGL4MapConstantBuffer(mfgV2XRenderDevice* rd, mfgV2XConstantBuffer* cb, void** memory)
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
	return MF_ERROR_OKAY;
}

mfError mfgOGL4UnmapConstantBuffer(mfgV2XRenderDevice* rd, mfgV2XConstantBuffer* cb)
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
	return MF_ERROR_OKAY;
}


void mfgOGL4DestroyVertexBuffer(void* buffer)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (buffer == NULL) abort();
#endif
	mfgOGL4VertexBuffer* oglVB = buffer;
	glDeleteBuffers(1, &oglVB->vb);
	if (mfmDecObjectRef(oglVB->base.renderDevice) != MF_ERROR_OKAY)
		abort();
	if (mfmDestroyObject(&oglVB->base.object) != MF_ERROR_OKAY)
		abort();
	if (mfmDeallocate(((mfgOGL4RenderDevice*)oglVB->base.renderDevice)->pool48, oglVB) != MF_ERROR_OKAY)
		abort();
#ifdef MAGMA_FRAMEWORK_DEBUG
	GLenum err = glGetError();
	if (err != 0)
		abort();
#endif
}

mfError mfgOGL4CreateVertexBuffer(mfgV2XRenderDevice* rd, mfgV2XVertexBuffer** vb, mfmU64 size, const void* data, mfgEnum usage)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{
		if (rd == NULL || vb == NULL) return MFG_ERROR_INVALID_ARGUMENTS;
	}
#endif

	mfgOGL4RenderDevice* oglRD = (mfgOGL4RenderDevice*)rd;

	// Allocate vertex buffer
	mfgOGL4VertexBuffer* oglVB = NULL;
	if (mfmAllocate(oglRD->pool48, &oglVB, sizeof(mfgOGL4VertexBuffer)) != MF_ERROR_OKAY)
		MFG_RETURN_ERROR(MFG_ERROR_ALLOCATION_FAILED, u8"Failed to allocate vertex buffer on pool");

	// Init object
	{
		mfError err = mfmInitObject(&oglVB->base.object);
		if (err != MF_ERROR_OKAY)
			return err;
	}
	oglVB->base.object.destructorFunc = &mfgOGL4DestroyVertexBuffer;
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
	mfError err = mfmIncObjectRef(rd);
	if (err != MF_ERROR_OKAY)
		MFG_RETURN_ERROR(err, u8"mfmIncObjectRef failed on render device");

	MFG_CHECK_GL_ERROR();
	return MF_ERROR_OKAY;
}


mfError mfgOGL4MapVertexBuffer(mfgV2XRenderDevice* rd, mfgV2XVertexBuffer* vb, void** memory)
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
	return MF_ERROR_OKAY;
}

mfError mfgOGL4UnmapVertexBuffer(mfgV2XRenderDevice* rd, mfgV2XVertexBuffer* vb)
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
	return MF_ERROR_OKAY;
}

void mfgOGL4DestroyIndexBuffer(void* buffer)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (buffer == NULL) abort();
#endif
	mfgOGL4IndexBuffer* oglIB = buffer;
	glDeleteBuffers(1, &oglIB->ib);
	if (mfmDecObjectRef(oglIB->base.renderDevice) != MF_ERROR_OKAY)
		abort();
	if (mfmDestroyObject(&oglIB->base.object) != MF_ERROR_OKAY)
		abort();
	if (mfmDeallocate(((mfgOGL4RenderDevice*)oglIB->base.renderDevice)->pool48, oglIB) != MF_ERROR_OKAY)
		abort();
#ifdef MAGMA_FRAMEWORK_DEBUG
	GLenum err = glGetError();
	if (err != 0)
		abort();
#endif
}

mfError mfgOGL4CreateIndexBuffer(mfgV2XRenderDevice* rd, mfgV2XIndexBuffer** ib, mfmU64 size, const void* data, mfgEnum format, mfgEnum usage)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{
		if (rd == NULL || ib == NULL) return MFG_ERROR_INVALID_ARGUMENTS;
	}
#endif

	mfgOGL4RenderDevice* oglRD = (mfgOGL4RenderDevice*)rd;

	// Allocate index buffer
	mfgOGL4IndexBuffer* oglIB = NULL;
	if (mfmAllocate(oglRD->pool48, &oglIB, sizeof(mfgOGL4IndexBuffer)) != MF_ERROR_OKAY)
		MFG_RETURN_ERROR(MFG_ERROR_ALLOCATION_FAILED, u8"Failed to allocate index buffer on pool");

	// Init object
	{
		mfError err = mfmInitObject(&oglIB->base.object);
		if (err != MF_ERROR_OKAY)
			return err;
	}
	oglIB->base.object.destructorFunc = &mfgOGL4DestroyIndexBuffer;
	oglIB->base.renderDevice = rd;

	// Create index buffer
	GLenum gl_usage;

	switch (format)
	{
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
	mfError err = mfmIncObjectRef(rd);
	if (err != MF_ERROR_OKAY)
		MFG_RETURN_ERROR(err, u8"mfmIncObjectRef failed on render device");

	MFG_CHECK_GL_ERROR();
	return MF_ERROR_OKAY;
}


mfError mfgOGL4MapIndexBuffer(mfgV2XRenderDevice* rd, mfgV2XIndexBuffer* ib, void** memory)
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
	return MF_ERROR_OKAY;
}

mfError mfgOGL4UnmapIndexBuffer(mfgV2XRenderDevice* rd, mfgV2XIndexBuffer* ib)
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
	return MF_ERROR_OKAY;
}

mfError mfgOGL4SetIndexBuffer(mfgV2XRenderDevice* rd, mfgV2XIndexBuffer* ib)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{
		if (rd == NULL) return MFG_ERROR_INVALID_ARGUMENTS;
	}
#endif

	mfgOGL4RenderDevice* oglRD = (mfgOGL4RenderDevice*)rd;

	mfError err;
	if (oglRD->currentIndexBuffer != NULL)
	{
		err = mfmDecObjectRef(oglRD->currentIndexBuffer);
		if (err != MF_ERROR_OKAY)
			return err;
	}
	oglRD->currentIndexBuffer = ib;

	// Set index buffer
	mfgOGL4IndexBuffer* oglIB = ib;
	if (oglIB == NULL)
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	else
	{
		err = mfmIncObjectRef(oglRD->currentIndexBuffer);
		if (err != MF_ERROR_OKAY)
			return err;
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, oglIB->ib);
	}

	oglRD->currentIndexBuffer = oglIB;

	MFG_CHECK_GL_ERROR();
	return MF_ERROR_OKAY;
}

void mfgOGL4DestroyVertexLayout(void* vl)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (vl == NULL) abort();
#endif
	mfgOGL4VertexLayout* oglVL = vl;
	if (mfmDecObjectRef(oglVL->base.renderDevice) != MF_ERROR_OKAY)
		abort();
	if (mfmDestroyObject(&oglVL->base.object) != MF_ERROR_OKAY)
		abort();
	if (mfmDeallocate(((mfgOGL4RenderDevice*)oglVL->base.renderDevice)->pool512, oglVL) != MF_ERROR_OKAY)
		abort();
#ifdef MAGMA_FRAMEWORK_DEBUG
	GLenum err = glGetError();
	if (err != 0)
		abort();
#endif
}

mfError mfgOGL4CreateVertexLayout(mfgV2XRenderDevice* rd, mfgV2XVertexLayout** vl, mfmU64 elementCount, const mfgV2XVertexElement* elements, mfgV2XVertexShader* vs)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{ if (rd == NULL || vl == NULL || elementCount == 0 || elementCount > MFG_OGL4_SHADER_MAX_ELEMENT_COUNT || vs == NULL || elements == NULL) return MFG_ERROR_INVALID_ARGUMENTS; }
#endif

	mfgOGL4RenderDevice* oglRD = (mfgOGL4RenderDevice*)rd;

	// Allocate vertex layout
	mfgOGL4VertexLayout* oglVL = NULL;
	if (mfmAllocate(oglRD->pool512, &oglVL, sizeof(mfgOGL4VertexLayout)) != MF_ERROR_OKAY)
		MFG_RETURN_ERROR(MFG_ERROR_ALLOCATION_FAILED, u8"Failed to allocate vertex layout on pool");

	// Init object
	{
		mfError err = mfmInitObject(&oglVL->base.object);
		if (err != MF_ERROR_OKAY)
			return err;
	}
	oglVL->base.object.destructorFunc = &mfgOGL4DestroyVertexLayout;
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
	mfError err = mfmIncObjectRef(rd);
	if (err != MF_ERROR_OKAY)
		MFG_RETURN_ERROR(err, u8"mfmIncObjectRef failed on render device");

	MFG_CHECK_GL_ERROR();
	return MF_ERROR_OKAY;
}

void mfgOGL4DestroyVertexArray(void* va)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (va == NULL) abort();
#endif
	mfgOGL4VertexArray* oglVA = va;
	glDeleteVertexArrays(1, &oglVA->va);
	if (mfmDecObjectRef(oglVA->vl) != MF_ERROR_OKAY)
		abort();
	for (mfmU64 i = 0; i < 16; ++i)
	{
		if (oglVA->vb[i] == NULL)
			continue;
		if (mfmDecObjectRef(oglVA->vb[i]) != MF_ERROR_OKAY)
			abort();
	}
	if (mfmDecObjectRef(oglVA->base.renderDevice) != MF_ERROR_OKAY)
		abort();
	if (mfmDestroyObject(&oglVA->base.object) != MF_ERROR_OKAY)
		abort();
	if (mfmDeallocate(((mfgOGL4RenderDevice*)oglVA->base.renderDevice)->pool256, oglVA) != MF_ERROR_OKAY)
		abort();
#ifdef MAGMA_FRAMEWORK_DEBUG
	GLenum err = glGetError();
	if (err != 0)
		abort();
#endif
}

mfError mfgOGL4CreateVertexArray(mfgV2XRenderDevice* rd, mfgV2XVertexArray** va, mfmU64 bufferCount, mfgV2XVertexBuffer** buffers, mfgV2XVertexLayout* vl)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{ if (rd == NULL || va == NULL || bufferCount == 0 || vl == NULL || buffers == NULL) return MFG_ERROR_INVALID_ARGUMENTS; }
#endif

	mfgOGL4RenderDevice* oglRD = (mfgOGL4RenderDevice*)rd;

	// Allocate vertex array
	mfgOGL4VertexArray* oglVA = NULL;
	if (mfmAllocate(oglRD->pool256, &oglVA, sizeof(mfgOGL4VertexArray)) != MF_ERROR_OKAY)
		MFG_RETURN_ERROR(MFG_ERROR_ALLOCATION_FAILED, u8"Failed to allocate vertex array on pool");
	mfgOGL4VertexLayout* oglVL = vl;

	// Init object
	{
		mfError err = mfmInitObject(&oglVA->base.object);
		if (err != MF_ERROR_OKAY)
			return err;
	}
	oglVA->base.object.destructorFunc = &mfgOGL4DestroyVertexArray;
	oglVA->base.renderDevice = rd;

	// Create vertex array
	glGenVertexArrays(1, &oglVA->va);
	glBindVertexArray(oglVA->va);

	for (mfmU64 i = 0; i < 16; ++i)
		oglVA->vb[i] = NULL;
	oglVA->vl = vl;

	mfError err;
	err = mfmIncObjectRef(oglVA->vl);
	if (err  != MF_ERROR_OKAY)
		return err;

	for (mfmU64 i = 0; i < bufferCount; ++i)
	{
		mfgOGL4VertexBuffer* buffer = buffers[i];

		oglVA->vb[i] = buffer;
		err = mfmIncObjectRef(oglVA->vb[i]);
		if (err != MF_ERROR_OKAY)
			return err;

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
	err = mfmIncObjectRef(rd);
	if (err != MF_ERROR_OKAY)
		MFG_RETURN_ERROR(err, u8"mfmIncObjectRef failed on render device");

	MFG_CHECK_GL_ERROR();
	return MF_ERROR_OKAY;
}

mfError mfgOGL4SetVertexArray(mfgV2XRenderDevice* rd, mfgV2XVertexArray* va)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{ if (rd == NULL) return MFG_ERROR_INVALID_ARGUMENTS; }
#endif

	mfgOGL4RenderDevice* oglRD = (mfgOGL4RenderDevice*)rd;

	mfError err;
	if (oglRD->currentVertexArray != NULL)
	{
		err = mfmDecObjectRef(oglRD->currentVertexArray);
		if (err != MF_ERROR_OKAY)
			return err;
	}
	oglRD->currentVertexArray = va;

	// Set vertex array as active
	if (va == NULL)
		glBindVertexArray(0);
	else
	{
		err = mfmIncObjectRef(oglRD->currentVertexArray);
		if (err != MF_ERROR_OKAY)
			return err;
		mfgOGL4VertexArray* oglVA = va;
		glBindVertexArray(oglVA->va);
	}

	MFG_CHECK_GL_ERROR();
	return MF_ERROR_OKAY;
}


void mfgOGL4DestroyTexture1D(void* tex)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (tex == NULL) abort();
#endif
	mfgOGL4Texture1D* oglTex = tex;
	glDeleteTextures(1, &oglTex->tex);
	if (mfmDestroyObject(&oglTex->base.object) != MF_ERROR_OKAY)
		abort();
	if (mfmDeallocate(((mfgOGL4RenderDevice*)oglTex->base.renderDevice)->pool64, oglTex) != MF_ERROR_OKAY)
		abort();
#ifdef MAGMA_FRAMEWORK_DEBUG
	GLenum err = glGetError();
	if (err != 0)
		abort();
#endif
}

mfError mfgOGL4CreateTexture1D(mfgV2XRenderDevice* rd, mfgV2XTexture1D** tex, mfmU64 width, mfgEnum format, const void* data, mfgEnum usage)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{ if (rd == NULL || tex == NULL) return MFG_ERROR_INVALID_ARGUMENTS; }
#endif

	mfgOGL4RenderDevice* oglRD = (mfgOGL4RenderDevice*)rd;

	// Allocate texture
	mfgOGL4Texture1D* oglTex = NULL;
	if (mfmAllocate(oglRD->pool64, &oglTex, sizeof(mfgOGL4Texture1D)) != MF_ERROR_OKAY)
		MFG_RETURN_ERROR(MFG_ERROR_ALLOCATION_FAILED, u8"Failed to allocate texture 1D on pool");

	// Init object
	{
		mfError err = mfmInitObject(&oglTex->base.object);
		if (err != MF_ERROR_OKAY)
			return err;
	}
	oglTex->base.object.destructorFunc = &mfgOGL4DestroyTexture1D;
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

	oglTex->width = width;

	*tex = oglTex;
	mfError err = mfmIncObjectRef(rd);
	if (err != MF_ERROR_OKAY)
		MFG_RETURN_ERROR(err, u8"mfmIncObjectRef failed on render device");

	MFG_CHECK_GL_ERROR();
	return MF_ERROR_OKAY;
}

mfError mfgOGL4UpdateTexture1D(mfgV2XRenderDevice* rd, mfgV2XTexture1D* tex, mfmU64 dstX, mfmU64 width, const void* data)
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
	return MF_ERROR_OKAY;
}

mfError mfgOGL4GenerateTexture1DMipmaps(mfgV2XRenderDevice* rd, mfgV2XTexture1D* tex)
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
	return MF_ERROR_OKAY;
}

void mfgOGL4DestroyTexture2D(void* tex)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (tex == NULL) abort();
#endif
	mfgOGL4Texture2D* oglTex = tex;
	glDeleteTextures(1, &oglTex->tex);
	if (mfmDecObjectRef(oglTex->base.renderDevice) != MF_ERROR_OKAY)
		abort();
	if (mfmDestroyObject(&oglTex->base.object) != MF_ERROR_OKAY)
		abort();
	if (mfmDeallocate(((mfgOGL4RenderDevice*)oglTex->base.renderDevice)->pool64, oglTex) != MF_ERROR_OKAY)
		abort();
#ifdef MAGMA_FRAMEWORK_DEBUG
	GLenum err = glGetError();
	if (err != 0)
		abort();
#endif
}

mfError mfgOGL4CreateTexture2D(mfgV2XRenderDevice* rd, mfgV2XTexture2D** tex, mfmU64 width, mfmU64 height, mfgEnum format, const void* data, mfgEnum usage)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{ if (rd == NULL || tex == NULL) return MFG_ERROR_INVALID_ARGUMENTS; }
#endif

	mfgOGL4RenderDevice* oglRD = (mfgOGL4RenderDevice*)rd;

	// Allocate texture
	mfgOGL4Texture2D* oglTex = NULL;
	if (mfmAllocate(oglRD->pool64, &oglTex, sizeof(mfgOGL4Texture2D)) != MF_ERROR_OKAY)
		MFG_RETURN_ERROR(MFG_ERROR_ALLOCATION_FAILED, u8"Failed to allocate texture 2D on pool");

	// Init object
	{
		mfError err = mfmInitObject(&oglTex->base.object);
		if (err != MF_ERROR_OKAY)
			return err;
	}
	oglTex->base.object.destructorFunc = &mfgOGL4DestroyTexture2D;
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

	oglTex->width = width;
	oglTex->height = height;

	*tex = oglTex;
	mfError err = mfmIncObjectRef(rd);
	if (err != MF_ERROR_OKAY)
		MFG_RETURN_ERROR(err, u8"mfmIncObjectRef failed on render device");

	MFG_CHECK_GL_ERROR();
	return MF_ERROR_OKAY;
}

mfError mfgOGL4UpdateTexture2D(mfgV2XRenderDevice* rd, mfgV2XTexture2D* tex, mfmU64 dstX, mfmU64 dstY, mfmU64 width, mfmU64 height, const void* data)
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
	return MF_ERROR_OKAY;
}

mfError mfgOGL4GenerateTexture2DMipmaps(mfgV2XRenderDevice* rd, mfgV2XTexture2D* tex)
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
	return MF_ERROR_OKAY;
}

void mfgOGL4DestroyTexture3D(void* tex)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (tex == NULL) abort();
#endif
	mfgOGL4Texture3D* oglTex = tex;
	glDeleteTextures(1, &oglTex->tex);
	if (mfmDecObjectRef(oglTex->base.renderDevice) != MF_ERROR_OKAY)
		abort();
	if (mfmDestroyObject(&oglTex->base.object) != MF_ERROR_OKAY)
		abort();
	if (mfmDeallocate(((mfgOGL4RenderDevice*)oglTex->base.renderDevice)->pool64, oglTex) != MF_ERROR_OKAY)
		abort();
#ifdef MAGMA_FRAMEWORK_DEBUG
	GLenum err = glGetError();
	if (err != 0)
		abort();
#endif
}

mfError mfgOGL4CreateTexture3D(mfgV2XRenderDevice* rd, mfgV2XTexture3D** tex, mfmU64 width, mfmU64 height, mfmU64 depth, mfgEnum format, const void* data, mfgEnum usage)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{ if (rd == NULL || tex == NULL) return MFG_ERROR_INVALID_ARGUMENTS; }
#endif

	mfgOGL4RenderDevice* oglRD = (mfgOGL4RenderDevice*)rd;

	// Allocate texture
	mfgOGL4Texture3D* oglTex = NULL;
	if (mfmAllocate(oglRD->pool64, &oglTex, sizeof(mfgOGL4Texture3D)) != MF_ERROR_OKAY)
		MFG_RETURN_ERROR(MFG_ERROR_ALLOCATION_FAILED, u8"Failed to allocate texture 3D on pool");

	// Init object
	{
		mfError err = mfmInitObject(&oglTex->base.object);
		if (err != MF_ERROR_OKAY)
			return err;
	}
	oglTex->base.object.destructorFunc = &mfgOGL4DestroyTexture3D;
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
	mfError err = mfmIncObjectRef(rd);
	if (err != MF_ERROR_OKAY)
		MFG_RETURN_ERROR(err, u8"mfmIncObjectRef failed on render device");

	oglTex->width = width;
	oglTex->height = height;
	oglTex->depth = depth;

	MFG_CHECK_GL_ERROR();
	return MF_ERROR_OKAY;
}

mfError mfgOGL4UpdateTexture3D(mfgV2XRenderDevice* rd, mfgV2XTexture3D* tex, mfmU64 dstX, mfmU64 dstY, mfmU64 dstZ, mfmU64 width, mfmU64 height, mfmU64 depth, const void* data)
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
	return MF_ERROR_OKAY;
}

mfError mfgOGL4GenerateTexture3DMipmaps(mfgV2XRenderDevice* rd, mfgV2XTexture3D* tex)
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
	return MF_ERROR_OKAY;
}

void mfgOGL4DestroySampler(void* sampler)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (sampler == NULL) abort();
#endif
	mfgOGL4Sampler* oglS = sampler;
	glDeleteSamplers(1, &oglS->sampler);
	if (mfmDecObjectRef(oglS->base.renderDevice) != MF_ERROR_OKAY)
		abort();
	if (mfmDestroyObject(&oglS->base.object) != MF_ERROR_OKAY)
		abort();
	if (mfmDeallocate(((mfgOGL4RenderDevice*)oglS->base.renderDevice)->pool48, oglS) != MF_ERROR_OKAY)
		abort();
#ifdef MAGMA_FRAMEWORK_DEBUG
	GLenum err = glGetError();
	if (err != 0)
		abort();
#endif
}

mfError mfgOGL4CreateSampler(mfgV2XRenderDevice* rd, mfgV2XSampler** sampler, const mfgV2XSamplerDesc* desc)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{ if (rd == NULL || sampler == NULL || desc == NULL) return MFG_ERROR_INVALID_ARGUMENTS; }
#endif

	mfgOGL4RenderDevice* oglRD = (mfgOGL4RenderDevice*)rd;

	// Allocate sampler
	mfgOGL4Sampler* oglS = NULL;
	if (mfmAllocate(oglRD->pool48, &oglS, sizeof(mfgOGL4Sampler)) != MF_ERROR_OKAY)
		MFG_RETURN_ERROR(MFG_ERROR_ALLOCATION_FAILED, u8"Failed to allocate sampler on pool");

	// Init object
	{
		mfError err = mfmInitObject(&oglS->base.object);
		if (err != MF_ERROR_OKAY)
			return err;
	}
	oglS->base.object.destructorFunc = &mfgOGL4DestroySampler;
	oglS->base.renderDevice = rd;

	// Create sampler
	GLenum minFilter, magFilter;
	GLenum wrapS, wrapT, wrapR;

	switch (desc->addressU)
	{
		case MFG_REPEAT: wrapS = GL_REPEAT; break;
		case MFG_MIRROR: wrapS = GL_MIRRORED_REPEAT; break;
		case MFG_CLAMP: wrapS = GL_CLAMP_TO_EDGE; break;
		case MFG_BORDER: wrapS = GL_CLAMP_TO_BORDER; break;
		default: MFG_RETURN_ERROR(MFG_ERROR_INVALID_ARGUMENTS, u8"Unsupported texture U adress mode");
	}

	switch (desc->addressV)
	{
		case MFG_REPEAT: wrapT = GL_REPEAT; break;
		case MFG_MIRROR: wrapT = GL_MIRRORED_REPEAT; break;
		case MFG_CLAMP: wrapT = GL_CLAMP_TO_EDGE; break;
		case MFG_BORDER: wrapT = GL_CLAMP_TO_BORDER; break;
		default: MFG_RETURN_ERROR(MFG_ERROR_INVALID_ARGUMENTS, u8"Unsupported texture V adress mode");
	}

	switch (desc->addressW)
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
	mfError err = mfmIncObjectRef(rd);
	if (err != MF_ERROR_OKAY)
		MFG_RETURN_ERROR(err, u8"mfmIncObjectRef failed on render device");

	MFG_CHECK_GL_ERROR();
	return MF_ERROR_OKAY;
}

void mfgOGL4DestroyRenderTexture(void* tex)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (tex == NULL) abort();
#endif
	mfgOGL4RenderTexture* oglTex = tex;
	glDeleteTextures(1, &oglTex->tex);
	if (mfmDecObjectRef(oglTex->base.renderDevice) != MF_ERROR_OKAY)
		abort();
	if (mfmDestroyObject(&oglTex->base.object) != MF_ERROR_OKAY)
		abort();
	if (mfmDeallocate(((mfgOGL4RenderDevice*)oglTex->base.renderDevice)->pool64, oglTex) != MF_ERROR_OKAY)
		abort();
#ifdef MAGMA_FRAMEWORK_DEBUG
	GLenum err = glGetError();
	if (err != 0)
		abort();
#endif
}

mfError mfgOGL4CreateRenderTexture(mfgV2XRenderDevice* rd, mfgV2XRenderTexture** tex, mfmU64 width, mfmU64 height, mfgEnum format)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{ if (rd == NULL || tex == NULL) return MFG_ERROR_INVALID_ARGUMENTS; }
#endif

	mfgOGL4RenderDevice* oglRD = (mfgOGL4RenderDevice*)rd;

	// Allocate texture
	mfgOGL4RenderTexture* oglTex = NULL;
	if (mfmAllocate(oglRD->pool64, &oglTex, sizeof(mfgOGL4RenderTexture)) != MF_ERROR_OKAY)
		MFG_RETURN_ERROR(MFG_ERROR_ALLOCATION_FAILED, u8"Failed to allocate render texture on pool");

	// Init object
	{
		mfError err = mfmInitObject(&oglTex->base.object);
		if (err != MF_ERROR_OKAY)
			return err;
	}
	oglTex->base.object.destructorFunc = &mfgOGL4DestroyRenderTexture;
	oglTex->base.renderDevice = rd;

	// Create texture
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
	glTexStorage2D(GL_TEXTURE_2D, 1, oglTex->internalFormat, width, height);

	oglTex->width = width;
	oglTex->height = height;
	
	*tex = oglTex;
	mfError err = mfmIncObjectRef(rd);
	if (err != MF_ERROR_OKAY)
		MFG_RETURN_ERROR(err, u8"mfmIncObjectRef failed on render device");

	MFG_CHECK_GL_ERROR();
	return MF_ERROR_OKAY;
}

void mfgOGL4DestroyDepthStencilTexture(void* tex)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (tex == NULL) abort();
#endif
	mfgOGL4DepthStencilTexture* oglTex = tex;
	glDeleteRenderbuffers(1, &oglTex->rbo);
	if (mfmDecObjectRef(oglTex->base.renderDevice) != MF_ERROR_OKAY)
		abort();
	if (mfmDestroyObject(&oglTex->base.object) != MF_ERROR_OKAY)
		abort();
	if (mfmDeallocate(((mfgOGL4RenderDevice*)oglTex->base.renderDevice)->pool64, oglTex) != MF_ERROR_OKAY)
		abort();
#ifdef MAGMA_FRAMEWORK_DEBUG
	GLenum err = glGetError();
	if (err != 0)
		abort();
#endif
}

mfError mfgOGL4CreateDepthStencilTexture(mfgV2XRenderDevice* rd, mfgV2XDepthStencilTexture** tex, mfmU64 width, mfmU64 height, mfgEnum format)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{ if (rd == NULL || tex == NULL) return MFG_ERROR_INVALID_ARGUMENTS; }
#endif

	mfgOGL4RenderDevice* oglRD = (mfgOGL4RenderDevice*)rd;

	// Allocate texture
	mfgOGL4DepthStencilTexture* oglTex = NULL;
	if (mfmAllocate(oglRD->pool64, &oglTex, sizeof(mfgOGL4DepthStencilTexture)) != MF_ERROR_OKAY)
		MFG_RETURN_ERROR(MFG_ERROR_ALLOCATION_FAILED, u8"Failed to allocate depth stencil texture on pool");

	// Init object
	{
		mfError err = mfmInitObject(&oglTex->base.object);
		if (err != MF_ERROR_OKAY)
			return err;
	}
	oglTex->base.object.destructorFunc = &mfgOGL4DestroyDepthStencilTexture;
	oglTex->base.renderDevice = rd;

	// Create texture
	glGenRenderbuffers(1, &oglTex->rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, oglTex->rbo);

	switch (format)
	{
		case MFG_DEPTH24STENCIL8: glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height); break;
		case MFG_DEPTH32STENCIL8: glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH32F_STENCIL8, width, height); break;
		default: MFG_RETURN_ERROR(MFG_ERROR_INVALID_ARGUMENTS, u8"Unsupported depth stencil format");
	}

	oglTex->width = width;
	oglTex->height = height;

	*tex = oglTex;
	mfError err = mfmIncObjectRef(rd);
	if (err != MF_ERROR_OKAY)
		MFG_RETURN_ERROR(err, u8"mfmIncObjectRef failed on render device");

	MFG_CHECK_GL_ERROR();
	return MF_ERROR_OKAY;
}

void mfgOGL4DestroyFramebuffer(void* fb)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (fb == NULL) abort();
#endif
	mfgOGL4Framebuffer* oglFB = fb;
	glDeleteFramebuffers(1, &oglFB->fbo);
	if (oglFB->depthStencilTexture != NULL)
		if (mfmDecObjectRef(oglFB->depthStencilTexture) != MF_ERROR_OKAY)
			abort();
	for (mfmU64 i = 0; i < 8; ++i)
		if (oglFB->textures[i] != NULL)
			if (mfmDecObjectRef(oglFB->textures[i]) != MF_ERROR_OKAY)
				abort();
	if (mfmDecObjectRef(oglFB->base.renderDevice) != MF_ERROR_OKAY)
		abort();
	if (mfmDestroyObject(&oglFB->base.object) != MF_ERROR_OKAY)
		abort();
	if (mfmDeallocate(((mfgOGL4RenderDevice*)oglFB->base.renderDevice)->pool64, oglFB) != MF_ERROR_OKAY)
		abort();
#ifdef MAGMA_FRAMEWORK_DEBUG
	GLenum err = glGetError();
	if (err != 0)
		abort();
#endif
}

mfError mfgOGL4CreateFramebuffer(mfgV2XRenderDevice* rd, mfgV2XFramebuffer** fb, mfmU64 textureCount, mfgV2XRenderTexture** textures, mfgV2XDepthStencilTexture* depthStencilTexture)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{ if (rd == NULL || fb == NULL || textures == NULL || textureCount == 0) return MFG_ERROR_INVALID_ARGUMENTS; }
#endif

	mfgOGL4RenderDevice* oglRD = (mfgOGL4RenderDevice*)rd;

	// Allocate texture
	mfgOGL4Framebuffer* oglFB = NULL;
	if (mfmAllocate(oglRD->pool64, &oglFB, sizeof(mfgOGL4Framebuffer)) != MF_ERROR_OKAY)
		MFG_RETURN_ERROR(MFG_ERROR_ALLOCATION_FAILED, u8"Failed to allocate framebuffer on pool");

	// Init object
	{
		mfError err = mfmInitObject(&oglFB->base.object);
		if (err != MF_ERROR_OKAY)
			return err;
	}
	oglFB->base.object.destructorFunc = &mfgOGL4DestroyFramebuffer;
	oglFB->base.renderDevice = rd;

	// Create framebuffer
	GLuint width = ((mfgOGL4RenderTexture*)textures[0])->width;
	GLuint height = ((mfgOGL4RenderTexture*)textures[0])->height;

	glGenFramebuffers(1, &oglFB->fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, oglFB->fbo);

	GLenum* drawBuffers;
	if (mfmAllocate(oglRD->stack, &drawBuffers, textureCount * sizeof(GLenum)) != MF_ERROR_OKAY)
		MFG_RETURN_ERROR(MFG_ERROR_ALLOCATION_FAILED, u8"Failed to allocate draw buffers on stack");

	for (mfmU64 i = 0; i < 8; ++i)
		oglFB->textures[i] = NULL;

	mfError err;

	for (mfmU64 i = 0; i < textureCount; ++i)
	{
		if (((mfgOGL4RenderTexture*)textures[i])->width != width ||
			((mfgOGL4RenderTexture*)textures[i])->height != height)
			MFG_RETURN_ERROR(MFG_ERROR_INVALID_ARGUMENTS, "All textures must have the same size");

		oglFB->textures[i] = textures[i];
		err = mfmIncObjectRef(textures[i]);
		if (err != MF_ERROR_OKAY)
			return err;

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, ((mfgOGL4RenderTexture*)textures[i])->tex, 0);
		drawBuffers[i] = GL_COLOR_ATTACHMENT0 + i;
	}

	glDrawBuffers(textureCount, drawBuffers);
	if (mfmDeallocate(oglRD->stack, drawBuffers) != MF_ERROR_OKAY)
		MFG_RETURN_ERROR(MFG_ERROR_ALLOCATION_FAILED, u8"Failed to deallocate draw buffers on stack");

	if (depthStencilTexture != NULL)
	{
		err = mfmIncObjectRef(depthStencilTexture);
		if (err != MF_ERROR_OKAY)
			return err;
		oglFB->depthStencilTexture = depthStencilTexture;

		if (width != ((mfgOGL4DepthStencilTexture*)depthStencilTexture)->width ||
			height != ((mfgOGL4DepthStencilTexture*)depthStencilTexture)->height)
			MFG_RETURN_ERROR(MFG_ERROR_INVALID_ARGUMENTS, "The depth stencil texture must have the same size as the color textures");
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, ((mfgOGL4DepthStencilTexture*)depthStencilTexture)->rbo);
	}

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		MFG_RETURN_ERROR(MFG_ERROR_INTERNAL, "Framebuffer not complete");

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	oglRD->stack;

	*fb = oglFB;
	err = mfmIncObjectRef(rd);
	if (err != MF_ERROR_OKAY)
		MFG_RETURN_ERROR(err, u8"mfmIncObjectRef failed on render device");

	MFG_CHECK_GL_ERROR();
	return MF_ERROR_OKAY;
}

mfError mfgOGL4SetFramebuffer(mfgV2XRenderDevice* rd, mfgV2XFramebuffer* fb)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{ if (rd == NULL) return MFG_ERROR_INVALID_ARGUMENTS; }
#endif

	mfgOGL4RenderDevice* oglRD = (mfgOGL4RenderDevice*)rd;
	mfgOGL4Framebuffer* oglFB = (mfgOGL4Framebuffer*)fb;

	if (oglFB == NULL)
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	else
		glBindFramebuffer(GL_FRAMEBUFFER, oglFB->fbo);

	MFG_CHECK_GL_ERROR();
	return MF_ERROR_OKAY;
}

mfError mfgOGL4ClearColor(mfgV2XRenderDevice* rd, mfmF32 r, mfmF32 g, mfmF32 b, mfmF32 a)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{ if (rd == NULL) return MFG_ERROR_INVALID_ARGUMENTS; }
#endif
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT);
	MFG_CHECK_GL_ERROR();
	return MF_ERROR_OKAY;
}

mfError mfgOGL4ClearDepth(mfgV2XRenderDevice* rd, mfmF32 depth)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{ if (rd == NULL) return MFG_ERROR_INVALID_ARGUMENTS; }
#endif
	glClearDepth(depth);
	glClear(GL_DEPTH_BUFFER_BIT);
	MFG_CHECK_GL_ERROR();
	return MF_ERROR_OKAY;
}

mfError mfgOGL4ClearStencil(mfgV2XRenderDevice* rd, mfmI32 stencil)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{ if (rd == NULL) return MFG_ERROR_INVALID_ARGUMENTS; }
#endif
	glClearStencil(stencil);
	glClear(GL_STENCIL_BUFFER_BIT);
	MFG_CHECK_GL_ERROR();
	return MF_ERROR_OKAY;
}

mfError mfgOGL4SwapBuffers(mfgV2XRenderDevice* rd)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{ if (rd == NULL) return MFG_ERROR_INVALID_ARGUMENTS; }
#endif
	glfwSwapBuffers((GLFWwindow*)mfiGetGLWindowGLFWHandle(((mfgOGL4RenderDevice*)rd)->window));
	MFG_CHECK_GL_ERROR();
	return MF_ERROR_OKAY;
}

void mfgOGL4DestroyRasterState(void* state)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (state == NULL) abort();
#endif
	mfgOGL4RasterState* oglState = state;
	if (state != ((mfgOGL4RenderDevice*)oglState->base.renderDevice)->defaultRasterState)
		if (mfmDecObjectRef(oglState->base.renderDevice) != MF_ERROR_OKAY)
			abort();
	if (mfmDestroyObject(&oglState->base.object) != MF_ERROR_OKAY)
		abort();
	if (mfmDeallocate(((mfgOGL4RenderDevice*)oglState->base.renderDevice)->pool64, oglState) != MF_ERROR_OKAY)
		abort();
#ifdef MAGMA_FRAMEWORK_DEBUG
	GLenum err = glGetError();
	if (err != 0)
		abort();
#endif
}

mfError mfgOGL4CreateRasterState(mfgV2XRenderDevice* rd, mfgV2XRasterState** state, const mfgV2XRasterStateDesc* desc)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{ if (rd == NULL || state == NULL || desc == NULL) return MFG_ERROR_INVALID_ARGUMENTS; }
#endif

	mfgOGL4RenderDevice* oglRD = (mfgOGL4RenderDevice*)rd;

	// Allocate state
	mfgOGL4RasterState* oglState = NULL;
	if (mfmAllocate(oglRD->pool64, &oglState, sizeof(mfgOGL4RasterState)) != MF_ERROR_OKAY)
		MFG_RETURN_ERROR(MFG_ERROR_ALLOCATION_FAILED, u8"Failed to allocate raster state on pool");

	// Init object
	{
		mfError err = mfmInitObject(&oglState->base.object);
		if (err != MF_ERROR_OKAY)
			return err;
	}
	oglState->base.object.destructorFunc = &mfgOGL4DestroyRasterState;
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

	if (oglRD->defaultRasterState != NULL)
	{
		mfError err = mfmIncObjectRef(rd);
		if (err != MF_ERROR_OKAY)
			MFG_RETURN_ERROR(err, u8"mfmIncObjectRef failed on render device");
	}

	MFG_CHECK_GL_ERROR();
	return MF_ERROR_OKAY;
}

mfError mfgOGL4SetRasterState(mfgV2XRenderDevice* rd, mfgV2XRasterState* state)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{ if (rd == NULL) return MFG_ERROR_INVALID_ARGUMENTS; }
#endif
	if (state == NULL)
		return mfgOGL4SetRasterState(rd, ((mfgOGL4RenderDevice*)rd)->defaultRasterState);

	mfError err;

	mfgOGL4RasterState* oglRS = (mfgOGL4RasterState*)state;

	if (oglRS->cullEnabled)
		glEnable(GL_CULL_FACE);
	else
		glDisable(GL_CULL_FACE);
	glFrontFace(oglRS->frontFace);
	glCullFace(oglRS->cullFace);
	glPolygonMode(GL_FRONT_AND_BACK, oglRS->polygonMode);

	if (oglRS != ((mfgOGL4RenderDevice*)rd)->defaultRasterState)
	{
		if (((mfgOGL4RenderDevice*)rd)->currentRasterState != NULL)
		{
			err = mfmDecObjectRef(((mfgOGL4RenderDevice*)rd)->currentRasterState);
			if (err != MF_ERROR_OKAY)
				return err;
		}

		err = mfmIncObjectRef(state);
		if (err != MF_ERROR_OKAY)
			return err;
	}

	((mfgOGL4RenderDevice*)rd)->currentRasterState = state;

	MFG_CHECK_GL_ERROR();

	return MF_ERROR_OKAY;
}

void mfgOGL4DestroyDepthStencilState(void* state)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (state == NULL) abort();
#endif
	mfgOGL4DepthStencilState* oglState = state;
	if (state != ((mfgOGL4RenderDevice*)oglState->base.renderDevice)->defaultDepthStencilState)
		if (mfmDecObjectRef(oglState->base.renderDevice) != MF_ERROR_OKAY)
			abort();
	if (mfmDestroyObject(&oglState->base.object) != MF_ERROR_OKAY)
		abort();
	if (mfmDeallocate(((mfgOGL4RenderDevice*)oglState->base.renderDevice)->pool256, oglState) != MF_ERROR_OKAY)
		abort();
#ifdef MAGMA_FRAMEWORK_DEBUG
	GLenum err = glGetError();
	if (err != 0)
		abort();
#endif
}

mfError mfgOGL4CreateDepthStencilState(mfgV2XRenderDevice* rd, mfgV2XDepthStencilState** state, const mfgV2XDepthStencilStateDesc* desc)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{ if (rd == NULL || state == NULL || desc == NULL) return MFG_ERROR_INVALID_ARGUMENTS; }
#endif

	mfgOGL4RenderDevice* oglRD = (mfgOGL4RenderDevice*)rd;

	// Allocate state
	mfgOGL4DepthStencilState* oglState = NULL;
	if (mfmAllocate(oglRD->pool256, &oglState, sizeof(mfgOGL4DepthStencilState)) != MF_ERROR_OKAY)
		MFG_RETURN_ERROR(MFG_ERROR_ALLOCATION_FAILED, u8"Failed to allocate depth stencil state on pool");

	// Init object
	{
		mfError err = mfmInitObject(&oglState->base.object);
		if (err != MF_ERROR_OKAY)
			return err;
	}
	oglState->base.object.destructorFunc = &mfgOGL4DestroyDepthStencilState;
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
	if (oglRD->defaultDepthStencilState != NULL)
	{
		mfError err = mfmIncObjectRef(rd);
		if (err != MF_ERROR_OKAY)
			MFG_RETURN_ERROR(err, u8"mfmIncObjectRef failed on render device");
	}

	MFG_CHECK_GL_ERROR();
	return MF_ERROR_OKAY;
}

mfError mfgOGL4SetDepthStencilState(mfgV2XRenderDevice* rd, mfgV2XDepthStencilState* state)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{ if (rd == NULL) return MFG_ERROR_INVALID_ARGUMENTS; }
#endif
	if (state == NULL)
		return mfgOGL4SetDepthStencilState(rd, ((mfgOGL4RenderDevice*)rd)->defaultDepthStencilState);

	mfgOGL4DepthStencilState* oglDSS = (mfgOGL4DepthStencilState*)state;

	mfError err;

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

	if (oglDSS != ((mfgOGL4RenderDevice*)rd)->defaultDepthStencilState)
	{
		if (((mfgOGL4RenderDevice*)rd)->currentDepthStencilState != NULL)
		{
			err = mfmDecObjectRef(((mfgOGL4RenderDevice*)rd)->currentDepthStencilState);
			if (err != MF_ERROR_OKAY)
				return err;
		}

		err = mfmIncObjectRef(state);
		if (err != MF_ERROR_OKAY)
			return err;
	}

	((mfgOGL4RenderDevice*)rd)->currentDepthStencilState = state;

	MFG_CHECK_GL_ERROR();

	return MF_ERROR_OKAY;
}

void mfgOGL4DestroyBlendState(void* state)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (state == NULL) abort();
#endif
	mfgOGL4BlendState* oglState = state;
	if (state != ((mfgOGL4RenderDevice*)oglState->base.renderDevice)->defaultBlendState)
		if (mfmDecObjectRef(oglState->base.renderDevice) != MF_ERROR_OKAY)
			abort();
	if (mfmDestroyObject(&oglState->base.object) != MF_ERROR_OKAY)
		abort();
	if (mfmDeallocate(((mfgOGL4RenderDevice*)oglState->base.renderDevice)->pool64, oglState) != MF_ERROR_OKAY)
		abort();
#ifdef MAGMA_FRAMEWORK_DEBUG
	GLenum err = glGetError();
	if (err != 0)
		abort();
#endif
}

mfError mfgOGL4CreateBlendState(mfgV2XRenderDevice* rd, mfgV2XBlendState** state, const mfgV2XBlendStateDesc* desc)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{ if (rd == NULL || state == NULL || desc == NULL) return MFG_ERROR_INVALID_ARGUMENTS; }
#endif

	mfgOGL4RenderDevice* oglRD = (mfgOGL4RenderDevice*)rd;

	// Allocate state
	mfgOGL4BlendState* oglState = NULL;
	if (mfmAllocate(oglRD->pool64, &oglState, sizeof(mfgOGL4BlendState)) != MF_ERROR_OKAY)
		MFG_RETURN_ERROR(MFG_ERROR_ALLOCATION_FAILED, u8"Failed to allocate raster state on pool");

	// Init object
	{
		mfError err = mfmInitObject(&oglState->base.object);
		if (err != MF_ERROR_OKAY)
			return err;
	}
	oglState->base.object.destructorFunc = &mfgOGL4DestroyRasterState;
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
	if (oglRD->defaultBlendState != NULL)
	{
		mfError err = mfmIncObjectRef(rd);
		if (err != MF_ERROR_OKAY)
			MFG_RETURN_ERROR(err, u8"mfmIncObjectRef failed on render device");
	}

	MFG_CHECK_GL_ERROR();
	return MF_ERROR_OKAY;
}

mfError mfgOGL4SetBlendState(mfgV2XRenderDevice* rd, mfgV2XBlendState* state)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{ if (rd == NULL) return MFG_ERROR_INVALID_ARGUMENTS; }
#endif
	if (state == NULL)
		return mfgOGL4SetBlendState(rd, ((mfgOGL4RenderDevice*)rd)->defaultBlendState);

	mfError err;

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

	if (oglBS != ((mfgOGL4RenderDevice*)rd)->defaultBlendState)
	{
		if (((mfgOGL4RenderDevice*)rd)->currentBlendState != NULL)
		{
			err = mfmDecObjectRef(((mfgOGL4RenderDevice*)rd)->currentBlendState);
			if (err != MF_ERROR_OKAY)
				return err;
		}

		err = mfmIncObjectRef(state);
		if (err != MF_ERROR_OKAY)
			return err;
	}

	((mfgOGL4RenderDevice*)rd)->currentBlendState = state;

	MFG_CHECK_GL_ERROR();

	return MF_ERROR_OKAY;
}

mfmBool mfgOGL4GetErrorString(mfgV2XRenderDevice* rd, mfsUTF8CodeUnit* str, mfmU64 maxSize)
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

mfError mfgOGL4DrawTriangles(mfgV2XRenderDevice* rd, mfmU64 offset, mfmU64 count)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{ if (rd == NULL) return MFG_ERROR_INVALID_ARGUMENTS; }
#endif
	glDrawArrays(GL_TRIANGLES, offset, count);
	MFG_CHECK_GL_ERROR();
	return MF_ERROR_OKAY;
}

mfError mfgOGL4DrawTrianglesIndexed(mfgV2XRenderDevice* rd, mfmU64 offset, mfmU64 count)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{
		if (rd == NULL ||
			((mfgOGL4RenderDevice*)rd)->currentIndexBuffer == NULL) return MFG_ERROR_INVALID_ARGUMENTS;
	}
#endif
	glDrawElements(GL_TRIANGLES, count, ((mfgOGL4RenderDevice*)rd)->currentIndexBuffer->indexType, (char*)NULL + offset);
	MFG_CHECK_GL_ERROR();
	return MF_ERROR_OKAY;
}

mfError mfgOGL4GetPropertyI(mfgV2XRenderDevice* rd, mfgEnum id, mfmI32* value)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (rd == NULL || value == NULL) return MFG_ERROR_INVALID_ARGUMENTS;
#endif

	GLint out;

	if (id == MFG_MAX_ANISOTROPY)
		glGetIntegerv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &out);
	else if (id == MFG_CONSTANT_ALIGN)
		glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &out);
	else
		MFG_RETURN_ERROR(MFG_ERROR_INVALID_ARGUMENTS, u8"Unsupported property ID");

	*value = out;

	MFG_CHECK_GL_ERROR();
	return MF_ERROR_OKAY;
}

mfError mfgOGL4GetPropertyF(mfgV2XRenderDevice* rd, mfgEnum id, mfmF32* value)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (rd == NULL || value == NULL) return MFG_ERROR_INVALID_ARGUMENTS;
#endif

	GLfloat out;

	if (id == MFG_MAX_ANISOTROPY)
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &out);
	else if (id == MFG_CONSTANT_ALIGN)
		glGetFloatv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &out);
	else
		MFG_RETURN_ERROR(MFG_ERROR_INVALID_ARGUMENTS, u8"Unsupported property ID");

	*value = out;

	MFG_CHECK_GL_ERROR();
	return MF_ERROR_OKAY;
}

mfError mfgV2XCreateOGL4RenderDevice(mfgV2XRenderDevice ** renderDevice, mfiWindow* window, const mfgV2XRenderDeviceDesc * desc, void * allocator)
{
	// Check if params are valid
	if (renderDevice == NULL || window == NULL || desc == NULL || strcmp(window->type, MFI_OGLWINDOW_TYPE_NAME) != 0)
		return MFG_ERROR_INVALID_ARGUMENTS;
	
	// Allocate render device
	mfgOGL4RenderDevice* rd;
	if (mfmAllocate(allocator, &rd, sizeof(mfgOGL4RenderDevice)) != MF_ERROR_OKAY)
		return MFG_ERROR_ALLOCATION_FAILED;

	// Create 32 bytes pool
	{
		mfmPoolAllocatorDesc desc;
		desc.expandable = MFM_FALSE;
		desc.slotCount = MFG_POOL_48_ELEMENT_COUNT;
		desc.slotSize = 32;
		mfError err = mfmCreatePoolAllocatorOnMemory(&rd->pool48, &desc, rd->pool48Memory, sizeof(rd->pool48Memory));
		if (err != MF_ERROR_OKAY)
			return MFG_ERROR_ALLOCATION_FAILED;
	}

	// Create 64 bytes pool
	{
		mfmPoolAllocatorDesc desc;
		desc.expandable = MFM_FALSE;
		desc.slotCount = MFG_POOL_64_ELEMENT_COUNT;
		desc.slotSize = 64;
		mfError err = mfmCreatePoolAllocatorOnMemory(&rd->pool64, &desc, rd->pool64Memory, sizeof(rd->pool64Memory));
		if (err != MF_ERROR_OKAY)
			return MFG_ERROR_ALLOCATION_FAILED;
	}

	// Create 256 bytes pool
	{
		mfmPoolAllocatorDesc desc;
		desc.expandable = MFM_FALSE;
		desc.slotCount = MFG_POOL_256_ELEMENT_COUNT;
		desc.slotSize = 256;
		mfError err = mfmCreatePoolAllocatorOnMemory(&rd->pool256, &desc, rd->pool256Memory, sizeof(rd->pool256Memory));
		if (err != MF_ERROR_OKAY)
			return MFG_ERROR_ALLOCATION_FAILED;
	}

	// Create 512 bytes pool
	{
		mfmPoolAllocatorDesc desc;
		desc.expandable = MFM_FALSE;
		desc.slotCount = MFG_POOL_512_ELEMENT_COUNT;
		desc.slotSize = 512;
		mfError err = mfmCreatePoolAllocatorOnMemory(&rd->pool512, &desc, rd->pool512Memory, sizeof(rd->pool512Memory));
		if (err != MF_ERROR_OKAY)
			return MFG_ERROR_ALLOCATION_FAILED;
	}

	// Create stack
	{
		mfError err = mfmCreateStackAllocatorOnMemory(&rd->stack, 2048, rd->stackMemory, sizeof(rd->stackMemory));
		if (err != MF_ERROR_OKAY)
			return MFG_ERROR_ALLOCATION_FAILED;
	}

	// Initialize some properties
	{
		mfError err = mfmInitObject(&rd->base.object);
		if (err != MF_ERROR_OKAY)
			return err;
	}
	rd->base.object.destructorFunc = &mfgV2XDestroyOGL4RenderDevice;
	rd->window = window;
	rd->allocator = allocator;
	memset(rd->errorString, '\0', sizeof(rd->errorString));
	rd->errorStringSize = 0;

	rd->currentBlendState = NULL;
	rd->currentDepthStencilState = NULL;
	rd->currentRasterState = NULL;
	rd->currentFramebuffer = NULL;
	rd->currentPipeline = NULL;
	rd->currentVertexArray = NULL;
	rd->currentIndexBuffer = NULL;

	// Init context
	glfwMakeContextCurrent((GLFWwindow*)mfiGetGLWindowGLFWHandle(((mfgOGL4RenderDevice*)rd)->window));
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		mfsPrintFormat(mfsErrStream, u8"Failed to create OGL4RenderDevice:\nFailed to init GLEW:\n%s", (const mfsUTF8CodeUnit*)glewGetErrorString(err));
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
	rd->base.bindRenderTexture = &mfgOGL4BindRenderTexture;
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

	rd->base.createRenderTexture = &mfgOGL4CreateRenderTexture;
	rd->base.destroyRenderTexture = &mfgOGL4DestroyRenderTexture;
	rd->base.createDepthStencilTexture = &mfgOGL4CreateDepthStencilTexture;
	rd->base.destroyDepthStencilTexture = &mfgOGL4DestroyDepthStencilTexture;
	rd->base.createFramebuffer = &mfgOGL4CreateFramebuffer;
	rd->base.destroyFramebuffer = &mfgOGL4DestroyFramebuffer;
	rd->base.setFramebuffer = &mfgOGL4SetFramebuffer;

	rd->base.clearColor = &mfgOGL4ClearColor;
	rd->base.clearDepth = &mfgOGL4ClearDepth;
	rd->base.clearStencil = &mfgOGL4ClearStencil;
	rd->base.drawTriangles = &mfgOGL4DrawTriangles;
	rd->base.drawTrianglesIndexed = &mfgOGL4DrawTrianglesIndexed;
	rd->base.swapBuffers = &mfgOGL4SwapBuffers;

	rd->base.getPropertyI = &mfgOGL4GetPropertyI;
	rd->base.getPropertyF = &mfgOGL4GetPropertyF;

	rd->base.getErrorString = &mfgOGL4GetErrorString;

	// Get and set the default raster state
	{
		mfgV2XRasterStateDesc desc;
		mfgV2XDefaultRasterStateDesc(&desc);
		rd->defaultRasterState = NULL;
		mfError err = mfgV2XCreateRasterState(rd, &rd->defaultRasterState, &desc);
		if (err != MF_ERROR_OKAY)
			return err;
		err = mfgV2XSetRasterState(rd, rd->defaultRasterState);
		if (err != MF_ERROR_OKAY)
			return err;
	}

	// Get and set the default depth stencil state
	{
		mfgV2XDepthStencilStateDesc desc;
		mfgV2XDefaultDepthStencilStateDesc(&desc);
		rd->defaultDepthStencilState = NULL;
		mfError err = mfgV2XCreateDepthStencilState(rd, &rd->defaultDepthStencilState, &desc);
		if (err != MF_ERROR_OKAY)
			return err;
		err = mfgV2XSetDepthStencilState(rd, rd->defaultDepthStencilState);
		if (err != MF_ERROR_OKAY)
			return err;
	}

	// Get and set the default blend state
	{
		mfgV2XBlendStateDesc desc;
		mfgV2XDefaultBlendStateDesc(&desc);
		rd->defaultBlendState = NULL;
		mfError err = mfgV2XCreateBlendState(rd, &rd->defaultBlendState, &desc);
		if (err != MF_ERROR_OKAY)
			return err;
		err = mfgV2XSetBlendState(rd, rd->defaultBlendState);
		if (err != MF_ERROR_OKAY)
			return err;
	}

	if (mfmIncObjectRef(rd->window) != MF_ERROR_OKAY)
		abort();

	if (mfmIncObjectRef(rd->defaultRasterState) != MF_ERROR_OKAY)
		abort();
	if (mfmIncObjectRef(rd->defaultDepthStencilState) != MF_ERROR_OKAY)
		abort();
	if (mfmIncObjectRef(rd->defaultBlendState) != MF_ERROR_OKAY)
		abort();

	// Successfully inited render device
	*renderDevice = rd;
	return MF_ERROR_OKAY;
}

void mfgV2XDestroyOGL4RenderDevice(void * renderDevice)
{
	if (renderDevice == NULL)
		abort();

	mfgOGL4RenderDevice* rd = (mfgOGL4RenderDevice*)renderDevice;

	if (mfmDecObjectRef(rd->window) != MF_ERROR_OKAY)
		abort();

	// Destroy default states
	if (mfgV2XSetRasterState(rd, NULL) != MF_ERROR_OKAY)
		abort();
	if (mfgV2XSetDepthStencilState(rd, NULL) != MF_ERROR_OKAY)
		abort();
	if (mfgV2XSetBlendState(rd, NULL) != MF_ERROR_OKAY)
		abort();

	if (mfmDecObjectRef(rd->defaultRasterState) != MF_ERROR_OKAY)
		abort();
	if (mfmDecObjectRef(rd->defaultDepthStencilState) != MF_ERROR_OKAY)
		abort();
	if (mfmDecObjectRef(rd->defaultBlendState) != MF_ERROR_OKAY)
		abort();

	// Destroy pools
	mfmDestroyStackAllocator(rd->stack);
	mfmDestroyPoolAllocator(rd->pool512);
	mfmDestroyPoolAllocator(rd->pool256);
	mfmDestroyPoolAllocator(rd->pool64);
	mfmDestroyPoolAllocator(rd->pool48);

	// Deallocate render device
	if (mfmDestroyObject(&rd->base.object) != MF_ERROR_OKAY)
		abort();
	if (mfmDeallocate(rd->allocator, rd) != MF_ERROR_OKAY)
		abort();
}

#else

mfError mfgV2XCreateOGL4RenderDevice(mfgV2XRenderDevice ** renderDevice, mfiWindow* window, const mfgV2XRenderDeviceDesc * desc, void * allocator)
{
	return MFG_ERROR_NOT_SUPPORTED;
}

void mfgV2XDestroyOGL4RenderDevice(void * renderDevice)
{
	abort();
}

#endif