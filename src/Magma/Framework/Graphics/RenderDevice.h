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

#define MFG_BYTE			0x04
#define MFG_SHORT			0x05
#define MFG_INT				0x06
#define MFG_UBYTE			0x07
#define MFG_USHORT			0x08
#define MFG_UINT			0x09
#define MFG_NBYTE			0x0A
#define MFG_NSHORT			0x0B
#define MFG_NUBYTE			0x0D
#define MFG_NUSHORT			0x0E
#define MFG_FLOAT			0x10

#define MFG_R8SNORM			0x11
#define MFG_R16SNORM		0x12
#define MFG_RG8SNORM		0x13
#define MFG_RG16SNORM		0x14
#define MFG_RGBA8SNORM		0x15
#define MFG_RGBA16SNORM		0x16
#define MFG_R8UNORM			0x17
#define MFG_R16UNORM		0x18
#define MFG_RG8UNORM		0x19
#define MFG_RG16UNORM		0x1A
#define MFG_RGBA8UNORM		0x1B
#define MFG_RGBA16UNORM		0x1C
#define MFG_R8SINT			0x1D
#define MFG_R16SINT			0x1E
#define MFG_RG8SINT			0x1F
#define MFG_RG16SINT		0x20
#define MFG_RGBA8SINT		0x21
#define MFG_RGBA16SINT		0x22
#define MFG_R8UINT			0x23
#define MFG_R16UINT			0x24
#define MFG_RG8UINT			0x25
#define MFG_RG16UINT		0x26
#define MFG_RGBA8UINT		0x27
#define MFG_RGBA16UINT		0x28
#define MFG_R32FLOAT		0x29
#define MFG_RG32FLOAT		0x2A
#define MFG_RGB32FLOAT		0x2B
#define MFG_RGBA32FLOAT		0x2C

#define MFG_REPEAT			0x2D
#define MFG_MIRROR			0x2E
#define MFG_CLAMP			0x2F
#define MFG_BORDER			0x30

#define MFG_NEAREST			0x31
#define MFG_LINEAR			0x32

#define MFG_CW				0x33
#define MFG_CCW				0x34

#define MFG_FRONT			0x35
#define MFG_BACK			0x36
#define MFG_FRONT_AND_BACK	0x37

#define MFG_WIREFRAME		0x38
#define MFG_FILL			0x39

#define MFG_NEVER			0x3A
#define MFG_LESS			0x3B
#define MFG_LEQUAL			0x3C
#define MFG_GREATER			0x3D
#define MFG_GEQUAL			0x3E
#define MFG_EQUAL			0x3F
#define MFG_NEQUAL			0x40
#define MFG_ALWAYS			0x41

#define MFG_ZERO			0x42
#define MFG_KEEP			0x43
#define MFG_REPLACE			0x44
#define MFG_INCREMENT		0x45
#define MFG_INCREMENT_WRAP	0x46
#define MFG_DECREMENT		0x47
#define MFG_DECREMENT_WRAP	0x48
#define MFG_INVERT			0x49

#define MFG_ONE				0X4A
#define MFG_SRC_COLOR		0x4B
#define MFG_INV_SRC_COLOR	0x4C
#define MFG_DST_COLOR		0x4D
#define MFG_INV_DST_COLOR	0x4E
#define MFG_SRC_ALPHA		0x4F
#define MFG_INV_SRC_ALPHA	0x50
#define MFG_DST_ALPHA		0x51
#define MFG_INV_DST_ALPHA	0x52

#define MFG_ADD				0x53
#define MFG_SUBTRACT		0x54
#define MFG_REV_SUBTRACT	0x55
#define MFG_MIN				0x56
#define MFG_MAX				0x57

#define MFG_DEPTH24STENCIL8	0x58
#define MFG_DEPTH32STENCIL8 0x59

