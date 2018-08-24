#pragma once

#include "../Memory/Object.hpp"
#include "../String/UTF8.hpp"
#include "Window.h"

namespace Magma
{
	namespace Framework
	{
		namespace Input
		{
			enum class WindowMode : mfiEnum
			{
				Windowed = MFI_WINDOWED,
				Fullscreen = MFI_FULLSCREEN,
			};

			/// <summary>
			///		Used as a window handle.
			///		Destroys the window automatically when there are no more references to it.
			/// </summary>
			class Window : public Memory::Object
			{
			public:
				using Object::Object;
				using Object::operator=;
				inline Window(const Memory::Object& object) : Memory::Object(object) {}

				/// <summary>
				///		Polls events from this window.
				/// </summary>
				void PollEvents();

				/// <summary>
				///		Waits for an event and handles it.
				/// </summary>
				void WaitForEvents();

				/// <summary>
				///		Gets the width of the window.
				/// </summary>
				/// <returns>Window width</returns>
				mfmU32 GetWidth();
				
				/// <summary>
				///		Gets the height of the window.
				/// </summary>
				/// <returns>Window height</returns>
				mfmU32 GetHeight();

				/// <summary>
				///		Gets the window mode
				/// </summary>
				/// <returns>Window mode</returns>
				WindowMode GetMode();
			};

			/// <summary>
			///		Creates a new window.
			/// </summary>
			/// <param name="type">Window type name</param>
			/// <param name="width">Window width</param>
			/// <param name="height">Window height</param>
			/// <param name="mode">Window mode</param>
			/// <param name="title">Window title</param>
			/// <returns>Window handle</returns>
			Window CreateWindow(const mfsUTF8CodeUnit* type, mfmU32 width, mfmU32 height, WindowMode mode, const mfsUTF8CodeUnit* title);
		}
	}
}
