#pragma once

#include "Context.hpp"
#include "../../Input/D3DWindow.hpp"

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
				virtual void * CreateVertexBuffer(void * data, size_t size, const VertexLayoutDesc & layout, void* program, Usage usage) final;
				virtual void UpdateVertexBuffer(void* vertexBuffer, void* data, size_t size, size_t offset) final;
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
				virtual void * CreateConstantBuffer(void * data, size_t size) final;
				virtual void DestroyConstantBuffer(void * constantBuffer) final;
				virtual void UpdateConstantBuffer(void * constantBuffer, void * data, size_t offset, size_t size) final;
				virtual void BindConstantBuffer(void * constantBuffer, void * bindingPoint) final;
				virtual void * GetConstantBindingPoint(void * program, const std::string & name) final;
				virtual void * CreateTexture2D(const void * data, size_t width, size_t height, TextureFormat format) final;
				virtual void DestroyTexture2D(void * texture) final;
				virtual void UpdateTexture2D(void* texture, const void* data, size_t width, size_t height, size_t dstX, size_t dstY, TextureFormat format) final;
				virtual void * GetTextureBindingPoint(void * program, const std::string & name) final;
				virtual void BindTexture2D(void * texture, void * bindPoint) final;
				virtual void * CreateSampler(const SamplerSettings & settings) final;
				virtual void DestroySampler(void * sampler) final;
				virtual void BindSampler(void * sampler, void * bindPoint) final;
				virtual void * CreateRenderTexture(size_t width, size_t height, TextureFormat format, bool depthBuffer = true) final;
				virtual void DestroyRenderTexture(void * renderTexture) final;
				virtual void SetRenderTarget(void * renderTexture) final;

			private:
				Input::D3DWindow* m_window;
				ContextSettings m_settings;

				void* m_swapChain;
				void* m_device;
				void* m_deviceContext;
				void* m_defaultRenderTargetView;
				void* m_renderTargetView;
				void* m_depthStencilBuffer;
				void* m_depthStencilState;
				void* m_defaultDepthStencilView;
				void* m_depthStencilView;
				void* m_rasterState;

				void* m_compiler;

				glm::vec4 m_clearColor;

				void* m_boundProgram;
				void* m_boundVertexBuffer;
				void* m_boundIndexBuffer;
			};
		}
	}
}