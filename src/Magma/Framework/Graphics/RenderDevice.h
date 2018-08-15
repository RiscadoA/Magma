#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "Error.h"
#include "Bytecode.h"

#include "../Memory/Object.h"
#include "../String/UTF8.h"

#define MFG_NONE			0x00

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

#define MFG_R8SNORM			0x01
#define MFG_R16SNORM		0x02
#define MFG_RG8SNORM		0x03
#define MFG_RG16SNORM		0x04
#define MFG_RGBA8SNORM		0x05
#define MFG_RGBA16SNORM		0x06
#define MFG_R8UNORM			0x07
#define MFG_R16UNORM		0x08
#define MFG_RG8UNORM		0x09
#define MFG_RG16UNORM		0x0A
#define MFG_RGBA8UNORM		0x0B
#define MFG_RGBA16UNORM		0x0C
#define MFG_R8SINT			0x0D
#define MFG_R16SINT			0x0E
#define MFG_RG8SINT			0x0F
#define MFG_RG16SINT		0x10
#define MFG_RGBA8SINT		0x11
#define MFG_RGBA16SINT		0x12
#define MFG_R8UINT			0x13
#define MFG_R16UINT			0x14
#define MFG_RG8UINT			0x15
#define MFG_RG16UINT		0x16
#define MFG_RGBA8UINT		0x17
#define MFG_RGBA16UINT		0x18
#define MFG_R32FLOAT		0x19
#define MFG_RG32FLOAT		0x1A
#define MFG_RGB32FLOAT		0x1B
#define MFG_RGBA32FLOAT		0x1C

#define MFG_REPEAT			0x01
#define MFG_MIRROR			0x02
#define MFG_CLAMP			0x03
#define MFG_BORDER			0x04

