#include "Bytecode.hpp"
#include "../Exception.hpp"
#include "../../ErrorString.h"

Magma::Framework::Graphics::V2X::MetaDataHandle Magma::Framework::Graphics::V2X::LoadMetaData(const mfmU8 * binaryData, mfmU64 binarySize, Memory::AllocatorHandle allocator)
{
	mfgMetaData* md = NULL;
	mfError err = mfgLoadMetaData(binaryData, binarySize, &md, allocator.GetNoChecks());
	if (err != MF_ERROR_OKAY)
		throw ShaderError(mfErrorToString(err));
	return md;
}
