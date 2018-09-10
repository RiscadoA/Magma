#pragma once

#include "../Memory/Handle.hpp"
#include "../Memory/Allocator.hpp"
#include "Mutex.h"

namespace Magma
{
	namespace Framework
	{
		namespace Thread
		{
			/// <summary>
			///		Thread handle.
			/// </summary>
			class HThread : public Memory::Handle
			{
			public:
				using Handle::Handle;
				using Handle::operator=;
				inline HThread(const Memory::Handle& object) : Memory::Handle(object) {}

				/// <summary>
				///		Waits for the thread to finish.
				/// </summary>
				/// <param name="timeOut">Wait timeout in milliseconds (set to 0 to be infinite)</param>
				/// <returns>True if thread finished, otherwise false (timeout expired)</returns>
				bool Wait(mfmU64 timeOut = 0);
			};

			/// <summary>
			///		Creates a thread.
			/// </summary>
			/// <param name="allocator">Allocator used to allocate the thread object</param>
			/// <returns>Thread handle</returns>
			HThread CreateThread(Memory::HAllocator allocator = Memory::StandardAllocator);
		}
	}
}
