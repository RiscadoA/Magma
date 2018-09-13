#pragma once

#include "../Memory/Handle.hpp"
#include "../Memory/Allocator.hpp"
#include "../Memory/Functor.hpp"
#include "Atomic.h"

namespace Magma
{
	namespace Framework
	{
		namespace Thread
		{
			template <typename T>
			class Atomic
			{
			public:
				inline Atomic() { static_assert(false); /* Unsupported atomic type */ }			
			};

			/// <summary>
			///		Encapsulates an atomic pointer.
			/// </summary>
			template <typename T>
			class Atomic<T*> final
			{
			public:
				Atomic(T* value = nullptr);
				~Atomic();

				/// <summary>
				///		Gets the current value (thread-safe).
				/// </summary>
				/// <returns>Current value</returns>
				T* Load() const;

				/// <summary>
				///		Sets the current value (thread-safe).
				/// </summary>
				/// <param name="value">New value</param>
				void Store(T* value);

			private:
				volatile void* m_value;
			};

			/// <summary>
			///		Encapsulates an atomic 8bit signed integer.
			/// </summary>
			template <>
			class Atomic<mfmI8> final
			{
			public:
				Atomic(mfmI8 value = 0);
				~Atomic();

				/// <summary>
				///		Gets the current value (thread-safe).
				/// </summary>
				/// <returns>Current value</returns>
				mfmI8 Load() const;

				/// <summary>
				///		Sets the current value (thread-safe).
				/// </summary>
				/// <param name="value">New value</param>
				void Store(mfmI8 value);

			private:
				volatile mfmI8 m_value;
			};

			/// <summary>
			///		Encapsulates an atomic 16bit signed integer.
			/// </summary>
			template <>
			class Atomic<mfmI16> final
			{
			public:
				Atomic(mfmI16 value = 0);
				~Atomic();

				/// <summary>
				///		Gets the current value (thread-safe).
				/// </summary>
				/// <returns>Current value</returns>
				mfmI16 Load() const;

				/// <summary>
				///		Sets the current value (thread-safe).
				/// </summary>
				/// <param name="value">New value</param>
				void Store(mfmI16 value);

			private:
				volatile mfmI16 m_value;
			};

			/// <summary>
			///		Encapsulates an atomic 32bit signed integer.
			/// </summary>
			template <>
			class Atomic<mfmI32> final
			{
			public:
				Atomic(mfmI32 value = 0);
				~Atomic();

				/// <summary>
				///		Gets the current value (thread-safe).
				/// </summary>
				/// <returns>Current value</returns>
				mfmI32 Load() const;

				/// <summary>
				///		Sets the current value (thread-safe).
				/// </summary>
				/// <param name="value">New value</param>
				void Store(mfmI32 value);

				/// <summary>
				///		Adds a value to the current value (thread-safe).
				/// </summary>
				/// <param name="value">Value to add</param>
				void Add(mfmI32 value);

				/// <summary>
				///		Subtracts a value from the current value (thread-safe).
				/// </summary>
				/// <param name="value">Value to subtract</param>
				void Subtract(mfmI32 value);

			private:
				volatile mfmI32 m_value;
			};
		}
	}
}
