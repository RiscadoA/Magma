#pragma once

#include "../Memory/Type.h"
#include "Bytecode.h"
#include "Error.h"

#ifdef __cplusplus
extern "C"
{
#endif

	mfgError mfgD3D11Assemble(const mfmU8* data, mfmU64 dataSize);

#ifdef __cplusplus
}
#endif
