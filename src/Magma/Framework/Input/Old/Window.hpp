#pragma once

#include <string>

#include "../Event.hpp"
#include "../Keyboard.hpp"
#include "../Mouse.hpp"

#include "../Window.h"

namespace Magma
{
	namespace Framework
	{
		namespace Input
		{
			/// <summary>
			///		Abstract class that handles window creations and events.
			/// </summary>
			class HWindow
			{
			public:
				/// <summary>
				///		WindowHandle modes.
				/// </summary>
				enum class Mode
				{
					Windowed = MFI_WINDOWED,
					Fullscreen = MFI_FULLSCREEN,
				};
				
				/// <summary>
				///		Opens a window.
				/// </summary>
				HWindow() = default;

				/// <summary>
				///		Closes a window.
				/// </summary>
				virtual ~HWindow() = default;

				/// <summary>
				///		Polls events from this window.
				/// </summary>
				virtual void PollEvents() = 0;

				/// <summary>
				///		Waits for an event and handles it.
				/// </summary>
				virtual void WaitForEvents() = 0;

				/// <summary>
				///		Fired when the user tries to close the window.
				/// </summary>
				Event<> OnClose;

				/// <summary>
				///		Fired when the mouse enters the window.
				/// </summary>
				Event<> OnMouseEnter;

				/// <summary>
				///		Fired when the mouse leaves the window.
				/// </summary>
				Event<> OnMouseLeave;

				/// <summary>
				///		Fired when the mouse moves.
				///		Params: { mouse position X; mouse position Y; }
				/// </summary>
				Event<float, float> OnMouseMove;

				/// <summary>
				///		Fired when the mouse wheel is scrolled.
				///		Params: { mouse scroll delta; }
				/// </summary>
				Event<float> OnMouseScroll;

				/// <summary>
				///		Fired when a key goes up.
				///		Params: { key code; key modifiers; }
				/// </summary>
				Event<Keyboard, KeyModifiers> OnKeyUp;

				/// <summary>
				///		Fired when a key goes down.
				///		Params: { key code; key modifiers; }
				/// </summary>
				Event<Keyboard, KeyModifiers> OnKeyDown;

				/// <summary>
				///		Fired when a mouse button goes up.
				///		Params: { mouse button; }
				/// </summary>
				Event<Mouse> OnMouseUp;

				/// <summary>
				///		Fired when a mouse button goes down.
				///		Params: { mouse button; }
				/// </summary>
				Event<Mouse> OnMouseDown;

				/// <summary>
				///		Gets the width of the window.
				/// </summary>
				/// <returns>WindowHandle width</returns>
				virtual mfmU32 GetWidth() = 0;

				/// <summary>
				///		Gets the height of the window.
				/// </summary>
				/// <returns>WindowHandle height</returns>
				virtual mfmU32 GetHeight() = 0;

				/// <summary>
				///		Gets the window mode
				/// </summary>
				/// <returns>WindowHandle mode</returns>
				virtual HWindow::Mode GetMode() = 0;
			};
		}
	}
}
