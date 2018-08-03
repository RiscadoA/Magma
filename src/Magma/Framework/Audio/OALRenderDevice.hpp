#pragma once

#include "RenderDevice.hpp"

namespace Magma
{
	namespace Framework
	{
		namespace Audio
		{
			/// <summary>
			///		OpenAL Soft audio render device implementation
			/// </summary>
			class OALRenderDevice final : public RenderDevice
			{
			public:
				virtual void Init(const RenderDeviceSettings& settings) override;
				virtual void Terminate() override;
				virtual Source* CreateSource() override;
				virtual void DestroySource(Source* source)override;
				virtual Buffer* CreateBuffer() override;
				virtual Buffer* CreateBuffer(void* data, size_t size, Format format, size_t frequency) override;
				virtual void DestroyBuffer(Buffer* buffer) override;
				virtual void SetListenerGain(float gain) override;
				virtual void SetListenerPosition(float x, float y, float z) override;
				virtual void SetListenerVelocity(float x, float y, float z) override;
				virtual void SetListenerOrientation(float atX, float atY, float atZ, float upX, float upY, float upZ) override;

				void * m_device;
				void * m_context;
			};
		}
	}
}