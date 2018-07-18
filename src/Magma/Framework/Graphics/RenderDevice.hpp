#pragma once

#include <string>
#include <stdexcept>
#include <glm/glm.hpp>

#include "ShaderData.hpp"
#include "Exception.hpp"

#include "../Input/Window.hpp"

namespace Magma
{
	namespace Framework
	{
		namespace Graphics
		{
			/// <summary>
			///		Encapsulates a 2D texture
			/// </summary>
			class Texture2D
			{
			public:
				virtual ~Texture2D() = default;

				/// <summary>
				///		Updates the texture data or part of it
				/// </summary>
				/// <param name="dstX">Destination X</param>
				/// <param name="dstY">Destination Y</param>
				/// <param name="width">New data width</param>
				/// <param name="height">New data height</param>
				/// <param name="data">New data pointer</param>
				virtual void Update(size_t dstX, size_t dstY, size_t width, size_t height, void* data) = 0;

				/// <summary>
				///		Generates mipmaps for this texture
				/// </summary>
				virtual void GenerateMipmaps() = 0;

			protected:
				/// <summary>
				///		Used to ensure that these are never created directly
				/// </summary>
				Texture2D() = default;
			};

			/// <summary>
			///		Describes a texture format
			/// </summary>
			enum class TextureFormat
			{
				/// <summary>
				///		Invalid texture format
				/// </summary>
				Invalid = -1,

				/// <summary>
				///		Signed char that gets normalized into a [-1; 1] floating point
				/// </summary>
				R8SNorm,

				/// <summary>
				///		Signed short that gets normalized into a [-1; 1] floating point
				/// </summary>
				R16SNorm,

				/// <summary>
				///		Signed chars that get normalized into a [-1; 1] floating point
				/// </summary>
				RG8SNorm,

				/// <summary>
				///		Signed shorts that get normalized into a [-1; 1] floating point
				/// </summary>
				RG16SNorm,

				/// <summary>
				///		Signed chars that get normalized into a [-1; 1] floating point
				/// </summary>
				RGBA8SNorm,

				/// <summary>
				///		Signed shorts that get normalized into a [-1; 1] floating point
				/// </summary>
				RGBA16SNorm,

				/// <summary>
				///		Unsigned char that gets normalized into a [0; 1] floating point
				/// </summary>
				R8UNorm,

				/// <summary>
				///		Unsigned short that gets normalized into a [0; 1] floating point
				/// </summary>
				R16UNorm,

				/// <summary>
				///		Unsigned chars that get normalized into a [0; 1] floating point
				/// </summary>
				RG8UNorm,

				/// <summary>
				///		Unsigned shorts that get normalized into a [0; 1] floating point
				/// </summary>
				RG16UNorm,

				/// <summary>
				///		Unsigned chars that get normalized into a [0; 1] floating point
				/// </summary>
				RGBA8UNorm,

				/// <summary>
				///		Unsigned shorts that get normalized into a [0; 1] floating point
				/// </summary>
				RGBA16UNorm,

				/// <summary>
				///		Signed char
				/// </summary>
				R8Int,

				/// <summary>
				///		Signed short
				/// </summary>
				R16Int,

				/// <summary>
				///		Signed chars
				/// </summary>
				RG8Int,

				/// <summary>
				///		Signed shorts
				/// </summary>
				RG16Int,

				/// <summary>
				///		Signed chars
				/// </summary>
				RGB8Int,

				/// <summary>
				///		Signed shorts
				/// </summary>
				RGB16Int,

				/// <summary>
				///		Signed chars
				/// </summary>
				RGBA8Int,

				/// <summary>
				///		Signed shorts
				/// </summary>
				RGBA16Int,

				/// <summary>
				///		Unsigned char
				/// </summary>
				R8UInt,

				/// <summary>
				///		Unsigned short
				/// </summary>
				R16UInt,

				/// <summary>
				///		Unsigned chars
				/// </summary>
				RG8UInt,

				/// <summary>
				///		Unsigned shorts
				/// </summary>
				RG16UInt,

				/// <summary>
				///		Unsigned chars
				/// </summary>
				RGBA8UInt,

				/// <summary>
				///		Unsigned shorts
				/// </summary>
				RGBA16UInt,

				/// <summary>
				///		4 byte floating point
				/// </summary>
				R32Float,

				/// <summary>
				///		4 byte floating points
				/// </summary>
				RG32Float,

				/// <summary>
				///		4 byte floating points
				/// </summary>
				RGB32Float,

				/// <summary>
				///		4 byte floating points
				/// </summary>
				RGBA32Float,

				/// <summary>
				///		Texture format type count
				/// </summary>
				Count
			};

			/// <summary>
			///		Encapsulates a 2D texture sampler
			/// </summary>
			class Sampler2D
			{
			public:
				virtual ~Sampler2D() = default;

			protected:
				/// <summary>
				///		Used to ensure that these are never created directly
				/// </summary>
				Sampler2D() = default;
			};

