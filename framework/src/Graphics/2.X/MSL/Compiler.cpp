#include <Magma/Framework/Graphics/2.X/MSL/Compiler.hpp>
#include <Magma/Framework/Graphics/Exception.hpp>
#include <Magma/Framework/ErrorString.h>
#include <Magma/Framework/String/StringStream.hpp>

void Magma::Framework::Graphics::V2X::MSL::Compile(const mfsUTF8CodeUnit * src, mfmU8 * outBytecode, mfmU64 bytecodeMaxSize, mfmU64 * outBytecodeSize, mfmU8 * outMetaData, mfmU64 metaDataMaxSize, mfmU64 * outMetaDataSize, ShaderType type)
{
	mfgV2XMVLCompilerInfo info;
	mfError err;

	err = mfgV2XRunMSLCompiler(src, outBytecode, bytecodeMaxSize, outMetaData, metaDataMaxSize, static_cast<mfgV2XEnum>(type), &info);
	if (err != MF_ERROR_OKAY)
	{
		mfsUTF8CodeUnit msg[512];
		String::StringStream ss(msg, sizeof(msg));
		ss.Get().PutString(mfErrorToString(err));
		ss.Get().PutString(":\n");
		ss.Get().PutString(info.errorMsg);
		ss.Get().PutByte('\n');
		ss.Get().Release();
		throw ShaderError(msg);
	}

	if (outBytecodeSize != nullptr)
		*outBytecodeSize = info.bytecodeSize;
	if (outMetaDataSize != nullptr)
		*outMetaDataSize = info.metaDataSize;
}