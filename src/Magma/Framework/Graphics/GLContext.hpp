#pragma once

#include "Context.hpp"
#include "../Input/GLWindow.hpp"

namespace Magma
{
	namespace Framework
	{
		namespace Graphics
		{
			/// <summary>
			///		OpenGL implementation of the rendering context
			/// </summary>
			class GLContext final : public Context
			{
			public:
				// Inherited via Context
				virtual void Init(Input::Window * window, const ContextSettings& settings) final;
				virtual void Terminate() final;
				virtual void SwapBuffers() final;
				virtual void SetClearColor(glm::vec4 color) final;
				virtual void Clear(BufferBit mask) final;
				virtual void * CreateVertexBuffer(void * data, size_t size, const VertexLayout & layout, void * vertexShader) final;
				virtual void DestroyVertexBuffer(void * vertexBuffer) final;
				virtual void BindVertexBuffer(void * vertexBuffer) final;
				virtual void Draw(size_t vertexCount, size_t offset, DrawMode mode) final;
				virtual void * CreateShader(ShaderType type, const std::string & src) final;
				virtual void DestroyShader(void * shader) final;
				virtual void * CreateProgram() final;
				virtual void DestroyProgram(void * program) final;
				virtual void AttachShader(void * program, void * shader) final;
				virtual void DetachShader(void * program, void * shader) final;
				virtual void LinkProgram(void * program) final;
				virtual void BindProgram(void * program) final;
				virtual void * CreateIndexBuffer(void * data, size_t size, IndexFormat type) final;
				virtual void DestroyIndexBuffer(void * indexBuffer) final;
				virtual void BindIndexBuffer(void * indexBuffer) final;
				virtual void DrawIndexed(size_t indexCount, size_t offset, DrawMode mode) final;

			private:
				Input::GLWindow* m_window;
			};
		}
	}
}