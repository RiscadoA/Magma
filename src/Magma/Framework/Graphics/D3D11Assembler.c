#include "D3D11Assembler.h"

const static mfmU8 mfgD3D11AssemblerMinorVersion = 0x00;

mfgError mfgD3D11WriteType(mfmU8 type, mfsStream* out)
{
	if (out == NULL)
		return MFG_ERROR_INVALID_ARGUMENTS;

	const mfsUTF8CodeUnit* str = NULL;

	if (type == MFG_INT1)
		str = u8"int";
	else if (type == MFG_INT2)
		str = u8"int2";
	else if (type == MFG_INT3)
		str = u8"int3";
	else if (type == MFG_INT4)
		str = u8"int4";
	else if (type == MFG_INT22)
		str = u8"int2x2";
	else if (type == MFG_INT33)
		str = u8"int3x3";
	else if (type == MFG_INT44)
		str = u8"int4x4";

	else if (type == MFG_FLOAT1)
		str = u8"float";
	else if (type == MFG_FLOAT2)
		str = u8"float2";
	else if (type == MFG_FLOAT3)
		str = u8"float3";
	else if (type == MFG_FLOAT4)
		str = u8"float4";
	else if (type == MFG_FLOAT22)
		str = u8"float2x2";
	else if (type == MFG_FLOAT33)
		str = u8"float3x3";
	else if (type == MFG_FLOAT44)
		str = u8"float4x4";

	else return MFG_ERROR_INVALID_DATA;

	mfsError err = mfsPrintFormatUTF8(out, str);
	if (err != MFS_ERROR_OKAY)
		return MFG_ERROR_FAILED_TO_WRITE;
	return MFG_ERROR_OKAY;
}

