#pragma once

#include "Window.hpp"
#include "../D3DWindow.h"
#include "../../String/UTF8.hpp"

namespace Magma
{
	namespace Framework
	{
		namespace Input
		{
			/// <summary>
			///		WindowHandle class implementation for DirectX (uses Windows API)
			/// </summary>
			class D3DWindow : public WindowHandle
			{
			public:
				/// <summary>
				///		Opens a window (OpenGL implementation).
				/// </summary>
				/// <param name="width">WindowHandle width</param>
				/// <param name="height">WindowHandle height</param>
				/// <param name="title">WindowHandle title</param>
				/// <param name="mode">WindowHandle mode</param>
				D3DWindow(mfmU32 width, mfmU32 height, const String::UTF8CodeUnit* title, WindowHandle::Mode mode = WindowHandle::Mode::Windowed);

				/// <summary>
				///		Closes a window.
				/// </summary>
				~D3DWindow();

				/// <summary>
				///		Polls events from this window.
				/// </summary>
				virtual void PollEvents() final;

				/// <summary>
				///		Waits for an event and handles it.
				/// </summary>
				virtual void WaitForEvents() final;

				/// <summary>
				///		Gets the width of the window.
				/// </summary>
				/// <returns>WindowHandle width</returns>
				inline virtual mfmU32 GetWidth() final { return m_window->getWidth(m_window); }

				/// <summary>
				///		Gets the height of the window.
				/// </summary>
				/// <returns>WindowHandle height</returns>
				inline virtual mfmU32 GetHeight() final { return m_window->getHeight(m_window); }

				/// <summary>
				///		Gets the window mode
				/// </summary>
				/// <returns>WindowHandle mode</returns>
				inline virtual WindowHandle::Mode GetMode() final { return (WindowHandle::Mode)m_window->getMode(m_window); }

				/// <summary>
				///		Returns a pointer to the underlying C window
				/// </summary>
				/// <returns>C window pinter</returns>
				inline mfiWindow* GetWindow() const { return m_window; }

			private:
				mfiWindow * m_window;
			};
		}
	}
}