			/// <summary>
			///		Describes a texture adress mode
			/// </summary>
			enum class TextureAdressMode
			{
				/// <summary>
				///		Invalid texture adress mode
				/// </summary>
				Invalid = -1,

				/// <summary>
				///		Repeats the coordinates when out of bounds
				/// </summary>
				Repeat,

				/// <summary>
				///		Mirrors the coordinates when out of bounds
				/// </summary>
				Mirror,

				/// <summary>
				///		Clamps the coordinates when out of bounds
				/// </summary>
				Clamp,

				/// <summary>
				///		Sets a border color when out of bounds
				/// </summary>
				Border,

				/// <summary>
				///		Texture adress mode count
				/// </summary>
				Count
			};

			/// <summary>
			///		Describes a texture filter type
			/// </summary>
			enum class TextureFilter
			{
				/// <summary>
				///		Invalid texture filter type
				/// </summary>
				Invalid = -1,

				/// <summary>
				///		No filter
				/// </summary>
				Nearest,

				/// <summary>
				///		Linear filtering
				/// </summary>
				Linear,

				/// <summary>
				///		Texture filter type count
				/// </summary>
				Count
			};

			/// <summary>
			///		Describes a 2D texture sampler
			/// </summary>
			struct Sampler2DDesc
			{
				/// <summary>
				///		Minifying filter
				/// </summary>
				TextureFilter minFilter = TextureFilter::Nearest;

				/// <summary>
				///		Magnifying filter
				/// </summary>
				TextureFilter magFilter = TextureFilter::Nearest;

				/// <summary>
				///		Mipmap filter (set to invalid to don't use mipmaps)
				/// </summary>
				TextureFilter mipmapFilter = TextureFilter::Invalid;

				/// <summary>
				///		Texture adress mode on coordinate U
				/// </summary>
				TextureAdressMode addressU = TextureAdressMode::Repeat;

				/// <summary>
				///		Texture adress mode on coordinate V
				/// </summary>
				TextureAdressMode addressV = TextureAdressMode::Repeat;

				/// <summary>
				///		Border color (when using TextureAdressMode::Border)
				/// </summary>
				glm::vec4 border = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);

				/// <summary>
				///		Max texture anisotropy (uses anistropic filtering if bigger than 1)
				/// </summary>
				unsigned int maxAnisotropy = 1;
			};

			/// <summary>
			///		Encapsulates a constant buffer
			/// </summary>
			class ConstantBuffer
			{
			public:
				virtual ~ConstantBuffer() = default;

				/// <summary>
				///		Maps the constant buffer to a location in memory to update it
				/// </summary>
				/// <returns>Pointer to the constant buffer data</returns>
				virtual void* Map() = 0;

				/// <summary>
				///		Unmaps the constant buffer and sends the data to the GPU
				/// </summary>
				virtual void Unmap() = 0;

			protected:
				/// <summary>
				///		Used to ensure that these are never created directly
				/// </summary>
				ConstantBuffer() = default;
			};


			/// <summary>
			///		Encapsulates a vertex shader binding point
			/// </summary>
			class VertexBindingPoint
			{
			public:
				virtual ~VertexBindingPoint() = default;

				/// <summary>
				///		Binds a 2D texture to this vertex shader binding point
				/// </summary>
				/// <param name="texture">Texture handle</param>
				virtual void BindTexture2D(Texture2D* texture) = 0;

				/// <summary>
				///		Binds a 2D sampler to this vertex shader binding point
				/// </summary>
				/// <param name="sampler">2D sampler handle</param>
				virtual void BindSampler2D(Sampler2D* sampler) = 0;

				/// <summary>
				///		Binds a constant buffer to this vertex shader binding point
				/// </summary>
				/// <param name="buffer">Constant buffer handle</param>
				virtual void BindConstantBuffer(ConstantBuffer* buffer) = 0;

			protected:
				/// <summary>
				///		Used to ensure that these are never created directly
				/// </summary>
				VertexBindingPoint() = default;
			};

			/// <summary>
			///		Encapsulates a vertex shader
			/// </summary>
			class VertexShader
			{
			public:
				virtual ~VertexShader() = default;

				/// <summary>
				///		Gets a binding point from this shader
				/// </summary>
				/// <param name="name">Binding point name</param>
				/// <returns>A binding point handle</returns>
				virtual VertexBindingPoint* GetBindingPoint(const char* name) = 0;

			protected:
				/// <summary>
				///		Used to ensure that these are never created directly
				/// </summary>
				VertexShader() = default;
			};

			/// <summary>
			///		Encapsulates a pixel shader binding point
			/// </summary>
			class PixelBindingPoint
			{
			public:
				virtual ~PixelBindingPoint() = default;

				/// <summary>
				///		Binds a 2D texture to this pixel shader binding point
				/// </summary>
				/// <param name="texture">Texture handle</param>
				virtual void BindTexture2D(Texture2D* texture) = 0;

				/// <summary>
				///		Binds a 2D sampler to this vertex shader binding point
				/// </summary>
				/// <param name="sampler">2D sampler handle</param>
				virtual void BindSampler2D(Sampler2D* sampler) = 0;

