#include <Magma/Framework/Exception.hpp>
#include <Magma/Framework/Memory/Allocator.hpp>

Magma::Framework::Exception::Exception(const mfsUTF8CodeUnit * msg) noexcept
{
	if (msg == nullptr)
		m_msg = nullptr;
	else
	{
		m_msg = (mfsUTF8CodeUnit*)Memory::StandardAllocator.Allocate(MaxSize);
		for (mfmU64 i = 0; i < MaxSize; ++i)
		{
			m_msg[i] = msg[i];
			if (msg[i] == '\0')
				break;
		}
		m_msg[MaxSize] = '\0';
	}
}

Magma::Framework::Exception::Exception(Exception && e) noexcept
{
	m_msg = e.m_msg;
	e.m_msg = nullptr;
}

Magma::Framework::Exception::~Exception() noexcept
{
	if (m_msg != nullptr)
		Memory::StandardAllocator.Deallocate(m_msg);
}

const mfsUTF8CodeUnit * Magma::Framework::Exception::GetMessage() noexcept
{
	return m_msg;
}
