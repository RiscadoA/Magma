#pragma once

#include "../../Memory/Object.hpp"
#include "../../Memory/Allocator.hpp"
#include "../../Input/Window.hpp"

#include "RenderDevice.h"

namespace Magma
{
	namespace Framework
	{
		namespace Graphics
		{
			namespace V2X
			{
				/// <summary>
				///		Usage modes for buffers and textures.
				/// </summary>
				enum class Usage : mfgEnum
				{
					Default = MFG_USAGE_DEFAULT,
					Dynamic = MFG_USAGE_DYNAMIC,
					Static	= MFG_USAGE_STATIC,
				};

				/// <summary>
				///		Data types.
				/// </summary>
				enum class Type : mfgEnum
				{
					Byte	= MFG_BYTE,
					Short	= MFG_SHORT,
					Int		= MFG_INT,
					UByte	= MFG_UBYTE,
					UShort	= MFG_USHORT,
					UInt	= MFG_UINT,
					NByte	= MFG_NBYTE,
					NShort	= MFG_NSHORT,
					NUByte	= MFG_NUBYTE,
					NUShort	= MFG_NUSHORT,
					Float	= MFG_FLOAT,
				};

				/// <summary>
				///		Texture data formats.
				/// </summary>
				enum class Format : mfgEnum
				{
					R8SNorm			= MFG_R8SNORM,
					R16SNorm		= MFG_R16SNORM,
					RG8SNorm		= MFG_RG8SNORM,
					RG16SNorm		= MFG_RG16SNORM,
					RGBA8SNorm		= MFG_RGBA8SNORM,
					RGBA16SNorm		= MFG_RGBA16SNORM,
					R8UNorm			= MFG_R8UNORM,
					R16UNorm		= MFG_R16UNORM,
					RG8UNorm		= MFG_RG8UNORM,
					RG16UNorm		= MFG_RG16UNORM,
					RGBA8UNorm		= MFG_RGBA8UNORM,
					RGBA16UNorm		= MFG_RGBA16UNORM,
					R8SInt			= MFG_R8SINT,
					R16SInt			= MFG_R16SINT,
					RG8SInt			= MFG_RG8SINT,
					RG16SInt		= MFG_RG16SINT,
					RGBA8SInt		= MFG_RGBA8SINT,
					RGBA16SInt		= MFG_RGBA16SINT,
					R8UInt			= MFG_R8UINT,
					R16UInt			= MFG_R16UINT,
					RG8UInt			= MFG_RG8UINT,
					RG16UInt		= MFG_RG16UINT,
					RGBA8UInt		= MFG_RGBA8UINT,
					RGBA16UInt		= MFG_RGBA16UINT,
					R32Float		= MFG_R32FLOAT,
					RG32Float		= MFG_RG32FLOAT,
					RGB32Float		= MFG_RGB32FLOAT,
					RGBA32Float		= MFG_RGBA32FLOAT,

					Depth24Stencil8	= MFG_DEPTH24STENCIL8,
					Depth32Stencil8	= MFG_DEPTH32STENCIL8,
				};

				/// <summary>
				///		Texture address modes.
				/// </summary>
				enum class AddressMode : mfgEnum
				{
					Repeat	= MFG_REPEAT,
					Mirror	= MFG_MIRROR,
					Clamp	= MFG_CLAMP,
					Border	= MFG_BORDER,
				};

				/// <summary>
				///		Texture filter types.
				/// </summary>
				enum class TextureFilter : mfgEnum
				{
					None	= MFG_NONE,
					Nearest	= MFG_NEAREST,
					Linear	= MFG_LINEAR,
				};

				/// <summary>
				///		Winding modes.
				/// </summary>
				enum class Winding : mfgEnum
				{
					CW	= MFG_CW,
					CCW	= MFG_CCW,
				};

				/// <summary>
				///		Faces.
				/// </summary>
				enum class Face : mfgEnum
				{
					Front			= MFG_FRONT,
					Back			= MFG_BACK,
					FrontAndBack	= MFG_FRONT_AND_BACK,
				};

				/// <summary>
				///		Rasterizer modes.
				/// </summary>
				enum class RasterMode : mfgEnum
				{
					Wireframe	= MFG_WIREFRAME,
					Fill		= MFG_FILL,
				};

