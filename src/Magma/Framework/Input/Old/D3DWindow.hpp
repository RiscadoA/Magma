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
			///		Window class implementation for DirectX (uses Windows API)
			/// </summary>
			class D3DWindow : public Window
			{
			public:
				/// <summary>
				///		Opens a window (OpenGL implementation).
				/// </summary>
				/// <param name="width">Window width</param>
				/// <param name="height">Window height</param>
				/// <param name="title">Window title</param>
				/// <param name="mode">Window mode</param>
				D3DWindow(mfmU32 width, mfmU32 height, const String::UTF8CodeUnit* title, Window::Mode mode = Window::Mode::Windowed);

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
				/// <returns>Window width</returns>
				inline virtual mfmU32 GetWidth() final { return m_window->getWidth(m_window); }

				/// <summary>
				///		Gets the height of the window.
				/// </summary>
				/// <returns>Window height</returns>
				inline virtual mfmU32 GetHeight() final { return m_window->getHeight(m_window); }

				/// <summary>
				///		Gets the window mode
				/// </summary>
				/// <returns>Window mode</returns>
				inline virtual Window::Mode GetMode() final { return (Window::Mode)m_window->getMode(m_window); }

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