				/// <summary>
				///		Binds a constant buffer to this pixel shader binding point
				/// </summary>
				/// <param name="buffer">Constant buffer handle</param>
				virtual void BindConstantBuffer(ConstantBuffer* buffer) = 0;

			protected:
				/// <summary>
				///		Used to ensure that these are never created directly
				/// </summary>
				PixelBindingPoint() = default;
			};

			/// <summary>
			///		Encapsulates a vertex shader
			/// </summary>
			class PixelShader
			{
			public:
				virtual ~PixelShader() = default;

				/// <summary>
				///		Gets a binding point from this shader
				/// </summary>
				/// <param name="name">Binding point name</param>
				/// <returns>A binding point handle</returns>
				virtual PixelBindingPoint* GetBindingPoint(const char* name) = 0;

			protected:
				/// <summary>
				///		Used to ensure that these are never created directly
				/// </summary>
				PixelShader() = default;
			};

			/// <summary>
			///		Encapsulates a shader pipeline
			/// </summary>
			class Pipeline
			{
			public:
				virtual ~Pipeline() = default;

			protected:
				/// <summary>
				///		Used to ensure that these are never created directly
				/// </summary>
				Pipeline() = default;
			};

			/// <summary>
			///		Encapsulates a vertex buffer
			/// </summary>
			class VertexBuffer
			{
			public:
				virtual ~VertexBuffer() = default;

				/// <summary>
				///		Maps the vertex buffer to a location in memory to update it
				/// </summary>
				/// <returns>Pointer to the vertex buffer data</returns>
				virtual void* Map() = 0;

				/// <summary>
				///		Unmaps the vertex buffer and sends the data to the GPU
				/// </summary>
				virtual void Unmap() = 0;

			protected:
				/// <summary>
				///		Used to ensure that these are never created directly
				/// </summary>
				VertexBuffer() = default;
			};

			/// <summary>
			///		Encapsulates a vertex buffer layout description
			/// </summary>
			class VertexLayout
			{
			public:
				virtual ~VertexLayout() = default;

			protected:
				/// <summary>
				///		Used to ensure that these are never created directly
				/// </summary>
				VertexLayout() = default;
			};

			/// <summary>
			///		Encapsulates a collection of vertex buffers and their semantic descriptions
			/// </summary>
			class VertexArray
			{
			public:
				virtual ~VertexArray() = default;

			protected:
				/// <summary>
				///		Used to ensure that these are never created directly
				/// </summary>
				VertexArray() = default;
			};

			/// <summary>
			///		Encapsulates a index buffer
			/// </summary>
			class IndexBuffer
			{
			public:
				virtual ~IndexBuffer() = default;
				
				/// <summary>
				///		Maps the index buffer to a location in memory to update it
				/// </summary>
				/// <returns>Pointer to the index buffer data</returns>
				virtual void* Map() = 0;

				/// <summary>
				///		Unmaps the index buffer and sends the data to the GPU
				/// </summary>
				virtual void Unmap() = 0;

			protected:
				/// <summary>
				///		Used to ensure that these are never created directly
				/// </summary>
				IndexBuffer() = default;
			};

			/// <summary>
			///		Describes a index buffer index type
			/// </summary>
			enum class IndexType
			{
				/// <summary>
				///		Invalid index type
				/// </summary>
				Invalid = -1,

				/// <summary>
				///		Unsigned 8 bit integer
				/// </summary>
				UByte,

				/// <summary>
				///		Unsigned 16 bit integer
				/// </summary>
				UShort,

				/// <summary>
				///		Unsigned 32 bit integer
				/// </summary>
				UInt,

				/// <summary>
				///		Index type count
				/// </summary>
				Count
			};

			/// <summary>
			///		Describes a vertex element's type
			/// </summary>
			enum class VertexElementType
			{
				/// <summary>
				///		Invalid vertex element type
				/// </summary>
				Invalid = -1,

				/// <summary>
				///		Signed byte (8 bits)
				/// </summary>
				Byte,

				/// <summary>
				///		Signed short (16 bits)
				/// </summary>
				Short,

				/// <summary>
				///		Signed int (32 bits)
				/// </summary>
				Int,

				/// <summary>
				///		Unsigned byte (8 bits)
				/// </summary>
				UByte,

				/// <summary>
				///		Unsigned short (16 bits)
				/// </summary>
				UShort,

				/// <summary>
				///		Unsigned int (32 bits)
				/// </summary>
				UInt,

				/// <summary>
				///		Normalized signed byte (8 bits)
				/// </summary>
				NByte,

				/// <summary>
				///		Normalized signed short (16 bits)
				/// </summary>
				NShort,

				/// <summary>
				///		Normalized unsigned byte (8 bits)
				/// </summary>
				NUByte,
				
				/// <summary>
				///		Normalized unsigned short (16 bits)
				/// </summary>
				NUShort,

				/// <summary>
				///		Half float (16 bits)
				/// </summary>
				HalfFloat,