				/// <summary>
				///		Comparison functions.
				/// </summary>
				enum class Compare : mfgEnum
				{
					Never		= MFG_NEVER,
					Less		= MFG_LESS,
					LEqual		= MFG_LEQUAL,
					Greater		= MFG_GREATER,
					GEqual		= MFG_GEQUAL,
					Equal		= MFG_EQUAL,
					NEqual		= MFG_NEQUAL,
					Always		= MFG_ALWAYS,
				};

				/// <summary>
				///		Stencil actions.
				/// </summary>
				enum class StencilAction : mfgEnum
				{
					Zero			= MFG_ZERO,
					Keep			= MFG_KEEP,
					Replace			= MFG_REPLACE,
					Increment		= MFG_INCREMENT,
					IncrementWrap	= MFG_INCREMENT_WRAP,
					Decrement		= MFG_DECREMENT,
					DecrementWrap	= MFG_DECREMENT_WRAP,
					Invert			= MFG_INVERT,
				};

				/// <summary>
				///		Blend factors.
				/// </summary>
				enum class BlendFactor : mfgEnum
				{
					Zero			= MFG_ZERO,
					One				= MFG_ONE,
					SrcColor		= MFG_SRC_COLOR,
					InvSrcColor		= MFG_INV_SRC_COLOR,
					DstColor		= MFG_DST_COLOR,
					InvDstColor		= MFG_INV_DST_COLOR,
					SrcAlpha		= MFG_SRC_ALPHA,
					InvSrcAlpha		= MFG_INV_SRC_ALPHA,
					DstAlpha		= MFG_DST_ALPHA,
					InvDstAlpha		= MFG_INV_DST_ALPHA,
				};

				/// <summary>
				///		Blend operations.
				/// </summary>
				enum class BlendOperation : mfgEnum
				{
					Add			= MFG_ADD,
					Subtract	= MFG_SUBTRACT,
					RevSubtract = MFG_REV_SUBTRACT,
					Min			= MFG_MIN,
					Max			= MFG_MAX,
				};

				/// <summary>
				///		Render device description.
				/// </summary>
				struct RenderDeviceDesc
				{
					/// <summary>
					///		Enable vertical synchronization on this render device?
					/// </summary>
					bool vsyncEnabled = false;
				};

				/// <summary>
				///		Vertex element description.
				/// </summary>
				struct VertexElement
				{
					/// <summary>
					///		Vertex element name (16 bytes max).
					/// </summary>
					mfsUTF8CodeUnit name[16] = { '\0' };

					/// <summary>
					///		Stride between each element in memory.
					/// </summary>
					mfmU64 stride = 0;

					/// <summary>
					///		Offset of the first element in the buffer.
					/// </summary>
					mfmU64 offset = 0;

					/// <summary>
					///		Vertex buffer index.
					/// </summary>
					mfmU64 bufferIndex = 0;

					/// <summary>
					///		Vertex element component count.
					///		Valid values: 1; 2; 3; 4;
					/// </summary>
					mfmU64 size = 1;

					/// <summary>
					///		Vertex element component type.
					///		Valid values:
					///			Byte;
					///			Short;
					///			Int;
					///			UByte;
					///			UShort;
					///			UInt;
					///			NByte;
					///			NShort;
					///			NUByte;
					///			NUShort;
					///			Float;
					/// </summary>
					Type type = Type::Float;
				};

				/// <summary>
				///		Sampler object description.
				/// </summary>
				struct SamplerDesc
				{
					/// <summary>
					///		Border color (RGBA).
					/// </summary>
					mfmF32 borderColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

					/// <summary>
					///		Max anisotropy for filtering.
					///		Set to 1 to anisotropic filtering.
					///		Valid values: 1 - RenderDevice.GetMaxAnisotropy().
					/// </summary>
					mfmU32 maxAnisotropy = 1;

					/// <summary>
					///		Minifying filter.
					///		Valid values:
					///			Nearest;
					///			Linear;
					/// </summary>
					TextureFilter minFilter = TextureFilter::Nearest;

					/// <summary>
					///		Magnifying filter.
					///		Valid values:
					///			Nearest;
					///			Linear;
					/// </summary>
					TextureFilter magFilter = TextureFilter::Nearest;

					/// <summary>
					///		Mipmap filter (set to MFG_NONE to disable mipmaps).
					///		Valid values:
					///			None;
					///			Nearest;
					///			Linear;
					/// </summary>
					TextureFilter mipmapFilter = TextureFilter::None;

