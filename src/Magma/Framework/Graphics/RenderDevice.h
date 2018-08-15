#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "Error.h"
#include "Bytecode.h"

#include "../Memory/Object.h"
#include "../String/UTF8.h"

#define MFG_USAGE_DEFAULT	0x01
#define MFG_USAGE_DYNAMIC	0x02
#define MFG_USAGE_STATIC	0x03

	typedef struct mfgRenderDevice mfgRenderDevice;

	// Shader functions
	typedef mfgError(*mfgRDCreateVertexShaderFunction)(mfgRenderDevice* rd, void** vs, const mfmU8* bc, mfmU64 bcSize, const mfgMetaData* md);
	typedef mfgError(*mfgRDDestroyVertexShaderFunction)(mfgRenderDevice* rd, void* vs);
	typedef mfgError(*mfgRDCreatePixelShaderFunction)(mfgRenderDevice* rd, void** ps, const mfmU8* bc, mfmU64 bcSize, const mfgMetaData* md);
	typedef mfgError(*mfgRDDestroyPixelShaderFunction)(mfgRenderDevice* rd, void* ps);
	typedef mfgError(*mfgRDCreatePipelineFunction)(mfgRenderDevice* rd, void** pp, void* vs, void* ps);
	typedef mfgError(*mfgRDDestroyPipelineFunction)(mfgRenderDevice* rd, void* pp);
	typedef mfgError(*mfgRDSetPipelineFunction)(mfgRenderDevice* rd, void* pp);

	// Vertex array/buffer/layout functions
	typedef mfgError(*mfgRDCreateVertexBufferFunction)(mfgRenderDevice*, void**, mfmU64 size, const void* data, mfmU8 usage);
	typedef mfgError(*mfgRDDestroyVertexBufferFunction)(mfgRenderDevice*, void*);

	// Error functions
	typedef void(*mfgRDGetErrorString)();

	typedef struct
	{
		mfmObject object;

		mfgRDCreateVertexShaderFunction createVertexShader;
		mfgRDDestroyVertexShaderFunction destroyVertexShader;

		mfgRDCreatePixelShaderFunction createPixelShader;
		mfgRDDestroyPixelShaderFunction destroyPixelShader;

		mfgRDGetErrorString getErrorString;

		void* allocator;
	} mfgRenderDevice;

#ifdef __cplusplus
}
#endif
