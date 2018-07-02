#pragma once

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
			///		Abstract class for handling low level rendering calls
			/// </summary>
			class Context
			{

			};
		}
	}
}