					/// <summary>
					///		Texture adress mode on coordinate U.
					///		Valid values:
					///			Repeat;
					///			Mirror;
					///			Clamp;
					///			Border;
					/// </summary>
					AddressMode addressU = AddressMode::Clamp;

					/// <summary>
					///		Texture adress mode on coordinate V.
					///		Valid values:
					///			Repeat;
					///			Mirror;
					///			Clamp;
					///			Border;
					/// </summary>
					AddressMode addressV = AddressMode::Clamp;

					/// <summary>
					///		Texture adress mode on coordinate W.
					///		Valid values:
					///			Repeat;
					///			Mirror;
					///			Clamp;
					///			Border;
					/// </summary>
					AddressMode addressW = AddressMode::Clamp;
				};

				/// <summary>
				///		Rasterizer state description.
				/// </summary>
				struct RasterStateDesc
				{
					/// <summary>
					///		Is culling enabled?
					/// </summary>
					bool cullEnabled = false;

					/// <summary>
					///		Front face winding.
					///		Valid values:
					///			CW;
					///			CCW;
					/// </summary>
					Winding frontFace = Winding::CCW;

					/// <summary>
					///		The face which gets culled.
					///		Valid values:
					///			Front;
					///			Back;
					///			FrontAndBack;
					/// </summary>
					Face cullFace = Face::Back;

					/// <summary>
					///		Rasterizer mode.
					///		Valid values:
					///			MFG_FILL;
					///			MFG_WIREFRAME;
					/// </summary>
					RasterMode rasterMode = RasterMode::Fill;
				};

				struct DepthStencilStateDesc
				{
					/// <summary>
					///		Performs depth checks?
					/// </summary>
					bool depthEnabled = false;

					/// <summary>
					///		Write depth?
					/// </summary>
					bool depthWriteEnabled = false;

					/// <summary>
					///		Depth 'near' value.
					/// </summary>
					mfmF32 depthNear = 0.0f;

					/// <summary>
					///		Depth 'far' value.
					/// </summary>
					mfmF32 depthFar = 1.0f;

					/// <summary>
					///		Depth comparison function.
					///		Valid values:
					///			Never;
					///			Less;
					///			LEqual;
					///			Greater;
					///			GEqual;
					///			Equal;
					///			NEqual;
					///			Always;
					/// </summary>
					Compare depthCompare = Compare::Less;

					/// <summary>
					///		Stencil ref value.
					/// </summary>
					mfmU32 stencilRef = 0;

					/// <summary>
					///		Is stencil enabled?
					/// </summary>
					bool stencilEnabled = false;

					/// <summary>
					///		Stencil read mask.
					/// </summary>
					mfmU8 stencilReadMask = 0xFF;

					/// <summary>
					///		Stencil write mask.
					/// </summary>
					mfmU8 stencilWriteMask = 0xFF;

					/// <summary>
					///		Front face stencil comparison function.
					///		Valid values:
					///			Never;
					///			Less;
					///			LEqual;
					///			Greater;
					///			GEqual;
					///			Equal;
					///			NEqual;
					///			Always;
					/// </summary>
					Compare frontFaceStencilCompare = Compare::Always;

					/// <summary>
					///		Front face steincil fail action.
					///		Valid values:
					///			Keep;
					///			Zero;
					///			Replace;
					///			Increment;
					///			IncrementWrap;
					///			Decrement;
					///			DecrementWrap;
					///			Invert;
					/// </summary>
					StencilAction frontFaceStencilFail = StencilAction::Keep;

					/// <summary>
					///		Front face steincil pass action.
					///		Valid values:
					///			Keep;
					///			Zero;
					///			Replace;
					///			Increment;
					///			IncrementWrap;
					///			Decrement;
					///			DecrementWrap;
					///			Invert;
					/// </summary>
					StencilAction frontFaceStencilPass = StencilAction::Keep;

					/// <summary>
					///		Front face stencil depth fail action.
					///		Valid values:
					///			Keep;
					///			Zero;
					///			Replace;
					///			Increment;
					///			IncrementWrap;
					///			Decrement;
					///			DecrementWrap;
					///			Invert;
					/// </summary>
					StencilAction frontFaceDepthFail = StencilAction::Keep;