				/// <summary>
				///		Float (32 bits)
				/// </summary>
				Float,

				/// <summary>
				///		Number of vertex element types
				/// </summary>
				Count
			};

			/// <summary>
			///		Describes a vertex element to be sent to a vertex shader
			/// </summary>
			struct VertexElement
			{
				/// <summary>
				///		Vertex element binding point name
				/// </summary>
				std::string name;

				/// <summary>
				///		Type of vertex element
				/// </summary>
				VertexElementType type;

				/// <summary>
				///		Number of components
				/// </summary>
				size_t size;

				/// <summary>
				///		Number of bytes between each successive element
				/// </summary>
				size_t stride;

				/// <summary>
				///		Offset where the first occurence of this vertex element resides in the buffer
				/// </summary>
				size_t offset;

				/// <summary>
				///		Vertex buffer index
				/// </summary>
				size_t bufferIndex;
			};

			/// <summary>
			///		Encapsulates the rasterizer state
			/// </summary>
			class RasterState
			{
			public:
				virtual ~RasterState() = default;

			protected:
				/// <summary>
				///		Used to ensure that these are never created directly
				/// </summary>
				RasterState() = default;
			};

			/// <summary>
			///		Describes the winding used by the rasterizer
			/// </summary>
			enum class Winding
			{
				/// <summary>
				///		Invalid winding
				/// </summary>
				Invalid = -1,

				/// <summary>
				///		Clock wise
				/// </summary>
				CW,

				/// <summary>
				///		Counter clock wise
				/// </summary>
				CCW,

				/// <summary>
				///		Winding type count
				/// </summary>
				Count
			};

			/// <summary>
			///		Describes the faces used
			/// </summary>
			enum class Face
			{
				/// <summary>
				///		Invalid face
				/// </summary>
				Invalid = -1,

				/// <summary>
				///		Front face
				/// </summary>
				Front,

				/// <summary>
				///		Back face
				/// </summary>
				Back,

				/// <summary>
				///		Both front and back faces
				/// </summary>
				FrontAndBack,

				/// <summary>
				///		Number of face modes
				/// </summary>
				Count
			};

			/// <summary>
			///		Describes the rasterizer mode
			/// </summary>
			enum class RasterMode
			{
				/// <summary>
				///		Invalid rasterizer mode
				/// </summary>
				Invalid = -1,

				/// <summary>
				///		Only draws lines
				/// </summary>
				Wireframe,
				
				/// <summary>
				///		Draws the whole triangles
				/// </summary>
				Fill,

				/// <summary>
				///		Number of rasterizer modes
				/// </summary>
				Count
			};
		
			/// <summary>
			///		Used to create a rasterizer state
			/// </summary>
			struct RasterStateDesc
			{
				bool cullEnabled = true;
				Winding frontFace = Winding::CCW;
				Face cullFace = Face::Back;
				RasterMode rasterMode = RasterMode::Fill;
			};

			/// <summary>
			///		Encapsulates the depth/stencil state
			/// </summary>
			class DepthStencilState
			{
			public:
				virtual ~DepthStencilState() = default;

			protected:
				/// <summary>
				///		Used to ensure that these are never created directly
				/// </summary>
				DepthStencilState() = default;
			};

			/// <summary>
			///		Describes a comparison mode
			/// </summary>
			enum class Compare
			{
				/// <summary>
				///		Invalid comparison mdoe
				/// </summary>
				Invalid = -1,

				/// <summary>
				///		Test comparison never passes
				/// </summary>
				Never,

				/// <summary>
				///		Test comparison passes if the incoming value is less than the stored value
				/// </summary>
				Less,

				/// <summary>
				///		Test comparison passes if the incoming value is less than or equal to the stored value
				/// </summary>
				LEqual,

				/// <summary>
				///		Test comparison passes if the incoming value is greater than the stored value
				/// </summary>
				Greater,

				/// <summary>
				///		Test comparison passes if the incoming value is greater than or equal the stored value
				/// </summary>
				GEqual,

				/// <summary>
				///		Test comparison passes if the incoming value is equal to the stored value
				/// </summary>
				Equal,

				/// <summary>
				///		Test comparison passes if the incoming value is not equal to the stored value
				/// </summary>
				NotEqual,

				/// <summary>
				///		Test comparison always passes
				/// </summary>
				Always,

				/// <summary>
				///		Comparison mode count
				/// </summary>
				Count
			};

			/// <summary>
			///		Describes a stencil action
			/// </summary>
			enum class StencilAction
			{
				/// <summary>
				///		Invalid stencil action
				/// </summary>
				Invalid = -1,

				/// <summary>
				///		Keeps the current stencil buffer value
				/// </summary>
				Keep,

				/// <summary>
				///		Sets the stencil buffer to zero
				/// </summary>
				Zero,

				/// <summary>
				///		Sets the stencil buffer to the reference value masked with the write mask
				/// </summary>
				Replace,

