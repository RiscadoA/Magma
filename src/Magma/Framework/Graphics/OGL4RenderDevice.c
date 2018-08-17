#include "OGL4RenderDevice.h"
#include "OGL4Assembler.h"
#include "Config.h"
	
#include "../Memory/PoolAllocator.h"
#include "../String/StringStream.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef MAGMA_FRAMEWORK_USE_OPENGL

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

typedef struct
{
	mfgRenderDevice base;

	mfiWindow* window;

	void* allocator;

	mfsUTF8CodeUnit errorString[256];
	mfmU64 errorStringSize;

	mfmPoolAllocator* pool;
	mfmU8 poolMemory[280000];

	mfgRasterState* defaultRasterState;
	mfgDepthStencilState* defaultDepthStencilState;
	mfgBlendState* defaultBlendState;
} mfgOGL4RenderDevice;

typedef struct
{
	mfgRenderDeviceObject base;
	GLint program;
	const mfgMetaData* md;
} mfgOGL4Shader;

typedef struct
{
	mfgRenderDeviceObject base;
	GLint pipeline;
} mfgOGL4Pipeline;

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
	if (mfmDeallocate(((mfgOGL4RenderDevice*)oglVS->base.renderDevice)->pool, oglVS) != MFM_ERROR_OKAY)
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
	if (mfmAllocate(oglRD->pool, &oglVS, sizeof(mfgOGL4Shader)) != MFM_ERROR_OKAY)
		MFG_RETURN_ERROR(MFM_ERROR_ALLOCATION_FAILED, u8"Failed to allocate vertex shader on pool");

	// Init object
	oglVS->base.object.destructorFunc = &mfgOGL4DestroyVertexShader;
	oglVS->base.object.referenceCount = 0;
	oglVS->base.renderDevice = rd;

	// Create string stream
	mfsStream* ss;
	GLchar buffer[4096];
	if (mfsCreateStringStream(&ss, buffer, sizeof(buffer), oglRD->allocator) != MFM_ERROR_OKAY)
		MFG_RETURN_ERROR(MFG_ERROR_INTERNAL, u8"Failed to create string stream for mfgOGL4Assemble");
	mfgError err = mfgOGL4Assemble(bytecode, bytecodeSize, metaData, ss);
	if (err != MFG_ERROR_OKAY)
		MFG_RETURN_ERROR(MFG_ERROR_INTERNAL, u8"mfgOGL4Assemble failed");
	mfsPutByte(ss, '\0');
	mfsDestroyStringStream(ss);

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

	*vs = oglVS;

	MFG_CHECK_GL_ERROR();
	return MFG_ERROR_OKAY;
}

void mfgOGL4DestroyPixelShader(void* ps)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (ps == NULL) abort();
#endif
	mfgOGL4Shader* oglPS = ps;
	glDeleteProgram(oglPS->program);
	if (mfmDeallocate(((mfgOGL4RenderDevice*)oglPS->base.renderDevice)->pool, oglPS) != MFM_ERROR_OKAY)
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
	if (mfmAllocate(oglRD->pool, &oglPS, sizeof(mfgOGL4Shader)) != MFM_ERROR_OKAY)
		MFG_RETURN_ERROR(MFM_ERROR_ALLOCATION_FAILED, u8"Failed to allocate pixel shader on pool");

	// Init object
	oglPS->base.object.destructorFunc = &mfgOGL4DestroyPixelShader;
	oglPS->base.object.referenceCount = 0;
	oglPS->base.renderDevice = rd;

	// Create string stream
	mfsStream* ss;
	GLchar buffer[4096];
	if (mfsCreateStringStream(&ss, buffer, sizeof(buffer), oglRD->allocator) != MFM_ERROR_OKAY)
		MFG_RETURN_ERROR(MFG_ERROR_INTERNAL, u8"Failed to create string stream for mfgOGL4Assemble");
	mfgError err = mfgOGL4Assemble(bytecode, bytecodeSize, metaData, ss);
	if (err != MFG_ERROR_OKAY)
		MFG_RETURN_ERROR(MFG_ERROR_INTERNAL, u8"mfgOGL4Assemble failed");
	mfsPutByte(ss, '\0');
	mfsDestroyStringStream(ss);

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

	*ps = oglPS;

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
	if (mfmDeallocate(((mfgOGL4RenderDevice*)oglPP->base.renderDevice)->pool, oglPP) != MFM_ERROR_OKAY)
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
	if (mfmAllocate(oglRD->pool, &oglPP, sizeof(mfgOGL4Pipeline)) != MFM_ERROR_OKAY)
		MFG_RETURN_ERROR(MFM_ERROR_ALLOCATION_FAILED, u8"Failed to allocate pipeline on pool");

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
	if (mfmDeallocate(((mfgOGL4RenderDevice*)oglState->base.renderDevice)->pool, oglState) != MFM_ERROR_OKAY)
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
	if (mfmAllocate(oglRD->pool, &oglState, sizeof(mfgOGL4RasterState)) != MFM_ERROR_OKAY)
		MFG_RETURN_ERROR(MFM_ERROR_ALLOCATION_FAILED, u8"Failed to allocate raster state on pool");

	// Init object
	oglState->base.object.destructorFunc = &mfgOGL4DestroyRasterState;
	oglState->base.object.referenceCount = 0;
	oglState->base.renderDevice = rd;

	// Set properties
	// TO DO

	*state = oglState;

	MFG_CHECK_GL_ERROR();
	return MFG_ERROR_OKAY;
}

