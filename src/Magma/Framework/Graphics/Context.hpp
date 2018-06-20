#pragma once

#include "../Input/Window.hpp"

#include <glm/glm.hpp>
#include <vector>
#include <string>

namespace Magma
{
	namespace Framework
	{
		namespace Graphics
		{
			/// <summary>
			///		Draw mode
			/// </summary>
			enum class DrawMode
			{
				Invalid = -1,

				Points,
				Lines,
				LineStrip,
				Triangles,
				TriangleStrip,

				Count
			};

			/// <summary>
			///		Shader type
			/// </summary>
			enum class ShaderType
			{
				Invalid = -1,

				Vertex,
				Pixel,

				Count
			};

			/// <summary>
			///		Buffer bit mask
			/// </summary>
			enum class BufferBit
			{
				None	= 0x00,
				Color	= 0x01,
				Depth	= 0x02,
				Stencil	= 0x04,
			};

			/// <summary>
			///		Data type for index buffers
			/// </summary>
			enum class IndexFormat
			{
				Invalid = -1,

				UInt8,
				UInt16,
				UInt32,

				Count
			};

			inline BufferBit operator|(const BufferBit& lhs, const BufferBit& rhs) { return static_cast<BufferBit>(static_cast<int>(lhs) | static_cast<int>(rhs)); }
			inline BufferBit operator&(const BufferBit& lhs, const BufferBit& rhs) { return static_cast<BufferBit>(static_cast<int>(lhs) & static_cast<int>(rhs)); }
			inline BufferBit& operator|=(BufferBit& lhs, const BufferBit& rhs) { lhs = static_cast<BufferBit>(static_cast<int>(lhs) | static_cast<int>(rhs)); return lhs; }
			inline BufferBit& operator&=(BufferBit& lhs, const BufferBit& rhs) { lhs = static_cast<BufferBit>(static_cast<int>(lhs) & static_cast<int>(rhs)); return lhs; }

			/// <summary>
			///		Settings used to create a rendering context
			/// </summary>
			struct ContextSettings
			{
				unsigned int multisampleCount = 4;
			};

			/// <summary>
			///		Vertex data formats
			/// </summary>
			enum class VertexElementFormat
			{
				Invalid = -1,

				Float1,
				Float2,
				Float3,
				Float4,

				Int1,
				Int2,
				Int3,
				Int4,

				UInt1,
				UInt2,
				UInt3,
				UInt4,

				Count
			};

			/// <summary>
			///		Vertex data
			/// </summary>
			struct VertexLayoutElement
			{
				VertexElementFormat format;
				std::string name;
				unsigned int index;
				unsigned int offset;
			};

			struct VertexLayout
			{
				size_t size;
				std::vector<VertexLayoutElement> elements;
			};

			/// <summary>
			///		Abstract class for low level rendering calls
			/// </summary>
			class Context
			{
			public:
				Context() = default;
				virtual ~Context() = default;

				/// <summary>
				///		Inits the rendering context on a window.
				/// </summary>
				/// <param name="window">Window</param>
				virtual void Init(Input::Window* window, const ContextSettings& settings) = 0;

				/// <summary>
				///		Terminates the rendering context.
				/// </summary>
				virtual void Terminate() = 0;

				/// <summary>
				///		Swaps the front and back buffers.
				/// </summary>
				virtual void SwapBuffers() = 0;

				/// <summary>
				///		Sets the color to be set when clearing.
				/// </summary>
				/// <param name="color">Color</param>
				virtual void SetClearColor(glm::vec4 color) = 0;

				/// <summary>
				///		Clears the current target.
				/// </summary>
				/// <param name="mask">Buffer bit mask</param>
				virtual void Clear(BufferBit mask) = 0;

				/// <summary>
				///		Creates a vertex buffer
				/// </summary>
				/// <param name="data">Data to fill buffer</param>
				/// <param name="size">Data size</param>
				/// <param name="layout">Data layout</param>
				/// <param name="vertexShader">Vertex shader handle</param>
				/// <returns>Vertex buffer handle</returns>
				virtual void* CreateVertexBuffer(void* data, size_t size, const VertexLayout& layout, void* vertexShader) = 0;

				/// <summary>
				///		Destroys a vertex buffer
				/// </summary>
				/// <param name="vertexBuffer">Vertex buffer handle</param>
				virtual void DestroyVertexBuffer(void* vertexBuffer) = 0;

				/// <summary>
				///		Sets a vertex buffer as active
				/// </summary>
				/// <param name="vertexBuffer">Vertex buffer handle to set as active</param>
				virtual void BindVertexBuffer(void* vertexBuffer) = 0;

				/// <summary>
				///		Draws the current active vertex buffer
				/// </summary>
				/// <param name="vertexCount">Number of vertexes to be drawn</param>
				/// <param name="offset">Starting vertex ID</param>
				/// <param name="mode">Draw mode</param>
				virtual void Draw(size_t vertexCount, size_t offset, DrawMode mode) = 0;

				/// <summary>
				///		Creates a new shader
				/// </summary>
				/// <param name="type">Shader type</param>
				/// <param name="src">Shader source code</param>
				/// <returns>Shader handle</returns>
				virtual void* CreateShader(ShaderType type, const std::string& src) = 0;

				/// <summary>
				///		Destroys a shader
				/// </summary>
				/// <param name="shader">Shader handle</param>
				virtual void DestroyShader(void* shader) = 0;

				/// <summary>
				///		Creates a program
				/// </summary>
				/// <returns>Program handle</returns>
				virtual void* CreateProgram() = 0;

				/// <summary>
				///		Destroys a program
				/// </summary>
				/// <param name="program">Program handle</param>
				virtual void DestroyProgram(void* program) = 0;

				/// <summary>
				///		Attaches a shader to a program
				/// </summary>
				/// <param name="program">Program handle</param>
				/// <param name="shader">Shader handle</param>
				virtual void AttachShader(void* program, void* shader) = 0;
				
				/// <summary>
				///		Detaches a shader from a program
				/// </summary>
				/// <param name="program">Program handle</param>
				/// <param name="shader">Shader handle</param>
				virtual void DetachShader(void* program, void* shader) = 0;

				/// <summary>
				///		Links a program
				/// </summary>
				/// <param name="program">Program handle</param>
				virtual void LinkProgram(void* program) = 0;

				/// <summary>
				///		Binds a program
				/// </summary>
				/// <param name="program">Program handle</param>
				virtual void BindProgram(void* program) = 0;

				/// <summary>
				///		Creates a new index buffer
				/// </summary>
				/// <param name="data">Bufer containing the indexes</param>
				/// <param name="size">Buffer size</param>
				/// <param name="type">Index data type</param>
				/// <returns>Index buffer handle</returns>
				virtual void* CreateIndexBuffer(void* data, size_t size, IndexFormat type) = 0;

				/// <summary>
				///		Destroys an index buffer
				/// </summary>
				/// <param name="indexBuffer">Index buffer handle</param>
				virtual void DestroyIndexBuffer(void* indexBuffer) = 0;

				/// <summary>
				///		Binds an index buffer
				/// </summary>
				/// <param name="indexBuffer"></param>
				virtual void BindIndexBuffer(void* indexBuffer) = 0;

				/// <summary>
				///		Draws the current active vertex buffer
				/// </summary>
				/// <param name="vertexCount">Number of vertexes to be drawn</param>
				/// <param name="offset">Starting vertex ID</param>
				/// <param name="mode">Draw mode</param>
				virtual void DrawIndexed(size_t indexCount, size_t offset, DrawMode mode) = 0;
			};
		}
	}
}