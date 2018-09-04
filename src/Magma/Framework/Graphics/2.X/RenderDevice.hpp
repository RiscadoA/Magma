#pragma once

#include "../../Memory/Handle.hpp"
#include "../../Memory/Allocator.hpp"
#include "../../Input/Window.hpp"

#include "RenderDevice.h"
#include "Bytecode.hpp"

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
				class VertexShader : public Memory::Handle
				{
				public:
					using Handle::Handle;
					using Handle::operator=;
					explicit inline VertexShader(const Memory::Handle& object) : Memory::Handle(object) {}

					/// <summary>
					///		Gets a binding point from this shader.
					/// </summary>
					/// <param name="name">Binding point name</param>
					/// <returns>Binding point handle</returns>
					void* GetBindingPoint(const mfsUTF8CodeUnit* name);
				};

				/// <summary>
				///		Used as a pixel shader handle.
				///		Destroys the pixel shader automatically when there are no more references to it.
				/// </summary>
				class PixelShader : public Memory::Handle
				{
				public:
					using Handle::Handle;
					using Handle::operator=;
					explicit inline PixelShader(const Memory::Handle& object) : Memory::Handle(object) {}

					/// <summary>
					///		Gets a binding point from this shader.
					/// </summary>
					/// <param name="name">Binding point name</param>
					/// <returns>Binding point handle</returns>
					void* GetBindingPoint(const mfsUTF8CodeUnit* name);
				};

				/// <summary>
				///		Used as a pipeline handle.
				///		Destroys the pipeline automatically when there are no more references to it.
				/// </summary>
				class Pipeline : public Memory::Handle
				{
				public:
					using Handle::Handle;
					using Handle::operator=;
					explicit inline Pipeline(const Memory::Handle& object) : Memory::Handle(object) {}
				};

				/// <summary>
				///		Used as a constant buffer handle.
				///		Destroys the constant buffer automatically when there are no more references to it.
				/// </summary>
				class ConstantBuffer : public Memory::Handle
				{
				public:
					using Handle::Handle;
					using Handle::operator=;
					explicit inline ConstantBuffer(const Memory::Handle& object) : Memory::Handle(object) {}

					/// <summary>
					///		Maps the constant buffer to a region in memory.
					/// </summary>
					/// <returns>Memory region pointer</returns>
					void* Map();
					
					/// <summary>
					///		Unmaps the constant buffer from a region in memory.
					/// </summary>
					void Unmap();
				};

				/// <summary>
				///		Used as a texture 1D handle.
				///		Destroys the texture 1D automatically when there are no more references to it.
				/// </summary>
				class Texture1D : public Memory::Handle
				{
				public:
					using Handle::Handle;
					using Handle::operator=;
					explicit inline Texture1D(const Memory::Handle& object) : Memory::Handle(object) {}

					/// <summary>
					///		Updates a texture with new data.
					/// </summary>
					/// <param name="dstX">X destination</param>
					/// <param name="width">New data width</param>
					/// <param name="data">New data</param>
					void Update(mfmU32 dstX, mfmU32 width, const void* data);

					/// <summary>
					///		Generates mipmaps on this texture.
					/// </summary>
					void GenerateMipmaps();
				};

				/// <summary>
				///		Used as a texture 2D handle.
				///		Destroys the texture 2D automatically when there are no more references to it.
				/// </summary>
				class Texture2D : public Memory::Handle
				{
				public:
					using Handle::Handle;
					using Handle::operator=;
					explicit inline Texture2D(const Memory::Handle& object) : Memory::Handle(object) {}

					/// <summary>
					///		Updates a texture with new data.
					/// </summary>
					/// <param name="dstX">X destination</param>
					/// <param name="dstY">Y destination</param>
					/// <param name="width">New data width</param>
					/// <param name="height">New data height</param>
					/// <param name="data">New data</param>
					void Update(mfmU32 dstX, mfmU32 dstY, mfmU32 width, mfmU32 height, const void* data);

					/// <summary>
					///		Generates mipmaps on this texture.
					/// </summary>
					void GenerateMipmaps();
				};

				/// <summary>
				///		Used as a texture 3D handle.
				///		Destroys the texture 3D automatically when there are no more references to it.
				/// </summary>
				class Texture3D : public Memory::Handle
				{
				public:
					using Handle::Handle;
					using Handle::operator=;
					explicit inline Texture3D(const Memory::Handle& object) : Memory::Handle(object) {}

					/// <summary>
					///		Updates a texture with new data.
					/// </summary>
					/// <param name="dstX">X destination</param>
					/// <param name="dstY">Y destination</param>
					/// <param name="dstZ">Z destination</param>
					/// <param name="width">New data width</param>
					/// <param name="height">New data height</param>
					/// <param name="depth">New data depth</param>
					/// <param name="data">New data</param>
					void Update(mfmU32 dstX, mfmU32 dstY, mfmU32 dstZ, mfmU32 width, mfmU32 height, mfmU32 depth, const void* data);
				
					/// <summary>
					///		Generates mipmaps on this texture.
					/// </summary>
					void GenerateMipmaps();
				};

				/// <summary>
				///		Used as a sampler handle.
				///		Destroys the sampler automatically when there are no more references to it.
				/// </summary>
				class Sampler : public Memory::Handle
				{
				public:
					using Handle::Handle;
					using Handle::operator=;
					explicit inline Sampler(const Memory::Handle& object) : Memory::Handle(object) {}
				};

				/// <summary>
				///		Used as a vertex buffer handle.
				///		Destroys the vertex buffer automatically when there are no more references to it.
				/// </summary>
				class VertexBuffer : public Memory::Handle
				{
				public:
					using Handle::Handle;
					using Handle::operator=;
					explicit inline VertexBuffer(const Memory::Handle& object) : Memory::Handle(object) {}

					/// <summary>
					///		Maps the vertex buffer to a region in memory.
					/// </summary>
					/// <returns>Memory region pointer</returns>
					void* Map();

					/// <summary>
					///		Unmaps the vertex buffer from a region in memory.
					/// </summary>
					void Unmap();
				};

				/// <summary>
				///		Used as a vertex layout handle.
				///		Destroys the vertex layout automatically when there are no more references to it.
				/// </summary>
				class VertexLayout : public Memory::Handle
				{
				public:
					using Handle::Handle;
					using Handle::operator=;
					explicit inline VertexLayout(const Memory::Handle& object) : Memory::Handle(object) {}
				};

				/// <summary>
				///		Used as a vertex array handle.
				///		Destroys the vertex array automatically when there are no more references to it.
				/// </summary>
				class VertexArray : public Memory::Handle
				{
				public:
					using Handle::Handle;
					using Handle::operator=;
					explicit inline VertexArray(const Memory::Handle& object) : Memory::Handle(object) {}
				};

				/// <summary>
				///		Used as a index buffer handle.
				///		Destroys the index buffer automatically when there are no more references to it.
				/// </summary>
				class IndexBuffer : public Memory::Handle
				{
				public:
					using Handle::Handle;
					using Handle::operator=;
					explicit inline IndexBuffer(const Memory::Handle& object) : Memory::Handle(object) {}

					/// <summary>
					///		Maps the index buffer to a region in memory.
					/// </summary>
					/// <returns>Memory region pointer</returns>
					void* Map();

					/// <summary>
					///		Unmaps the index buffer from a region in memory.
					/// </summary>
					void Unmap();
				};

				/// <summary>
				///		Used as a rasterizer state handle.
				///		Destroys the state automatically when there are no more references to it.
				/// </summary>
				class RasterState : public Memory::Handle
				{
				public:
					using Handle::Handle;
					using Handle::operator=;
					explicit inline RasterState(const Memory::Handle& object) : Memory::Handle(object) {}
				};

				/// <summary>
				///		Used as a depth stencil state handle.
				///		Destroys the state automatically when there are no more references to it.
				/// </summary>
				class DepthStencilState : public Memory::Handle
				{
				public:
					using Handle::Handle;
					using Handle::operator=;
					explicit inline DepthStencilState(const Memory::Handle& object) : Memory::Handle(object) {}
				};

				/// <summary>
				///		Used as a blend state handle.
				///		Destroys the state automatically when there are no more references to it.
				/// </summary>
				class BlendState : public Memory::Handle
				{
				public:
					using Handle::Handle;
					using Handle::operator=;
					explicit inline BlendState(const Memory::Handle& object) : Memory::Handle(object) {}
				};

				/// <summary>
				///		Used as a render texture handle.
				///		Destroys the texture automatically when there are no more references to it.
				/// </summary>
				class RenderTexture : public Memory::Handle
				{
				public:
					using Handle::Handle;
					using Handle::operator=;
					explicit inline RenderTexture(const Memory::Handle& object) : Memory::Handle(object) {}
				};

				/// <summary>
				///		Used as a depth stencil texture handle.
				///		Destroys the texture automatically when there are no more references to it.
				/// </summary>
				class DepthStencilTexture : public Memory::Handle
				{
				public:
					using Handle::Handle;
					using Handle::operator=;
					explicit inline DepthStencilTexture(const Memory::Handle& object) : Memory::Handle(object) {}
				};

				/// <summary>
				///		Used as a framebuffer handle.
				///		Destroys the framebuffer automatically when there are no more references to it.
				/// </summary>
				class Framebuffer : public Memory::Handle
				{
				public:
					using Handle::Handle;
					using Handle::operator=;
					explicit inline Framebuffer(const Memory::Handle& object) : Memory::Handle(object) {}
				};

				/// <summary>
				///		Used as a render device handle.
				///		Destroys the render device automatically when there are no more references to it.
				/// </summary>
				class RenderDevice : public Memory::Handle
				{
				public:
					using Handle::Handle;
					using Handle::operator=;
					explicit inline RenderDevice(const Memory::Handle& object) : Memory::Handle(object) {}

					/// <summary>
					///		Creates a new vertex shader.
					/// </summary>
					/// <param name="bytecode">Shader bytecode</param>
					/// <param name="bytecodeSize">Shader bytecode size</param>
					/// <returns>Shader handle</returns>
					VertexShader CreateVertexShader(const mfmU8* bytecode, mfmU64 bytecodeSize, MetaDataHandle metaData);

					/// <summary>
					///		Creates a new pixel shader.
					/// </summary>
					/// <param name="bytecode">Shader bytecode</param>
					/// <param name="bytecodeSize">Shader bytecode size</param>
					/// <returns>Shader handle</returns>
					PixelShader CreatePixelShader(const mfmU8* bytecode, mfmU64 bytecodeSize, MetaDataHandle metaData);

					/// <summary>
					///		Creates a new pipeline,
					/// </summary>
					/// <param name="vs">Vertex shader handle</param>
					/// <param name="ps">Pixel shader handle</param>
					/// <returns>Pipeline handle</returns>
					Pipeline CreatePipeline(VertexShader vs, PixelShader ps);

					/// <summary>
					///		Sets the pipeline used for rendering.
					/// </summary>
					/// <param name="pipeline">Pipeline handle</param>
					void SetPipeline(Pipeline pipeline);

					/// <summary>
					///		Binds a constant buffer to a binding point.
					/// </summary>
					/// <param name="bp">Binding point</param>
					/// <param name="cb">Cosntant buffer handle</param>
					void BindConstantBuffer(void* bp, ConstantBuffer cb);

					/// <summary>
					///		Binds a section of a constant buffer to a binding point.
					/// </summary>
					/// <param name="bp">Binding point</param>
					/// <param name="cb">Cosntant buffer handle</param>
					/// <param name="offset">Offset in the constant buffer (in shader constants, one shader constant = 16 bytes, the number of constants must be a multiple of MFG_CONSTANT_ALIGN)</param>
					/// <param name="size">Memory size in the constant buffer (in shader constants, one shader constant = 16 bytes, the number of constants must be a multiple of MFG_CONSTANT_ALIGN)</param>
					void BindConstantBufferRange(void* bp, ConstantBuffer cb, mfmU64 offset, mfmU64 size);

					/// <summary>
					///		Binds a 1D texture to a binding point.
					/// </summary>
					/// <param name="bp">Binding point</param>
					/// <param name="tex">Texture handle</param>
					void BindTexture1D(void* bp, Texture1D tex);

					/// <summary>
					///		Binds a 2D texture to a binding point.
					/// </summary>
					/// <param name="bp">Binding point</param>
					/// <param name="tex">Texture handle</param>
					void BindTexture2D(void* bp, Texture2D tex);

					/// <summary>
					///		Binds a 3D texture to a binding point.
					/// </summary>
					/// <param name="bp">Binding point</param>
					/// <param name="tex">Texture handle</param>
					void BindTexture3D(void* bp, Texture3D tex);

					/// <summary>
					///		Binds a render texture to a binding point.
					/// </summary>
					/// <param name="bp">Binding point</param>
					/// <param name="tex">Render texture handle</param>
					void BindRenderTexture(void* bp, RenderTexture tex);

					/// <summary>
					///		Binds a sampler to a binding point.
					/// </summary>
					/// <param name="bp">Binding point</param>
					/// <param name="sampler">Sampler handle</param>
					void BindSampler(void* bp, Sampler sampler);

					/// <summary>
					///		Creates a new constant buffer.
					/// </summary>
					/// <param name="size">Constant buffer size in bytes</param>
					/// <param name="data">Constant buffer initial data (may be set to NULL if the buffer isn't static)</param>
					/// <param name="usage">Buffer usage mode</param>
					/// <returns>Buffer handle</returns>
					ConstantBuffer CreateConstantBuffer(mfmU64 size, const void* data, Usage usage);

					/// <summary>
					///		Creates a new vertex buffer.
					/// </summary>
					/// <param name="size">Vertex buffer size in bytes</param>
					/// <param name="data">Vertex buffer initial data (may be set to NULL if the buffer isn't static)</param>
					/// <param name="usage">Buffer usage mode</param>
					/// <returns>Buffer handle</returns>
					VertexBuffer CreateVertexBuffer(mfmU64 size, const void* data, Usage usage);

					/// <summary>
					///		Creates a new vertex layout.
					/// </summary>
					/// <param name="elementCount">Vertex element count</param>
					/// <param name="elements">Vertex elements</param>
					/// <param name="vs">Vertex shader handle</param>
					/// <returns>Vertex layout handle</returns>
					VertexLayout CreateVertexLayout(mfmU64 elementCount, const VertexElement* elements, VertexShader vs);

					/// <summary>
					///		Creates a new vertex array.
					/// </summary>
					/// <param name="bufferCount">Vertex buffer count</param>
					/// <param name="buffers">Vertex buffer handles</param>
					/// <param name="layout">Vertex layout handle</param>
					/// <returns>Vertex array handle</returns>
					VertexArray CreateVertexArray(mfmU64 bufferCount, VertexBuffer* buffers, VertexLayout layout);

					/// <summary>
					///		Sets the currently active vertex array.
					/// </summary>
					/// <param name="ib">Vertex array handle</param>
					void SetVertexArray(VertexArray va);

					/// <summary>
					///		Creates a new index buffer.
					/// </summary>
					/// <param name="size">Index buffer size in bytes</param>
					/// <param name="data">Index buffer initial data (may be set to NULL if the buffer isn't static)</param>
					/// <param name="type">Index data type (valid: Type::UShort; Type::UInt)</param>
					/// <param name="usage">Buffer usage mode</param>
					/// <returns>Buffer handle</returns>
					IndexBuffer CreateIndexBuffer(mfmU64 size, const void* data, Type type, Usage usage);

					/// <summary>
					///		Sets the currently active index buffer.
					/// </summary>
					/// <param name="ib">Index buffer handle</param>
					void SetIndexBuffer(IndexBuffer ib);

					/// <summary>
					///		Creates a new 1D texture.
					/// </summary>
					/// <param name="width">Texture width</param>
					/// <param name="format">Texture data format</param>
					/// <param name="data">Texture data</param>
					/// <param name="usage">Texture usage mode</param>
					/// <returns>Texture handle</returns>
					Texture1D CreateTexture1D(mfmU64 width, Format format, const void* data, Usage usage);

					/// <summary>
					///		Creates a new 2D texture.
					/// </summary>
					/// <param name="width">Texture width</param>
					/// <param name="height">Texture height</param>
					/// <param name="format">Texture data format</param>
					/// <param name="data">Texture data</param>
					/// <param name="usage">Texture usage mode</param>
					/// <returns>Texture handle</returns>
					Texture2D CreateTexture2D(mfmU64 width, mfmU64 height, Format format, const void* data, Usage usage);

					/// <summary>
					///		Creates a new 3D texture.
					/// </summary>
					/// <param name="width">Texture width</param>
					/// <param name="height">Texture height</param>
					/// <param name="depth">Texture depth</param>
					/// <param name="format">Texture data format</param>
					/// <param name="data">Texture data</param>
					/// <param name="usage">Texture usage mode</param>
					/// <returns>Texture handle</returns>
					Texture3D CreateTexture3D(mfmU64 width, mfmU64 height, mfmU64 depth, Format format, const void* data, Usage usage);

					/// <summary>
					///		Creates a new sampler.
					/// </summary>
					/// <param name="desc">Sampler description</param>
					/// <returns>Sampler handle</returns>
					Sampler CreateSampler(const SamplerDesc & desc);

					/// <summary>
					///		Creates a new rasterizer state.
					/// </summary>
					/// <param name="desc">Rasterizer state description</param>
					/// <returns>Rasterizer state handle</returns>
					RasterState CreateRasterState(const RasterStateDesc & desc);

					/// <summary>
					///		Creates a new depth stencil state.
					/// </summary>
					/// <param name="desc">Depth stencil state description</param>
					/// <returns>Depth stencil state handle</returns>
					DepthStencilState CreateDepthStencilState(const DepthStencilStateDesc & desc);

					/// <summary>
					///		Creates a new blend state.
					/// </summary>
					/// <param name="desc">Blend state description</param>
					/// <returns>Blend state handle</returns>
					BlendState CreateBlendState(const BlendStateDesc & desc);

					/// <summary>
					///		Sets the currently active rasterizer state.
					/// </summary>
					/// <param name="state">State handle</param>
					void SetRasterState(RasterState state);

					/// <summary>
					///		Sets the currently active depth stencil state.
					/// </summary>
					/// <param name="state">State handle</param>
					void SetDepthStencilState(DepthStencilState state);

					/// <summary>
					///		Sets the currently active blend state.
					/// </summary>
					/// <param name="state">State handle</param>
					void SetBlendState(BlendState state);

					/// <summary>
					///		Creates a new render texture.
					/// </summary>
					/// <param name="width">Texture width</param>
					/// <param name="height">Texture height</param>
					/// <param name="format">Texture data format</param>
					/// <returns>Texture handle</returns>
					RenderTexture CreateRenderTexture(mfmU64 width, mfmU64 height, Format format);

					/// <summary>
					///		Creates a new depth stencil texture.
					/// </summary>
					/// <param name="width">Texture width</param>
					/// <param name="height">Texture height</param>
					/// <param name="format">Texture data format</param>
					/// <returns>Texture handle</returns>
					DepthStencilTexture CreateDepthStencilTexture(mfmU64 width, mfmU64 height, Format format);

					/// <summary>
					///		Creates a new framebuffer.
					/// </summary>
					/// <param name="textureCount">Render texture count</param>
					/// <param name="textures">Render texture handles</param>
					/// <param name="depthStencilTexture">Depth stencil texture handle (optional, may be NULL)</param>
					/// <returns>Framebuffer handle</returns>
					Framebuffer CreateFramebuffer(mfmU64 textureCount, RenderTexture* textures, DepthStencilTexture depthStencilTexture);

					/// <summary>
					///		Sets the currently active framebuffer.
					/// </summary>
					/// <param name="framebuffer">Framebuffer handle</param>
					void SetFramebuffer(Framebuffer framebuffer);

					/// <summary>
					///		Clears the current framebuffer color textures.
					/// </summary>
					/// <param name="r">New red component value</param>
					/// <param name="g">New green component value</param>
					/// <param name="b">New blue component value</param>
					/// <param name="a">New alpha component value</param>
					void ClearColor(mfmF32 r, mfmF32 g, mfmF32 b, mfmF32 a);

					/// <summary>
					///		Clears the current framebuffer depth texture.
					/// </summary>
					/// <param name="depth">New depth value</param>
					void ClearDepth(mfmF32 depth);

					/// <summary>
					///		Clears the current framebuffer stencil texture.
					/// </summary>
					/// <param name="stencil">New stencil value</param>
					void ClearStencil(mfmI32 stencil);

					/// <summary>
					///		Draws the triangles stored in the currently active vertex array using the currently active pipeline.
					/// </summary>
					/// <param name="offset">First vertex offset</param>
					/// <param name="count">Vertex count</param>
					void DrawTriangles(mfmU64 offset, mfmU64 count);

					/// <summary>
					///		Draws the triangles stored in the currently active vertex array using the currently active pipeline and index buffer.
					/// </summary>
					/// <param name="offset">First index offset</param>
					/// <param name="count">Index count</param>
					void DrawTrianglesIndexed(mfmU64 offset, mfmU64 count);

					/// <summary>
					///		Swaps the front and back buffers.
					/// </summary>
					void SwapBuffers();
				};

				/// <summary>
				///		Creates a new render device.
				/// </summary>
				/// <param name="type">Render device type name</param>
				/// <param name="window">Render device window</param>
				/// <param name="desc">Render device description</param>
				/// <param name="allocator">Render device allocator</param>
				/// <returns>Render device handle</returns>
				RenderDevice CreateRenderDevice(const mfsUTF8CodeUnit* type, Input::WindowHandle window, const RenderDeviceDesc* desc, Memory::AllocatorHandle allocator);
			}
		}
	}
}
