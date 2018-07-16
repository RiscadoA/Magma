#pragma once

#include "../../Input/Window.hpp"

#include <glm/glm.hpp>
#include <vector>
#include <string>

/*
	Each context implementation will convert the framework's own shading language (Magma Shading Language, ".msl") into the context shading language (GLSL, HLSL, etc).
*/

namespace Magma
{
	namespace Framework
	{
		namespace Graphics
		{
			/// <summary>
			///		Resource usage types.
			/// </summary>
			enum class Usage
			{
				Invalid = -1,

				/// <summary>
				///		Requires read and write access by the GPU.
				/// </summary>
				Default,

				/// <summary>
				///		Can only be read by the GPU and cannot be written to by any means.
				/// </summary>
				Immutable,

				/// <summary>
				///		Read only to the GPU and write only to the CPU.
				/// </summary>
				Dynamic,

				Count
			};

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

			/// <summary>
			///		Texture formats
			/// </summary>
			enum class TextureFormat
			{
				Invalid,

				R8UInt,
				R16UInt,

				RG8UInt,
				RG16UInt,
				
				RGBA8UInt,
				RGBA16UInt,

				R32Float,
				RG32Float,
				RGB32Float,
				RGBA32Float,

				Count,
			};

			/// <summary>
			///		Texture filter types
			/// </summary>
			enum class TextureFilter
			{
				Invalid = -1,

				Nearest,
				Linear,

				Count
			};

			/// <summary>
			///		Texture adress mode
			/// </summary>
			enum class TextureAdressMode
			{
				Invalid = -1,
				
				Wrap,
				Mirror,
				Clamp,
				Border,

				Count
			};

			/// <summary>
			///		Sampler settings
			/// </summary>
			struct SamplerSettings
			{
				TextureFilter minFilter = TextureFilter::Nearest;
				TextureFilter magFilter = TextureFilter::Nearest;
				TextureAdressMode addressU = TextureAdressMode::Border;
				TextureAdressMode addressV = TextureAdressMode::Border;
				glm::vec4 borderColor = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
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
				bool enableVsync = false;
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
			///		Vertex layout element data
			/// </summary>
			struct VertexLayoutElement
			{
				VertexElementFormat format;
				std::string name;
				unsigned int offset;
			};

			/// <summary>
			///		Vertex layout data
			/// </summary>
			struct VertexLayoutDesc
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
				///		Creates a vertex buffer.
				/// </summary>
				/// <param name="data">Data to fill buffer</param>
				/// <param name="size">Data size</param>
				/// <param name="layout">Data layout</param>
				/// <param name="program">Program handle</param>
				/// <param name="usage">Usage type</param>
				/// <returns>Vertex buffer handle</returns>
				virtual void* CreateVertexBuffer(void* data, size_t size, const VertexLayoutDesc& layout, void* program, Usage usage = Usage::Default) = 0;

				/// <summary>
				///		Destroys a vertex buffer
				/// </summary>
				/// <param name="vertexBuffer">Vertex buffer handle</param>
				virtual void DestroyVertexBuffer(void* vertexBuffer) = 0;

				/// <summary>
				///		Updates a vertex buffer data.
				/// </summary>
				/// <param name="vertexBuffer">Vertex buffer handle</param>
				/// <param name="data">Data pointer</param>
				/// <param name="size">Data size in bytes</param>
				/// <param name="offset">Data offset relative to the beginning of the vertex buffer</param>
				virtual void UpdateVertexBuffer(void* vertexBuffer, void* data, size_t size, size_t offset) = 0;

				/// <summary>
				///		Sets a vertex buffer as active.
				/// </summary>
				/// <param name="vertexBuffer">Vertex buffer handle to set as active</param>
				virtual void BindVertexBuffer(void* vertexBuffer) = 0;

				/// <summary>
				///		Draws the current active vertex buffer.
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

				/// <summary>
				///		Creates a new constant buffer
				/// </summary>
				/// <param name="data">Constant buffer data</param>
				/// <param name="size">Constant buffer size</param>
				/// <returns>Constant buffer handle</returns>
				virtual void* CreateConstantBuffer(void* data, size_t size) = 0;

				/// <summary>
				///		Destroys a constant buffer
				/// </summary>
				/// <param name="constantBuffer">Constant buffer handle</param>
				virtual void DestroyConstantBuffer(void* constantBuffer) = 0;