#define MFG_MAX_ANISOTROPY	0x5A
#define MFG_CONSTANT_ALIGN	0x5B

	typedef mfmI32 mfgEnum;
	
	typedef struct mfgRenderDevice mfgRenderDevice;

	typedef struct
	{
		mfmObject object;
		mfgRenderDevice* renderDevice;
	} mfgRenderDeviceObject;

	typedef mfgRenderDeviceObject mfgVertexShader;
	typedef mfgRenderDeviceObject mfgPixelShader;
	typedef mfgRenderDeviceObject mfgPipeline;
	typedef mfgRenderDeviceObject mfgBindingPoint;
	typedef mfgRenderDeviceObject mfgConstantBuffer;
	typedef mfgRenderDeviceObject mfgTexture1D;
	typedef mfgRenderDeviceObject mfgTexture2D;
	typedef mfgRenderDeviceObject mfgTexture3D;
	typedef mfgRenderDeviceObject mfgSampler;
	typedef mfgRenderDeviceObject mfgVertexBuffer;
	typedef mfgRenderDeviceObject mfgVertexLayout;
	typedef mfgRenderDeviceObject mfgVertexArray;
	typedef mfgRenderDeviceObject mfgIndexBuffer;
	typedef mfgRenderDeviceObject mfgRasterState;
	typedef mfgRenderDeviceObject mfgDepthStencilState;
	typedef mfgRenderDeviceObject mfgBlendState;
	typedef mfgRenderDeviceObject mfgRenderTexture;
	typedef mfgRenderDeviceObject mfgDepthStencilTexture;
	typedef mfgRenderDeviceObject mfgFramebuffer;

	typedef struct
	{
		/// <summary>
		///		Valid values:
		///			MFG_FALSE;
		///			MFG_TRUE;
		/// </summary>
		mfmBool vsyncEnabled;
	} mfgRenderDeviceDesc;

	void mfgDefaultRenderDeviceDesc(mfgRenderDeviceDesc* desc);

	typedef struct
	{
		/// <summary>
		///		Vertex element name (16 bytes max).
		/// </summary>
		mfsUTF8CodeUnit name[16];

		/// <summary>
		///		Stride between each element in memory.
		/// </summary>
		mfmU64 stride;

		/// <summary>
		///		First element in the buffer offset.
		/// </summary>
		mfmU64 offset;

		/// <summary>
		///		Vertex buffer index.
		/// </summary>
		mfmU64 bufferIndex;

		/// <summary>
		///		Valid values:
		///			MFG_BYTE;
		///			MFG_SHORT;
		///			MFG_INT;
		///			MFG_UBYTE;
		///			MFG_USHORT;
		///			MFG_UINT;
		///			MFG_NBYTE;
		///			MFG_NSHORT;
		///			MFG_NUBYTE;
		///			MFG_NUSHORT;
		///			MFG_FLOAT;
		/// </summary>
		mfgEnum type;

		/// <summary>
		///		Vertex element component count.
		///		Valid values: 1; 2; 3; 4;
		/// </summary>
		mfgEnum size;
	} mfgVertexElement;

	void mfgDefaultVertexElement(mfgVertexElement* element);

	typedef struct
	{
		/// <summary>
		///		Border color (RGBA).
		/// </summary>
		mfmF32 borderColor[4];

		/// <summary>
		///		Max anisotropy for filtering.
		///		Set to 1 to anisotropic filtering.
		///		Valid values: 1 - MFG_MAX_ANISOTROPY.
		/// </summary>
		mfmU32 maxAnisotropy;

		/// <summary>
		///		Minifying filter.
		///		Valid values:
		///			MFG_NEAREST;
		///			MFG_LINEAR;
		/// </summary>
		mfgEnum minFilter;

		/// <summary>
		///		Magnifying filter.
		///		Valid values:
		///			MFG_NEAREST;
		///			MFG_LINEAR;
		/// </summary>
		mfgEnum magFilter;

		/// <summary>
		///		Mipmap filter (set to MFG_NONE to disable mipmaps).
		///		Valid values:
		///			MFG_NONE;
		///			MFG_NEAREST;
		///			MFG_LINEAR;
		/// </summary>
		mfgEnum mipmapFilter;

		/// <summary>
		///		Texture adress mode on coordinate U.
		///		Valid values:
		///			MFG_REPEAT;
		///			MFG_MIRROR;
		///			MFG_CLAMP;
		///			MFG_BORDER;
		/// </summary>
		mfgEnum adressU;

		/// <summary>
		///		Texture adress mode on coordinate V.
		///		Valid values:
		///			MFG_REPEAT;
		///			MFG_MIRROR;
		///			MFG_CLAMP;
		///			MFG_BORDER;
		/// </summary>
		mfgEnum adressV;

		/// <summary>
		///		Texture adress mode on coordinate W.
		///		Valid values:
		///			MFG_REPEAT;
		///			MFG_MIRROR;
		///			MFG_CLAMP;
		///			MFG_BORDER;
		/// </summary>
		mfgEnum adressW;
	} mfgSamplerDesc;

	void mfgDefaultSamplerDesc(mfgSamplerDesc* desc);

	typedef struct
	{
		/// <summary>
		///		Valid values:
		///			MFG_FALSE;
		///			MFG_TRUE;
		/// </summary>
		mfmBool cullEnabled;

		/// <summary>
		///		Valid values:
		///			MFG_CW;
		///			MFG_CCW;
		/// </summary>
		mfgEnum frontFace;

		/// <summary>
		///		Valid values:
		///			MFG_FRONT;
		///			MFG_BACK;
		///			MFG_FRONT_AND_BACK;
		/// </summary>
		mfgEnum cullFace;

		/// <summary>
		///		Valid values:
		///			MFG_FILL;
		///			MFG_WIREFRAME;
		/// </summary>
		mfgEnum rasterMode;
	} mfgRasterStateDesc;

	void mfgDefaultRasterStateDesc(mfgRasterStateDesc* desc);

	typedef struct
	{
		/// <summary>
		///		Performs depth checks?
		///		Valid values:
		///			MFG_FALSE;
		///			MFG_TRUE;
		/// </summary>
		mfmBool depthEnabled;

		/// <summary>
		///		Write depth?
		///		Valid values:
		///			MFG_FALSE;
		///			MFG_TRUE;
		/// </summary>
		mfmBool depthWriteEnabled;

		/// <summary>
		///		Depth 'near' value.
		/// </summary>
		mfmF32 depthNear;

		/// <summary>
		///		Depth 'far' value.
		/// </summary>
		mfmF32 depthFar;

		/// <summary>
		///		Depth comparison function.
		///		Valid values:
		///			MFG_NEVER;
		///			MFG_LESS;
		///			MFG_LEQUAL;
		///			MFG_GREATER;
		///			MFG_GEQUAL;
		///			MFG_EQUAL;
		///			MFG_NEQUAL;
		///			MFG_ALWAYS;
		/// </summary>
		mfgEnum depthCompare;

		mfmU32 stencilRef;

		/// <summary>
		///		Is stencil enabled?
		///		Valid values:
		///			MFG_FALSE;
		///			MFG_TRUE;
		/// </summary>
		mfmBool stencilEnabled;
		mfmU8 stencilReadMask;
		mfmU8 stencilWriteMask;

		/// <summary>
		///		Front face stencil comparison function.
		///		Valid values:
		///			MFG_NEVER;
		///			MFG_LESS;
		///			MFG_LEQUAL;
		///			MFG_GREATER;
		///			MFG_GEQUAL;
		///			MFG_EQUAL;
		///			MFG_NEQUAL;
		///			MFG_ALWAYS;
		/// </summary>
		mfgEnum frontFaceStencilCompare;

		/// <summary>
		///		Front face steincil fail action.
		///		Valid values:
		///			MFG_KEEP;
		///			MFG_ZERO;
		///			MFG_REPLACE;
		///			MFG_INCREMENT;
		///			MFG_INCREMENT_WRAP;
		///			MFG_DECREMENT;
		///			MFG_DECREMENT_WRAP;
		///			MFG_INVERT;
		/// </summary>
		mfgEnum frontFaceStencilFail;

		/// <summary>
		///		Front face steincil pass action.
		///		Valid values:
		///			MFG_KEEP;
		///			MFG_ZERO;
		///			MFG_REPLACE;
		///			MFG_INCREMENT;
		///			MFG_INCREMENT_WRAP;
		///			MFG_DECREMENT;
		///			MFG_DECREMENT_WRAP;
		///			MFG_INVERT;
		/// </summary>
		mfgEnum frontFaceStencilPass;

		/// <summary>
		///		Front face stencil depth fail action.
		///		Valid values:
		///			MFG_KEEP;
		///			MFG_ZERO;
		///			MFG_REPLACE;
		///			MFG_INCREMENT;
		///			MFG_INCREMENT_WRAP;
		///			MFG_DECREMENT;
		///			MFG_DECREMENT_WRAP;
		///			MFG_INVERT;
		/// </summary>
		mfgEnum frontFaceDepthFail;

		/// <summary>
		///		Back face stencil comparison function.
		///		Valid values:
		///			MFG_NEVER;
		///			MFG_LESS;
		///			MFG_LEQUAL;
		///			MFG_GREATER;
		///			MFG_GEQUAL;
		///			MFG_EQUAL;
		///			MFG_NEQUAL;
		///			MFG_ALWAYS;
		/// </summary>
		mfgEnum backFaceStencilCompare;

		/// <summary>
		///		Back face steincil fail action.
		///		Valid values:
		///			MFG_KEEP;
		///			MFG_ZERO;
		///			MFG_REPLACE;
		///			MFG_INCREMENT;
		///			MFG_INCREMENT_WRAP;
		///			MFG_DECREMENT;
		///			MFG_DECREMENT_WRAP;
		///			MFG_INVERT;
		/// </summary>
		mfgEnum backFaceStencilFail;

		/// <summary>
		///		Back face steincil pass action.
		///		Valid values:
		///			MFG_KEEP;
		///			MFG_ZERO;
		///			MFG_REPLACE;
		///			MFG_INCREMENT;
		///			MFG_INCREMENT_WRAP;
		///			MFG_DECREMENT;
		///			MFG_DECREMENT_WRAP;
		///			MFG_INVERT;
		/// </summary>
		mfgEnum backFaceStencilPass;

		/// <summary>
		///		Back face stencil depth fail action.
		///		Valid values:
		///			MFG_KEEP;
		///			MFG_ZERO;
		///			MFG_REPLACE;
		///			MFG_INCREMENT;
		///			MFG_INCREMENT_WRAP;
		///			MFG_DECREMENT;
		///			MFG_DECREMENT_WRAP;
		///			MFG_INVERT;
		/// </summary>
		mfgEnum backFaceDepthFail;

	} mfgDepthStencilStateDesc;

	void mfgDefaultDepthStencilStateDesc(mfgDepthStencilStateDesc* desc);

	typedef struct
	{
		/// <summary>
		///		Is blending enabled?
		///		Valid values:
		///			MFG_FALSE;
		///			MFG_TRUE;
		/// </summary>
		mfmBool blendEnabled;

		/// <summary>
		///		Source color factor.
		///		Valid values:
		///			MFG_ZERO;
		///			MFG_ONE;
		///			MFG_SRC_COLOR;
		///			MFG_INV_SRC_COLOR;
		///			MFG_DST_COLOR;
		///			MFG_INV_DST_COLOR;
		///			MFG_SRC_ALPHA;
		///			MFG_INV_SRC_ALPHA;
		///			MFG_DST_ALPHA;
		///			MFG_INV_DST_ALPHA;
		/// </summary>
		mfgEnum sourceFactor;

		/// <summary>
		///		Destination color factor.
		///		Valid values:
		///			MFG_ZERO;
		///			MFG_ONE;
		///			MFG_SRC_COLOR;
		///			MFG_INV_SRC_COLOR;
		///			MFG_DST_COLOR;
		///			MFG_INV_DST_COLOR;
		///			MFG_SRC_ALPHA;
		///			MFG_INV_SRC_ALPHA;
		///			MFG_DST_ALPHA;
		///			MFG_INV_DST_ALPHA;
		/// </summary>
		mfgEnum destinationFactor;

		/// <summary>
		///		Color blend operation.
		///		Valid values:
		///			MFG_ADD;
		///			MFG_SUBTRACT;
		///			MFG_REV_SUBTRACT;
		///			MFG_MIN;
		///			MFG_MAX;
		/// </summary>
		mfgEnum blendOperation;

		/// <summary>
		///		Source alpha factor.
		///		Valid values:
		///			MFG_ZERO;
		///			MFG_ONE;
		///			MFG_SRC_COLOR;
		///			MFG_INV_SRC_COLOR;
		///			MFG_DST_COLOR;
		///			MFG_INV_DST_COLOR;
		///			MFG_SRC_ALPHA;
		///			MFG_INV_SRC_ALPHA;
		///			MFG_DST_ALPHA;
		///			MFG_INV_DST_ALPHA;
		/// </summary>
		mfgEnum sourceAlphaFactor;

		/// <summary>
		///		Destination alpha factor.
		///		Valid values:
		///			MFG_ZERO;
		///			MFG_ONE;
		///			MFG_SRC_COLOR;
		///			MFG_INV_SRC_COLOR;
		///			MFG_DST_COLOR;
		///			MFG_INV_DST_COLOR;
		///			MFG_SRC_ALPHA;
		///			MFG_INV_SRC_ALPHA;
		///			MFG_DST_ALPHA;
		///			MFG_INV_DST_ALPHA;
		/// </summary>
		mfgEnum destinationAlphaFactor;

		/// <summary>
		///		Color alpha operation.
		///		Valid values:
		///			MFG_ADD;
		///			MFG_SUBTRACT;
		///			MFG_REV_SUBTRACT;
		///			MFG_MIN;
		///			MFG_MAX;
		/// </summary>
		mfgEnum blendAlphaOperation;
	} mfgBlendStateDesc;

	void mfgDefaultBlendStateDesc(mfgBlendStateDesc* desc);

	// Shader functions
	typedef mfgError(*mfgRDCreateVertexShaderFunction)(mfgRenderDevice* rd, mfgVertexShader** vs, const mfmU8* bc, mfmU64 bcSize, const mfgMetaData* md);
	typedef void(*mfgRDDestroyVertexShaderFunction)(void* vs);
	typedef mfgError(*mfgRDCreatePixelShaderFunction)(mfgRenderDevice* rd, mfgPixelShader** ps, const mfmU8* bc, mfmU64 bcSize, const mfgMetaData* md);
	typedef void(*mfgRDDestroyPixelShaderFunction)(void* ps);
	typedef mfgError(*mfgRDCreatePipelineFunction)(mfgRenderDevice* rd, mfgPipeline** pp, mfgVertexShader* vs, mfgPixelShader* ps);
	typedef void(*mfgRDDestroyPipelineFunction)(void* pp);
	typedef mfgError(*mfgRDSetPipelineFunction)(mfgRenderDevice* rd, mfgPipeline* pp);

	typedef mfgError(*mfgRDGetVertexShaderBindingPointFunction)(mfgRenderDevice* rd, mfgBindingPoint** bp, mfgVertexShader* vs, const mfsUTF8CodeUnit* name);
	typedef mfgError(*mfgRDGetPixelShaderBindingPointFunction)(mfgRenderDevice* rd, mfgBindingPoint** bp, mfgPixelShader* ps, const mfsUTF8CodeUnit* name);
	typedef mfgError(*mfgRDBindConstantBufferFunction)(mfgRenderDevice* rd, mfgBindingPoint* bp, mfgConstantBuffer* cb);
	typedef mfgError(*mfgRDBindConstantBufferRangeFunction)(mfgRenderDevice* rd, mfgBindingPoint* bp, mfgConstantBuffer* cb, mfmU64 offset, mfmU64 size);
	typedef mfgError(*mfgRDBindTexture1DFunction)(mfgRenderDevice* rd, mfgBindingPoint* bp, mfgTexture1D* tex);
	typedef mfgError(*mfgRDBindTexture2DFunction)(mfgRenderDevice* rd, mfgBindingPoint* bp, mfgTexture2D* tex);
	typedef mfgError(*mfgRDBindTexture3DFunction)(mfgRenderDevice* rd, mfgBindingPoint* bp, mfgTexture3D* tex);
	typedef mfgError(*mfgRDBindRenderTextureFunction)(mfgRenderDevice* rd, mfgBindingPoint* bp, mfgRenderTexture* tex);
	typedef mfgError(*mfgRDBindSamplerFunction)(mfgRenderDevice* rd, mfgBindingPoint* bp, mfgSampler* sampler);

	typedef mfgError(*mfgRDCreateConstantBufferFunction)(mfgRenderDevice* rd, mfgConstantBuffer** cb, mfmU64 size, const void* data, mfgEnum usage);
	typedef void(*mfgRDDestroyConstantBufferFunction)(void* cb);
	typedef mfgError(*mfgRDMapConstantBufferFunction)(mfgRenderDevice* rd, mfgConstantBuffer* cb, void** memory);
	typedef mfgError(*mfgRDUnmapConstantBufferFunction)(mfgRenderDevice* rd, mfgConstantBuffer* cb);

	// Vertex functions
	typedef mfgError(*mfgRDCreateVertexBufferFunction)(mfgRenderDevice* rd, mfgVertexBuffer** vb, mfmU64 size, const void* data, mfgEnum usage);
	typedef void(*mfgRDDestroyVertexBufferFunction)(void* vb);
	typedef mfgError(*mfgRDMapVertexBufferFunction)(mfgRenderDevice* rd, mfgVertexBuffer* vb, void** memory);
	typedef mfgError(*mfgRDUnmapVertexBufferFunction)(mfgRenderDevice* rd, mfgVertexBuffer* vb);
	typedef mfgError(*mfgRDCreateVertexLayoutFunction)(mfgRenderDevice* rd, mfgVertexLayout** vl, mfmU64 elementCount, const mfgVertexElement* elements, mfgVertexShader* vs);
	typedef void(*mfgRDDestroyVertexLayoutFunction)(void* vl);
	typedef mfgError(*mfgRDCreateVertexArrayFunction)(mfgRenderDevice* rd, mfgVertexArray** va, mfmU64 vbCount, mfgVertexBuffer** vbs, mfgVertexLayout* vl);
	typedef void(*mfgRDDestroyVertexArrayFunction)(void* va);
	typedef mfgError(*mfgRDSetVertexArrayFunction)(mfgRenderDevice* rd, mfgVertexArray* va);
	typedef mfgError(*mfgRDCreateIndexBufferFunction)(mfgRenderDevice* rd, mfgIndexBuffer** ib, mfmU64 size, const void* data, mfgEnum format, mfgEnum usage);
	typedef void(*mfgRDDestroyIndexBufferFunction)(void* ib);
	typedef mfgError(*mfgRDSetIndexBufferFunction)(mfgRenderDevice* rd, mfgIndexBuffer* ib);
	typedef mfgError(*mfgRDMapIndexBufferFunction)(mfgRenderDevice* rd, mfgIndexBuffer* ib, void** memory);
	typedef mfgError(*mfgRDUnmapIndexBufferFunction)(mfgRenderDevice* rd, mfgIndexBuffer* ib);

	// Texture functions
	typedef mfgError(*mfgRDCreateTexture1DFunction)(mfgRenderDevice* rd, mfgTexture1D** tex, mfmU64 width, mfgEnum format, const void* data, mfgEnum usage);
	typedef void(*mfgRDDestroyTexture1DFunction)(void* tex);
	typedef mfgError(*mfgRDUpdateTexture1DFunction)(mfgRenderDevice* rd, mfgTexture1D* tex, mfmU64 dstX, mfmU64 width, const void* data);
	typedef mfgError(*mfgRDGenerateTexture1DMipmapsFunction)(mfgRenderDevice* rd, mfgTexture1D* tex);

	typedef mfgError(*mfgRDCreateTexture2DFunction)(mfgRenderDevice* rd, mfgTexture2D** tex, mfmU64 width, mfmU64 height, mfgEnum format, const void* data, mfgEnum usage);
	typedef void(*mfgRDDestroyTexture2DFunction)(void* tex);
	typedef mfgError(*mfgRDUpdateTexture2DFunction)(mfgRenderDevice* rd, mfgTexture2D* tex, mfmU64 dstX, mfmU64 dstY, mfmU64 width, mfmU64 height, const void* data);
	typedef mfgError(*mfgRDGenerateTexture2DMipmapsFunction)(mfgRenderDevice* rd, mfgTexture2D* tex);

	typedef mfgError(*mfgRDCreateTexture3DFunction)(mfgRenderDevice* rd, mfgTexture3D** tex, mfmU64 width, mfmU64 height, mfmU64 depth, mfgEnum format, const void* data, mfgEnum usage);
	typedef void(*mfgRDDestroyTexture3DFunction)(void* tex);
	typedef mfgError(*mfgRDUpdateTexture3DFunction)(mfgRenderDevice* rd, mfgTexture3D* tex, mfmU64 dstX, mfmU64 dstY, mfmU64 dstZ, mfmU64 width, mfmU64 height, mfmU64 depth, const void* data);
	typedef mfgError(*mfgRDGenerateTexture3DMipmapsFunction)(mfgRenderDevice* rd, mfgTexture3D* tex);

	typedef mfgError(*mfgRDCreateSamplerFunction)(mfgRenderDevice* rd, mfgSampler** sampler, const mfgSamplerDesc* desc);
	typedef void(*mfgRDDestroySamplerFunction)(void* sampler);

	// Raster/depth stencil/blend state functions
	typedef mfgError(*mfgRDCreateRasterStateFunction)(mfgRenderDevice* rd, mfgRasterState** state, const mfgRasterStateDesc* desc);
	typedef void(*mfgRDDestroyRasterStateFunction)(void* state);
	typedef mfgError(*mfgRDSetRasterStateFunction)(mfgRenderDevice* rd, mfgRasterState* state);
	typedef mfgError(*mfgRDCreateDepthStencilStateFunction)(mfgRenderDevice* rd, mfgDepthStencilState** state, const mfgDepthStencilStateDesc* desc);
	typedef void(*mfgRDDestroyDepthStencilFunction)(void* state);
	typedef mfgError(*mfgRDSetDepthStencilFunction)(mfgRenderDevice* rd, mfgDepthStencilState* state);
	typedef mfgError(*mfgRDCreateBlendStateFunction)(mfgRenderDevice* rd, mfgBlendState** state, const mfgBlendStateDesc* desc);
	typedef void(*mfgRDDestroyBlendStateFunction)(void* state);
	typedef mfgError(*mfgRDSetBlendStateFunction)(mfgRenderDevice* rd, mfgBlendState* state);

	// Framebuffer functions
	typedef mfgError(*mfgRDCreateRenderTextureFunction)(mfgRenderDevice* rd, mfgRenderTexture** tex, mfmU64 width, mfmU64 height, mfgEnum format);
	typedef void(*mfgRDDestroyRenderTextureFunction)(void* tex);
	typedef mfgError(*mfgRDCreateDepthStencilTextureFunction)(mfgRenderDevice* rd, mfgDepthStencilTexture** tex, mfmU64 width, mfmU64 height, mfgEnum format);
	typedef void(*mfgRDDestroyDepthStencilTextureFunction)(void* tex);
	typedef mfgError(*mfgRDCreateFramebufferFunction)(mfgRenderDevice* rd, mfgFramebuffer** fb, mfmU64 textureCount, mfgRenderTexture** textures, mfgDepthStencilTexture* depthStencilTexture);
	typedef void(*mfgRDDestroyFramebufferFunction)(void* fb);
	typedef mfgError(*mfgRDSetFramebufferFunction)(mfgRenderDevice* rd, mfgFramebuffer* fb);

	// Draw functions
	typedef mfgError(*mfgRDClearColorFunction)(mfgRenderDevice* rd, mfmF32 r, mfmF32 g, mfmF32 b, mfmF32 a);
	typedef mfgError(*mfgRDClearDepthFunction)(mfgRenderDevice* rd, mfmF32 depth);
	typedef mfgError(*mfgRDClearStencilFunction)(mfgRenderDevice* rd, mfmI32 stencil);
	typedef mfgError(*mfgRDDrawTrianglesFunction)(mfgRenderDevice* rd, mfmU64 offset, mfmU64 count);
	typedef mfgError(*mfgRDDrawTrianglesIndexedFunction)(mfgRenderDevice* rd, mfmU64 offset, mfmU64 count);
	typedef mfgError(*mfgRDSwapBuffersFunction)(mfgRenderDevice* rd);

	// Getter functions
	typedef mfgError(*mfgRDGetPropertyI)(mfgRenderDevice* rd, mfgEnum propID, mfmI32* value);
	typedef mfgError(*mfgRDGetPropertyF)(mfgRenderDevice* rd, mfgEnum propID, mfmF32* value);

	// Error functions
	typedef mfmBool(*mfgRDGetErrorString)(mfgRenderDevice* rd, mfsUTF8CodeUnit* str, mfmU64 maxSize);

	struct mfgRenderDevice
	{
		mfmObject object;

		mfgRDCreateVertexShaderFunction createVertexShader;
		mfgRDDestroyVertexShaderFunction destroyVertexShader;
		mfgRDCreatePixelShaderFunction createPixelShader;
		mfgRDDestroyPixelShaderFunction destroyPixelShader;
		mfgRDCreatePipelineFunction createPipeline;
		mfgRDDestroyPipelineFunction destroyPipeline;
		mfgRDSetPipelineFunction setPipeline;

		mfgRDGetVertexShaderBindingPointFunction getVertexShaderBindingPoint;
		mfgRDGetPixelShaderBindingPointFunction getPixelShaderBindingPoint;
		mfgRDBindConstantBufferFunction bindConstantBuffer;
		mfgRDBindConstantBufferRangeFunction bindConstantBufferRange;
		mfgRDBindTexture1DFunction bindTexture1D;
		mfgRDBindTexture2DFunction bindTexture2D;
		mfgRDBindTexture3DFunction bindTexture3D;
		mfgRDBindRenderTextureFunction bindRenderTexture;
		mfgRDBindSamplerFunction bindSampler;

		mfgRDCreateConstantBufferFunction createConstantBuffer;
		mfgRDDestroyConstantBufferFunction destroyConstantBuffer;
		mfgRDMapConstantBufferFunction mapConstantBuffer;
		mfgRDUnmapConstantBufferFunction unmapConstantBuffer;

		mfgRDCreateVertexBufferFunction createVertexBuffer;
		mfgRDDestroyVertexBufferFunction destroyVertexBuffer;
		mfgRDMapVertexBufferFunction mapVertexBuffer;
		mfgRDUnmapVertexBufferFunction unmapVertexBuffer;
		mfgRDCreateVertexLayoutFunction createVertexLayout;
		mfgRDDestroyVertexLayoutFunction destroyVertexLayout;
		mfgRDCreateVertexArrayFunction createVertexArray;
		mfgRDDestroyVertexArrayFunction destroyVertexArray;
		mfgRDSetVertexArrayFunction setVertexArray;
		mfgRDCreateIndexBufferFunction createIndexBuffer;
		mfgRDDestroyIndexBufferFunction destroyIndexBuffer;
		mfgRDSetIndexBufferFunction setIndexBuffer;
		mfgRDMapIndexBufferFunction mapIndexBuffer;
		mfgRDUnmapIndexBufferFunction unmapIndexBuffer;

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

		mfgRDCreateRasterStateFunction createRasterState;
		mfgRDDestroyRasterStateFunction destroyRasterState;
		mfgRDSetRasterStateFunction setRasterState;
		mfgRDCreateDepthStencilStateFunction createDepthStencilState;
		mfgRDDestroyDepthStencilFunction destroyDepthStencilState;
		mfgRDSetDepthStencilFunction setDepthStencilState;
		mfgRDCreateBlendStateFunction createBlendState;
		mfgRDDestroyBlendStateFunction destroyBlendState;
		mfgRDSetBlendStateFunction setBlendState;

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

		mfgRDGetPropertyI getPropertyI;
		mfgRDGetPropertyF getPropertyF;

		mfgRDGetErrorString getErrorString;
	};

	/// <summary>
	///		Creates a new vertex shader.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="vertexShader">Pointer to vertex shader handle</param>
	/// <param name="bytecode">Shader bytecode</param>
	/// <param name="bytecodeSize">Shader bytecode size</param>
	/// <param name="metaData">Shader meta data</param>
	/// <returns>
	///		MFG_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfgError mfgCreateVertexShader(mfgRenderDevice* rd, mfgVertexShader** vertexShader, const mfmU8* bytecode, mfmU64 bytecodeSize, const mfgMetaData* metaData);

	/// <summary>
	///		Destroys a vertex shader.
	/// </summary>
	/// <param name="vertexShader">Vertex shader handle</param>
	void mfgDestroyVertexShader(void* vertexShader);

	/// <summary>
	///		Creates a new pixel shader.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="pixelShader">Pointer to pixel shader handle</param>
	/// <param name="bytecode">Shader bytecode</param>
	/// <param name="bytecodeSize">Shader bytecode size</param>
	/// <param name="metaData">Shader meta data</param>
	/// <returns>
	///		MFG_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfgError mfgCreatePixelShader(mfgRenderDevice* rd, mfgPixelShader** pixelShader, const mfmU8* bytecode, mfmU64 bytecodeSize, const mfgMetaData* metaData);

	/// <summary>
	///		Destroys a pixel shader.
	/// </summary>
	/// <param name="pixelShader">Pixel shader handle</param>
	void mfgDestroyPixelShader(void* pixelShader);

	/// <summary>
	///		Creates a new pipeline.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="pipeline">Pointer to pipeline handle</param>
	/// <param name="vs">Vertex shader handle</param>
	/// <param name="ps">Pixel shader handle</param>
	/// <returns>
	///		MFG_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfgError mfgCreatePipeline(mfgRenderDevice* rd, mfgPipeline** pipeline, mfgVertexShader* vs, mfgPixelShader* ps);

	/// <summary>
	///		Destroys a pipeline.
	/// </summary>
	/// <param name="pipeline">Pipeline handle</param>
	void mfgDestroyPipeline(void* pipeline);

	/// <summary>
	///		Sets a pipeline as the one used for rendering.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="pipeline">Pipeline handle</param>
	/// <returns>
	///		MFG_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfgError mfgSetPipeline(mfgRenderDevice* rd, mfgPipeline* pipeline);

	/// <summary>
	///		Gets a vertex shader binding point.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="bp">Pointer to binding point handle</param>
	/// <param name="vertexShader">Vertex shader handle</param>
	/// <param name="name">Binding point name</param>
	/// <returns>
	///		MFG_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfgError mfgGetVertexShaderBindingPoint(mfgRenderDevice* rd, mfgBindingPoint** bp, mfgVertexShader* vertexShader, const mfsUTF8CodeUnit* name);

	/// <summary>
	///		Gets a pixel shader binding point.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="bp">Pointer to binding point handle</param>
	/// <param name="pixelShader">Pixel shader handle</param>
	/// <param name="name">Binding point name</param>
	/// <returns>
	///		MFG_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfgError mfgGetPixelShaderBindingPoint(mfgRenderDevice* rd, mfgBindingPoint** bp, mfgPixelShader* pixelShader, const mfsUTF8CodeUnit* name);

	/// <summary>
	///		Binds a constant buffer to a binding point.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="bp">Binding point handle</param>
	/// <param name="cb">Constant buffer handle</param>
	/// <returns>
	///		MFG_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfgError mfgBindConstantBuffer(mfgRenderDevice* rd, mfgBindingPoint* bp, mfgConstantBuffer* cb);

	/// <summary>
	///		Binds a section of a constant buffer to a binding point.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="bp">Binding point handle</param>
	/// <param name="cb">Constant buffer handle</param>
	/// <param name="offset">Starting memory offset in the buffer (in shader constants, one shader constant = 16 bytes, the number of constants must be a multiple of MFG_CONSTANT_ALIGN)</param>
	/// <param name="size">Memory size in the buffer (in shader constants, one shader constant = 16 bytes, the number of constants must be a multiple of MFG_CONSTANT_ALIGN)</param>
	/// <returns>
	///		MFG_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfgError mfgBindConstantBufferRange(mfgRenderDevice* rd, mfgBindingPoint* bp, mfgConstantBuffer* cb, mfmU64 offset, mfmU64 size);

	/// <summary>
	///		Binds a texture 1D to a binding point.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="bp">Binding point handle</param>
	/// <param name="tex">Texture handle</param>
	/// <returns>
	///		MFG_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfgError mfgBindTexture1D(mfgRenderDevice* rd, mfgBindingPoint* bp, mfgTexture1D* tex);

	/// <summary>
	///		Binds a texture 2D to a binding point.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="bp">Binding point handle</param>
	/// <param name="tex">Texture handle</param>
	/// <returns>
	///		MFG_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfgError mfgBindTexture2D(mfgRenderDevice* rd, mfgBindingPoint* bp, mfgTexture2D* tex);

	/// <summary>
	///		Binds a texture 3D to a binding point.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="bp">Binding point handle</param>
	/// <param name="tex">Texture handle</param>
	/// <returns>
	///		MFG_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfgError mfgBindTexture3D(mfgRenderDevice* rd, mfgBindingPoint* bp, mfgTexture3D* tex);

	/// <summary>
	///		Binds a render texture to a binding point.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="bp">Binding point handle</param>
	/// <param name="tex">Texture handle</param>
	/// <returns>
	///		MFG_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfgError mfgBindRenderTexture(mfgRenderDevice* rd, mfgBindingPoint* bp, mfgRenderTexture* tex);

	/// <summary>
	///		Binds a sampler to a binding point.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="bp">Binding point handle</param>
	/// <param name="sampler">Sampler handle</param>
	/// <returns>
	///		MFG_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfgError mfgBindSampler(mfgRenderDevice* rd, mfgBindingPoint* bp, mfgSampler* sampler);

	/// <summary>
	///		Creates a new constant buffer.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="cb">Pointer to constant buffer handle</param>
	/// <param name="size">Constant buffer size</param>
	/// <param name="data">Constant buffer initial data (set to NULL to create empty buffer, only works if the usage isn't set to MFG_STATIC)</param>
	/// <param name="usage">Constant buffer usage mode (valid: MFG_DEFAULT; MFG_DYNAMIC; MFG_STATIC)</param>
	/// <returns>
	///		MFG_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfgError mfgCreateConstantBuffer(mfgRenderDevice* rd, mfgConstantBuffer** cb, mfmU64 size, const void* data, mfgEnum usage);
	
	/// <summary>
	///		Destroys a constant buffer.
	/// </summary>
	/// <param name="cb">Constant buffer handle</param>
	void mfgDestroyConstantBuffer(void* cb);

	/// <summary>
	///		Maps the constant buffer data to a accessible memory location.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="cb">Constant buffer handle</param>
	/// <param name="memory">Pointer to memory pointer</param>
	/// <returns>
	///		MFG_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfgError mfgMapConstantBuffer(mfgRenderDevice* rd, mfgConstantBuffer* cb, void** memory);

	/// <summary>
	///		Unmaps the constant buffer pointer data.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="cb">Constant buffer handle</param>
	/// <returns>
	///		MFG_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfgError mfgUnmapConstantBuffer(mfgRenderDevice* rd, mfgConstantBuffer* cb);

	/// <summary>
	///		Creates a new vertex buffer.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="vb">Pointer to vertex buffer handle</param>
	/// <param name="size">Vertex buffer size</param>
	/// <param name="data">Vertex buffer initial data (set to NULL to create empty buffer, only works if the usage isn't set to MFG_STATIC)</param>
	/// <param name="usage">Vertex buffer usage mode (valid: MFG_DEFAULT; MFG_DYNAMIC; MFG_STATIC)</param>
	/// <returns>
	///		MFG_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfgError mfgCreateVertexBuffer(mfgRenderDevice* rd, mfgVertexBuffer** vb, mfmU64 size, const void* data, mfgEnum usage);
	
	/// <summary>
	///		Destroys a vertex buffer.
	/// </summary>
	/// <param name="vb">Vertex buffer handle</param>
	void mfgDestroyVertexBuffer(void* vb);

	/// <summary>
	///		Maps the vertex buffer data to a accessible memory location.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="vb">Vertex buffer handle</param>
	/// <param name="memory">Pointer to memory pointer</param>
	/// <returns>
	///		MFG_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfgError mfgMapVertexBuffer(mfgRenderDevice* rd, mfgVertexBuffer* vb, void** memory);

	/// <summary>
	///		Unmaps the vertex buffer pointer data.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="vb">Vertex buffer handle</param>
	/// <returns>
	///		MFG_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfgError mfgUnmapVertexBuffer(mfgRenderDevice* rd, mfgVertexBuffer* vb);

	/// <summary>
	///		Creates a new vertex layout.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="vl">Pointer to vertex layout handle</param>
	/// <param name="elementCount">Vertex element count</param>
	/// <param name="elements">Vertex elements array pointer</param>
	/// <param name="vs">Vertex shader handle</param>
	/// <returns>
	///		MFG_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfgError mfgCreateVertexLayout(mfgRenderDevice* rd, mfgVertexLayout** vl, mfmU64 elementCount, const mfgVertexElement* elements, mfgVertexShader* vs);

	/// <summary>
	///		Destroys a vertex layout.
	/// </summary>
	/// <param name="vb">Vertex layout handle</param>
	void mfgDestroyVertexLayout(void* vl);

	/// <summary>
	///		Creates a new vertex array.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="va">Pointer to vertex array handle</param>
	/// <param name="vbCount">Vertex buffer count</param>
	/// <param name="vbs">Vertex buffers array pointer</param>
	/// <param name="vl">Vertex layout handle</param>
	/// <returns>
	///		MFG_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfgError mfgCreateVertexArray(mfgRenderDevice* rd, mfgVertexArray** va, mfmU64 vbCount, mfgVertexBuffer** vbs, mfgVertexLayout* vl);

	/// <summary>
	///		Destroys a vertex array.
	/// </summary>
	/// <param name="va">Vertex array handle</param>
	void mfgDestroyVertexArray(void* va);
	
	/// <summary>
	///		Sets a vertex array as the one used in draw calls.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="va">Vertex array handle</param>
	/// <returns>
	///		MFG_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfgError mfgSetVertexArray(mfgRenderDevice* rd, mfgVertexArray* va);

	/// <summary>
	///		Creates a new index buffer.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="ib">Pointer to index buffer handle</param>
	/// <param name="size">Index buffer size</param>
	/// <param name="data">Index buffer initial data (set to NULL to create empty buffer, only works if the usage isn't set to MFG_STATIC)</param>
	/// <param name="format">Index data format (valid: MFG_USHORT; MFG_UINT)</param>
	/// <param name="usage">Index buffer usage mode (valid: MFG_DEFAULT; MFG_DYNAMIC; MFG_STATIC)</param>
	/// <returns>
	///		MFG_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfgError mfgCreateIndexBuffer(mfgRenderDevice* rd, mfgIndexBuffer** ib, mfmU64 size, const void* data, mfgEnum format, mfgEnum usage);

	/// <summary>
	///		Destroys an index buffer.
	/// </summary>
	/// <param name="ib">Index buffer handle</param>
	void mfgDestroyIndexBuffer(void* ib);

	/// <summary>
	///		Maps the index buffer data to a accessible memory location.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="ib">Index buffer handle</param>
	/// <param name="memory">Pointer to memory pointer</param>
	/// <returns>
	///		MFG_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfgError mfgMapIndexBuffer(mfgRenderDevice* rd, mfgIndexBuffer* ib, void** memory);

	/// <summary>
	///		Unmaps the index buffer pointer data.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="ib">Index buffer handle</param>
	/// <returns>
	///		MFG_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfgError mfgUnmapIndexBuffer(mfgRenderDevice* rd, mfgIndexBuffer* ib);

	/// <summary>
	///		Sets the index buffer to use on draw calls.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="ib">Index buffer handle</param>
	/// <returns>
	///		MFG_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfgError mfgSetIndexBuffer(mfgRenderDevice* rd, mfgIndexBuffer* ib);

	/// <summary>
	///		Creates a new texture 1D.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="tex">Pointer to texture handle</param>
	/// <param name="width">Texture width</param>
	/// <param name="format">Texture data format</param>
	/// <param name="data">Texture initial data (set to NULL to create empty texture, only works if the usage isn't set to MFG_STATIC)</param>
	/// <param name="usage">Texture usage mode (valid: MFG_DEFAULT; MFG_DYNAMIC; MFG_STATIC)</param>
	/// <returns>
	///		MFG_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfgError mfgCreateTexture1D(mfgRenderDevice* rd, mfgTexture1D** tex, mfmU64 width, mfgEnum format, const void* data, mfgEnum usage);

	/// <summary>
	///		Destroys a texture 1D.
	/// </summary>
	/// <param name="tex">Texture handle</param>
	void mfgDestroyTexture1D(void* tex);

	/// <summary>
	///		Updates a texture 1D.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="tex">Texture handle</param>
	/// <param name="dstX">Update destination X coordinate</param>
	/// <param name="width">Update data width</param>
	/// <param name="data">Update data</param>
	/// <returns>
	///		MFG_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfgError mfgUpdateTexture1D(mfgRenderDevice* rd, mfgTexture1D* tex, mfmU64 dstX, mfmU64 width, const void* data);

	/// <summary>
	///		Generatesd mipmaps a texture 1D.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="tex">Texture handle</param>
	/// <returns>
	///		MFG_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfgError mfgGenerateTexture1DMipmaps(mfgRenderDevice* rd, mfgTexture1D* tex);

	/// <summary>
	///		Creates a new texture 2D.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="tex">Pointer to texture handle</param>
	/// <param name="width">Texture width</param>
	/// <param name="height">Texture height</param>
	/// <param name="format">Texture data format</param>
	/// <param name="data">Texture initial data (set to NULL to create empty texture, only works if the usage isn't set to MFG_STATIC)</param>
	/// <param name="usage">Texture usage mode (valid: MFG_DEFAULT; MFG_DYNAMIC; MFG_STATIC)</param>
	/// <returns>
	///		MFG_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfgError mfgCreateTexture2D(mfgRenderDevice* rd, mfgTexture2D** tex, mfmU64 width, mfmU64 height, mfgEnum format, const void* data, mfgEnum usage);

	/// <summary>
	///		Destroys a texture 2D.
	/// </summary>
	/// <param name="tex">Texture handle</param>
	void mfgDestroyTexture2D(void* tex);

	/// <summary>
	///		Updates a texture 2D.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="tex">Texture handle</param>
	/// <param name="dstX">Update destination X coordinate</param>
	/// <param name="dstY">Update destination Y coordinate</param>
	/// <param name="width">Update data width</param>
	/// <param name="height">Update data height</param>
	/// <param name="data">Update data</param>
	/// <returns>
	///		MFG_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfgError mfgUpdateTexture2D(mfgRenderDevice* rd, mfgTexture2D* tex, mfmU64 dstX, mfmU64 dstY, mfmU64 width, mfmU64 height, const void* data);

	/// <summary>
	///		Generates mipmaps a texture 2D.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="tex">Texture handle</param>
	/// <returns>
	///		MFG_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfgError mfgGenerateTexture2DMipmaps(mfgRenderDevice* rd, mfgTexture2D* tex);

	/// <summary>
	///		Creates a new texture 3D.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="tex">Pointer to texture handle</param>
	/// <param name="width">Texture width</param>
	/// <param name="height">Texture height</param>
	/// <param name="depth">Texture depth</param>
	/// <param name="format">Texture data format</param>
	/// <param name="data">Texture initial data (set to NULL to create empty texture, only works if the usage isn't set to MFG_STATIC)</param>
	/// <param name="usage">Texture usage mode (valid: MFG_DEFAULT; MFG_DYNAMIC; MFG_STATIC)</param>
	/// <returns>
	///		MFG_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfgError mfgCreateTexture3D(mfgRenderDevice* rd, mfgTexture3D** tex, mfmU64 width, mfmU64 height, mfmU64 depth, mfgEnum format, const void* data, mfgEnum usage);

	/// <summary>
	///		Destroys a texture 3D.
	/// </summary>
	/// <param name="tex">Texture handle</param>
	void mfgDestroyTexture3D(void* tex);

	/// <summary>
	///		Updates a texture 3D.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="tex">Texture handle</param>
	/// <param name="dstX">Update destination X coordinate</param>
	/// <param name="dstY">Update destination Y coordinate</param>
	/// <param name="dstZ">Update destination Z coordinate</param>
	/// <param name="width">Update data width</param>
	/// <param name="height">Update data height</param>
	/// <param name="depth">Update data depth</param>
	/// <param name="data">Update data</param>
	/// <returns>
	///		MFG_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfgError mfgUpdateTexture3D(mfgRenderDevice * rd, mfgTexture3D * tex, mfmU64 dstX, mfmU64 dstY, mfmU64 dstZ, mfmU64 width, mfmU64 height, mfmU64 depth, const void * data);

	/// <summary>
	///		Generates mipmaps on a texture 3D.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="tex">Texture handle</param>
	/// <returns>
	///		MFG_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfgError mfgGenerateTexture3DMipmaps(mfgRenderDevice* rd, mfgTexture3D* tex);

	/// <summary>
	///		Creates a new sampler.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="sampler">Pointer to sampler handle</param>
	/// <param name="desc">Sampler description</param>
	/// <returns>
	///		MFG_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfgError mfgCreateSampler(mfgRenderDevice* rd, mfgSampler** sampler, const mfgSamplerDesc* desc);

	/// <summary>
	///		Destroys a sampler.
	/// </summary>
	/// <param name="sampler">Sampler handle</param>
	void mfgDestroySampler(void* sampler);

	/// <summary>
	///		Creates a new rasterizer state.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="state">Pointer to raster state handle</param>
	/// <param name="desc">State description</param>
	/// <returns>
	///		MFG_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfgError mfgCreateRasterState(mfgRenderDevice* rd, mfgRasterState** state, const mfgRasterStateDesc* desc);

	/// <summary>
	///		Destroys a rasterizer state.
	/// </summary>
	/// <param name="state">Raster state handle</param>
	void mfgDestroyRasterState(void* state);

	/// <summary>
	///		Sets a rasterizer state as the one used for draw calls.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="state">Raster state handle</param>
	/// <returns>
	///		MFG_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfgError mfgSetRasterState(mfgRenderDevice* rd, mfgRasterState* state);

	/// <summary>
	///		Creates a new depth stencil state.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="state">Pointer to depth stencil state handle</param>
	/// <param name="desc">State description</param>
	/// <returns>
	///		MFG_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfgError mfgCreateDepthStencilState(mfgRenderDevice* rd, mfgDepthStencilState** state, const mfgDepthStencilStateDesc* desc);

	/// <summary>
	///		Destroys a depth stencil state.
	/// </summary>
	/// <param name="state">Depth stencil state handle</param>
	void mfgDestroyDepthStencilState(void* state);

	/// <summary>
	///		Sets a depth stencil state as the one used for draw calls.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="state">Depth stencil state handle</param>
	/// <returns>
	///		MFG_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfgError mfgSetDepthStencilState(mfgRenderDevice* rd, mfgDepthStencilState* state);

	/// <summary>
	///		Creates a new blend state.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="state">Pointer to blend state handle</param>
	/// <param name="desc">State description</param>
	/// <returns>
	///		MFG_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfgError mfgCreateBlendState(mfgRenderDevice* rd, mfgBlendState** state, const mfgBlendStateDesc* desc);

	/// <summary>
	///		Destroys a blend state.
	/// </summary>
	/// <param name="state">Blend state handle</param>
	void mfgDestroyBlendState(void* state);

	/// <summary>
	///		Sets a blend state as the one used for draw calls.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="state">Blend state handle</param>
	/// <returns>
	///		MFG_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfgError mfgSetBlendState(mfgRenderDevice* rd, mfgBlendState* state);

	/// <summary>
	///		Creates a new render texture.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="tex">Pointer to texture handle</param>
	/// <param name="width">Texture width</param>
	/// <param name="height">Texture height</param>
	/// <param name="format">Texture data format</param>
	/// <returns>
	///		MFG_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfgError mfgCreateRenderTexture(mfgRenderDevice* rd, mfgRenderTexture** tex, mfmU64 width, mfmU64 height, mfgEnum format);

	/// <summary>
	///		Destroys a render texture.
	/// </summary>
	/// <param name="tex">Texture handle</param>
	void mfgDestroyRenderTexture(void* tex);

	/// <summary>
	///		Creates a new depth stencil texture.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="tex">Pointer to texture handle</param>
	/// <param name="width">Texture width</param>
	/// <param name="height">Texture height</param>
	/// <param name="format">Texture data format</param>
	/// <returns>
	///		MFG_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfgError mfgCreateDepthStencilTexture(mfgRenderDevice* rd, mfgDepthStencilTexture** tex, mfmU64 width, mfmU64 height, mfgEnum format);

	/// <summary>
	///		Destroys a depth stencil texture.
	/// </summary>
	/// <param name="tex">Texture handle</param>
	void mfgDestroyDepthStencilTexture(void* tex);

	/// <summary>
	///		Creates a new framebuffer.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="state">Pointer to framebuffer handle</param>
	/// <param name="textureCount">Number of texture attachments</param>
	/// <param name="texture">Texture attachments array</param>
	/// <param name="depthStencilTexture">Depth stencil texture (optional, can be set to NULL)</param>
	/// <returns>
	///		MFG_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfgError mfgCreateFramebuffer(mfgRenderDevice* rd, mfgFramebuffer** fb, mfmU64 textureCount, mfgRenderTexture** textures, mfgDepthStencilTexture* depthStencilTexture);

	/// <summary>
	///		Destroys a framebuffer.
	/// </summary>
	/// <param name="fb">Framebuffer handle</param>
	void mfgDestroyFramebuffer(void* fb);

	/// <summary>
	///		Sets a framebuffer as the one used for draw calls.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="fb">Framebuffer handle</param>
	/// <returns>
	///		MFG_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfgError mfgSetFramebuffer(mfgRenderDevice* rd, mfgFramebuffer* fb);

	/// <summary>
	///		Clears the current framebuffer color buffers.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="r">Color red component</param>
	/// <param name="g">Color green component</param>
	/// <param name="b">Color blue component</param>
	/// <param name="a">Color alpha component</param>
	/// <returns>
	///		MFG_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfgError mfgClearColor(mfgRenderDevice* rd, mfmF32 r, mfmF32 g, mfmF32 b, mfmF32 a);

	/// <summary>
	///		Clears the current framebuffer depth buffer.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="depth">Depth value</param>
	/// <returns>
	///		MFG_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfgError mfgClearDepth(mfgRenderDevice* rd, mfmF32 depth);

	/// <summary>
	///		Clears the current framebuffer stencil buffer.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="stencil">Stencil value</param>
	/// <returns>
	///		MFG_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfgError mfgClearStencil(mfgRenderDevice* rd, mfmI32 stencil);

	/// <summary>
	///		Draws the triangles stored in the currently active vertex array using the currently active pipeline.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="offset">First vertex offset</param>
	/// <param name="count">Vertex count</param>
	/// <returns>
	///		MFG_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfgError mfgDrawTriangles(mfgRenderDevice* rd, mfmU64 offset, mfmU64 count);

	/// <summary>
	///		Draws the triangles stored in the currently active vertex array using the currently active pipeline and index buffer.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="offset">First index offset</param>
	/// <param name="count">Index count</param>
	/// <returns>
	///		MFG_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfgError mfgDrawTrianglesIndexed(mfgRenderDevice* rd, mfmU64 offset, mfmU64 count);

	/// <summary>
	///		Swaps the front and back buffers (displays the stuff drawn until this point).
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <returns>
	///		MFG_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfgError mfgSwapBuffers(mfgRenderDevice* rd);

	/// <summary>
	///		Gets a render device integer property.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="id">Property identifier</param>
	/// <param name="value">Output property value</param>
	/// <returns>
	///		MFG_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfgError mfgGetPropertyI(mfgRenderDevice* rd, mfgEnum id, mfmI32* value);

	/// <summary>
	///		Gets a render device floating point property.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="id">Property identifier</param>
	/// <param name="value">Output property value</param>
	/// <returns>
	///		MFG_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfgError mfgGetPropertyF(mfgRenderDevice* rd, mfgEnum id, mfmF32* value);

	/// <summary>
	///		Gets the last error string.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="str">Output error string</param>
	/// <param name="maxSize">Maximum error string size</param>
	/// <returns>
	///		True if there was an error, otherwise false.
	/// </returns>
	mfmBool mfgGetErrorString(mfgRenderDevice* rd, mfsUTF8CodeUnit* str, mfmU64 maxSize);

#ifdef __cplusplus
}
#endif
