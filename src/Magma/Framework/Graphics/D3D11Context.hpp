#pragma once

#include "Context.hpp"
#include "../Input/D3DWindow.hpp"

namespace Magma
{
	namespace Framework
	{
		namespace Graphics
		{
			/// <summary>
			///		Direct3D 11 implementation of the rendering context
			/// </summary>
			class D3D11Context final : public Context
			{
			public:
				D3D11Context();

				// Inherited via Context
				virtual void Init(Input::Window * window, const ContextSettings& settings) final;
				virtual void Terminate() final;
				virtual void SetClearColor(glm::vec4 color) final;
				virtual void Clear(BufferBit mask) final;
				virtual void SwapBuffers() final;
				virtual void * CreateVertexBuffer(void * data, size_t size, const VertexLayout & layout, void* vertexShader) final;
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
				Input::D3DWindow* m_window;
				
				void* m_swapChain;
				void* m_device;
				void* m_deviceContext;
				void* m_renderTargetView;
				void* m_depthStencilBuffer;
				void* m_depthStencilState;
				void* m_depthStencilView;
				void* m_rasterState;

				glm::vec4 m_clearColor;

				void* m_boundProgram;
				void* m_boundVertexBuffer;
				void* m_boundIndexBuffer;
			};
		}
	}
}