				/// <summary>
				///		Updates a constant buffer with new data
				/// </summary>
				/// <param name="constantBuffer">Constant buffer handle</param>
				/// <param name="data">New data pointer</param>
				/// <param name="offset">Offset to set the data</param>
				/// <param name="size">Data size</param>
				virtual void UpdateConstantBuffer(void* constantBuffer, void* data, size_t offset, size_t size) = 0;

				/// <summary>
				///		Binds a constant buffer to the the currently bound program
				/// </summary>
				/// <param name="constantBuffer">Constant buffer handle</param>
				/// <param name="bindingPoint">Constant buffer binding point handle</param>
				virtual void BindConstantBuffer(void* constantBuffer, void* bindingPoint) = 0;

				/// <summary>
				///		Gets a program constant buffer binding point
				/// </summary>
				/// <param name="program">Program handle</param>
				/// <param name="name">Binding point name</param>
				/// <returns>Binding point handle</returns>
				virtual void* GetConstantBindingPoint(void* program, const std::string& name) = 0;

				/// <summary>
				///		Creates a new 2D texture
				/// </summary>
				/// <param name="data">Texture pixel data (can be set to null to create an empty texture)</param>
				/// <param name="width">Texture width</param>
				/// <param name="height">Texture height</param>
				/// <param name="format">Texture data format</param>
				/// <returns>2D Texture handle</returns>
				virtual void* CreateTexture2D(const void* data, size_t width, size_t height, TextureFormat format) = 0;

				/// <summary>
				///		Destroys a 2D texture
				/// </summary>
				/// <param name="texture">2D Texture handle</param>
				virtual void DestroyTexture2D(void* texture) = 0;

				/// <summary>
				///		Updates a texture data
				/// </summary>
				/// <param name="texture">Texture handle</param>
				/// <param name="data">New data</param>
				/// <param name="width">Data width</param>
				/// <param name="height">Data height</param>
				/// <param name="dstX">Destination X coordinate</param>
				/// <param name="dstY">Destination Y coordinate</param>
				/// <param name="format">Texture format</param>
				virtual void UpdateTexture2D(void* texture, const void* data, size_t width, size_t height, size_t dstX, size_t dstY, TextureFormat format) = 0;

				/// <summary>
				///		Gets a program texture binding point.
				///		This texture sampler binding point will be got from name_sampler.
				/// </summary>
				/// <param name="program">Program handle</param>
				/// <param name="name">Binding point name</param>
				/// <returns>Binding point handle</returns>
				virtual void* GetTextureBindingPoint(void* program, const std::string& name) = 0;

				/// <summary>
				///		Binds a texture to a texture binding point
				/// </summary>
				/// <param name="texture">Texture handle</param>
				/// <param name="bindPoint">Binding point handle</param>
				virtual void BindTexture2D(void* texture, void* bindPoint) = 0;

				/// <summary>
				///		Creates a new sampler
				/// </summary>
				/// <param name="settings">Sampler settings</param>
				/// <returns>Sampler handle</returns>
				virtual void* CreateSampler(const SamplerSettings& settings) = 0;

				/// <summary>
				///		Destroys a sampler
				/// </summary>
				/// <param name="sampler">Sampler handle</param>
				virtual void DestroySampler(void* sampler) = 0;

				/// <summary>
				///		Binds a sampler to a texture binding point
				/// </summary>
				/// <param name="texture">Sampler handle</param>
				/// <param name="bindPoint">Texture binding point handle</param>
				virtual void BindSampler(void* sampler, void* bindPoint) = 0;

				/// <summary>
				///		Creates a new render texture that can be used as a render target
				/// </summary>
				/// <param name="width">Texture width</param>
				/// <param name="height">Texture height</param>
				/// <param name="format">Texture format</param>
				/// <param name="depthBuffer">Create a depth buffer for this render texture?</param>
				virtual void* CreateRenderTexture(size_t width, size_t height, TextureFormat format, bool depthBuffer = true) = 0;

				/// <summary>
				///		Destroys a render texture
				/// </summary>
				/// <param name="renderTexture">Render texture handle</param>
				virtual void DestroyRenderTexture(void* renderTexture) = 0;

				/// <summary>
				///		Sets the current render target
				/// </summary>
				/// <param name="renderTexture">Render texture handle (set to nullptr to return to the default render target)</param>
				virtual void SetRenderTarget(void* renderTexture) = 0;
			};
		}
	}
}