				/// <summary>
				///		Increments the stencil buffer value and clamps to the maximum unsigned value
				/// </summary>
				Increment,

				/// <summary>
				///		Increments the stencil buffer value and wraps the stencil buffer value to zero when passing the maximum representable unsigned value
				/// </summary>
				IncrementWrap,

				/// <summary>
				///		Decrements the stencil buffer value and clamps to zero
				/// </summary>
				Decrement,

				/// <summary>
				///		Decrements the stencil buffer value and wraps the stencil buffer value to the maximum unsigned value
				/// </summary>
				DecrementWrap,

				/// <summary>
				///		Bitwise inverts the current stencil buffer value
				/// </summary>
				Invert,

				/// <summary>
				///		Stencil actions count
				/// </summary>
				Count
			};

			/// <summary>
			///		Used to create a depth stencil state
			/// </summary>
			struct DepthStencilStateDesc
			{
				bool depthEnabled = true;
				bool depthWriteEnabled = true;
				float depthNear = 0.0f;
				float depthFar = 1.0f;
				Compare depthCompare = Compare::Less;

				unsigned int stencilRef = 0;
				bool stencilEnabled = false;
				unsigned char stencilReadMask = 0xFF;
				unsigned char stencilWriteMask = 0xFF;

				Compare frontFaceStencilCompare = Compare::Always;
				StencilAction frontFaceStencilFail = StencilAction::Keep;
				StencilAction frontFaceStencilPass = StencilAction::Keep;
				StencilAction frontFaceDepthFail = StencilAction::Keep;
			
				Compare backFaceStencilCompare = Compare::Always;
				StencilAction backFaceStencilFail = StencilAction::Keep;
				StencilAction backFaceStencilPass = StencilAction::Keep;
				StencilAction backFaceDepthFail = StencilAction::Keep;
			};

			/// <summary>
			///		Encapsulates the blend state
			/// </summary>
			class BlendState
			{
			public:
				virtual ~BlendState() = default;

			protected:
				/// <summary>
				///		Used to ensure that these are never created directly
				/// </summary>
				BlendState() = default;
			};

			/// <summary>
			///		Describes how a blend function factor is computed
			/// </summary>
			enum class BlendFactor
			{
				/// <summary>
				///		Invalid blend factor type
				/// </summary>
				Invalid = -1,

				/// <summary>
				///		Factor is equal to zero
				/// </summary>
				Zero,

				/// <summary>
				///		Factor is equal to one
				/// </summary>
				One,

				/// <summary>
				///		Factor is equal to the source color
				/// </summary>
				SourceColor,

				/// <summary>
				///		Factor is equal to one minus the source color
				/// </summary>
				InverseSourceColor,

				/// <summary>
				///		Factor is equal to the destination color
				/// </summary>
				DestinationColor,

				/// <summary>
				///		Factor is equal to one minus the destination color
				/// </summary>
				InverseDestinationColor,

				/// <summary>
				///		Factor is equal to the source alpha color component
				/// </summary>
				SourceAlpha,

				/// <summary>
				///		Factor is equal to one minus the source alpha color component
				/// </summary>
				InverseSourceAlpha,

				/// <summary>
				///		Factor is equal to the destination alpha color component
				/// </summary>
				DestinationAlpha,

				/// <summary>
				///		Factor is equal to one minus the destination alpha color component
				/// </summary>
				InverseDestinationAlpha,

				/// <summary>
				///		Number of blend factor types
				/// </summary>
				Count
			};

			/// <summary>
			///		Describes a blend operation
			/// </summary>
			enum class BlendOperation
			{
				/// <summary>
				///		Invalid blend operation type
				/// </summary>
				Invalid = -1,

				/// <summary>
				///		Adds the source and the destination colors
				/// </summary>
				Add,

				/// <summary>
				///		Subtracts the destination from the source color
				/// </summary>
				Subtract,

				/// <summary>
				///		Substract the source from the destination color
				/// </summary>
				ReverseSubtract,

				/// <summary>
				///		The output color is the minimum value of the source and the destination colors
				/// </summary>
				Min,

				/// <summary>
				///		The output color is the maximum value of the source and the destination colors
				/// </summary>
				Max,

				/// <summary>
				///		Number of blend operation types
				/// </summary>
				Count
			};

			/// <summary>
			///		Describes a blend state
			/// </summary>
			struct BlendStateDesc
			{
				/// <summary>
				///		Is blending enabled?
				/// </summary>
				bool blendEnabled = false;

				/// <summary>
				///		Source color factor
				/// </summary>
				BlendFactor sourceFactor = BlendFactor::One;

				/// <summary>
				///		Destination color factor
				/// </summary>
				BlendFactor destinationFactor = BlendFactor::Zero;

				/// <summary>
				///		Color blend operation
				/// </summary>
				BlendOperation blendOperation = BlendOperation::Add;

				/// <summary>
				///		Source alpha factor
				/// </summary>
				BlendFactor sourceAlphaFactor = BlendFactor::One;

