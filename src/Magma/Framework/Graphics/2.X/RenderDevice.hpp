#pragma once

#include "../../Memory/Object.hpp"
#include "../../Memory/Allocator.hpp"
#include "../../Input/Window.hpp"

#include "RenderDevice.h"

namespace Magma
{
	namespace Framework
	{
		namespace Graphics
		{
			namespace V2X
			{
				/// <summary>
				///		Used as a render device handle.
				///		Destroys the render device automatically when there are no more references to it.
				/// </summary>
				class RenderDevice : public Memory::Object
				{
				public:
					using Object::Object;
					using Object::operator=;
					inline RenderDevice(const Memory::Object& object) : Memory::Object(object) {}


				};

				/// <summary>
				///		Creates a new render device.
				/// </summary>
				/// <param name="type">Render device type name</param>
				/// <param name="window">Render device window</param>
				/// <param name="desc">Render device description</param>
				/// <param name="allocator">Render device allocator</param>
				/// <returns>Render device handle</returns>
				RenderDevice CreateRenderDevice(const mfsUTF8CodeUnit* type, Input::Window window, const mfgV2XRenderDeviceDesc* desc, Memory::Allocator allocator);
			}
		}
	}
}
