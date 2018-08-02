#pragma once

namespace Magma
{
	namespace Framework
	{
		namespace Audio
		{
			/// <summary>
			///		Render device initialization settings
			/// </summary>
			struct RenderDeviceSettings
			{

			};

			/// <summary>
			///		Abstract class that encapsulates the low level API specific audio rendering calls
			/// </summary>
			class RenderDevice
			{
			public:
				virtual ~RenderDevice() = default;

				/// <summary>
				///		Inits the render device
				/// </summary>
				/// <param name="settings">Render device settings</param>
				virtual void Init(const RenderDeviceSettings& settings) = 0;

				/// <summary>
				///		Terminates the render device
				/// </summary>
				virtual void Terminate() = 0;
			};
		}
	}
}
