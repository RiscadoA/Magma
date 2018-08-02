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

				void * m_device;
			};
		}
	}
}