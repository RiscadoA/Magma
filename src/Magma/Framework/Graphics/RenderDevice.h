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
#define MFG_NINT			0x0C
#define MFG_NUBYTE			0x0D
#define MFG_NUSHORT			0x0E
#define MFG_NUINT			0x0F
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

	typedef mfmI32 mfgEnum;

	typedef void mfgVertexShader;
	typedef void mfgPixelShader;
	typedef void mfgPipeline;
	typedef void mfgBindingPoint;
	typedef void mfgConstantBuffer;
	typedef void mfgTexture1D;
	typedef void mfgTexture2D;
	typedef void mfgTexture3D;
	typedef void mfgSampler;
	typedef void mfgVertexBuffer;
	typedef void mfgVertexLayout;
	typedef void mfgVertexArray;
	typedef void mfgIndexBuffer;
	typedef void mfgRasterState;
	typedef void mfgDepthStencilState;
	typedef void mfgBlendState;
	typedef void mfgRenderTexture;
	typedef void mfgDepthStencilTexture;
	typedef void mfgFramebuffer;

	typedef struct mfgRenderDevice mfgRenderDevice;

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
		///			MFG_NINT;
		///			MFG_NUBYTE;
		///			MFG_NUSHORT;
		///			MFG_NUINT;
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
	typedef mfgError(*mfgRDDestroyVertexShaderFunction)(mfgRenderDevice* rd, mfgVertexShader* vs);
	typedef mfgError(*mfgRDCreatePixelShaderFunction)(mfgRenderDevice* rd, mfgPixelShader** ps, const mfmU8* bc, mfmU64 bcSize, const mfgMetaData* md);
	typedef mfgError(*mfgRDDestroyPixelShaderFunction)(mfgRenderDevice* rd, mfgPixelShader* ps);
	typedef mfgError(*mfgRDCreatePipelineFunction)(mfgRenderDevice* rd, mfgPipeline** pp, mfgVertexShader* vs, mfgPixelShader* ps);
	typedef mfgError(*mfgRDDestroyPipelineFunction)(mfgRenderDevice* rd, mfgPipeline* pp);
	typedef mfgError(*mfgRDSetPipelineFunction)(mfgRenderDevice* rd, mfgPipeline* pp);

	typedef mfgError(*mfgRDGetVertexShaderBindingPointFunction)(mfgRenderDevice* rd, mfgBindingPoint** bp, mfgVertexShader* vs, const mfsUTF8CodeUnit* name);
	typedef mfgError(*mfgRDGetPixelShaderBindingPointFunction)(mfgRenderDevice* rd, mfgBindingPoint** bp, mfgPixelShader* ps, const mfsUTF8CodeUnit* name);
	typedef mfgError(*mfgRDBindConstantBufferFunction)(mfgRenderDevice* rd, mfgBindingPoint* bp, mfgConstantBuffer* cb);
	typedef mfgError(*mfgRDBindTexture1DFunction)(mfgRenderDevice* rd, mfgBindingPoint* bp, mfgTexture1D* tex);
	typedef mfgError(*mfgRDBindTexture2DFunction)(mfgRenderDevice* rd, mfgBindingPoint* bp, mfgTexture2D* tex);
	typedef mfgError(*mfgRDBindTexture3DFunction)(mfgRenderDevice* rd, mfgBindingPoint* bp, mfgTexture3D* tex);
	typedef mfgError(*mfgRDBindSamplerFunction)(mfgRenderDevice* rd, mfgBindingPoint* bp, mfgSampler* sampler);

	typedef mfgError(*mfgRDCreateConstantBufferFunction)(mfgRenderDevice* rd, mfgConstantBuffer** cb, mfmU64 size, const void* data, mfgEnum usage);
	typedef mfgError(*mfgRDDestroyConstantBufferFunction)(mfgRenderDevice* rd, mfgConstantBuffer* cb);
	typedef mfgError(*mfgRDMapConstantBufferFunction)(mfgRenderDevice* rd, mfgConstantBuffer* cb, void** memory);
	typedef mfgError(*mfgRDUnmapConstantBufferFunction)(mfgRenderDevice* rd, mfgConstantBuffer* cb);

	// Vertex functions
	typedef mfgError(*mfgRDCreateVertexBufferFunction)(mfgRenderDevice* rd, mfgVertexBuffer** vb, mfmU64 size, const void* data, mfgEnum usage);
	typedef mfgError(*mfgRDDestroyVertexBufferFunction)(mfgRenderDevice* rd, mfgVertexBuffer* vb);
	typedef mfgError(*mfgRDMapVertexBufferFunction)(mfgRenderDevice* rd, mfgVertexBuffer* vb, void** memory);
	typedef mfgError(*mfgRDUnmapVertexBufferFunction)(mfgRenderDevice* rd, mfgVertexBuffer* vb);
	typedef mfgError(*mfgRDCreateVertexLayoutFunction)(mfgRenderDevice* rd, mfgVertexLayout** vl, mfmU64 elementCount, const mfgVertexElement* elements, mfgVertexShader* vs);
	typedef mfgError(*mfgRDDestroyVertexLayoutFunction)(mfgRenderDevice* rd, mfgVertexLayout* vl);
	typedef mfgError(*mfgRDCreateVertexArrayFunction)(mfgRenderDevice* rd, mfgVertexArray** va, mfmU64 vbCount, mfgVertexBuffer** vbs, mfgVertexLayout* vl);
	typedef mfgError(*mfgRDDestroyVertexArrayFunction)(mfgRenderDevice* rd, mfgVertexArray* va);
	typedef mfgError(*mfgRDSetVertexArrayFunction)(mfgRenderDevice* rd, mfgVertexArray* va);
	typedef mfgError(*mfgRDCreateIndexBufferFunction)(mfgRenderDevice* rd, mfgIndexBuffer** ib, mfmU64 size, const void* data, mfgEnum usage);
	typedef mfgError(*mfgRDDestroyIndexBufferFunction)(mfgRenderDevice* rd, mfgIndexBuffer* ib);
	typedef mfgError(*mfgRDSetIndexBufferFunction)(mfgRenderDevice* rd, mfgIndexBuffer* ib);
	typedef mfgError(*mfgRDMapIndexBufferFunction)(mfgRenderDevice* rd, mfgIndexBuffer* ib, void** memory);
	typedef mfgError(*mfgRDUnmapIndexBufferFunction)(mfgRenderDevice* rd, mfgIndexBuffer* ib);

	// Texture functions
	typedef mfgError(*mfgRDCreateTexture1DFunction)(mfgRenderDevice* rd, mfgTexture1D** tex, mfmU64 width, mfgEnum format, const void* data, mfgEnum usage);
	typedef mfgError(*mfgRDDestroyTexture1DFunction)(mfgRenderDevice* rd, mfgTexture1D* tex);
	typedef mfgError(*mfgRDUpdateTexture1DFunction)(mfgRenderDevice* rd, mfgTexture1D* tex, mfmU64 dstX, mfmU64 width, const void* data);
	typedef mfgError(*mfgRDGenerateTexture1DMipmapsFunction)(mfgRenderDevice* rd, mfgTexture1D* tex);

	typedef mfgError(*mfgRDCreateTexture2DFunction)(mfgRenderDevice* rd, mfgTexture2D** tex, mfmU64 width, mfmU64 height, mfgEnum format, const void* data, mfgEnum usage);
	typedef mfgError(*mfgRDDestroyTexture2DFunction)(mfgRenderDevice* rd, mfgTexture2D* tex);
	typedef mfgError(*mfgRDUpdateTexture2DFunction)(mfgRenderDevice* rd, mfgTexture2D* tex, mfmU64 dstX, mfmU64 dstY, mfmU64 width, mfmU64 height, const void* data);
	typedef mfgError(*mfgRDGenerateTexture2DMipmapsFunction)(mfgRenderDevice* rd, mfgTexture2D* tex);

	typedef mfgError(*mfgRDCreateTexture3DFunction)(mfgRenderDevice* rd, mfgTexture3D** tex, mfmU64 width, mfmU64 height, mfmU64 depth, mfgEnum format, const void* data, mfgEnum usage);
	typedef mfgError(*mfgRDDestroyTexture3DFunction)(mfgRenderDevice* rd, mfgTexture3D* tex);
	typedef mfgError(*mfgRDUpdateTexture3DFunction)(mfgRenderDevice* rd, mfgTexture3D* tex, mfmU64 dstX, mfmU64 dstY, mfmU64 dstZ, mfmU64 width, mfmU64 height, mfmU64 depth, const void* data);
	typedef mfgError(*mfgRDGenerateTexture3DMipmapsFunction)(mfgRenderDevice* rd, mfgTexture3D* tex);

	typedef mfgError(*mfgRDCreateSamplerFunction)(mfgRenderDevice* rd, mfgSampler** sampler, const mfgSamplerDesc* desc);
	typedef mfgError(*mfgRDDestroySamplerFunction)(mfgRenderDevice* rd, mfgSampler* sampler);

	// Raster/depth stencil/blend state functions
	typedef mfgError(*mfgRDCreateRasterStateFunction)(mfgRenderDevice* rd, mfgRasterState** state, const mfgRasterStateDesc* desc);
	typedef mfgError(*mfgRDDestroyRasterStateFunction)(mfgRenderDevice* rd, mfgRasterState* state);
	typedef mfgError(*mfgRDSetRasterStateFunction)(mfgRenderDevice* rd, mfgRasterState* state);
	typedef mfgError(*mfgRDCreateDepthStencilStateFunction)(mfgRenderDevice* rd, mfgDepthStencilState** state, const mfgDepthStencilStateDesc* desc);
	typedef mfgError(*mfgRDDestroyDepthStencilFunction)(mfgRenderDevice* rd, mfgDepthStencilState* state);
	typedef mfgError(*mfgRDSetDepthStencilFunction)(mfgRenderDevice* rd, mfgDepthStencilState* state);
	typedef mfgError(*mfgRDCreateBlendStateFunction)(mfgRenderDevice* rd, mfgBlendState** state, const mfgBlendStateDesc* desc);
	typedef mfgError(*mfgRDDestroyBlendStateFunction)(mfgRenderDevice* rd, mfgBlendState* state);
	typedef mfgError(*mfgRDSetBlendStateFunction)(mfgRenderDevice* rd, mfgBlendState* state);

	// Framebuffer functions
	typedef mfgError(*mfgRDCreateRenderTextureFunction)(mfgRenderDevice* rd, mfgRenderTexture** tex, mfmU64 width, mfmU64 height, mfgEnum format);
	typedef mfgError(*mfgRDDestroyRenderTextureFunction)(mfgRenderDevice* rd, mfgRenderTexture* tex);
	typedef mfgError(*mfgRDCreateDepthStencilTextureFunction)(mfgRenderDevice* rd, mfgDepthStencilTexture** tex, mfmU64 width, mfmU64 height, mfgEnum format);
	typedef mfgError(*mfgRDDestroyDepthStencilTextureFunction)(mfgRenderDevice* rd, mfgDepthStencilTexture* tex);
	typedef mfgError(*mfgRDCreateFramebufferFunction)(mfgRenderDevice* rd, mfgFramebuffer** fb, mfmU64 textureCount, mfgRenderTexture** textures, mfgDepthStencilTexture* depthStencilTexture);
	typedef mfgError(*mfgRDDestroyFramebufferFunction)(mfgRenderDevice* rd, mfgFramebuffer* fb);
	typedef mfgError(*mfgRDSetFramebufferFunction)(mfgRenderDevice* rd, mfgFramebuffer* fb);

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
		mfgRDBindTexture1DFunction bindTexture1D;
		mfgRDBindTexture2DFunction bindTexture2D;
		mfgRDBindTexture3DFunction bindTexture3D;
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
	};

	/// <summary>
	///		Creates a new vertex shader.
	/// </summary>
	/// <param name="rd">Render device where the vertex shader will be created</param>
	/// <param name="vertexShader">Pointer to vertex shader handle</param>
	mfgError mfgCreateVertexShader(mfgRenderDevice* rd, void** vertexShader, const mfmU8* bytecode, mfmU64 bytecodeSie, const mfgMetaData* metaData);

#ifdef __cplusplus
}
#endif
