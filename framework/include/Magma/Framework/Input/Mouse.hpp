#pragma once

#include "Mouse.h"

namespace Magma
{
	namespace Framework
	{
		namespace Input
		{
			/// <summary>
			///		Mouse button codes
			/// </summary>
			enum class Mouse
			{
				Invalid = MFI_MOUSE_INVALID,

				Left = MFI_MOUSE_LEFT,
				Right = MFI_MOUSE_RIGHT,
				Middle = MFI_MOUSE_MIDDLE,

				Max = MFI_MOUSE_MAX,
			};
		}
	}
}