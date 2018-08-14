#include "D3D11Assembler.h"

const static mfmU8 mfgD3D11AssemblerMinorVersion = 0x00;

mfgError mfgD3D11PutID(mfmU16 id, const mfgMetaData* metaData, mfsStream* out)
{
	// TO DO
	return MFG_ERROR_OKAY;
}

mfgError mfgD3D11Assemble(const mfmU8* bytecode, mfmU64 bytecodeSize, const mfgMetaData* metaData, mfsStream* outputStream)
{
	// Check if the arguments are valid
	if (bytecode == NULL || metaData == NULL || outputStream == NULL)
		return MFG_ERROR_INVALID_ARGUMENTS;

	// Check if the file is valid
	if (bytecodeSize < 6)
		return MFG_ERROR_INVALID_DATA;
	if (bytecode[0] != MFG_BYTECODE_HEADER_MARKER_0 ||
		bytecode[1] != MFG_BYTECODE_HEADER_MARKER_1 ||
		bytecode[2] != MFG_BYTECODE_HEADER_MARKER_2 ||
		bytecode[3] != MFG_BYTECODE_HEADER_MARKER_3)
		return MFG_ERROR_INVALID_DATA;

	// Check version
	if (bytecode[4] != 0x02) // Check if the major version is supported
		return MFG_ERROR_UNSUPPORTED_MAJOR_VER;
	if (bytecode[5] > mfgD3D11AssemblerMinorVersion) // Check if minor version is supported
		return MFG_ERROR_UNSUPPORTED_MINOR_VER;

	// Check shader type
	if (metaData->shaderType == MFG_VERTEX_SHADER)
	{
		const mfsUTF8CodeUnit str[] =
			u8"// This HLSL shader was automatically generated from binary bytecode by the mfgD3D11Assemble function\n"
			u8"// Vertex shader\n"
			u8"// DO NOT MODIFY THIS FILE BY HAND\n\n";
		mfsError err = mfsWrite(outputStream, str, sizeof(str), NULL);
		if (err != MFS_ERROR_OKAY)
			return MFG_ERROR_FAIELD_TO_WRITE;
	}
	else if (metaData->shaderType == MFG_PIXEL_SHADER)
	{
		const mfsUTF8CodeUnit str[] =
			u8"// This HLSL shader was automatically generated from binary bytecode by the mfgD3D11Assemble function\n"
			u8"// Pixel shader\n"
			u8"// DO NOT MODIFY THIS FILE BY HAND\n\n";
		mfsError err = mfsWrite(outputStream, str, sizeof(str), NULL);
		if (err != MFS_ERROR_OKAY)
			return MFG_ERROR_FAIELD_TO_WRITE;
	}
	else return MFG_ERROR_INVALID_DATA;

	// Add binding points
	{
		mfgMetaDataBindingPoint* bp  = metaData->firstBindingPoint;
		while (bp != NULL)
		{
			mfsPrintFormatUTF8(outputStream, u8"");
			bp = bp->next;
		}
	}

	/*const mfmU8* it = bytecode + 6;
	while (it < bytecode + bytecodeSize)
	{
		switch (*it)
		{
			// TO DO
			case MFG_BYTECODE_DECLB1:
			{
				++it;
			} break;
			default:
			{

			} break;
		}
	}*/
	
	return MFG_ERROR_OKAY;
}
