#include <Magma/Framework/Entry.h>
#include <Magma/Framework/String/Stream.h>
#include <Magma/Framework/Graphics/2.X/MSL/Compiler.h>
#include <Magma/Framework/Graphics/2.X/Bytecode.h>
#include <Magma/Framework/Graphics/2.X/OGL4Assembler.h>
#include <Magma/Framework/Graphics/2.X/D3D11Assembler.h>

#include <stdlib.h>

int main(int argc, const char** argv)
{
	if (mfInit(argc, argv) != MF_ERROR_OKAY)
		abort();

	const mfsUTF8CodeUnit* src =
		u8"Input { float4 position : position; int instanceID : _instanceID; };"
		u8"Output { float4 position : _position; };"
		u8"ConstantBuffer buffer : buffer { float4x4 transforms[256]; };"
		u8"Texture1D tex1d : texture1D;"
		u8"Texture2D tex2d : texture2D;"
		u8"Texture3D tex3d : texture3D;"

		u8"void main()"
		u8"{"
		u8"		float i = sample1D(tex1d, minf(2.0f, 4.0f));"
		u8"		Output.position = mulvec(buffer.transforms[Input.instanceID], Input.position);"
		u8"}"
		;

	mfmU8 bytecode[4096];
	mfmU8 metaData[4096];
	mfgV2XMVLCompilerInfo compilerInfo;
	
	if (mfgV2XRunMVLCompiler(src, bytecode, sizeof(bytecode), metaData, sizeof(metaData), MFG_VERTEX_SHADER, &compilerInfo) != MF_ERROR_OKAY)
	{
		mfsPutString(mfsErrStream, compilerInfo.errorMsg);
		abort();
	}

	mfgMetaData* md;
	if (mfgLoadMetaData(metaData, compilerInfo.metaDataSize, &md, NULL) != MF_ERROR_OKAY)
		abort();

	if(mfgV2XOGL4Assemble(bytecode, compilerInfo.bytecodeSize, md, mfsOutStream) != MF_ERROR_OKAY)
		abort();

	if (mfgV2XD3D11Assemble(bytecode, compilerInfo.bytecodeSize, md, mfsOutStream) != MF_ERROR_OKAY)
		abort();

	mfgUnloadMetaData(md);

	mfTerminate();

	for (;;);
	return 0;
}