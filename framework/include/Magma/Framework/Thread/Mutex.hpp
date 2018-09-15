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
			///		Mutex handle.
			/// </summary>
			class HMutex : public Memory::Handle
			{
			public:
				using Handle::Handle;
				using Handle::operator=;
				inline HMutex(const Memory::Handle& object) : Memory::Handle(object) {}

				/// <summary>
				///		Locks the mutex (if already locked wait until it is unlocked).
				/// </summary>
				/// <param name="timeOut">Wait timeout in milliseconds (set to 0 to be infinite)</param>
				/// <returns>True if locked the mutex, otherwise false</returns>
				bool Lock(mfmU64 timeOut = 0);

				/// <summary>
				///		Unlocks the mutex.
				/// </summary>
				void Unlock();

				/// <summary>
				///		Tries to lock the mutex. If it was already locked, returns false, otherwise returns true.
				/// </summary>
				/// <returns>True if locked the mutex, otherwise false</returns>
				bool TryLock();
			};

			/// <summary>
			///		Creates a mutex.
			/// </summary>
			/// <param name="allocator">Allocator used to allocate the mutex object</param>
			/// <returns>Mutex handle</returns>
			HMutex CreateMutex(Memory::HAllocator allocator = Memory::StandardAllocator);
		}
	}
}