					/// <summary>
					///		Back face stencil comparison function.
					///		Valid values:
					///			Never;
					///			Less;
					///			LEqual;
					///			Greater;
					///			GEqual;
					///			Equal;
					///			NEqual;
					///			Always;
					/// </summary>
					Compare backFaceStencilCompare = Compare::Always;

					/// <summary>
					///		Back face steincil fail action.
					///		Valid values:
					///			Keep;
					///			Zero;
					///			Replace;
					///			Increment;
					///			IncrementWrap;
					///			Decrement;
					///			DecrementWrap;
					///			Invert;
					/// </summary>
					StencilAction backFaceStencilFail = StencilAction::Keep;

					/// <summary>
					///		Back face steincil pass action.
					///		Valid values:
					///			Keep;
					///			Zero;
					///			Replace;
					///			Increment;
					///			IncrementWrap;
					///			Decrement;
					///			DecrementWrap;
					///			Invert;
					/// </summary>
					StencilAction backFaceStencilPass = StencilAction::Keep;

					/// <summary>
					///		Back face stencil depth fail action.
					///		Valid values:
					///			Keep;
					///			Zero;
					///			Replace;
					///			Increment;
					///			IncrementWrap;
					///			Decrement;
					///			DecrementWrap;
					///			Invert;
					/// </summary>
					StencilAction backFaceDepthFail = StencilAction::Keep;
				};

				struct BlendStateDesc
				{
					/// <summary>
					///		Is blending enabled?
					/// </summary>
					bool blendEnabled = false;

					/// <summary>
					///		Source color factor.
					///		Valid values:
					///			Zero;
					///			One;
					///			SrcColor;
					///			InvSrcColor;
					///			DstColor;
					///			InvDstColor;
					///			SrcAlpha;
					///			InvSrcAlpha;
					///			DstAlpha;
					///			InvDstAlpha;
					/// </summary>
					BlendFactor sourceFactor = BlendFactor::One;

					/// <summary>
					///		Destination color factor.
					///		Valid values:
					///			Zero;
					///			One;
					///			SrcColor;
					///			InvSrcColor;
					///			DstColor;
					///			InvDstColor;
					///			SrcAlpha;
					///			InvSrcAlpha;
					///			DstAlpha;
					///			InvDstAlpha;
					/// </summary>
					BlendFactor destinationFactor = BlendFactor::Zero;

					/// <summary>
					///		Color blend operation.
					///		Valid values:
					///			Add;
					///			Subtract;
					///			RevSubtract;
					///			Min;
					///			Max;
					/// </summary>
					BlendOperation blendOperation = BlendOperation::Add;

					/// <summary>
					///		Source alpha factor.
					///		Valid values:
					///			Zero;
					///			One;
					///			SrcColor;
					///			InvSrcColor;
					///			DstColor;
					///			InvDstColor;
					///			SrcAlpha;
					///			InvSrcAlpha;
					///			DstAlpha;
					///			InvDstAlpha;
					/// </summary>
					BlendFactor sourceAlphaFactor = BlendFactor::One;

					/// <summary>
					///		Destination alpha factor.
					///		Valid values:
					///			Zero;
					///			One;
					///			SrcColor;
					///			InvSrcColor;
					///			DstColor;
					///			InvDstColor;
					///			SrcAlpha;
					///			InvSrcAlpha;
					///			DstAlpha;
					///			InvDstAlpha;
					/// </summary>
					BlendFactor destinationAlphaFactor = BlendFactor::Zero;

					/// <summary>
					///		Color alpha operation.
					///		Valid values:
					///			Add;
					///			Subtract;
					///			RevSubtract;
					///			Min;
					///			Max;
					/// </summary>
					BlendOperation blendAlphaOperation = BlendOperation::Add;
				};

				/// <summary>
				///		Used as a vertex shader handle.
				///		Destroys the vertex shader automatically when there are no more references to it.
				/// </summary>
				class VertexShader : public Memory::Object
				{
				public:
					using Object::Object;
					using Object::operator=;
					inline VertexShader(const Memory::Object& object) : Memory::Object(object) {}

					void* GetBindingPoint(const mfsUTF8CodeUnit* name);
				};

				/// <summary>
				///		Used as a pixel shader handle.
				///		Destroys the pixel shader automatically when there are no more references to it.
				/// </summary>
				class PixelShader : public Memory::Object
				{
				public:
					using Object::Object;
					using Object::operator=;
					inline PixelShader(const Memory::Object& object) : Memory::Object(object) {}

