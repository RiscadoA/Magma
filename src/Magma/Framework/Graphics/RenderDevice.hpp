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
			class Texture2D;
			class ConstantBuffer;

			/// <summary>
			///		Encapsulates a vertex shader
			/// </summary>
			class VertexShader
			{
			public:
				virtual ~VertexShader() = default;

			protected:
				/// <summary>
				///		Used to ensure that these are never created directly
				/// </summary>
				VertexShader() = default;
			};

			/// <summary>
			///		Encapsulates a vertex shader
			/// </summary>
			class PixelShader
			{
			public:
				virtual ~PixelShader() = default;

			protected:
				/// <summary>
				///		Used to ensure that these are never created directly
				/// </summary>
				PixelShader() = default;
			};

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
			///		Encapsulates a vertex buffer
			/// </summary>
			class VertexBuffer
			{
			public:
				virtual ~VertexBuffer() = default;

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
				///		Signed byte (8 bits) that gets casted into the nearest floating point value
				/// </summary>
				FByte,

				/// <summary>
				///		Signed short (16 bits) that gets casted into the nearest floating point value
				/// </summary>
				FShort,

				/// <summary>
				///		Signed int (32 bits) that gets casted into the nearest floating point value
				/// </summary>
				FInt,

				/// <summary>
				///		Unsigned byte (8 bits) that gets casted into the nearest floating point value
				/// </summary>
				FUByte,

				/// <summary>
				///		Unsigned short (16 bits) that gets casted into the nearest floating point value
				/// </summary>
				FUShort,

				/// <summary>
				///		Unsigned int (32 bits) that gets casted into the nearest floating point value
				/// </summary>
				FUInt,

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

				bool frontFaceStencilEnabled = false;
				Compare frontFaceStencilCompare = Compare::Always;
				StencilAction frontFaceStencilFail = StencilAction::Keep;
				StencilAction frontFaceStencilPass = StencilAction::Keep;
				StencilAction frontFaceDepthFail = StencilAction::Keep;

				int frontFaceRef = 0;
				unsigned int frontFaceReadMask = 0xFFFFFFFF;
				unsigned int frontFaceWriteMask = 0xFFFFFFFF;

				bool backFaceStencilEnabled = false;
				Compare backFaceStencilCompare = Compare::Always;
				StencilAction backFaceStencilFail = StencilAction::Keep;
				StencilAction backFaceStencilPass = StencilAction::Keep;
				StencilAction backFaceDepthFail = StencilAction::Keep;

				int backFaceRef = 0;
				unsigned int backFaceReadMask = 0xFFFFFFFF;
				unsigned int backFaceWriteMask = 0xFFFFFFFF;
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
				/// <param name="data">Texture initial data (set to nullptr to create empty texture)</param>
				/// <param name="usage">Texture usage mode</param>
				/// <returns>2D texture handle</returns>
				virtual Texture2D* CreateTexture2D(size_t width, size_t height, const void* data = nullptr, BufferUsage usage = BufferUsage::Default) = 0;
				
				/// <summary>
				///		Destroys a 2D texture
				/// </summary>
				/// <param name="texture">2D texture handle</param>
				virtual void DestroyTexture2D(Texture2D* texture) = 0;

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
				///		Sets the current render targets
				/// </summary>
				/// <param name="textures">Textures array (set textures to nullptr to set the default render target)</param>
				/// <param name="count">Render target count</param>
				virtual void SetRenderTargets(Texture2D** textures, size_t count) = 0;

				/// <summary>
				///		Swaps the front and back buffers
				/// </summary>
				virtual void SwapBuffers() = 0;
			};
		}
	}
}