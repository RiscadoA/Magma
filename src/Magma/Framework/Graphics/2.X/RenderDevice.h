#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "../Error.h"
#include "Bytecode.h"

#include "../../Memory/Object.h"
#include "../../String/UTF8.h"

#include "../../Input/Window.h"

#define MFG_MAX_RENDER_DEVICE_CREATOR_REGISTER_ENTRIES 16

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
	
	typedef struct mfgV2XRenderDevice mfgV2XRenderDevice;

	typedef struct
	{
		mfmObject object;
		mfgV2XRenderDevice* renderDevice;
	} mfgV2XRenderDeviceObject;

	typedef mfgV2XRenderDeviceObject mfgV2XVertexShader;
	typedef mfgV2XRenderDeviceObject mfgV2XPixelShader;
	typedef mfgV2XRenderDeviceObject mfgV2XPipeline;
	typedef mfgV2XRenderDeviceObject mfgV2XBindingPoint;
	typedef mfgV2XRenderDeviceObject mfgV2XConstantBuffer;
	typedef mfgV2XRenderDeviceObject mfgV2XTexture1D;
	typedef mfgV2XRenderDeviceObject mfgV2XTexture2D;
	typedef mfgV2XRenderDeviceObject mfgV2XTexture3D;
	typedef mfgV2XRenderDeviceObject mfgV2XSampler;
	typedef mfgV2XRenderDeviceObject mfgV2XVertexBuffer;
	typedef mfgV2XRenderDeviceObject mfgV2XVertexLayout;
	typedef mfgV2XRenderDeviceObject mfgV2XVertexArray;
	typedef mfgV2XRenderDeviceObject mfgV2XIndexBuffer;
	typedef mfgV2XRenderDeviceObject mfgV2XRasterState;
	typedef mfgV2XRenderDeviceObject mfgV2XDepthStencilState;
	typedef mfgV2XRenderDeviceObject mfgV2XBlendState;
	typedef mfgV2XRenderDeviceObject mfgV2XRenderTexture;
	typedef mfgV2XRenderDeviceObject mfgV2XDepthStencilTexture;
	typedef mfgV2XRenderDeviceObject mfgV2XFramebuffer;

	typedef struct
	{
		/// <summary>
		///		Valid values:
		///			MFG_FALSE;
		///			MFG_TRUE;
		/// </summary>
		mfmBool vsyncEnabled;
	} mfgV2XRenderDeviceDesc;

	void mfgV2XDefaultRenderDeviceDesc(mfgV2XRenderDeviceDesc* desc);

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
		///		Offset of the first element in the buffer.
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
		mfmU64 size;
	} mfgV2XVertexElement;

	void mfgV2XDefaultVertexElement(mfgV2XVertexElement* element);

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
		mfgEnum addressU;

		/// <summary>
		///		Texture adress mode on coordinate V.
		///		Valid values:
		///			MFG_REPEAT;
		///			MFG_MIRROR;
		///			MFG_CLAMP;
		///			MFG_BORDER;
		/// </summary>
		mfgEnum addressV;

		/// <summary>
		///		Texture adress mode on coordinate W.
		///		Valid values:
		///			MFG_REPEAT;
		///			MFG_MIRROR;
		///			MFG_CLAMP;
		///			MFG_BORDER;
		/// </summary>
		mfgEnum addressW;
	} mfgV2XSamplerDesc;

	void mfgV2XDefaultSamplerDesc(mfgV2XSamplerDesc* desc);

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
	} mfgV2XRasterStateDesc;

	void mfgV2XDefaultRasterStateDesc(mfgV2XRasterStateDesc* desc);

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

		/// <summary>
		///		Stencil ref value.
		/// </summary>
		mfmU32 stencilRef;

		/// <summary>
		///		Is stencil enabled?
		///		Valid values:
		///			MFG_FALSE;
		///			MFG_TRUE;
		/// </summary>
		mfmBool stencilEnabled;

		/// <summary>
		///		Stencil read mask.
		/// </summary>
		mfmU8 stencilReadMask;

		/// <summary>
		///		Stencil write mask.
		/// </summary>
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

	} mfgV2XDepthStencilStateDesc;

	void mfgV2XDefaultDepthStencilStateDesc(mfgV2XDepthStencilStateDesc* desc);

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
	} mfgV2XBlendStateDesc;

	void mfgV2XDefaultBlendStateDesc(mfgV2XBlendStateDesc* desc);

	// Shader functions
	typedef mfError(*mfgV2XRDCreateVertexShaderFunction)(mfgV2XRenderDevice* rd, mfgV2XVertexShader** vs, const mfmU8* bc, mfmU64 bcSize, const mfgMetaData* md);
	typedef void(*mfgV2XRDDestroyVertexShaderFunction)(void* vs);
	typedef mfError(*mfgV2XRDCreatePixelShaderFunction)(mfgV2XRenderDevice* rd, mfgV2XPixelShader** ps, const mfmU8* bc, mfmU64 bcSize, const mfgMetaData* md);
	typedef void(*mfgV2XRDDestroyPixelShaderFunction)(void* ps);
	typedef mfError(*mfgV2XRDCreatePipelineFunction)(mfgV2XRenderDevice* rd, mfgV2XPipeline** pp, mfgV2XVertexShader* vs, mfgV2XPixelShader* ps);
	typedef void(*mfgV2XRDDestroyPipelineFunction)(void* pp);
	typedef mfError(*mfgV2XRDSetPipelineFunction)(mfgV2XRenderDevice* rd, mfgV2XPipeline* pp);

	typedef mfError(*mfgV2XRDGetVertexShaderBindingPointFunction)(mfgV2XRenderDevice* rd, mfgV2XBindingPoint** bp, mfgV2XVertexShader* vs, const mfsUTF8CodeUnit* name);
	typedef mfError(*mfgV2XRDGetPixelShaderBindingPointFunction)(mfgV2XRenderDevice* rd, mfgV2XBindingPoint** bp, mfgV2XPixelShader* ps, const mfsUTF8CodeUnit* name);
	typedef mfError(*mfgV2XRDBindConstantBufferFunction)(mfgV2XRenderDevice* rd, mfgV2XBindingPoint* bp, mfgV2XConstantBuffer* cb);
	typedef mfError(*mfgV2XRDBindConstantBufferRangeFunction)(mfgV2XRenderDevice* rd, mfgV2XBindingPoint* bp, mfgV2XConstantBuffer* cb, mfmU64 offset, mfmU64 size);
	typedef mfError(*mfgV2XRDBindTexture1DFunction)(mfgV2XRenderDevice* rd, mfgV2XBindingPoint* bp, mfgV2XTexture1D* tex);
	typedef mfError(*mfgV2XRDBindTexture2DFunction)(mfgV2XRenderDevice* rd, mfgV2XBindingPoint* bp, mfgV2XTexture2D* tex);
	typedef mfError(*mfgV2XRDBindTexture3DFunction)(mfgV2XRenderDevice* rd, mfgV2XBindingPoint* bp, mfgV2XTexture3D* tex);
	typedef mfError(*mfgV2XRDBindRenderTextureFunction)(mfgV2XRenderDevice* rd, mfgV2XBindingPoint* bp, mfgV2XRenderTexture* tex);
	typedef mfError(*mfgV2XRDBindSamplerFunction)(mfgV2XRenderDevice* rd, mfgV2XBindingPoint* bp, mfgV2XSampler* sampler);

	typedef mfError(*mfgV2XRDCreateConstantBufferFunction)(mfgV2XRenderDevice* rd, mfgV2XConstantBuffer** cb, mfmU64 size, const void* data, mfgEnum usage);
	typedef void(*mfgV2XRDDestroyConstantBufferFunction)(void* cb);
	typedef mfError(*mfgV2XRDMapConstantBufferFunction)(mfgV2XRenderDevice* rd, mfgV2XConstantBuffer* cb, void** memory);
	typedef mfError(*mfgV2XRDUnmapConstantBufferFunction)(mfgV2XRenderDevice* rd, mfgV2XConstantBuffer* cb);

	// Vertex functions
	typedef mfError(*mfgV2XRDCreateVertexBufferFunction)(mfgV2XRenderDevice* rd, mfgV2XVertexBuffer** vb, mfmU64 size, const void* data, mfgEnum usage);
	typedef void(*mfgV2XRDDestroyVertexBufferFunction)(void* vb);
	typedef mfError(*mfgV2XRDMapVertexBufferFunction)(mfgV2XRenderDevice* rd, mfgV2XVertexBuffer* vb, void** memory);
	typedef mfError(*mfgV2XRDUnmapVertexBufferFunction)(mfgV2XRenderDevice* rd, mfgV2XVertexBuffer* vb);
	typedef mfError(*mfgV2XRDCreateVertexLayoutFunction)(mfgV2XRenderDevice* rd, mfgV2XVertexLayout** vl, mfmU64 elementCount, const mfgV2XVertexElement* elements, mfgV2XVertexShader* vs);
	typedef void(*mfgV2XRDDestroyVertexLayoutFunction)(void* vl);
	typedef mfError(*mfgV2XRDCreateVertexArrayFunction)(mfgV2XRenderDevice* rd, mfgV2XVertexArray** va, mfmU64 vbCount, mfgV2XVertexBuffer** vbs, mfgV2XVertexLayout* vl);
	typedef void(*mfgV2XRDDestroyVertexArrayFunction)(void* va);
	typedef mfError(*mfgV2XRDSetVertexArrayFunction)(mfgV2XRenderDevice* rd, mfgV2XVertexArray* va);
	typedef mfError(*mfgV2XRDCreateIndexBufferFunction)(mfgV2XRenderDevice* rd, mfgV2XIndexBuffer** ib, mfmU64 size, const void* data, mfgEnum format, mfgEnum usage);
	typedef void(*mfgV2XRDDestroyIndexBufferFunction)(void* ib);
	typedef mfError(*mfgV2XRDSetIndexBufferFunction)(mfgV2XRenderDevice* rd, mfgV2XIndexBuffer* ib);
	typedef mfError(*mfgV2XRDMapIndexBufferFunction)(mfgV2XRenderDevice* rd, mfgV2XIndexBuffer* ib, void** memory);
	typedef mfError(*mfgV2XRDUnmapIndexBufferFunction)(mfgV2XRenderDevice* rd, mfgV2XIndexBuffer* ib);

	// Texture functions
	typedef mfError(*mfgV2XRDCreateTexture1DFunction)(mfgV2XRenderDevice* rd, mfgV2XTexture1D** tex, mfmU64 width, mfgEnum format, const void* data, mfgEnum usage);
	typedef void(*mfgV2XRDDestroyTexture1DFunction)(void* tex);
	typedef mfError(*mfgV2XRDUpdateTexture1DFunction)(mfgV2XRenderDevice* rd, mfgV2XTexture1D* tex, mfmU64 dstX, mfmU64 width, const void* data);
	typedef mfError(*mfgV2XRDGenerateTexture1DMipmapsFunction)(mfgV2XRenderDevice* rd, mfgV2XTexture1D* tex);

	typedef mfError(*mfgV2XRDCreateTexture2DFunction)(mfgV2XRenderDevice* rd, mfgV2XTexture2D** tex, mfmU64 width, mfmU64 height, mfgEnum format, const void* data, mfgEnum usage);
	typedef void(*mfgV2XRDDestroyTexture2DFunction)(void* tex);
	typedef mfError(*mfgV2XRDUpdateTexture2DFunction)(mfgV2XRenderDevice* rd, mfgV2XTexture2D* tex, mfmU64 dstX, mfmU64 dstY, mfmU64 width, mfmU64 height, const void* data);
	typedef mfError(*mfgV2XRDGenerateTexture2DMipmapsFunction)(mfgV2XRenderDevice* rd, mfgV2XTexture2D* tex);

	typedef mfError(*mfgV2XRDCreateTexture3DFunction)(mfgV2XRenderDevice* rd, mfgV2XTexture3D** tex, mfmU64 width, mfmU64 height, mfmU64 depth, mfgEnum format, const void* data, mfgEnum usage);
	typedef void(*mfgV2XRDDestroyTexture3DFunction)(void* tex);
	typedef mfError(*mfgV2XRDUpdateTexture3DFunction)(mfgV2XRenderDevice* rd, mfgV2XTexture3D* tex, mfmU64 dstX, mfmU64 dstY, mfmU64 dstZ, mfmU64 width, mfmU64 height, mfmU64 depth, const void* data);
	typedef mfError(*mfgV2XRDGenerateTexture3DMipmapsFunction)(mfgV2XRenderDevice* rd, mfgV2XTexture3D* tex);

	typedef mfError(*mfgV2XRDCreateSamplerFunction)(mfgV2XRenderDevice* rd, mfgV2XSampler** sampler, const mfgV2XSamplerDesc* desc);
	typedef void(*mfgV2XRDDestroySamplerFunction)(void* sampler);

	// Raster/depth stencil/blend state functions
	typedef mfError(*mfgV2XRDCreateRasterStateFunction)(mfgV2XRenderDevice* rd, mfgV2XRasterState** state, const mfgV2XRasterStateDesc* desc);
	typedef void(*mfgV2XRDDestroyRasterStateFunction)(void* state);
	typedef mfError(*mfgV2XRDSetRasterStateFunction)(mfgV2XRenderDevice* rd, mfgV2XRasterState* state);
	typedef mfError(*mfgV2XRDCreateDepthStencilStateFunction)(mfgV2XRenderDevice* rd, mfgV2XDepthStencilState** state, const mfgV2XDepthStencilStateDesc* desc);
	typedef void(*mfgV2XRDDestroyDepthStencilFunction)(void* state);
	typedef mfError(*mfgV2XRDSetDepthStencilFunction)(mfgV2XRenderDevice* rd, mfgV2XDepthStencilState* state);
	typedef mfError(*mfgV2XRDCreateBlendStateFunction)(mfgV2XRenderDevice* rd, mfgV2XBlendState** state, const mfgV2XBlendStateDesc* desc);
	typedef void(*mfgV2XRDDestroyBlendStateFunction)(void* state);
	typedef mfError(*mfgV2XRDSetBlendStateFunction)(mfgV2XRenderDevice* rd, mfgV2XBlendState* state);

	// Framebuffer functions
	typedef mfError(*mfgV2XRDCreateRenderTextureFunction)(mfgV2XRenderDevice* rd, mfgV2XRenderTexture** tex, mfmU64 width, mfmU64 height, mfgEnum format);
	typedef void(*mfgV2XRDDestroyRenderTextureFunction)(void* tex);
	typedef mfError(*mfgV2XRDCreateDepthStencilTextureFunction)(mfgV2XRenderDevice* rd, mfgV2XDepthStencilTexture** tex, mfmU64 width, mfmU64 height, mfgEnum format);
	typedef void(*mfgV2XRDDestroyDepthStencilTextureFunction)(void* tex);
	typedef mfError(*mfgV2XRDCreateFramebufferFunction)(mfgV2XRenderDevice* rd, mfgV2XFramebuffer** fb, mfmU64 textureCount, mfgV2XRenderTexture** textures, mfgV2XDepthStencilTexture* depthStencilTexture);
	typedef void(*mfgV2XRDDestroyFramebufferFunction)(void* fb);
	typedef mfError(*mfgV2XRDSetFramebufferFunction)(mfgV2XRenderDevice* rd, mfgV2XFramebuffer* fb);

	// Draw functions
	typedef mfError(*mfgV2XRDClearColorFunction)(mfgV2XRenderDevice* rd, mfmF32 r, mfmF32 g, mfmF32 b, mfmF32 a);
	typedef mfError(*mfgV2XRDClearDepthFunction)(mfgV2XRenderDevice* rd, mfmF32 depth);
	typedef mfError(*mfgV2XRDClearStencilFunction)(mfgV2XRenderDevice* rd, mfmI32 stencil);
	typedef mfError(*mfgV2XRDDrawTrianglesFunction)(mfgV2XRenderDevice* rd, mfmU64 offset, mfmU64 count);
	typedef mfError(*mfgV2XRDDrawTrianglesIndexedFunction)(mfgV2XRenderDevice* rd, mfmU64 offset, mfmU64 count);
	typedef mfError(*mfgV2XRDSwapBuffersFunction)(mfgV2XRenderDevice* rd);

	// Getter functions
	typedef mfError(*mfgV2XRDGetPropertyI)(mfgV2XRenderDevice* rd, mfgEnum propID, mfmI32* value);
	typedef mfError(*mfgV2XRDGetPropertyF)(mfgV2XRenderDevice* rd, mfgEnum propID, mfmF32* value);

	// Error functions
	typedef mfmBool(*mfgV2XRDGetErrorString)(mfgV2XRenderDevice* rd, mfsUTF8CodeUnit* str, mfmU64 maxSize);

	struct mfgV2XRenderDevice
	{
		mfmObject object;

		mfgV2XRDCreateVertexShaderFunction createVertexShader;
		mfgV2XRDDestroyVertexShaderFunction destroyVertexShader;
		mfgV2XRDCreatePixelShaderFunction createPixelShader;
		mfgV2XRDDestroyPixelShaderFunction destroyPixelShader;
		mfgV2XRDCreatePipelineFunction createPipeline;
		mfgV2XRDDestroyPipelineFunction destroyPipeline;
		mfgV2XRDSetPipelineFunction setPipeline;

		mfgV2XRDGetVertexShaderBindingPointFunction getVertexShaderBindingPoint;
		mfgV2XRDGetPixelShaderBindingPointFunction getPixelShaderBindingPoint;
		mfgV2XRDBindConstantBufferFunction bindConstantBuffer;
		mfgV2XRDBindConstantBufferRangeFunction bindConstantBufferRange;
		mfgV2XRDBindTexture1DFunction bindTexture1D;
		mfgV2XRDBindTexture2DFunction bindTexture2D;
		mfgV2XRDBindTexture3DFunction bindTexture3D;
		mfgV2XRDBindRenderTextureFunction bindRenderTexture;
		mfgV2XRDBindSamplerFunction bindSampler;

		mfgV2XRDCreateConstantBufferFunction createConstantBuffer;
		mfgV2XRDDestroyConstantBufferFunction destroyConstantBuffer;
		mfgV2XRDMapConstantBufferFunction mapConstantBuffer;
		mfgV2XRDUnmapConstantBufferFunction unmapConstantBuffer;

		mfgV2XRDCreateVertexBufferFunction createVertexBuffer;
		mfgV2XRDDestroyVertexBufferFunction destroyVertexBuffer;
		mfgV2XRDMapVertexBufferFunction mapVertexBuffer;
		mfgV2XRDUnmapVertexBufferFunction unmapVertexBuffer;
		mfgV2XRDCreateVertexLayoutFunction createVertexLayout;
		mfgV2XRDDestroyVertexLayoutFunction destroyVertexLayout;
		mfgV2XRDCreateVertexArrayFunction createVertexArray;
		mfgV2XRDDestroyVertexArrayFunction destroyVertexArray;
		mfgV2XRDSetVertexArrayFunction setVertexArray;
		mfgV2XRDCreateIndexBufferFunction createIndexBuffer;
		mfgV2XRDDestroyIndexBufferFunction destroyIndexBuffer;
		mfgV2XRDSetIndexBufferFunction setIndexBuffer;
		mfgV2XRDMapIndexBufferFunction mapIndexBuffer;
		mfgV2XRDUnmapIndexBufferFunction unmapIndexBuffer;

		mfgV2XRDCreateTexture1DFunction createTexture1D;
		mfgV2XRDDestroyTexture1DFunction destroyTexture1D;
		mfgV2XRDUpdateTexture1DFunction updateTexture1D;
		mfgV2XRDGenerateTexture1DMipmapsFunction generateTexture1DMipmaps;

		mfgV2XRDCreateTexture2DFunction createTexture2D;
		mfgV2XRDDestroyTexture2DFunction destroyTexture2D;
		mfgV2XRDUpdateTexture2DFunction updateTexture2D;
		mfgV2XRDGenerateTexture2DMipmapsFunction generateTexture2DMipmaps;

		mfgV2XRDCreateTexture3DFunction createTexture3D;
		mfgV2XRDDestroyTexture3DFunction destroyTexture3D;
		mfgV2XRDUpdateTexture3DFunction updateTexture3D;
		mfgV2XRDGenerateTexture3DMipmapsFunction generateTexture3DMipmaps;

		mfgV2XRDCreateSamplerFunction createSampler;
		mfgV2XRDDestroySamplerFunction destroySampler;

		mfgV2XRDCreateRasterStateFunction createRasterState;
		mfgV2XRDDestroyRasterStateFunction destroyRasterState;
		mfgV2XRDSetRasterStateFunction setRasterState;
		mfgV2XRDCreateDepthStencilStateFunction createDepthStencilState;
		mfgV2XRDDestroyDepthStencilFunction destroyDepthStencilState;
		mfgV2XRDSetDepthStencilFunction setDepthStencilState;
		mfgV2XRDCreateBlendStateFunction createBlendState;
		mfgV2XRDDestroyBlendStateFunction destroyBlendState;
		mfgV2XRDSetBlendStateFunction setBlendState;

		mfgV2XRDCreateRenderTextureFunction createRenderTexture;
		mfgV2XRDDestroyRenderTextureFunction destroyRenderTexture;
		mfgV2XRDCreateDepthStencilTextureFunction createDepthStencilTexture;
		mfgV2XRDDestroyDepthStencilTextureFunction destroyDepthStencilTexture;
		mfgV2XRDCreateFramebufferFunction createFramebuffer;
		mfgV2XRDDestroyFramebufferFunction destroyFramebuffer;
		mfgV2XRDSetFramebufferFunction setFramebuffer;

		mfgV2XRDClearColorFunction clearColor;
		mfgV2XRDClearDepthFunction clearDepth;
		mfgV2XRDClearStencilFunction clearStencil;
		mfgV2XRDDrawTrianglesFunction drawTriangles;
		mfgV2XRDDrawTrianglesIndexedFunction drawTrianglesIndexed;
		mfgV2XRDSwapBuffersFunction swapBuffers;

		mfgV2XRDGetPropertyI getPropertyI;
		mfgV2XRDGetPropertyF getPropertyF;

		mfgV2XRDGetErrorString getErrorString;
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
	///		MF_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfError mfgV2XCreateVertexShader(mfgV2XRenderDevice* rd, mfgV2XVertexShader** vertexShader, const mfmU8* bytecode, mfmU64 bytecodeSize, const mfgMetaData* metaData);

	/// <summary>
	///		Destroys a vertex shader.
	/// </summary>
	/// <param name="vertexShader">Vertex shader handle</param>
	void mfgV2XDestroyVertexShader(void* vertexShader);

	/// <summary>
	///		Creates a new pixel shader.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="pixelShader">Pointer to pixel shader handle</param>
	/// <param name="bytecode">Shader bytecode</param>
	/// <param name="bytecodeSize">Shader bytecode size</param>
	/// <param name="metaData">Shader meta data</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfError mfgV2XCreatePixelShader(mfgV2XRenderDevice* rd, mfgV2XPixelShader** pixelShader, const mfmU8* bytecode, mfmU64 bytecodeSize, const mfgMetaData* metaData);

	/// <summary>
	///		Destroys a pixel shader.
	/// </summary>
	/// <param name="pixelShader">Pixel shader handle</param>
	void mfgV2XDestroyPixelShader(void* pixelShader);

	/// <summary>
	///		Creates a new pipeline.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="pipeline">Pointer to pipeline handle</param>
	/// <param name="vs">Vertex shader handle</param>
	/// <param name="ps">Pixel shader handle</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfError mfgV2XCreatePipeline(mfgV2XRenderDevice* rd, mfgV2XPipeline** pipeline, mfgV2XVertexShader* vs, mfgV2XPixelShader* ps);

	/// <summary>
	///		Destroys a pipeline.
	/// </summary>
	/// <param name="pipeline">Pipeline handle</param>
	void mfgV2XDestroyPipeline(void* pipeline);

	/// <summary>
	///		Sets a pipeline as the one used for rendering.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="pipeline">Pipeline handle</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfError mfgV2XSetPipeline(mfgV2XRenderDevice* rd, mfgV2XPipeline* pipeline);

	/// <summary>
	///		Gets a vertex shader binding point.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="bp">Pointer to binding point handle</param>
	/// <param name="vertexShader">Vertex shader handle</param>
	/// <param name="name">Binding point name</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfError mfgV2XGetVertexShaderBindingPoint(mfgV2XRenderDevice* rd, mfgV2XBindingPoint** bp, mfgV2XVertexShader* vertexShader, const mfsUTF8CodeUnit* name);

	/// <summary>
	///		Gets a pixel shader binding point.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="bp">Pointer to binding point handle</param>
	/// <param name="pixelShader">Pixel shader handle</param>
	/// <param name="name">Binding point name</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfError mfgV2XGetPixelShaderBindingPoint(mfgV2XRenderDevice* rd, mfgV2XBindingPoint** bp, mfgV2XPixelShader* pixelShader, const mfsUTF8CodeUnit* name);

	/// <summary>
	///		Binds a constant buffer to a binding point.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="bp">Binding point handle</param>
	/// <param name="cb">Constant buffer handle</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfError mfgV2XBindConstantBuffer(mfgV2XRenderDevice* rd, mfgV2XBindingPoint* bp, mfgV2XConstantBuffer* cb);

	/// <summary>
	///		Binds a section of a constant buffer to a binding point.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="bp">Binding point handle</param>
	/// <param name="cb">Constant buffer handle</param>
	/// <param name="offset">Starting memory offset in the buffer (in shader constants, one shader constant = 16 bytes, the number of constants must be a multiple of MFG_CONSTANT_ALIGN)</param>
	/// <param name="size">Memory size in the buffer (in shader constants, one shader constant = 16 bytes, the number of constants must be a multiple of MFG_CONSTANT_ALIGN)</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfError mfgV2XBindConstantBufferRange(mfgV2XRenderDevice* rd, mfgV2XBindingPoint* bp, mfgV2XConstantBuffer* cb, mfmU64 offset, mfmU64 size);

	/// <summary>
	///		Binds a texture 1D to a binding point.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="bp">Binding point handle</param>
	/// <param name="tex">Texture handle</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfError mfgV2XBindTexture1D(mfgV2XRenderDevice* rd, mfgV2XBindingPoint* bp, mfgV2XTexture1D* tex);

	/// <summary>
	///		Binds a texture 2D to a binding point.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="bp">Binding point handle</param>
	/// <param name="tex">Texture handle</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfError mfgV2XBindTexture2D(mfgV2XRenderDevice* rd, mfgV2XBindingPoint* bp, mfgV2XTexture2D* tex);

	/// <summary>
	///		Binds a texture 3D to a binding point.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="bp">Binding point handle</param>
	/// <param name="tex">Texture handle</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfError mfgV2XBindTexture3D(mfgV2XRenderDevice* rd, mfgV2XBindingPoint* bp, mfgV2XTexture3D* tex);

	/// <summary>
	///		Binds a render texture to a binding point.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="bp">Binding point handle</param>
	/// <param name="tex">Texture handle</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfError mfgV2XBindRenderTexture(mfgV2XRenderDevice* rd, mfgV2XBindingPoint* bp, mfgV2XRenderTexture* tex);

	/// <summary>
	///		Binds a sampler to a binding point.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="bp">Binding point handle</param>
	/// <param name="sampler">Sampler handle</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfError mfgV2XBindSampler(mfgV2XRenderDevice* rd, mfgV2XBindingPoint* bp, mfgV2XSampler* sampler);

	/// <summary>
	///		Creates a new constant buffer.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="cb">Pointer to constant buffer handle</param>
	/// <param name="size">Constant buffer size</param>
	/// <param name="data">Constant buffer initial data (set to NULL to create empty buffer, only works if the usage isn't set to MFG_STATIC)</param>
	/// <param name="usage">Constant buffer usage mode (valid: MFG_DEFAULT; MFG_DYNAMIC; MFG_STATIC)</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfError mfgV2XCreateConstantBuffer(mfgV2XRenderDevice* rd, mfgV2XConstantBuffer** cb, mfmU64 size, const void* data, mfgEnum usage);
	
	/// <summary>
	///		Destroys a constant buffer.
	/// </summary>
	/// <param name="cb">Constant buffer handle</param>
	void mfgV2XDestroyConstantBuffer(void* cb);

	/// <summary>
	///		Maps the constant buffer data to a accessible memory location.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="cb">Constant buffer handle</param>
	/// <param name="memory">Pointer to memory pointer</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfError mfgV2XMapConstantBuffer(mfgV2XRenderDevice* rd, mfgV2XConstantBuffer* cb, void** memory);

	/// <summary>
	///		Unmaps the constant buffer pointer data.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="cb">Constant buffer handle</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfError mfgV2XUnmapConstantBuffer(mfgV2XRenderDevice* rd, mfgV2XConstantBuffer* cb);

	/// <summary>
	///		Creates a new vertex buffer.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="vb">Pointer to vertex buffer handle</param>
	/// <param name="size">Vertex buffer size</param>
	/// <param name="data">Vertex buffer initial data (set to NULL to create empty buffer, only works if the usage isn't set to MFG_STATIC)</param>
	/// <param name="usage">Vertex buffer usage mode (valid: MFG_DEFAULT; MFG_DYNAMIC; MFG_STATIC)</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfError mfgV2XCreateVertexBuffer(mfgV2XRenderDevice* rd, mfgV2XVertexBuffer** vb, mfmU64 size, const void* data, mfgEnum usage);
	
	/// <summary>
	///		Destroys a vertex buffer.
	/// </summary>
	/// <param name="vb">Vertex buffer handle</param>
	void mfgV2XDestroyVertexBuffer(void* vb);

	/// <summary>
	///		Maps the vertex buffer data to a accessible memory location.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="vb">Vertex buffer handle</param>
	/// <param name="memory">Pointer to memory pointer</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfError mfgV2XMapVertexBuffer(mfgV2XRenderDevice* rd, mfgV2XVertexBuffer* vb, void** memory);

	/// <summary>
	///		Unmaps the vertex buffer pointer data.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="vb">Vertex buffer handle</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfError mfgV2XUnmapVertexBuffer(mfgV2XRenderDevice* rd, mfgV2XVertexBuffer* vb);

	/// <summary>
	///		Creates a new vertex layout.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="vl">Pointer to vertex layout handle</param>
	/// <param name="elementCount">Vertex element count</param>
	/// <param name="elements">Vertex elements array pointer</param>
	/// <param name="vs">Vertex shader handle</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfError mfgV2XCreateVertexLayout(mfgV2XRenderDevice* rd, mfgV2XVertexLayout** vl, mfmU64 elementCount, const mfgV2XVertexElement* elements, mfgV2XVertexShader* vs);

	/// <summary>
	///		Destroys a vertex layout.
	/// </summary>
	/// <param name="vb">Vertex layout handle</param>
	void mfgV2XDestroyVertexLayout(void* vl);

	/// <summary>
	///		Creates a new vertex array.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="va">Pointer to vertex array handle</param>
	/// <param name="vbCount">Vertex buffer count</param>
	/// <param name="vbs">Vertex buffers array pointer</param>
	/// <param name="vl">Vertex layout handle</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfError mfgV2XCreateVertexArray(mfgV2XRenderDevice* rd, mfgV2XVertexArray** va, mfmU64 vbCount, mfgV2XVertexBuffer** vbs, mfgV2XVertexLayout* vl);

	/// <summary>
	///		Destroys a vertex array.
	/// </summary>
	/// <param name="va">Vertex array handle</param>
	void mfgV2XDestroyVertexArray(void* va);
	
	/// <summary>
	///		Sets a vertex array as the one used in draw calls.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="va">Vertex array handle</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfError mfgV2XSetVertexArray(mfgV2XRenderDevice* rd, mfgV2XVertexArray* va);

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
	///		MF_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfError mfgV2XCreateIndexBuffer(mfgV2XRenderDevice* rd, mfgV2XIndexBuffer** ib, mfmU64 size, const void* data, mfgEnum format, mfgEnum usage);

	/// <summary>
	///		Destroys an index buffer.
	/// </summary>
	/// <param name="ib">Index buffer handle</param>
	void mfgV2XDestroyIndexBuffer(void* ib);

	/// <summary>
	///		Maps the index buffer data to a accessible memory location.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="ib">Index buffer handle</param>
	/// <param name="memory">Pointer to memory pointer</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfError mfgV2XMapIndexBuffer(mfgV2XRenderDevice* rd, mfgV2XIndexBuffer* ib, void** memory);

	/// <summary>
	///		Unmaps the index buffer pointer data.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="ib">Index buffer handle</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfError mfgV2XUnmapIndexBuffer(mfgV2XRenderDevice* rd, mfgV2XIndexBuffer* ib);

	/// <summary>
	///		Sets the index buffer to use on draw calls.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="ib">Index buffer handle</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfError mfgV2XSetIndexBuffer(mfgV2XRenderDevice* rd, mfgV2XIndexBuffer* ib);

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
	///		MF_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfError mfgV2XCreateTexture1D(mfgV2XRenderDevice* rd, mfgV2XTexture1D** tex, mfmU64 width, mfgEnum format, const void* data, mfgEnum usage);

	/// <summary>
	///		Destroys a texture 1D.
	/// </summary>
	/// <param name="tex">Texture handle</param>
	void mfgV2XDestroyTexture1D(void* tex);

	/// <summary>
	///		Updates a texture 1D.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="tex">Texture handle</param>
	/// <param name="dstX">Update destination X coordinate</param>
	/// <param name="width">Update data width</param>
	/// <param name="data">Update data</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfError mfgV2XUpdateTexture1D(mfgV2XRenderDevice* rd, mfgV2XTexture1D* tex, mfmU64 dstX, mfmU64 width, const void* data);

	/// <summary>
	///		Generatesd mipmaps a texture 1D.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="tex">Texture handle</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfError mfgV2XGenerateTexture1DMipmaps(mfgV2XRenderDevice* rd, mfgV2XTexture1D* tex);

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
	///		MF_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfError mfgV2XCreateTexture2D(mfgV2XRenderDevice* rd, mfgV2XTexture2D** tex, mfmU64 width, mfmU64 height, mfgEnum format, const void* data, mfgEnum usage);

	/// <summary>
	///		Destroys a texture 2D.
	/// </summary>
	/// <param name="tex">Texture handle</param>
	void mfgV2XDestroyTexture2D(void* tex);

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
	///		MF_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfError mfgV2XUpdateTexture2D(mfgV2XRenderDevice* rd, mfgV2XTexture2D* tex, mfmU64 dstX, mfmU64 dstY, mfmU64 width, mfmU64 height, const void* data);

	/// <summary>
	///		Generates mipmaps a texture 2D.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="tex">Texture handle</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfError mfgV2XGenerateTexture2DMipmaps(mfgV2XRenderDevice* rd, mfgV2XTexture2D* tex);

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
	///		MF_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfError mfgV2XCreateTexture3D(mfgV2XRenderDevice* rd, mfgV2XTexture3D** tex, mfmU64 width, mfmU64 height, mfmU64 depth, mfgEnum format, const void* data, mfgEnum usage);

	/// <summary>
	///		Destroys a texture 3D.
	/// </summary>
	/// <param name="tex">Texture handle</param>
	void mfgV2XDestroyTexture3D(void* tex);

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
	///		MF_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfError mfgV2XUpdateTexture3D(mfgV2XRenderDevice * rd, mfgV2XTexture3D * tex, mfmU64 dstX, mfmU64 dstY, mfmU64 dstZ, mfmU64 width, mfmU64 height, mfmU64 depth, const void * data);

	/// <summary>
	///		Generates mipmaps on a texture 3D.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="tex">Texture handle</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfError mfgV2XGenerateTexture3DMipmaps(mfgV2XRenderDevice* rd, mfgV2XTexture3D* tex);

	/// <summary>
	///		Creates a new sampler.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="sampler">Pointer to sampler handle</param>
	/// <param name="desc">Sampler description</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfError mfgV2XCreateSampler(mfgV2XRenderDevice* rd, mfgV2XSampler** sampler, const mfgV2XSamplerDesc* desc);

	/// <summary>
	///		Destroys a sampler.
	/// </summary>
	/// <param name="sampler">Sampler handle</param>
	void mfgV2XDestroySampler(void* sampler);

	/// <summary>
	///		Creates a new rasterizer state.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="state">Pointer to raster state handle</param>
	/// <param name="desc">State description</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfError mfgV2XCreateRasterState(mfgV2XRenderDevice* rd, mfgV2XRasterState** state, const mfgV2XRasterStateDesc* desc);

	/// <summary>
	///		Destroys a rasterizer state.
	/// </summary>
	/// <param name="state">Raster state handle</param>
	void mfgV2XDestroyRasterState(void* state);

	/// <summary>
	///		Sets a rasterizer state as the one used for draw calls.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="state">Raster state handle</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfError mfgV2XSetRasterState(mfgV2XRenderDevice* rd, mfgV2XRasterState* state);

	/// <summary>
	///		Creates a new depth stencil state.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="state">Pointer to depth stencil state handle</param>
	/// <param name="desc">State description</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfError mfgV2XCreateDepthStencilState(mfgV2XRenderDevice* rd, mfgV2XDepthStencilState** state, const mfgV2XDepthStencilStateDesc* desc);

	/// <summary>
	///		Destroys a depth stencil state.
	/// </summary>
	/// <param name="state">Depth stencil state handle</param>
	void mfgV2XDestroyDepthStencilState(void* state);

	/// <summary>
	///		Sets a depth stencil state as the one used for draw calls.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="state">Depth stencil state handle</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfError mfgV2XSetDepthStencilState(mfgV2XRenderDevice* rd, mfgV2XDepthStencilState* state);

	/// <summary>
	///		Creates a new blend state.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="state">Pointer to blend state handle</param>
	/// <param name="desc">State description</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfError mfgV2XCreateBlendState(mfgV2XRenderDevice* rd, mfgV2XBlendState** state, const mfgV2XBlendStateDesc* desc);

	/// <summary>
	///		Destroys a blend state.
	/// </summary>
	/// <param name="state">Blend state handle</param>
	void mfgV2XDestroyBlendState(void* state);

	/// <summary>
	///		Sets a blend state as the one used for draw calls.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="state">Blend state handle</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfError mfgV2XSetBlendState(mfgV2XRenderDevice* rd, mfgV2XBlendState* state);

	/// <summary>
	///		Creates a new render texture.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="tex">Pointer to texture handle</param>
	/// <param name="width">Texture width</param>
	/// <param name="height">Texture height</param>
	/// <param name="format">Texture data format</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfError mfgV2XCreateRenderTexture(mfgV2XRenderDevice* rd, mfgV2XRenderTexture** tex, mfmU64 width, mfmU64 height, mfgEnum format);

	/// <summary>
	///		Destroys a render texture.
	/// </summary>
	/// <param name="tex">Texture handle</param>
	void mfgV2XDestroyRenderTexture(void* tex);

	/// <summary>
	///		Creates a new depth stencil texture.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="tex">Pointer to texture handle</param>
	/// <param name="width">Texture width</param>
	/// <param name="height">Texture height</param>
	/// <param name="format">Texture data format</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfError mfgV2XCreateDepthStencilTexture(mfgV2XRenderDevice* rd, mfgV2XDepthStencilTexture** tex, mfmU64 width, mfmU64 height, mfgEnum format);

	/// <summary>
	///		Destroys a depth stencil texture.
	/// </summary>
	/// <param name="tex">Texture handle</param>
	void mfgV2XDestroyDepthStencilTexture(void* tex);

	/// <summary>
	///		Creates a new framebuffer.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="state">Pointer to framebuffer handle</param>
	/// <param name="textureCount">Number of texture attachments</param>
	/// <param name="texture">Texture attachments array</param>
	/// <param name="depthStencilTexture">Depth stencil texture (optional, can be set to NULL)</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfError mfgV2XCreateFramebuffer(mfgV2XRenderDevice* rd, mfgV2XFramebuffer** fb, mfmU64 textureCount, mfgV2XRenderTexture** textures, mfgV2XDepthStencilTexture* depthStencilTexture);

	/// <summary>
	///		Destroys a framebuffer.
	/// </summary>
	/// <param name="fb">Framebuffer handle</param>
	void mfgV2XDestroyFramebuffer(void* fb);

	/// <summary>
	///		Sets a framebuffer as the one used for draw calls.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="fb">Framebuffer handle</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfError mfgV2XSetFramebuffer(mfgV2XRenderDevice* rd, mfgV2XFramebuffer* fb);

	/// <summary>
	///		Clears the current framebuffer color buffers.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="r">Color red component</param>
	/// <param name="g">Color green component</param>
	/// <param name="b">Color blue component</param>
	/// <param name="a">Color alpha component</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfError mfgV2XClearColor(mfgV2XRenderDevice* rd, mfmF32 r, mfmF32 g, mfmF32 b, mfmF32 a);

	/// <summary>
	///		Clears the current framebuffer depth buffer.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="depth">Depth value</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfError mfgV2XClearDepth(mfgV2XRenderDevice* rd, mfmF32 depth);

	/// <summary>
	///		Clears the current framebuffer stencil buffer.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="stencil">Stencil value</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfError mfgV2XClearStencil(mfgV2XRenderDevice* rd, mfmI32 stencil);

	/// <summary>
	///		Draws the triangles stored in the currently active vertex array using the currently active pipeline.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="offset">First vertex offset</param>
	/// <param name="count">Vertex count</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfError mfgV2XDrawTriangles(mfgV2XRenderDevice* rd, mfmU64 offset, mfmU64 count);

	/// <summary>
	///		Draws the triangles stored in the currently active vertex array using the currently active pipeline and index buffer.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="offset">First index offset</param>
	/// <param name="count">Index count</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfError mfgV2XDrawTrianglesIndexed(mfgV2XRenderDevice* rd, mfmU64 offset, mfmU64 count);

	/// <summary>
	///		Swaps the front and back buffers (displays the stuff drawn until this point).
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfError mfgV2XSwapBuffers(mfgV2XRenderDevice* rd);

	/// <summary>
	///		Gets a render device integer property.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="id">Property identifier</param>
	/// <param name="value">Output property value</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfError mfgV2XGetPropertyI(mfgV2XRenderDevice* rd, mfgEnum id, mfmI32* value);

	/// <summary>
	///		Gets a render device floating point property.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="id">Property identifier</param>
	/// <param name="value">Output property value</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfError mfgV2XGetPropertyF(mfgV2XRenderDevice* rd, mfgEnum id, mfmF32* value);

	/// <summary>
	///		Gets the last error string.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="str">Output error string</param>
	/// <param name="maxSize">Maximum error string size</param>
	/// <returns>
	///		True if there was an error, otherwise false.
	/// </returns>
	mfmBool mfgV2XGetErrorString(mfgV2XRenderDevice* rd, mfsUTF8CodeUnit* str, mfmU64 maxSize);

	typedef mfError(*mfgV2XRenderDeviceCreatorFunction)(mfgV2XRenderDevice** renderDevice, mfiWindow* window, const mfgV2XRenderDeviceDesc* desc, void* allocator);

	/// <summary>
	///		Inits the render devices library.
	/// </summary>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		Otherwise returns an error code.
	/// </returns>
	mfError mfgV2XInitRenderDevices(void);

	/// <summary>
	///		Terminates the render devices library.
	/// </summary>
	void mfgV2XTerminateRenderDevices(void);

	/// <summary>
	///		Registers a new render device creator.
	/// </summary>
	/// <param name="type">Render device type name (with a maximum size of 16 bytes)</param>
	/// <param name="func">Render device creator function</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		MFG_ERROR_NO_REGISTER_ENTRIES if there are no more creator slots in the register.
	/// </returns>
	mfError mfgV2XRegisterRenderDeviceCreator(const mfsUTF8CodeUnit* type, mfgV2XRenderDeviceCreatorFunction func);

	/// <summary>
	///		Creates a new window.
	/// </summary>
	/// <param name="type">Render device type name</param>
	/// <param name="renderDevice">Out render device handle</param>
	/// <param name="window">Render device window</param>
	/// <param name="desc">Render device description</param>
	/// <param name="allocator">Render device allocator</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		MFG_ERROR_TYPE_NOT_REGISTERED if there isn't a creator with the type registered.
	///		Otherwise returns a render device creation error code.
	/// </returns>
	mfError mfgV2XCreateRenderDevice(const mfsUTF8CodeUnit* type, mfgV2XRenderDevice** renderDevice, mfiWindow* window, const mfgV2XRenderDeviceDesc* desc, void* allocator);

	/// <summary>
	///		Destroys a render device.
	/// </summary>
	/// <param name="renderDevice">Render device handle</param>
	void mfgV2XDestroyRenderDevice(void* renderDevice);

#ifdef __cplusplus
}
#endif
