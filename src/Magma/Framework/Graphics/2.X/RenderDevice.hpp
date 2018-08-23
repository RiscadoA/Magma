#pragma once

#include "../../Memory/Object.hpp"
#include "RenderDevice.h"

namespace Magma
{
	namespace Framework
	{
		namespace String
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
		}
	}
}
