#pragma once

#include <string>

namespace Magma
{
	namespace Framework
	{
		namespace Graphics
		{
			class Texture2D;
			class ConstantBuffer;

			/// <summary>
			///		Encapsulaes a shader pipeline binding point
			/// </summary>
			class PipelineBindingPoint
			{
			public:
				virtual ~PipelineBindingPoint() = default;

				/// <summary>
				///		Binds a 2D texture to this pipeline binding point
				/// </summary>
				/// <param name="texture">Texture handle</param>
				virtual void Bind(Texture2D* texture) = 0;

				/// <summary>
				///		Binds a constant buffer to this pipeline binding point
				/// </summary>
				/// <param name="buffer">Constant buffer handle</param>
				virtual void Bind(ConstantBuffer* buffer) = 0;

			protected:
				/// <summary>
				///		Used to ensure that these are never created directly
				/// </summary>
				PipelineBindingPoint() = default;
			};

			/// <summary>
			///		Encapsulates a shader pipeline
			/// </summary>
			class Pipeline
			{
			public:
				virtual ~Pipeline() = default;

				/// <summary>
				///		Gets a pipeline binding point from this pipeline
				/// </summary>
				/// <param name="name">Binding point name</param>
				/// <returns>A pipeline binding point handle</returns>
				virtual PipelineBindingPoint* GetBindingPoint(const char* name) = 0;

			protected:
				/// <summary>
				///		Used to ensure that these are never created directly
				/// </summary>
				Pipeline() = default;
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
				
			protected:
				/// <summary>
				///		Used to ensure that these are never created directly
				/// </summary>
				IndexBuffer() = default;
			};

			/// <summary>
			///		Encapsulates a 2D texture
			/// </summary>
			class Texture2D
			{
			public:
				virtual ~Texture2D() = default;

			protected:
				/// <summary>
				///		Used to ensure that these are never created directly
				/// </summary>
				Texture2D() = default;
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
				///		Normalized signed int (32 bits)
				/// </summary>
				NInt,

				/// <summary>
				///		Normalized unsigned byte (8 bits)
				/// </summary>
				NUByte,
				
				/// <summary>
				///		Normalized unsigned short (16 bits)
				/// </summary>
				NUShort,

				/// <summary>
				///		Normalized unsigned int (32 bits)
				/// </summary>
				NUInt,

				/// <summary>
				///		Half float (16 bits)
				/// </summary>
				HalfFloat,

				/// <summary>
				///		Float (32 bits)
				/// </summary>
				Float,

				/// <summary>
				///		
				/// </summary>
				Double,

				/// <summary>
				///		Number of vertex element types
				/// </summary>
				Count
			};

			/// <summary>
			///		Describes a vertex element within a vertex buffer
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
				///		Only draws points
				/// </summary>
				Point,

				/// <summary>
				///		Only draws lines
				/// </summary>
				Line,

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
			///		Abstract class that encapsulates the low level API specific rendering calls
			/// </summary>
			class RenderDevice
			{
			public:
				virtual ~RenderDevice() = default;

				/// <summary>
				///		Creates a shader pipeline from the supplied code (MSL)
				/// </summary>
				/// <param name="code">MSL source code</param>
				/// <returns>Pipeline handle</returns>
				virtual Pipeline* CreatePipeline(const char* code) = 0;

				/// <summary>
				///		Destroys a shader pipeline
				/// </summary>
				/// <param name="pipeline">Pipeline handle</param>
				virtual void DestroyPipeline(Pipeline* pipeline) = 0;
			};
		}
	}
}