#include "Compiler.hpp"

#include "../../Exception.hpp"
#include "../../../ErrorString.h"
#include "../../../String/StringStream.hpp"

void Magma::Framework::Graphics::V2X::MSL::Compile(const mfsUTF8CodeUnit * src, mfmU8 * outBytecode, mfmU64 bytecodeMaxSize, mfmU64 * outBytecodeSize, mfmU8 * outMetaData, mfmU64 metaDataMaxSize, mfmU64 * outMetaDataSize, ShaderType type)
{
	mfgV2XMVLCompilerInfo info;
	mfError err;

	err = mfgV2XRunMSLCompiler(src, outBytecode, bytecodeMaxSize, outMetaData, metaDataMaxSize, static_cast<mfgV2XEnum>(type), &info);
	if (err != MF_ERROR_OKAY)
	{
		mfsUTF8CodeUnit msg[512];
		String::StringStream ss = String::CreateStringStream(msg, sizeof(msg));
		ss.PutString(mfErrorToString(err));
		ss.PutString(":\n");
		ss.PutString(info.errorMsg);
		ss.PutByte('\n');
		ss.Release();

		throw ShaderError(msg);
	}

	if (outBytecodeSize != nullptr)
		*outBytecodeSize = info.bytecodeSize;
	if (outMetaDataSize != nullptr)
		*outMetaDataSize = info.metaDataSize;
}