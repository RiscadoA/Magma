#pragma once

#include "../Memory/Handle.hpp"
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
			class WindowHandle : public Memory::Handle
			{
			public:
				using Handle::Handle;
				using Handle::operator=;
				inline WindowHandle(const Memory::Handle& object) : Memory::Handle(object) {}

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
				/// <returns>WindowHandle width</returns>
				mfmU32 GetWidth();
				
				/// <summary>
				///		Gets the height of the window.
				/// </summary>
				/// <returns>WindowHandle height</returns>
				mfmU32 GetHeight();

				/// <summary>
				///		Gets the window mode
				/// </summary>
				/// <returns>WindowHandle mode</returns>
				WindowMode GetMode();

				/// <summary>
				///		Sets the callback that is called when the window is closed.
				///		Parameter 1: WindowHandle handle;
				/// </summary>
				/// <param name="callback">New callback (can be set to NULL)</param>
				void SetOnCloseCallback(mfiOnCloseCallback callback);

				/// <summary>
				///		Sets the callback that is called when mouse enters the window.
				///		Parameter 1: WindowHandle handle;
				/// </summary>
				/// <param name="callback">New callback (can be set to NULL)</param>
				void SetOnMouseEnterCallback(mfiOnMouseEnterCallback callback);

				/// <summary>
				///		Sets the callback that is called when the mouse leaves the window.
				///		Parameter 1: WindowHandle handle;
				/// </summary>
				/// <param name="callback">New callback (can be set to NULL)</param>
				void SetOnMouseLeaveCallback(mfiOnMouseLeaveCallback callback);

				/// <summary>
				///		Sets the callback that is called when the mouse moves.
				///		Parameter 1: WindowHandle handle;
				///		Parameter 2: Mouse new X coordinate;
				///		Parameter 3: Mouse new Y coordinate;
				/// </summary>
				/// <param name="callback">New callback (can be set to NULL)</param>
				void SetOnMouseMoveCallback(mfiOnMouseMoveCallback callback);

				/// <summary>
				///		Sets the callback that is called when the mouse wheel is scrolled.
				///		Parameter 1: WindowHandle handle;
				///		Parameter 2: Mouse wheel scroll delta;
				/// </summary>
				/// <param name="callback">New callback (can be set to NULL)</param>
				void SetOnMouseScrollCallback(mfiOnMouseScrollCallback callback);
				
				/// <summary>
				///		Sets the callback that is called when a mouse button goes up.
				///		Parameter 1: WindowHandle handle;
				///		Parameter 2: Mouse button code;
				/// </summary>
				/// <param name="callback">New callback (can be set to NULL)</param>
				void SetOnMouseUpCallback(mfiOnMouseUpCallback callback);

				/// <summary>
				///		Sets the callback that is called when a mouse button goes down.
				///		Parameter 1: WindowHandle handle;
				///		Parameter 2: Mouse button code;
				/// </summary>
				/// <param name="callback">New callback (can be set to NULL)</param>
				void SetOnMouseDownCallback(mfiOnMouseDownCallback callback);

				/// <summary>
				///		Sets the callback that is called when a key goes up.
				///		Parameter 1: WindowHandle handle;
				///		Parameter 2: Key code;
				/// </summary>
				/// <param name="callback">New callback (can be set to NULL)</param>
				void SetOnKeyUpCallback(mfiOnKeyUpCallback callback);

				/// <summary>
				///		Sets the callback that is called when a key goes down.
				///		Parameter 1: WindowHandle handle;
				///		Parameter 2: Key code;
				/// </summary>
				/// <param name="callback">New callback (can be set to NULL)</param>
				void SetOnKeyDownCallback(mfiOnKeyDownCallback callback);
			};
		

			/// <summary>
			///		Creates a new window.
			/// </summary>
			/// <param name="type">WindowHandle type name</param>
			/// <param name="width">WindowHandle width</param>
			/// <param name="height">WindowHandle height</param>
			/// <param name="mode">WindowHandle mode</param>
			/// <param name="title">WindowHandle title</param>
			/// <returns>WindowHandle handle</returns>
			WindowHandle CreateWindow(const mfsUTF8CodeUnit* type, mfmU32 width, mfmU32 height, WindowMode mode, const mfsUTF8CodeUnit* title);
		}
	}
}