				/// <summary>
				///		Destination alpha factor
				/// </summary>
				BlendFactor destinationAlphaFactor = BlendFactor::Zero;

				/// <summary>
				///		Alpha blend operation
				/// </summary>
				BlendOperation alphaBlendOperation = BlendOperation::Add;
			};

			/// <summary>
			///		Describes a buffer's usage
			/// </summary>
			enum class BufferUsage
			{
				Invalid = -1,

				/// <summary>
				///		Default usage (GPU can read and write)
				/// </summary>
				Default,

				/// <summary>
				///		Static usage (GPU can read)
				/// </summary>
				Static,

				/// <summary>
				///		Dynamic usage (GPU can read and CPU can write)
				/// </summary>
				Dynamic,
				
				/// <summary>
				///		Buffer usage types count
				/// </summary>
				Count
			};

			/// <summary>
			///		Encapsulates a depth and stencil buffer
			/// </summary>
			class DepthStencilBuffer
			{
			public:
				virtual ~DepthStencilBuffer() = default;

			protected:
				/// <summary>
				///		Used to ensure that these are never created directly
				/// </summary>
				DepthStencilBuffer() = default;
			};

			/// <summary>
			///		Represents a depth and stencil buffer format
			/// </summary>
			enum class DepthStencilFormat
			{
				/// <summary>
				///		Invalid depth and stencil buffer format type
				/// </summary>
				Invalid = -1,

				/// <summary>
				///		24 bits for depth and 8 bits for stencil on a 32 bit value
				/// </summary>
				Depth24Stencil8,

				/// <summary>
				///		32 bits for depth and 8 bits for stencil on two 32 bit values (24 bits unused)
				/// </summary>
				Depth32Stencil8,

				/// <summary>
				///		Depth and stencil buffer format type count
				/// </summary>
				Count
			};

			/// <summary>
			///		Encapsulates a framebuffer
			/// </summary>
			class Framebuffer
			{
			public:
				virtual ~Framebuffer() = default;

			protected:
				/// <summary>
				///		Used to ensure that these are never created directly
				/// </summary>
				Framebuffer() = default;
			};

			/// <summary>
			///		Render device initialization settings
			/// </summary>
			struct RenderDeviceSettings
			{
				/// <summary>
				///		Should VSync be enabled?
				/// </summary>
				bool enableVSync = false;
			};

			/// <summary>
			///		Abstract class that encapsulates the low level API specific rendering calls
			/// </summary>
			class RenderDevice
			{
			public:
				virtual ~RenderDevice() = default;

				/// <summary>
				///		Inits the render device
				/// </summary>
				/// <param name="window">Window to render to</param>
				/// <param name="settings">Render device settings</param>
				virtual void Init(Input::Window* window, const RenderDeviceSettings& settings) = 0;

				/// <summary>
				///		Terminates the render device
				/// </summary>
				virtual void Terminate() = 0;

				/// <summary>
				///		Creates a vertex shader
				/// </summary>
				/// <param name="data">Shader data</param>
				/// <returns>Vertex shader handle</returns>
				virtual VertexShader* CreateVertexShader(const ShaderData& data) = 0;

				/// <summary>
				///		Destroys a vertex shader
				/// </summary>
				/// <param name="vertexShader">Vertex shader handle</param>
				virtual void DestroyVertexShader(VertexShader* vertexShader) = 0;
				
				/// <summary>
				///		Creates a pixel shader
				/// </summary>
				/// <param name="code">Shader data</param>
				/// <returns>Pixel shader handle</returns>
				virtual PixelShader* CreatePixelShader(const ShaderData& data) = 0;

				/// <summary>
				///		Destroys a pixel shader
				/// </summary>
				/// <param name="pixelShader">Pixel shader handle</param>
				virtual void DestroyPixelShader(PixelShader* pixelShader) = 0;

				/// <summary>
				///		Creates a shader pipeline from the supplied code (MSL)
				/// </summary>
				/// <param name="vertexShader">Vertex shader handle</param>
				/// <param name="pixelShader">Pixel shader handle</param>
				/// <returns>Pipeline handle</returns>
				virtual Pipeline* CreatePipeline(VertexShader* vertexShader, PixelShader* pixelShader) = 0;

				/// <summary>
				///		Destroys a shader pipeline
				/// </summary>
				/// <param name="pipeline">Pipeline handle</param>
				virtual void DestroyPipeline(Pipeline* pipeline) = 0;

				/// <summary>
				///		Sets a shader pipeline as the active one for rendering
				/// </summary>
				/// <param name="pipeline">Pipeline handle</param>
				virtual void SetPipeline(Pipeline* pipeline) = 0;

				/// <summary>
				///		Creates a new vertex buffer
				/// </summary>
				/// <param name="size">Vertex buffer size in bytes</param>
				/// <param name="data">Vertex buffer data pointer (set to null to start empty)</param>
				/// <param name="usage">Vertex buffer usage mode</param>
				/// <returns>Vertex buffer handle</returns>
				virtual VertexBuffer* CreateVertexBuffer(size_t size, const void* data = nullptr, BufferUsage usage = BufferUsage::Default) = 0;

