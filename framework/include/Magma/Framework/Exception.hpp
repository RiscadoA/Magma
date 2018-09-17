#pragma once

#include <Magma/Framework/String/UTF8.h>

#include <stdexcept>

namespace Magma
{
	namespace Framework
	{
		class Exception
		{
		public:
			static constexpr mfmU64 MaxSize = 65536;

			Exception(const mfsUTF8CodeUnit* msg) noexcept;
			Exception(Exception&& e) noexcept;
			virtual ~Exception() noexcept;

			virtual const mfsUTF8CodeUnit* GetMessage() noexcept;

		private:
			Exception(const Exception& e) = delete;

			mfsUTF8CodeUnit * m_msg;
		};
	}
}