mfgError mfgD3D11WriteDeclType(mfmU8 type, mfsStream* out, mfmBool* isArray)
{
	if (out == NULL)
		return MFG_ERROR_INVALID_ARGUMENTS;

	const mfsUTF8CodeUnit* str = NULL;

	if (isArray != NULL)
		*isArray = MFM_FALSE;

	if (type == MFG_BYTECODE_DECLB1)
		str = u8"bool";

	else if (MFG_BYTECODE_DECLI1)
		str = u8"int";
	else if (MFG_BYTECODE_DECLI2)
		str = u8"int2";
	else if (MFG_BYTECODE_DECLI3)
		str = u8"int3";
	else if (MFG_BYTECODE_DECLI4)
		str = u8"int4";
	else if (MFG_BYTECODE_DECLI22)
		str = u8"int2x2";
	else if (MFG_BYTECODE_DECLI33)
		str = u8"int3x3";
	else if (MFG_BYTECODE_DECLI44)
		str = u8"int4x4";

	else if (MFG_BYTECODE_DECLF1)
		str = u8"float";
	else if (MFG_BYTECODE_DECLF2)
		str = u8"float2";
	else if (MFG_BYTECODE_DECLF3)
		str = u8"float3";
	else if (MFG_BYTECODE_DECLF4)
		str = u8"float4";
	else if (MFG_BYTECODE_DECLF22)
		str = u8"float2x2";
	else if (MFG_BYTECODE_DECLF33)
		str = u8"float3x3";
	else if (MFG_BYTECODE_DECLF44)
		str = u8"float4x4";

	else if (type == MFG_BYTECODE_DECLI1A)
	{
		str = u8"int";
		if (isArray != NULL)
			*isArray = MFM_TRUE;
	}
	else if (type == MFG_BYTECODE_DECLI2A)
	{
		str = u8"int2";
		if (isArray != NULL)
			*isArray = MFM_TRUE;
	}
	else if (type == MFG_BYTECODE_DECLI3A)
	{
		str = u8"int3";
		if (isArray != NULL)
			*isArray = MFM_TRUE;
	}
	else if (type == MFG_BYTECODE_DECLI4A)
	{
		str = u8"int4";
		if (isArray != NULL)
			*isArray = MFM_TRUE;
	}
	else if (type == MFG_BYTECODE_DECLI22A)
	{
		str = u8"int2x2";
		if (isArray != NULL)
			*isArray = MFM_TRUE;
	}
	else if (type == MFG_BYTECODE_DECLI33A)
	{
		str = u8"int3x3";
		if (isArray != NULL)
			*isArray = MFM_TRUE;
	}
	else if (type == MFG_BYTECODE_DECLI44A)
	{
		str = u8"int4x4";
		if (isArray != NULL)
			*isArray = MFM_TRUE;
	}

	else if (type == MFG_BYTECODE_DECLF1A)
	{
		str = u8"float";
		if (isArray != NULL)
			*isArray = MFM_TRUE;
	}
	else if (type == MFG_BYTECODE_DECLF2A)
	{
		str = u8"float2";
		if (isArray != NULL)
			*isArray = MFM_TRUE;
	}
	else if (type == MFG_BYTECODE_DECLF3A)
	{
		str = u8"float3";
		if (isArray != NULL)
			*isArray = MFM_TRUE;
	}
	else if (type == MFG_BYTECODE_DECLF4A)
	{
		str = u8"float4";
		if (isArray != NULL)
			*isArray = MFM_TRUE;
	}
	else if (type == MFG_BYTECODE_DECLF22A)
	{
		str = u8"float2x2";
		if (isArray != NULL)
			*isArray = MFM_TRUE;
	}
	else if (type == MFG_BYTECODE_DECLF33A)
	{
		str = u8"float3x3";
		if (isArray != NULL)
			*isArray = MFM_TRUE;
	}
	else if (type == MFG_BYTECODE_DECLF44A)
	{
		str = u8"float4x4";
		if (isArray != NULL)
			*isArray = MFM_TRUE;
	}

	else return MFG_ERROR_INVALID_DATA;

	mfsError err = mfsPrintFormatUTF8(out, str);
	if (err != MFS_ERROR_OKAY)
		return MFG_ERROR_FAILED_TO_WRITE;
	return MFG_ERROR_OKAY;
}

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
		mfsError err = mfsWrite(outputStream, str, sizeof(str) - 1, NULL);
		if (err != MFS_ERROR_OKAY)
			return MFG_ERROR_FAILED_TO_WRITE;
	}
	else if (metaData->shaderType == MFG_PIXEL_SHADER)
	{
		const mfsUTF8CodeUnit str[] =
			u8"// This HLSL shader was automatically generated from binary bytecode by the mfgD3D11Assemble function\n"
			u8"// Pixel shader\n"
			u8"// DO NOT MODIFY THIS FILE BY HAND\n\n";
		mfsError err = mfsWrite(outputStream, str, sizeof(str) - 1, NULL);
		if (err != MFS_ERROR_OKAY)
			return MFG_ERROR_FAILED_TO_WRITE;
	}
	else return MFG_ERROR_INVALID_DATA;

	// Add binding points
	{
		mfgMetaDataBindingPoint* bp  = metaData->firstBindingPoint;
		while (bp != NULL)
		{
			if (bp->type == MFG_CONSTANT_BUFFER)
			{
				// TO DO
			}
			else if (bp->type == MFG_TEXTURE_1D)
			{
				// TO DO
			}
			else if (bp->type == MFG_TEXTURE_2D)
			{
				// TO DO
			}
			else if (bp->type == MFG_TEXTURE_3D)
			{
				// TO DO
			}
			else
			{
				mfsError err = mfsPrintFormatUTF8(outputStream, u8"// UNSUPPORTED BINDING POINT TYPE '%x'\n\n", bp->type);
				if (err != MFS_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
			}

			bp = bp->next;
		}
	}

	// Add input variables
	{
		if (mfsPrintFormatUTF8(outputStream, u8"struct ShaderInput\n{\n") != MFS_ERROR_OKAY)
			return MFG_ERROR_FAILED_TO_WRITE;

		mfgMetaDataInputVariable* var = metaData->firstInputVar;
		while (var != NULL)
		{
			if (mfsPutByte(outputStream, '\t') != MFS_ERROR_OKAY)
				return MFG_ERROR_FAILED_TO_WRITE;

			mfgError err = mfgD3D11WriteType(var->type, outputStream);
			if (err != MFG_ERROR_OKAY)
				return err;

			if (metaData->shaderType == MFG_VERTEX_SHADER)
			{
				if (mfsPrintFormatUTF8(outputStream, u8" in_%d : IN%dIN;\n", var->id, var->id - 1) != MFS_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
			}
			else if (metaData->shaderType == MFG_PIXEL_SHADER)
			{
				if (mfsPrintFormatUTF8(outputStream, u8" in_%d : VOUT%dVOUT;\n", var->id, var->id) != MFS_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
			}

			var = var->next;
		}

		if (mfsPrintFormatUTF8(outputStream, u8"};\n\n") != MFS_ERROR_OKAY)
			return MFG_ERROR_FAILED_TO_WRITE;
	}

	// Add output variables
	{
		if (mfsPrintFormatUTF8(outputStream, u8"struct ShaderOutput\n{\n") != MFS_ERROR_OKAY)
			return MFG_ERROR_FAILED_TO_WRITE;

		mfgMetaDataOutputVariable* var = metaData->firstOutputVar;
		while (var != NULL)
		{
			if (mfsPutByte(outputStream, '\t') != MFS_ERROR_OKAY)
				return MFG_ERROR_FAILED_TO_WRITE;

			mfgError err = mfgD3D11WriteType(var->type, outputStream);
			if (err != MFG_ERROR_OKAY)
				return err;

			if (metaData->shaderType == MFG_VERTEX_SHADER)
			{
				if (var->id == 0)
				{
					if (mfsPrintFormatUTF8(outputStream, u8" out_0 : SV_POSITION;\n") != MFS_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				}
				else if (mfsPrintFormatUTF8(outputStream, u8" out_%d : VOUT%dVOUT;\n", var->id, var->id) != MFS_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
			}
			else if (metaData->shaderType == MFG_PIXEL_SHADER)
			{
				if (var->id == 0)
				{
					if (mfsPrintFormatUTF8(outputStream, u8" out_0 : SV_Depth;\n") != MFS_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				}
				else if (mfsPrintFormatUTF8(outputStream, u8" out_%d : SV_Target%dVOUT;\n", var->id, var->id - 1) != MFS_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
			}

			var = var->next;
		}

		if (mfsPrintFormatUTF8(outputStream, u8"};\n\n") != MFS_ERROR_OKAY)
			return MFG_ERROR_FAILED_TO_WRITE;
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
