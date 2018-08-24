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