mfgError mfgOGL4SetRasterState(mfgRenderDevice* rd, mfgRasterState* state)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{ if (rd == NULL || state == NULL) return MFG_ERROR_INVALID_ARGUMENTS; }
#endif
	return MFG_ERROR_OKAY;
}

void mfgOGL4DestroyDepthStencilState(void* state)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (state == NULL) abort();
#endif
	mfgOGL4DepthStencilState* oglState = state;
	if (mfmDeallocate(((mfgOGL4RenderDevice*)oglState->base.renderDevice)->pool, oglState) != MFM_ERROR_OKAY)
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
	if (mfmAllocate(oglRD->pool, &oglState, sizeof(mfgOGL4DepthStencilState)) != MFM_ERROR_OKAY)
		MFG_RETURN_ERROR(MFM_ERROR_ALLOCATION_FAILED, u8"Failed to allocate depth stencil state on pool");

	// Init object
	oglState->base.object.destructorFunc = &mfgOGL4DestroyDepthStencilState;
	oglState->base.object.referenceCount = 0;
	oglState->base.renderDevice = rd;

	// Set properties
	// TO DO

	*state = oglState;

	MFG_CHECK_GL_ERROR();
	return MFG_ERROR_OKAY;
}

mfgError mfgOGL4SetDepthStencilState(mfgRenderDevice* rd, mfgDepthStencilState* state)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{ if (rd == NULL || state == NULL) return MFG_ERROR_INVALID_ARGUMENTS; }
#endif
	return MFG_ERROR_OKAY;
}

void mfgOGL4DestroyBlendState(void* state)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (state == NULL) abort();
#endif
	mfgOGL4BlendState* oglState = state;
	if (mfmDeallocate(((mfgOGL4RenderDevice*)oglState->base.renderDevice)->pool, oglState) != MFM_ERROR_OKAY)
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
	if (mfmAllocate(oglRD->pool, &oglState, sizeof(mfgOGL4BlendState)) != MFM_ERROR_OKAY)
		MFG_RETURN_ERROR(MFM_ERROR_ALLOCATION_FAILED, u8"Failed to allocate raster state on pool");

	// Init object
	oglState->base.object.destructorFunc = &mfgOGL4DestroyRasterState;
	oglState->base.object.referenceCount = 0;
	oglState->base.renderDevice = rd;

	// Set properties
	// TO DO

	*state = oglState;

	MFG_CHECK_GL_ERROR();
	return MFG_ERROR_OKAY;
}

mfgError mfgOGL4SetBlendState(mfgRenderDevice* rd, mfgBlendState* state)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{ if (rd == NULL || state == NULL) return MFG_ERROR_INVALID_ARGUMENTS; }
#endif
	return MFG_ERROR_OKAY;
}

mfmBool mfgOGL4GetErrorString(mfgRenderDevice* rd, mfsUTF8CodeUnit* str, mfmU64 maxSize)
{
	if (rd == NULL || str == NULL || maxSize == 0)
		abort();

	mfgOGL4RenderDevice* oglRD = (mfgOGL4RenderDevice*)rd;

	if (oglRD->errorStringSize == 0)
		return MFM_FALSE;

	if (maxSize >= oglRD->errorStringSize)
		memcpy(str, oglRD->errorString, oglRD->errorStringSize);
	else
	{
		memcpy(str, oglRD->errorString, maxSize - 1);
		str[maxSize] = '\0';
	}

	return MFM_TRUE;
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

	// Create pool allocator
	{
		mfmPoolAllocatorDesc desc;
		desc.expandable = MFM_FALSE;
		desc.slotCount = 2048;
		desc.slotSize = 96;
		mfmError err = mfmCreatePoolAllocatorOnMemory(&rd->pool, &desc, rd->poolMemory, sizeof(rd->poolMemory));
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

	rd->base.createRasterState = &mfgOGL4CreateRasterState;
	rd->base.destroyRasterState = &mfgOGL4DestroyRasterState;
	rd->base.setRasterState = &mfgOGL4SetRasterState;
	rd->base.createDepthStencilState = &mfgOGL4CreateDepthStencilState;
	rd->base.destroyDepthStencilState = &mfgOGL4DestroyDepthStencilState;
	rd->base.setDepthStencilState = &mfgOGL4SetDepthStencilState;
	rd->base.createBlendState = &mfgOGL4CreateBlendState;
	rd->base.destroyBlendState = &mfgOGL4DestroyBlendState;
	rd->base.setBlendState = &mfgOGL4SetBlendState;

	rd->base.clearColor = &mfgOGL4ClearColor;
	rd->base.clearDepth = &mfgOGL4ClearDepth;
	rd->base.clearStencil = &mfgOGL4ClearStencil;
	rd->base.swapBuffers = &mfgOGL4SwapBuffers;

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

	// Destroy pool allocator
	mfmDestroyPoolAllocator(rd->pool);

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