				/// <summary>
				///		Destroys a vertex buffer
				/// </summary>
				/// <param name="vertexBuffer">Vertex buffer handle</param>
				virtual void DestroyVertexBuffer(VertexBuffer* vertexBuffer) = 0;

				/// <summary>
				///		Creates a new vertex layout
				/// </summary>
				/// <param name="elementCount">Number of vertex elements</param>
				/// <param name="elements">Vertex elements</param>
				/// <param name="vertexShader">Corresponding vertex shader</param>
				/// <returns>Vertex layout handle</returns>
				virtual VertexLayout* CreateVertexLayout(size_t elementCount, const VertexElement* elements, VertexShader* vertexShader) = 0;

				/// <summary>
				///		Destroys a vertex layout
				/// </summary>
				/// <param name="vertexLayout">Vertex layout handle</param>
				virtual void DestroyVertexLayout(VertexLayout* vertexLayout) = 0;

				/// <summary>
				///		Creates a vertex array
				/// </summary>
				/// <param name="bufferCount">Vertex buffer count</param>
				/// <param name="buffers">Pointer to array containing the vertex buffer handles</param>
				/// <param name="layout">Vertex layout handle</param>
				/// <returns>Vertex array handle</returns>
				virtual VertexArray* CreateVertexArray(size_t bufferCount, VertexBuffer** buffers, VertexLayout* layout) = 0;

				/// <summary>
				///		Destroys a vertex array
				/// </summary>
				/// <param name="vertexArray">Vertex array handle</param>
				virtual void DestroyVertexArray(VertexArray* vertexArray) = 0;

				/// <summary>
				///		Sets a vertex array as active for rendering calls
				/// </summary>
				/// <param name="vertexArray">Vertex array handle</param>
				virtual void SetVertexArray(VertexArray* vertexArray) = 0;

				/// <summary>
				///		Creates an index buffer
				/// </summary>
				/// <param name="type">Index type</param>
				/// <param name="size">Index buffer size</param>
				/// <param name="data">Index buffer data pointer (set to nullptr to create an empty index buffer)</param>
				/// <param name="usage">Index buffer usage mode</param>
				/// <returns>Index buffer handle</returns>
				virtual IndexBuffer* CreateIndexBuffer(IndexType type, size_t size, const void* data = nullptr, BufferUsage usage = BufferUsage::Default) = 0;

				/// <summary>
				///		Destroys an index buffer
				/// </summary>
				/// <param name="indexBuffer">Index buffer handle</param>
				virtual void DestroyIndexBuffer(IndexBuffer* indexBuffer) = 0;

				/// <summary>
				///		Sets an index buffer as active for rendering calls
				/// </summary>
				/// <param name="indexBuffer">Index buffer handle</param>
				virtual void SetIndexBuffer(IndexBuffer* indexBuffer) = 0;

				/// <summary>
				///		Creates a 2D texture
				/// </summary>
				/// <param name="width">Texture width</param>
				/// <param name="height">Texture height</param>
				/// <param name="format">Texture format</param>
				/// <param name="data">Texture initial data (set to nullptr to create empty texture)</param>
				/// <param name="usage">Texture usage mode</param>
				/// <param name="isRenderTarget">Can the texture be used as a render target?</param>
				/// <returns>2D texture handle</returns>
				virtual Texture2D* CreateTexture2D(size_t width, size_t height, TextureFormat format, const void* data = nullptr, BufferUsage usage = BufferUsage::Default, bool isRenderTarget = false) = 0;
				
				/// <summary>
				///		Destroys a 2D texture
				/// </summary>
				/// <param name="texture">2D texture handle</param>
				virtual void DestroyTexture2D(Texture2D* texture) = 0;

				/// <summary>
				///		Creates a 2D sampler
				/// </summary>
				/// <param name="desc">2D sampler description</param>
				/// <returns>2D sampler handle</returns>
				virtual Sampler2D* CreateSampler2D(const Sampler2DDesc& desc) = 0;

				/// <summary>
				///		Destroys a 2D sampler
				/// </summary>
				/// <param name="sampler">2D sampler handle</param>
				virtual void DestroySampler2D(Sampler2D* sampler) = 0;

				/// <summary>
				///		Creates a rasterizer state
				/// </summary>
				/// <param name="desc">Rasterizer state description</param>
				/// <returns>Rasterizer state handle</returns>
				virtual RasterState* CreateRasterState(const RasterStateDesc& desc) = 0;

				/// <summary>
				///		Destroys a rasterizer state
				/// </summary>
				/// <param name="rasterState">Rasterizer state handle</param>
				virtual void DestroyRasterState(RasterState* rasterState) = 0;

				/// <summary>
				///		Sets a rasterizer state as active for rendering calls
				/// </summary>
				virtual void SetRasterState(RasterState* rasterState) = 0;

