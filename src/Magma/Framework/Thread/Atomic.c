#include "Atomic.h"
#include "Config.h"

#if defined(MAGMA_FRAMEWORK_USE_WINDOWS_THREADS)

#include <Windows.h>

mfError mftAtomic8Load(const mfmI8 * atomic, mfmI8 * out)
{
#if defined(MAGMA_FRAMEWORK_DEBUG)
	if (atomic == NULL || out == NULL)
		return MFT_ERROR_INVALID_ARGUMENTS;
#endif
	*out = *atomic;
	return MF_ERROR_OKAY;
}

mfError mftAtomic8Store(mfmI8 * atomic, mfmI8 value)
{
#if defined(MAGMA_FRAMEWORK_DEBUG)
	if (atomic == NULL)
		return MFT_ERROR_INVALID_ARGUMENTS;
#endif
	InterlockedExchange8(atomic, value);
	return MF_ERROR_OKAY;
}

mfError mftAtomic16Load(const mfmI16 * atomic, mfmI16 * out)
{
#if defined(MAGMA_FRAMEWORK_DEBUG)
	if (atomic == NULL || out == NULL)
		return MFT_ERROR_INVALID_ARGUMENTS;
#endif
	*out = *atomic;
	return MF_ERROR_OKAY;
}

mfError mftAtomic16Store(mfmI16 * atomic, mfmI16 value)
{
#if defined(MAGMA_FRAMEWORK_DEBUG)
	if (atomic == NULL)
		return MFT_ERROR_INVALID_ARGUMENTS;
#endif
	InterlockedExchange16(atomic, value);
	return MF_ERROR_OKAY;
}

mfError mftAtomic32Load(const volatile mfmI32 * atomic, mfmI32 * out)
{
#if defined(MAGMA_FRAMEWORK_DEBUG)
	if (atomic == NULL || out == NULL)
		return MFT_ERROR_INVALID_ARGUMENTS;
#endif
	*out = *atomic;
	return MF_ERROR_OKAY;
}

mfError mftAtomic32Store(volatile mfmI32 * atomic, mfmI32 value)
{
#if defined(MAGMA_FRAMEWORK_DEBUG)
	if (atomic == NULL)
		return MFT_ERROR_INVALID_ARGUMENTS;
#endif
	InterlockedExchange(atomic, value);
	return MF_ERROR_OKAY;
}

mfError mftAtomic32Add(const volatile mfmI32 * atomic, mfmI32 value)
{
#if defined(MAGMA_FRAMEWORK_DEBUG)
	if (atomic == NULL)
		return MFT_ERROR_INVALID_ARGUMENTS;
#endif
	InterlockedAdd(atomic, value);
	return MF_ERROR_OKAY;
}

#else
#error No magma framework thread library support
#endif



