#pragma once

#include "Keyboard.h"

namespace Magma
{
	namespace Framework
	{
		namespace Input
		{
			/// <summary>
			///		Keyboard key codes
			/// </summary>
			enum class Keyboard
			{
				Invalid = MFI_KEYBOARD_INVALID,

				Q = MFI_KEYBOARD_Q,
				W = MFI_KEYBOARD_W,
				E = MFI_KEYBOARD_E,
				R = MFI_KEYBOARD_R,
				T = MFI_KEYBOARD_T,
				Y = MFI_KEYBOARD_Y,
				U = MFI_KEYBOARD_U,
				I = MFI_KEYBOARD_I,
				O = MFI_KEYBOARD_O,
				P = MFI_KEYBOARD_P,
				A = MFI_KEYBOARD_A,
				S = MFI_KEYBOARD_S,
				D = MFI_KEYBOARD_D,
				F = MFI_KEYBOARD_F,
				G = MFI_KEYBOARD_G,
				H = MFI_KEYBOARD_H,
				J = MFI_KEYBOARD_J,
				K = MFI_KEYBOARD_K,
				L = MFI_KEYBOARD_L,
				Z = MFI_KEYBOARD_Z,
				X = MFI_KEYBOARD_X,
				C = MFI_KEYBOARD_C,
				V = MFI_KEYBOARD_V,
				B = MFI_KEYBOARD_B,
				N = MFI_KEYBOARD_N,
				M = MFI_KEYBOARD_M,

				Num1 = MFI_KEYBOARD_NUM1,
				Num2 = MFI_KEYBOARD_NUM2,
				Num3 = MFI_KEYBOARD_NUM3,
				Num4 = MFI_KEYBOARD_NUM4,
				Num5 = MFI_KEYBOARD_NUM5,
				Num6 = MFI_KEYBOARD_NUM6,
				Num7 = MFI_KEYBOARD_NUM7,
				Num8 = MFI_KEYBOARD_NUM8,
				Num9 = MFI_KEYBOARD_NUM9,
				Num0 = MFI_KEYBOARD_NUM0,

				F1 = MFI_KEYBOARD_F1,
				F2 = MFI_KEYBOARD_F2,
				F3 = MFI_KEYBOARD_F3,
				F4 = MFI_KEYBOARD_F4,
				F5 = MFI_KEYBOARD_F5,
				F6 = MFI_KEYBOARD_F6,
				F7 = MFI_KEYBOARD_F7,
				F8 = MFI_KEYBOARD_F8,
				F9 = MFI_KEYBOARD_F9,
				F10 = MFI_KEYBOARD_F10,
				F11 = MFI_KEYBOARD_F11,
				F12 = MFI_KEYBOARD_F12,

				Escape = MFI_KEYBOARD_ESCAPE,
				Tab = MFI_KEYBOARD_TAB,
				Caps = MFI_KEYBOARD_CAPS,
				LShift = MFI_KEYBOARD_LSHIFT,
				RShift = MFI_KEYBOARD_RSHIFT,
				LControl = MFI_KEYBOARD_LCONTROL,
				RControl = MFI_KEYBOARD_RCONTROL,
				Alt = MFI_KEYBOARD_ALT,
				AltGr = MFI_KEYBOARD_ALTGR,
				Space = MFI_KEYBOARD_SPACE,
				Enter = MFI_KEYBOARD_ENTER,
				Backspace = MFI_KEYBOARD_BACKSPACE,
				Insert = MFI_KEYBOARD_INSERT,
				Delete = MFI_KEYBOARD_DELETE,
				Home = MFI_KEYBOARD_HOME,
				End = MFI_KEYBOARD_END,
				PageUp = MFI_KEYBOARD_PAGEUP,
				PageDown = MFI_KEYBOARD_PAGEDOWN,

				Max
			};

			/// <summary>
			///		Key modifiers
			/// </summary>
			enum class KeyModifiers
			{
				None = 0x00,
				Shift = 0x01,
				Control = 0x02,
				Alt = 0x04,
				System = 0x08,
			};

			inline KeyModifiers operator|(KeyModifiers lhs, KeyModifiers rhs) { return (KeyModifiers)((size_t)lhs | (size_t)rhs); }
			inline KeyModifiers operator&(KeyModifiers lhs, KeyModifiers rhs) { return (KeyModifiers)((size_t)lhs & (size_t)rhs); }
			inline KeyModifiers& operator|=(KeyModifiers& lhs, KeyModifiers rhs) { return lhs = (lhs | rhs); }
			inline KeyModifiers& operator&=(KeyModifiers& lhs, KeyModifiers rhs) { return lhs = (lhs & rhs); }
		}
	}
}