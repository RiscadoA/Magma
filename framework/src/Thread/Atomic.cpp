#pragma once

#include <Magma/Framework/Thread/Atomic.hpp>
#include <Magma/Framework/Thread/Exception.hpp>
#include <Magma/Framework/ErrorString.h>

template<typename T>
Magma::Framework::Thread::Atomic<T*>::Atomic(T * value)
{
	m_value = value;
}

template<typename T>
Magma::Framework::Thread::Atomic<T*>::~Atomic()
{
	// Do nothing
}

template<typename T>
T* Magma::Framework::Thread::Atomic<T*>::Load() const
{
	void* value;
	mfError err = mftAtomicPointerLoad(&m_value, &value);
	if (err != MF_ERROR_OKAY)
		throw AtomicError(mfErrorToString(err));
	return (T*)value;
}

template<typename T>
void Magma::Framework::Thread::Atomic<T*>::Store(T * value)
{
	mfError err = mftAtomicPointerStore(&m_value, value);
	if (err != MF_ERROR_OKAY)
		throw AtomicError(mfErrorToString(err));
}

Magma::Framework::Thread::Atomic<mfmI8>::Atomic(mfmI8 value)
{
	m_value = value;
}

Magma::Framework::Thread::Atomic<mfmI8>::~Atomic()
{
	// Do nothing
}

mfmI8 Magma::Framework::Thread::Atomic<mfmI8>::Load() const
{
	mfmI8 value;
	mfError err = mftAtomic8Load(&m_value, &value);
	if (err != MF_ERROR_OKAY)
		throw AtomicError(mfErrorToString(err));
	return value;
}

void Magma::Framework::Thread::Atomic<mfmI8>::Store(mfmI8 value)
{
	mfError err = mftAtomic8Store(&m_value, value);
	if (err != MF_ERROR_OKAY)
		throw AtomicError(mfErrorToString(err));
}

Magma::Framework::Thread::Atomic<mfmI16>::Atomic(mfmI16 value)
{
	m_value = value;
}

Magma::Framework::Thread::Atomic<mfmI16>::~Atomic()
{
	// Do nothing
}

mfmI16 Magma::Framework::Thread::Atomic<mfmI16>::Load() const
{
	mfmI16 value;
	mfError err = mftAtomic16Load(&m_value, &value);
	if (err != MF_ERROR_OKAY)
		throw AtomicError(mfErrorToString(err));
	return value;
}

void Magma::Framework::Thread::Atomic<mfmI16>::Store(mfmI16 value)
{
	mfError err = mftAtomic16Store(&m_value, value);
	if (err != MF_ERROR_OKAY)
		throw AtomicError(mfErrorToString(err));
}

Magma::Framework::Thread::Atomic<mfmI32>::Atomic(mfmI32 value)
{
	m_value = value;
}

Magma::Framework::Thread::Atomic<mfmI32>::~Atomic()
{
	// Do nothing
}

mfmI32 Magma::Framework::Thread::Atomic<mfmI32>::Load() const
{
	mfmI32 value;
	mfError err = mftAtomic32Load(&m_value, &value);
	if (err != MF_ERROR_OKAY)
		throw AtomicError(mfErrorToString(err));
	return value;
}

void Magma::Framework::Thread::Atomic<mfmI32>::Store(mfmI32 value)
{
	mfError err = mftAtomic32Store(&m_value, value);
	if (err != MF_ERROR_OKAY)
		throw AtomicError(mfErrorToString(err));
}

void Magma::Framework::Thread::Atomic<mfmI32>::Add(mfmI32 value)
{
	mfError err = mftAtomic32Add(&m_value, value);
	if (err != MF_ERROR_OKAY)
		throw AtomicError(mfErrorToString(err));
}

void Magma::Framework::Thread::Atomic<mfmI32>::Subtract(mfmI32 value)
{
	mfError err = mftAtomic32Add(&m_value, -value);
	if (err != MF_ERROR_OKAY)
		throw AtomicError(mfErrorToString(err));
}
