#pragma once

#include "RenderDevice.hpp"
#include "../Input/D3DWindow.hpp"

namespace Magma
{
	namespace Framework
	{
		namespace Graphics
		{
			/// <summary>
			///		Direct3D 11 render device implementation
			/// </summary>
			class D3D11RenderDevice final : public RenderDevice
			{
			public:
				// Inherited via RenderDevice
				virtual void Init(Input::Window * window, const RenderDeviceSettings & settings) override;
				virtual void Terminate() override;
				virtual VertexShader * CreateVertexShader(const ShaderData & data) override;
				virtual void DestroyVertexShader(VertexShader * vertexShader) override;
				virtual PixelShader * CreatePixelShader(const ShaderData & data) override;
				virtual void DestroyPixelShader(PixelShader * pixelShader) override;
				virtual Pipeline * CreatePipeline(VertexShader * vertexShader, PixelShader * pixelShader) override;
				virtual void DestroyPipeline(Pipeline * pipeline) override;
				virtual void SetPipeline(Pipeline * pipeline) override;
				virtual VertexBuffer * CreateVertexBuffer(size_t size, const void * data = nullptr, BufferUsage usage = BufferUsage::Default) override;
				virtual void DestroyVertexBuffer(VertexBuffer * vertexBuffer) override;
				virtual VertexLayout * CreateVertexLayout(size_t elementCount, const VertexElement * elements, VertexShader * vertexShader) override;
				virtual void DestroyVertexLayout(VertexLayout * vertexLayout) override;
				virtual VertexArray * CreateVertexArray(size_t bufferCount, VertexBuffer ** buffers, VertexLayout * layout) override;
				virtual void DestroyVertexArray(VertexArray * vertexArray) override;
				virtual void SetVertexArray(VertexArray * vertexArray) override;
				virtual IndexBuffer * CreateIndexBuffer(IndexType type, size_t size, const void * data = nullptr, BufferUsage usage = BufferUsage::Default) override;
				virtual void DestroyIndexBuffer(IndexBuffer * indexBuffer) override;
				virtual void SetIndexBuffer(IndexBuffer * indexBuffer) override;
				virtual Texture2D * CreateTexture2D(size_t width, size_t height, TextureFormat format, const void * data = nullptr, BufferUsage usage = BufferUsage::Default, bool isRenderTarget = false) override;
				virtual void DestroyTexture2D(Texture2D * texture) override;
				virtual Sampler2D * CreateSampler2D(const Sampler2DDesc & desc) override;
				virtual void DestroySampler2D(Sampler2D * sampler) override;
				virtual RasterState * CreateRasterState(const RasterStateDesc & desc) override;
				virtual void DestroyRasterState(RasterState * rasterState) override;
				virtual void SetRasterState(RasterState * rasterState) override;
				virtual DepthStencilState * CreateDepthStencilState(const DepthStencilStateDesc & desc) override;
				virtual void DestroyDepthStencilState(DepthStencilState * depthStencilState) override;
				virtual void SetDepthStencilState(DepthStencilState * depthStencilState) override;
				virtual void Clear(glm::vec4 color, float depth = 1.0f, int stencil = 0) override;
				virtual void DrawTriangles(size_t offset, size_t count) override;
				virtual void DrawTrianglesIndexed(size_t offset, size_t count) override;
				virtual void SwapBuffers() override;
				virtual ConstantBuffer * CreateConstantBuffer(size_t size, const void * data = nullptr, BufferUsage usage = BufferUsage::Dynamic) override;
				virtual void DestroyConstantBuffer(ConstantBuffer * constantBuffer) override;
				virtual BlendState * CreateBlendState(const BlendStateDesc & desc) override;
				virtual void DestroyBlendState(BlendState * blendState) override;
				virtual void SetBlendState(BlendState * blendState) override;
				virtual DepthStencilBuffer* CreateDepthStencilBuffer(size_t width, size_t height, DepthStencilFormat format) override;
				virtual void DestroyDepthStencilBuffer(DepthStencilBuffer* depthStencilBuffer) override;
				virtual Framebuffer* CreateFramebuffer(size_t attachmentCount, Texture2D** attachments, DepthStencilBuffer* depthStencilAttachment = nullptr) override;
				virtual void DestroyFramebuffer(Framebuffer* framebuffer) override;
				virtual void SetFramebuffer(Framebuffer* framebuffer) override;

				Input::D3DWindow* m_window;

				Graphics::IndexBuffer* m_currentIndexBuffer;

				Graphics::RasterState* m_defaultRasterState;
				Graphics::RasterState* m_currentRasterState;
				Graphics::DepthStencilState* m_defaultDepthStencilState;
				Graphics::DepthStencilState* m_currentDepthStencilState;
				Graphics::BlendState* m_defaultBlendState;
				Graphics::BlendState* m_currentBlendState;

				RenderDeviceSettings m_settings;

				void* m_swapChain;
				void* m_device;
				void* m_deviceContext;
				void* m_defaultRenderTargetView;
				void* m_renderTargetViews;
				size_t m_renderTargetCount = 1;
				void* m_depthStencilBuffer;
				void* m_defaultDepthStencilView;
				void* m_depthStencilView;
				void* m_rasterState;
			};
		}
	}
}