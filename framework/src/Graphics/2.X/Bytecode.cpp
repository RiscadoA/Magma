#include <Magma/Framework/Graphics/2.X/Bytecode.hpp>
#include <Magma/Framework/Graphics/Exception.hpp>
#include <Magma/Framework/ErrorString.h>

Magma::Framework::Graphics::V2X::HMetaData Magma::Framework::Graphics::V2X::LoadMetaData(const mfmU8 * binaryData, mfmU64 binarySize, Memory::HAllocator allocator)
{
	mfgMetaData* md = NULL;
	mfError err = mfgLoadMetaData(binaryData, binarySize, &md, allocator.GetNoChecks());
	if (err != MF_ERROR_OKAY)
		throw ShaderError(mfErrorToString(err));
	return md;
}