#define MFG_NEAREST			0x01
#define MFG_LINEAR			0x02

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

	void mfgDefaultVertexElement(mfgVertexElement* element);

	typedef struct
	{
		mfmF32 borderColor[4];
		mfmU32 maxAnisotropy;
		mfmU8 minFilter;
		mfmU8 magFilter;
		mfmU8 mipmapFilter;
		mfmU8 adressU;
		mfmU8 adressV;
		mfmU8 adressW;
	} mfgSamplerDesc;

	void mfgDefaultSamplerDesc(mfgSamplerDesc* desc);

	typedef struct
	{

	} mfgRasterStateDesc;

	void mfgDefaultRasterStateDesc(mfgRasterStateDesc* desc);

	typedef struct
	{

	} mfgDepthStencilStateDesc;

	void mfgDefaultDepthStencilStateDesc(mfgDepthStencilStateDesc* desc);

	typedef struct
	{

	} mfgBlendStateDesc;

	void mfgDefaultBlendStateDesc(mfgBlendStateDesc* desc);

	// Shader functions
	typedef mfgError(*mfgRDCreateVertexShaderFunction)(mfgRenderDevice* rd, void** vs, const mfmU8* bc, mfmU64 bcSize, const mfgMetaData* md);
	typedef mfgError(*mfgRDDestroyVertexShaderFunction)(mfgRenderDevice* rd, void* vs);
	typedef mfgError(*mfgRDCreatePixelShaderFunction)(mfgRenderDevice* rd, void** ps, const mfmU8* bc, mfmU64 bcSize, const mfgMetaData* md);
	typedef mfgError(*mfgRDDestroyPixelShaderFunction)(mfgRenderDevice* rd, void* ps);
	typedef mfgError(*mfgRDCreatePipelineFunction)(mfgRenderDevice* rd, void** pp, void* vs, void* ps);
	typedef mfgError(*mfgRDDestroyPipelineFunction)(mfgRenderDevice* rd, void* pp);
	typedef mfgError(*mfgRDSetPipelineFunction)(mfgRenderDevice* rd, void* pp);

	typedef mfgError(*mfgRDGetVertexShaderBindingPointFunction)(mfgRenderDevice* rd, void** bp, void* vs, const mfsUTF8CodeUnit* name);
	typedef mfgError(*mfgRDGetPixelShaderBindingPointFunction)(mfgRenderDevice* rd, void** bp, void* vs, const mfsUTF8CodeUnit* name);
	typedef mfgError(*mfgRDBindConstantBufferFunction)(mfgRenderDevice* rd, void* bp, void* cb);
	typedef mfgError(*mfgRDBindTexture1DFunction)(mfgRenderDevice* rd, void* bp, void* tex);
	typedef mfgError(*mfgRDBindTexture2DFunction)(mfgRenderDevice* rd, void* bp, void* tex);
	typedef mfgError(*mfgRDBindTexture3DFunction)(mfgRenderDevice* rd, void* bp, void* tex);
	typedef mfgError(*mfgRDBindSamplerFunction)(mfgRenderDevice* rd, void* bp, void* sampler);

	typedef mfgError(*mfgRDCreateConstantBufferFunction)(mfgRenderDevice* rd, void** cb, mfmU64 size, const void* data, mfmU8 usage);
	typedef mfgError(*mfgRDDestroyConstantBufferFunction)(mfgRenderDevice* rd, void* cb);
	typedef mfgError(*mfgRDMapConstantBufferFunction)(mfgRenderDevice* rd, void* cb, void** memory);
	typedef mfgError(*mfgRDUnmapConstantBufferFunction)(mfgRenderDevice* rd, void* cb);

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
	typedef mfgError(*mfgRDUpdateTexture1DFunction)(mfgRenderDevice* rd, void* tex, mfmU64 dstX, mfmU64 width, const void* data);
	typedef mfgError(*mfgRDGenerateTexture1DMipmapsFunction)(mfgRenderDevice* rd, void* tex);

	typedef mfgError(*mfgRDCreateTexture2DFunction)(mfgRenderDevice* rd, void** tex, mfmU64 width, mfmU64 height, mfmU8 format, const void* data, mfmU8 usage);
	typedef mfgError(*mfgRDDestroyTexture2DFunction)(mfgRenderDevice* rd, void* tex);
	typedef mfgError(*mfgRDUpdateTexture2DFunction)(mfgRenderDevice* rd, void* tex, mfmU64 dstX, mfmU64 dstY, mfmU64 width, mfmU64 height, const void* data);
	typedef mfgError(*mfgRDGenerateTexture2DMipmapsFunction)(mfgRenderDevice* rd, void* tex);

	typedef mfgError(*mfgRDCreateTexture3DFunction)(mfgRenderDevice* rd, void** tex, mfmU64 width, mfmU64 height, mfmU64 depth, mfmU8 format, const void* data, mfmU8 usage);
	typedef mfgError(*mfgRDDestroyTexture3DFunction)(mfgRenderDevice* rd, void* tex);
	typedef mfgError(*mfgRDUpdateTexture3DFunction)(mfgRenderDevice* rd, void* tex, mfmU64 dstX, mfmU64 dstY, mfmU64 dstZ, mfmU64 width, mfmU64 height, mfmU64 depth, const void* data);
	typedef mfgError(*mfgRDGenerateTexture3DMipmapsFunction)(mfgRenderDevice* rd, void* tex);

	typedef mfgError(*mfgRDCreateSamplerFunction)(mfgRenderDevice* rd, void** sampler, const mfgSamplerDesc* desc);
	typedef mfgError(*mfgRDDestroySamplerFunction)(mfgRenderDevice* rd, void* sampler);

	// Raster/depth stencil/blend state functions
	typedef mfgError(*mfgRDCreateRasterStateFunction)(mfgRenderDevice* rd, void** state, const mfgRasterStateDesc* desc);
	typedef mfgError(*mfgRDDestroyRasterStateFunction)(mfgRenderDevice* rd, void* state);
	typedef mfgError(*mfgRDSetRasterStateFunction)(mfgRenderDevice* rd, void* state);
	typedef mfgError(*mfgRDCreateDepthStencilStateFunction)(mfgRenderDevice* rd, void** state, const mfgDepthStencilStateDesc* desc);
	typedef mfgError(*mfgRDDestroyDepthStencilFunction)(mfgRenderDevice* rd, void* state);
	typedef mfgError(*mfgRDSetDepthStencilFunction)(mfgRenderDevice* rd, void* state);
	typedef mfgError(*mfgRDCreateBlendStateFunction)(mfgRenderDevice* rd, void** state, const mfgBlendStateDesc* desc);
	typedef mfgError(*mfgRDDestroyBlendStateFunction)(mfgRenderDevice* rd, void* state);
	typedef mfgError(*mfgRDSetBlendStateFunction)(mfgRenderDevice* rd, void* state);

	// Framebuffer functions
	typedef mfgError(*mfgRDCreateRenderTextureFunction)(mfgRenderDevice* rd, void** tex, mfmU64 width, mfmU64 height, mfmU8 format);
	typedef mfgError(*mfgRDDestroyRenderTextureFunction)(mfgRenderDevice* rd, void* tex);
	typedef mfgError(*mfgRDCreateDepthStencilTextureFunction)(mfgRenderDevice* rd, void** tex, mfmU64 width, mfmU64 height, mfmU8 format);
	typedef mfgError(*mfgRDDestroyDepthStencilTextureFunction)(mfgRenderDevice* rd, void* tex);
	typedef mfgError(*mfgRDCreateFramebufferFunction)(mfgRenderDevice* rd, void** fb, mfmU64 textureCount, void* textures, void* depthStencilTexture);
	typedef mfgError(*mfgRDDestroyFramebufferFunction)(mfgRenderDevice* rd, void* fb);
	typedef mfgError(*mfgRDSetFramebufferFunction)(mfgRenderDevice* rd, void* fb);

	// Draw functions
	typedef mfgError(*mfgRDClearColorFunction)(mfgRenderDevice* rd, mfmF32 r, mfmF32 g, mfmF32 b, mfmF32 a);
	typedef mfgError(*mfgRDClearDepthFunction)(mfgRenderDevice* rd, mfmF32 depth);
	typedef mfgError(*mfgRDClearStencilFunction)(mfgRenderDevice* rd, mfmI32 stencil);
	typedef mfgError(*mfgRDDrawTrianglesFunction)(mfgRenderDevice* rd, mfmU64 offset, mfmU64 count);
	typedef mfgError(*mfgRDDrawTrianglesIndexedFunction)(mfgRenderDevice* rd, mfmU64 offset, mfmU64 count);
	typedef mfgError(*mfgRDSwapBuffersFunction)(mfgRenderDevice* rd);

	// Getter functions
	typedef mfgError(*mfgRDGetUProperty)(mfgRenderDevice* rd, mfmU32 propID, mfmU32* prop);
	typedef mfgError(*mfgRDGetFProperty)(mfgRenderDevice* rd, mfmU32 propID, mfmF32* prop);

	// Error functions
	typedef mfmBool(*mfgRDGetErrorString)(mfgRenderDevice* rd, mfsUTF8CodeUnit* str, mfmU64 maxSize);

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
		mfgRDUpdateTexture1DFunction updateTexture1D;
		mfgRDGenerateTexture1DMipmapsFunction generateTexture1DMipmaps;

		mfgRDCreateTexture2DFunction createTexture2D;
		mfgRDDestroyTexture2DFunction destroyTexture2D;
		mfgRDUpdateTexture2DFunction updateTexture2D;
		mfgRDGenerateTexture2DMipmapsFunction generateTexture2DMipmaps;

		mfgRDCreateTexture3DFunction createTexture3D;
		mfgRDDestroyTexture3DFunction destroyTexture3D;
		mfgRDUpdateTexture3DFunction updateTexture3D;
		mfgRDGenerateTexture3DMipmapsFunction generateTexture3DMipmaps;

		mfgRDCreateSamplerFunction createSampler;
		mfgRDDestroySamplerFunction destroySampler;

		mfgRDCreateRenderTextureFunction createRenderTexture;
		mfgRDDestroyRenderTextureFunction destroyRenderTexture;
		mfgRDCreateDepthStencilTextureFunction createDepthStencilTexture;
		mfgRDDestroyDepthStencilTextureFunction destroyDepthStencilTexture;
		mfgRDCreateFramebufferFunction createFramebuffer;
		mfgRDDestroyFramebufferFunction destroyFramebuffer;
		mfgRDSetFramebufferFunction setFramebuffer;

		mfgRDClearColorFunction clearColor;
		mfgRDClearDepthFunction clearDepth;
		mfgRDClearStencilFunction clearStencil;
		mfgRDDrawTrianglesFunction drawTriangles;
		mfgRDDrawTrianglesIndexedFunction drawTrianglesIndexed;
		mfgRDSwapBuffersFunction swapBuffers;

		mfgRDGetUProperty getUProperty;
		mfgRDGetFProperty getFProperty;

		mfgRDGetErrorString getErrorString;
	} mfgRenderDevice;

#ifdef __cplusplus
}
#endif