					void* GetBindingPoint(const mfsUTF8CodeUnit* name);
				};

				/// <summary>
				///		Used as a pipeline handle.
				///		Destroys the pipeline automatically when there are no more references to it.
				/// </summary>
				class Pipeline : public Memory::Object
				{
				public:
					using Object::Object;
					using Object::operator=;
					inline Pipeline(const Memory::Object& object) : Memory::Object(object) {}
				};

				/// <summary>
				///		Used as a constant buffer handle.
				///		Destroys the constant buffer automatically when there are no more references to it.
				/// </summary>
				class ConstantBuffer : public Memory::Object
				{
				public:
					using Object::Object;
					using Object::operator=;
					inline ConstantBuffer(const Memory::Object& object) : Memory::Object(object) {}

					void* Map();

					void Unmap();
				};

				/// <summary>
				///		Used as a texture 1D handle.
				///		Destroys the texture 1D automatically when there are no more references to it.
				/// </summary>
				class Texture1D : public Memory::Object
				{
				public:
					using Object::Object;
					using Object::operator=;
					inline Texture1D(const Memory::Object& object) : Memory::Object(object) {}

					void Update(mfmU32 dstX, mfmU32 width, const void* data);
				};

				/// <summary>
				///		Used as a texture 2D handle.
				///		Destroys the texture 2D automatically when there are no more references to it.
				/// </summary>
				class Texture2D : public Memory::Object
				{
				public:
					using Object::Object;
					using Object::operator=;
					inline Texture2D(const Memory::Object& object) : Memory::Object(object) {}

					void Update(mfmU32 dstX, mfmU32 dstY, mfmU32 width, mfmU32 height, const void* data);
				};

				/// <summary>
				///		Used as a texture 3D handle.
				///		Destroys the texture 3D automatically when there are no more references to it.
				/// </summary>
				class Texture3D : public Memory::Object
				{
				public:
					using Object::Object;
					using Object::operator=;
					inline Texture3D(const Memory::Object& object) : Memory::Object(object) {}

					void Update(mfmU32 dstX, mfmU32 dstY, mfmU32 dstZ, mfmU32 width, mfmU32 height, mfmU32 depth, const void* data);
				};

				/// <summary>
				///		Used as a sampler handle.
				///		Destroys the sampler automatically when there are no more references to it.
				/// </summary>
				class Sampler : public Memory::Object
				{
				public:
					using Object::Object;
					using Object::operator=;
					inline Sampler(const Memory::Object& object) : Memory::Object(object) {}
				};

				/// <summary>
				///		Used as a vertex buffer handle.
				///		Destroys the vertex buffer automatically when there are no more references to it.
				/// </summary>
				class VertexBuffer : public Memory::Object
				{
				public:
					using Object::Object;
					using Object::operator=;
					inline VertexBuffer(const Memory::Object& object) : Memory::Object(object) {}

					void* Map();

					void Unmap();
				};

				class VertexLayout : public Memory::Object
				{
				public:
					using Object::Object;
					using Object::operator=;
					inline VertexLayout(const Memory::Object& object) : Memory::Object(object) {}
				};

				class VertexArray : public Memory::Object
				{
				public:
					using Object::Object;
					using Object::operator=;
					inline VertexArray(const Memory::Object& object) : Memory::Object(object) {}
				};

				class IndexBuffer : public Memory::Object
				{
				public:
					using Object::Object;
					using Object::operator=;
					inline IndexBuffer(const Memory::Object& object) : Memory::Object(object) {}

					void* Map();

					void Unmap();
				};



				/// <summary>
				///		Used as a render device handle.
				///		Destroys the render device automatically when there are no more references to it.
				/// </summary>
				class RenderDevice : public Memory::Object
				{
				public:
					using Object::Object;
					using Object::operator=;
					inline RenderDevice(const Memory::Object& object) : Memory::Object(object) {}


				};

				/// <summary>
				///		Creates a new render device.
				/// </summary>
				/// <param name="type">Render device type name</param>
				/// <param name="window">Render device window</param>
				/// <param name="desc">Render device description</param>
				/// <param name="allocator">Render device allocator</param>
				/// <returns>Render device handle</returns>
				RenderDevice CreateRenderDevice(const mfsUTF8CodeUnit* type, Input::Window window, const mfgV2XRenderDeviceDesc* desc, Memory::Allocator allocator);
			}
		}
	}
}
