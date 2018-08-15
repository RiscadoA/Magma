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

#define MFG_BYTE			0x01
#define MFG_SHORT			0x02
#define MFG_INT				0x03
#define MFG_UBYTE			0x04
#define MFG_USHORT			0x05
#define MFG_UINT			0x06
#define MFG_NBYTE			0x07
#define MFG_NSHORT			0x08
#define MFG_NINT			0x09
#define MFG_NUBYTE			0x0A
#define MFG_NUSHORT			0x0B
#define MFG_NUINT			0x0C
#define MFG_FLOAT			0x0D

	typedef struct mfgRenderDevice mfgRenderDevice;

	typedef struct
	{
		mfsUTF8CodeUnit name[16];
		mfmU64 stride;
		mfmU64 offset;
		mfmU64 bufferIndex;
		mfmU8 elementType;
		mfmU8 size;
	} mfgVertexElement;

	typedef struct
	{
		
	} mfgSamplerDesc;

	// Shader functions
	typedef mfgError(*mfgRDCreateVertexShaderFunction)(mfgRenderDevice* rd, void** vs, const mfmU8* bc, mfmU64 bcSize, const mfgMetaData* md);
	typedef mfgError(*mfgRDDestroyVertexShaderFunction)(mfgRenderDevice* rd, void* vs);
	typedef mfgError(*mfgRDCreatePixelShaderFunction)(mfgRenderDevice* rd, void** ps, const mfmU8* bc, mfmU64 bcSize, const mfgMetaData* md);
	typedef mfgError(*mfgRDDestroyPixelShaderFunction)(mfgRenderDevice* rd, void* ps);
	typedef mfgError(*mfgRDCreatePipelineFunction)(mfgRenderDevice* rd, void** pp, void* vs, void* ps);
	typedef mfgError(*mfgRDDestroyPipelineFunction)(mfgRenderDevice* rd, void* pp);
	typedef mfgError(*mfgRDSetPipelineFunction)(mfgRenderDevice* rd, void* pp);

	// Vertex functions
	typedef mfgError(*mfgRDCreateVertexBufferFunction)(mfgRenderDevice* rd, void** vb, mfmU64 size, const void* data, mfmU8 usage);
	typedef mfgError(*mfgRDDestroyVertexBufferFunction)(mfgRenderDevice* rd, void* vb);
	typedef mfgError(*mfgRDCreateVertexLayoutFunction)(mfgRenderDevice* rd, void** vl, mfmU64 elementCount, const mfgVertexElement* elements, void* vs);
	typedef mfgError(*mfgRDDestroyVertexLayoutFunction)(mfgRenderDevice* rd, void* vl);
	typedef mfgError(*mfgRDCreateVertexArrayFunction)(mfgRenderDevice* rd, void** va, mfmU64 vbCount, void** vbs, void* vl);
	typedef mfgError(*mfgRDDestroyVertexArrayFunction)(mfgRenderDevice* rd, void* va);
	typedef mfgError(*mfgRDSetVertexArrayFunction)(mfgRenderDevice* rd, void* va);
	typedef mfgError(*mfgRDCreateIndexBufferFunction)(mfgRenderDevice* rd, void** ib, mfmU64 size, const void* data, mfmU8 usage);
	typedef mfgError(*mfgRDDestroyIndexBufferFunction)(mfgRenderDevice* rd, void* ib);
	typedef mfgError(*mfgRDSetIndexBufferFunction)(mfgRenderDevice* rd, void* ib);

	// Texture functions
	typedef mfgError(*mfgRDCreateTexture1DFunction)(mfgRenderDevice* rd, void** tex, mfmU64 width, mfmU8 format, const void* data, mfmU8 usage);
	typedef mfgError(*mfgRDDestroyTexture1DFunction)(mfgRenderDevice* rd, void* tex);
	typedef mfgError(*mfgRDCreateTexture2DFunction)(mfgRenderDevice* rd, void** tex, mfmU64 width, mfmU64 height, mfmU8 format, const void* data, mfmU8 usage, mfmBool isRenderTarget);
	typedef mfgError(*mfgRDDestroyTexture2DFunction)(mfgRenderDevice* rd, void* tex);
	typedef mfgError(*mfgRDCreateTexture3DFunction)(mfgRenderDevice* rd, void** tex, mfmU64 width, mfmU64 height, mfmU64 depth, mfmU8 format, const void* data, mfmU8 usage);
	typedef mfgError(*mfgRDDestroyTexture3DFunction)(mfgRenderDevice* rd, void* tex);
	typedef mfgError(*mfgRDCreateSamplerFunction)(mfgRenderDevice* rd, void** sampler, const mfgSamplerDesc* desc);
	typedef mfgError(*mfgRDDestroySamplerFunction)(mfgRenderDevice* rd, void* sampler);

	// Error functions
	typedef mfmBool(*mfgRDGetErrorString)(mfsUTF8CodeUnit* str, mfmU64 maxSize);

	typedef struct
	{
		mfmObject object;

		mfgRDCreateVertexShaderFunction createVertexShader;
		mfgRDDestroyVertexShaderFunction destroyVertexShader;
		mfgRDCreatePixelShaderFunction createPixelShader;
		mfgRDDestroyPixelShaderFunction destroyPixelShader;
		mfgRDCreatePipelineFunction createPipeline;
		mfgRDDestroyPipelineFunction destroyPipeline;
		mfgRDSetPipelineFunction setPipeline;

		mfgRDCreateVertexBufferFunction createVertexBuffer;
		mfgRDDestroyVertexBufferFunction destroyVertexBuffer;
		mfgRDCreateVertexLayoutFunction createVertexLayout;
		mfgRDDestroyVertexLayoutFunction destroyVertexLayout;
		mfgRDCreateVertexArrayFunction createVertexArray;
		mfgRDDestroyVertexArrayFunction destroyVertexArray;
		mfgRDSetVertexArrayFunction setVertexArray;
		mfgRDCreateIndexBufferFunction createIndexBuffer;
		mfgRDDestroyIndexBufferFunction destroyIndexBuffer;
		mfgRDSetIndexBufferFunction setIndexBuffer;

		mfgRDCreateTexture1DFunction createTexture1D;
		mfgRDDestroyTexture1DFunction destroyTexture1D;
		mfgRDCreateTexture2DFunction createTexture2D;
		mfgRDDestroyTexture2DFunction destroyTexture2D;
		mfgRDCreateTexture3DFunction createTexture3D;
		mfgRDDestroyTexture3DFunction destroyTexture3D;
		mfgRDCreateSamplerFunction createSampler;
		mfgRDDestroySamplerFunction destroySampler;

		mfgRDGetErrorString getErrorString;
	} mfgRenderDevice;

#ifdef __cplusplus
}
#endif