				/// <summary>
				///		Creates a depth stencil state
				/// </summary>
				/// <param name="desc">Depth stencil state description</param>
				/// <returns>Depth stencil state handle</returns>
				virtual DepthStencilState* CreateDepthStencilState(const DepthStencilStateDesc& desc) = 0;

				/// <summary>
				///		Destroys a depth stencil state
				/// </summary>
				/// <param name="depthStencilState">Depth stencil state handle</param>
				virtual void DestroyDepthStencilState(DepthStencilState* depthStencilState) = 0;

				/// <summary>
				///		Sets a depth stencil state as active for rendering calls
				/// </summary>
				/// <param name="depthStencilState">Depth stencil state handle</param>
				virtual void SetDepthStencilState(DepthStencilState* depthStencilState) = 0;

				/// <summary>
				///		Clears the current render target color buffer, depth buffer and stencil buffer to the specified values (only clears them if the render target has them)
				/// </summary>
				/// <param name="color">Clear color</param>
				/// <param name="depth">Clear depth</param>
				/// <param name="stencil">Clear stencil</param>
				virtual void Clear(glm::vec4 color, float depth = 1.0f, int stencil = 0) = 0;
			
				/// <summary>
				///		Draws the triangles stored in the currently active vertex array and using the currently active shader pipeline
				/// </summary>
				/// <param name="offset">Vertex to start drawing from</param>
				/// <param name="count">Vertex count</param>
				virtual void DrawTriangles(size_t offset, size_t count) = 0;

				/// <summary>
				///		Draws the triangles stored in the currently active vertex array and using the currently active shader pipeline and index buffer
				/// </summary>
				/// <param name="offset">Vertex to start drawing from</param>
				/// <param name="count">Vertex count</param>
				virtual void DrawTrianglesIndexed(size_t offset, size_t count) = 0;

				/// <summary>
				///		Swaps the front and back buffers
				/// </summary>
				virtual void SwapBuffers() = 0;

				/// <summary>
				///		Creates a constant buffer
				/// </summary>
				/// <param name="size">Constant buffer size</param>
				/// <param name="data">Constant buffer initial data (set to nullptr to create an empty constant buffer)</param>
				/// <param name="usage">Constant buffer usage mode</param>
				/// <returns>Constant buffer handle</returns>
				virtual ConstantBuffer* CreateConstantBuffer(size_t size, const void* data = nullptr, BufferUsage usage = BufferUsage::Dynamic) = 0;

				/// <summary>
				///		Destroys a constant buffer
				/// </summary>
				/// <param name="constantBuffer">Constant buffer handle</param>
				virtual void DestroyConstantBuffer(ConstantBuffer* constantBuffer) = 0;

				/// <summary>
				///		Creates a blend state
				/// </summary>
				/// <param name="desc">Blend state description</param>
				/// <returns>Blend state handle</returns>
				virtual BlendState* CreateBlendState(const BlendStateDesc& desc) = 0;

				/// <summary>
				///		Destroys a blend state
				/// </summary>
				/// <param name="depthStencilState">Blend state handle</param>
				virtual void DestroyBlendState(BlendState* blendState) = 0;

				/// <summary>
				///		Sets a blend state as active for rendering calls
				/// </summary>
				/// <param name="blendState">Blend state handle</param>
				virtual void SetBlendState(BlendState* blendState) = 0;

				/// <summary>
				///		Creates a new depth stencil buffer
				/// </summary>
				/// <param name="width">Depth stencil buffer width</param>
				/// <param name="height">Depth stencil buffer height</param>
				/// <param name="format">Depth stencil buffer format</param>
				/// <returns>Depth stencil buffer handle</returns>
				virtual DepthStencilBuffer* CreateDepthStencilBuffer(size_t width, size_t height, DepthStencilFormat format) = 0;

				/// <summary>
				///		Destroys a depth stencil buffer
				/// </summary>
				/// <param name="depthStencilBuffer">Depth stencil buffer handle</param>
				virtual void DestroyDepthStencilBuffer(DepthStencilBuffer* depthStencilBuffer) = 0;

				/// <summary>
				///		Creates a new framebuffer
				/// </summary>
				/// <param name="attachmentCount">Number of color attachments</param>
				/// <param name="attachments">Color attachment array pointer</param>
				/// <param name="depthAttachment">Depth and stencil attachment pointer (set to nullptr to not use a depth and stencil attachment)</param>
				/// <returns>Framebuffer handle</returns>
				virtual Framebuffer* CreateFramebuffer(size_t attachmentCount, Texture2D** attachments, DepthStencilBuffer* depthStencilAttachment = nullptr) = 0;

				/// <summary>
				///		Destroys a framebuffer
				/// </summary>
				/// <param name="framebuffer">Framebuffer handle</param>
				virtual void DestroyFramebuffer(Framebuffer* framebuffer) = 0;

				/// <summary>
				///		Sets the current framebuffer
				/// </summary>
				/// <param name="framebuffer">Framebuffer to render to (set to nullptr to set the default framebuffer</param>
				virtual void SetFramebuffer(Framebuffer* framebuffer) = 0;
			};
		}
	}
}