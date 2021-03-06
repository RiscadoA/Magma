#include "OGL4Assembler.h"
#include "../../Memory/Endianness.h"

#include <string.h>

const static mfmU8 mfgOGL4AssemblerMinorVersion = 0x01;

typedef struct
{
	mfmU16 varIndex;
	mfmU16 id;
	mfmU8 index;
	mfmU8 rows;
	mfmU8 cols;
	mfmBool active;
	mfmBool isArray;
	mfmU16 accessID;
} mfgComponentReference;

typedef struct
{
	const mfgMetaData* metaData;
	mfgComponentReference references[128];
} mfgAssemblerData;

static mfError mfgOGL4WriteType(mfmU8 type, mfsStream* out)
{
	if (out == NULL)
		return MFG_ERROR_INVALID_ARGUMENTS;

	const mfsUTF8CodeUnit* str = NULL;

	if (type == MFG_INT1)
		str = u8"int";
	else if (type == MFG_INT2)
		str = u8"ivec2";
	else if (type == MFG_INT3)
		str = u8"ivec3";
	else if (type == MFG_INT4)
		str = u8"ivec4";
	else if (type == MFG_INT22)
		str = u8"imat2x2";
	else if (type == MFG_INT33)
		str = u8"imat3x3";
	else if (type == MFG_INT44)
		str = u8"imat4x4";

	else if (type == MFG_FLOAT1)
		str = u8"float";
	else if (type == MFG_FLOAT2)
		str = u8"vec2";
	else if (type == MFG_FLOAT3)
		str = u8"vec3";
	else if (type == MFG_FLOAT4)
		str = u8"vec4";
	else if (type == MFG_FLOAT22)
		str = u8"mat2";
	else if (type == MFG_FLOAT33)
		str = u8"mat3";
	else if (type == MFG_FLOAT44)
		str = u8"mat4";

	else return MFG_ERROR_INVALID_DATA;

	mfError err = mfsPrintFormat(out, str);
	if (err != MF_ERROR_OKAY)
		return MFG_ERROR_FAILED_TO_WRITE;
	return MF_ERROR_OKAY;
}

static mfError mfgOGL4PutID(mfmU16 id, const mfgAssemblerData* data, mfsStream* out)
{
	const mfgMetaData* metaData = data->metaData;

	// Check if it is an input variable
	{
		mfgMetaDataInputVariable* var = metaData->firstInputVar;
		while (var != NULL)
		{
			if (var->id == id)
			{
				if (data->metaData->shaderType == MFG_VERTEX_SHADER)
				{
					if (!strcmp(var->name, u8"_vertexID"))
					{
						if (mfsPrintFormat(out, u8"gl_VertexID") != MF_ERROR_OKAY)
							return MFG_ERROR_FAILED_TO_WRITE;
						return MF_ERROR_OKAY;
					}
					else if (!strcmp(var->name, u8"_instanceID"))
					{
						if (mfsPrintFormat(out, u8"gl_InstanceID") != MF_ERROR_OKAY)
							return MFG_ERROR_FAILED_TO_WRITE;
						return MF_ERROR_OKAY;
					}
					else
					{
						if (mfsPrintFormat(out, u8"in_%d", var->id) != MF_ERROR_OKAY)
							return MFG_ERROR_FAILED_TO_WRITE;
						return MF_ERROR_OKAY;
					}
				}
				else if (data->metaData->shaderType == MFG_PIXEL_SHADER)
				{
					if (!strcmp(var->name, u8"_position"))
					{
						if (mfsPrintFormat(out, u8"gl_FragCoord") != MF_ERROR_OKAY)
							return MFG_ERROR_FAILED_TO_WRITE;
						return MF_ERROR_OKAY;
					}
					else if (!strcmp(var->name, u8"_in0"))
					{
						if (mfsPrintFormat(out, u8"in_0") != MF_ERROR_OKAY)
							return MFG_ERROR_FAILED_TO_WRITE;
						return MF_ERROR_OKAY;
					}
					else if (!strcmp(var->name, u8"_in1"))
					{
						if (mfsPrintFormat(out, u8"in_1") != MF_ERROR_OKAY)
							return MFG_ERROR_FAILED_TO_WRITE;
						return MF_ERROR_OKAY;
					}
					else if (!strcmp(var->name, u8"_in2"))
					{
						if (mfsPrintFormat(out, u8"in_2") != MF_ERROR_OKAY)
							return MFG_ERROR_FAILED_TO_WRITE;
						return MF_ERROR_OKAY;
					}
					else if (!strcmp(var->name, u8"_in3"))
					{
						if (mfsPrintFormat(out, u8"in_3") != MF_ERROR_OKAY)
							return MFG_ERROR_FAILED_TO_WRITE;
						return MF_ERROR_OKAY;
					}
					else if (!strcmp(var->name, u8"_in4"))
					{
						if (mfsPrintFormat(out, u8"in_4") != MF_ERROR_OKAY)
							return MFG_ERROR_FAILED_TO_WRITE;
						return MF_ERROR_OKAY;
					}
					else if (!strcmp(var->name, u8"_in5"))
					{
						if (mfsPrintFormat(out, u8"in_5") != MF_ERROR_OKAY)
							return MFG_ERROR_FAILED_TO_WRITE;
						return MF_ERROR_OKAY;
					}
					else if (!strcmp(var->name, u8"_in6"))
					{
						if (mfsPrintFormat(out, u8"in_6") != MF_ERROR_OKAY)
							return MFG_ERROR_FAILED_TO_WRITE;
						return MF_ERROR_OKAY;
					}
					else if (!strcmp(var->name, u8"_in7"))
					{
						if (mfsPrintFormat(out, u8"in_7") != MF_ERROR_OKAY)
							return MFG_ERROR_FAILED_TO_WRITE;
						return MF_ERROR_OKAY;
					}
				}
			}
			var = var->next;
		}
	}

	// Check if it is an output variable
	{
		mfgMetaDataOutputVariable* var = metaData->firstOutputVar;
		while (var != NULL)
		{
			if (var->id == id)
			{
				if (data->metaData->shaderType == MFG_VERTEX_SHADER)
				{
					if (!strcmp(var->name, u8"_position"))
					{
						if (mfsPrintFormat(out, u8"gl_Position") != MF_ERROR_OKAY)
							return MFG_ERROR_FAILED_TO_WRITE;
						return MF_ERROR_OKAY;
					}
					else if (!strcmp(var->name, u8"_out0"))
					{
						if (mfsPrintFormat(out, u8"out_0") != MF_ERROR_OKAY)
							return MFG_ERROR_FAILED_TO_WRITE;
						return MF_ERROR_OKAY;
					}
					else if (!strcmp(var->name, u8"_out1"))
					{
						if (mfsPrintFormat(out, u8"out_1") != MF_ERROR_OKAY)
							return MFG_ERROR_FAILED_TO_WRITE;
						return MF_ERROR_OKAY;
					}
					else if (!strcmp(var->name, u8"_out2"))
					{
						if (mfsPrintFormat(out, u8"out_2") != MF_ERROR_OKAY)
							return MFG_ERROR_FAILED_TO_WRITE;
						return MF_ERROR_OKAY;
					}
					else if (!strcmp(var->name, u8"_out3"))
					{
						if (mfsPrintFormat(out, u8"out_3") != MF_ERROR_OKAY)
							return MFG_ERROR_FAILED_TO_WRITE;
						return MF_ERROR_OKAY;
					}
					else if (!strcmp(var->name, u8"_out4"))
					{
						if (mfsPrintFormat(out, u8"out_4") != MF_ERROR_OKAY)
							return MFG_ERROR_FAILED_TO_WRITE;
						return MF_ERROR_OKAY;
					}
					else if (!strcmp(var->name, u8"_out5"))
					{
						if (mfsPrintFormat(out, u8"out_5") != MF_ERROR_OKAY)
							return MFG_ERROR_FAILED_TO_WRITE;
						return MF_ERROR_OKAY;
					}
					else if (!strcmp(var->name, u8"_out6"))
					{
						if (mfsPrintFormat(out, u8"out_6") != MF_ERROR_OKAY)
							return MFG_ERROR_FAILED_TO_WRITE;
						return MF_ERROR_OKAY;
					}
					else if (!strcmp(var->name, u8"_out7"))
					{
						if (mfsPrintFormat(out, u8"out_7") != MF_ERROR_OKAY)
							return MFG_ERROR_FAILED_TO_WRITE;
						return MF_ERROR_OKAY;
					}
				}
				else if (data->metaData->shaderType == MFG_PIXEL_SHADER)
				{
					if (!strcmp(var->name, u8"_fragDepth"))
					{
						if (mfsPrintFormat(out, u8"gl_FragDepth") != MF_ERROR_OKAY)
							return MFG_ERROR_FAILED_TO_WRITE;
						return MF_ERROR_OKAY;
					}
					else if (!strcmp(var->name, u8"_target0"))
					{
						if (mfsPrintFormat(out, u8"target_0") != MF_ERROR_OKAY)
							return MFG_ERROR_FAILED_TO_WRITE;
						return MF_ERROR_OKAY;
					}
					else if (!strcmp(var->name, u8"_target1"))
					{
						if (mfsPrintFormat(out, u8"target_1") != MF_ERROR_OKAY)
							return MFG_ERROR_FAILED_TO_WRITE;
						return MF_ERROR_OKAY;
					}
					else if (!strcmp(var->name, u8"_target2"))
					{
						if (mfsPrintFormat(out, u8"target_2") != MF_ERROR_OKAY)
							return MFG_ERROR_FAILED_TO_WRITE;
						return MF_ERROR_OKAY;
					}
					else if (!strcmp(var->name, u8"_target3"))
					{
						if (mfsPrintFormat(out, u8"target_3") != MF_ERROR_OKAY)
							return MFG_ERROR_FAILED_TO_WRITE;
						return MF_ERROR_OKAY;
					}
					else if (!strcmp(var->name, u8"_target4"))
					{
						if (mfsPrintFormat(out, u8"target_4") != MF_ERROR_OKAY)
							return MFG_ERROR_FAILED_TO_WRITE;
						return MF_ERROR_OKAY;
					}
					else if (!strcmp(var->name, u8"_target5"))
					{
						if (mfsPrintFormat(out, u8"target_5") != MF_ERROR_OKAY)
							return MFG_ERROR_FAILED_TO_WRITE;
						return MF_ERROR_OKAY;
					}
					else if (!strcmp(var->name, u8"_target6"))
					{
						if (mfsPrintFormat(out, u8"target_6") != MF_ERROR_OKAY)
							return MFG_ERROR_FAILED_TO_WRITE;
						return MF_ERROR_OKAY;
					}
					else if (!strcmp(var->name, u8"_target7"))
					{
						if (mfsPrintFormat(out, u8"target_7") != MF_ERROR_OKAY)
							return MFG_ERROR_FAILED_TO_WRITE;
						return MF_ERROR_OKAY;
					}
				}
			}
			var = var->next;
		}
	}

	// Check if it is a binding point
	{
		mfgMetaDataBindingPoint* bp = metaData->firstBindingPoint;
		while (bp != NULL)
		{
			if (bp->type == MFG_CONSTANT_BUFFER)
			{
				mfgMetaDataConstantBuffer* cb = bp;
				mfgMetaDataConstantBufferVariable* var = cb->firstVariable;
				while (var != NULL)
				{
					if (var->id == id)
					{
						if (mfsPrintFormat(out, u8"buf_%s_%d", bp->name, id) != MF_ERROR_OKAY)
							return MFG_ERROR_FAILED_TO_WRITE;
						return MF_ERROR_OKAY;
					}
					var = var->next;
				}
			}
			else if (bp->type == MFG_TEXTURE_1D)
			{
				mfgMetaDataTexture1D* tex = bp;
				if (bp->id == id)
				{
					if (mfsPrintFormat(out, u8"tex1d_%d", id) != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
					return MF_ERROR_OKAY;
				}
			}
			else if (bp->type == MFG_TEXTURE_2D)
			{
				mfgMetaDataTexture2D* tex = bp;
				if (bp->id == id)
				{
					if (mfsPrintFormat(out, u8"tex2d_%d", id) != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
					return MF_ERROR_OKAY;
				}
			}
			else if (bp->type == MFG_TEXTURE_3D)
			{
				mfgMetaDataTexture3D* tex = bp;
				if (bp->id == id)
				{
					if (mfsPrintFormat(out, u8"tex3d_%d", id) != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
					return MF_ERROR_OKAY;
				}
			}

			bp = bp->next;
		}
	}

	// Check if it is a component reference
	{
		for (mfmU8 i = 0; i < 128; ++i)
		{
			if (data->references[i].active == MFM_FALSE)
				continue;
			if (id == data->references[i].id)
			{
				// Get variable reference
				mfError err = mfgOGL4PutID(data->references[i].varIndex, data, out);
				if (err != MF_ERROR_OKAY)
					return err;

				// Get component
				if (data->references[i].isArray == MFM_TRUE)
				{
					err = mfsPutString(out, u8"[");
					if (err != MF_ERROR_OKAY)
						return err;
					err = mfgOGL4PutID(data->references[i].accessID, data, out);
					if (err != MF_ERROR_OKAY)
						return err;
					err = mfsPutString(out, u8"]");
					if (err != MF_ERROR_OKAY)
						return err;
					return MF_ERROR_OKAY;
				}
				else if (data->references[i].rows == 2 && data->references[i].cols == 1)
					switch (data->references[i].index)
					{
						case 0x00:
							if (mfsPrintFormat(out, u8".x", id) != MF_ERROR_OKAY)
								return MFG_ERROR_FAILED_TO_WRITE;
							return MF_ERROR_OKAY;
						case 0x01:
							if (mfsPrintFormat(out, u8".y", id) != MF_ERROR_OKAY)
								return MFG_ERROR_FAILED_TO_WRITE;
							return MF_ERROR_OKAY;
						default:
							return MFG_ERROR_INVALID_DATA;
					}
				else if (data->references[i].rows == 3 && data->references[i].cols == 1)
					switch (data->references[i].index)
					{
						case 0x00:
							if (mfsPrintFormat(out, u8".x", id) != MF_ERROR_OKAY)
								return MFG_ERROR_FAILED_TO_WRITE;
							return MF_ERROR_OKAY;
						case 0x01:
							if (mfsPrintFormat(out, u8".y", id) != MF_ERROR_OKAY)
								return MFG_ERROR_FAILED_TO_WRITE;
							return MF_ERROR_OKAY;
						case 0x02:
							if (mfsPrintFormat(out, u8".z", id) != MF_ERROR_OKAY)
								return MFG_ERROR_FAILED_TO_WRITE;
							return MF_ERROR_OKAY;
						default:
							return MFG_ERROR_INVALID_DATA;
					}
				else if (data->references[i].rows == 4 && data->references[i].cols == 1)
					switch (data->references[i].index)
					{
						case 0x00:
							if (mfsPrintFormat(out, u8".x", id) != MF_ERROR_OKAY)
								return MFG_ERROR_FAILED_TO_WRITE;
							return MF_ERROR_OKAY;
						case 0x01:
							if (mfsPrintFormat(out, u8".y", id) != MF_ERROR_OKAY)
								return MFG_ERROR_FAILED_TO_WRITE;
							return MF_ERROR_OKAY;
						case 0x02:
							if (mfsPrintFormat(out, u8".z", id) != MF_ERROR_OKAY)
								return MFG_ERROR_FAILED_TO_WRITE;
							return MF_ERROR_OKAY;
						case 0x03:
							if (mfsPrintFormat(out, u8".w", id) != MF_ERROR_OKAY)
								return MFG_ERROR_FAILED_TO_WRITE;
							return MF_ERROR_OKAY;
						default:
							return MFG_ERROR_INVALID_DATA;
					}
				else if (data->references[i].rows == 2 && data->references[i].cols == 2)
				{
					mfmU8 cols = data->references[i].index % 2;
					mfmU8 rows = data->references[i].index / 2;
					if (cols >= 2 || rows >= 2)
						return MFG_ERROR_INVALID_DATA;
					if (mfsPrintFormat(out, u8"[%d][%d]", cols, rows) != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
					return MF_ERROR_OKAY;
				}
				else if (data->references[i].rows == 3 && data->references[i].cols == 3)
				{
					mfmU8 cols = data->references[i].index % 3;
					mfmU8 rows = data->references[i].index / 3;
					if (cols >= 3 || rows >= 3)
						return MFG_ERROR_INVALID_DATA;
					if (mfsPrintFormat(out, u8"[%d][%d]", cols, rows) != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
					return MF_ERROR_OKAY;
				}
				else if (data->references[i].rows == 4 && data->references[i].cols == 4)
				{
					mfmU8 cols = data->references[i].index % 4;
					mfmU8 rows = data->references[i].index / 4;
					if (cols >= 4 || rows >= 4)
						return MFG_ERROR_INVALID_DATA;
					if (mfsPrintFormat(out, u8"[%d][%d]", cols, rows) != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
					return MF_ERROR_OKAY;
				}
			}
		}
	}

	if (mfsPrintFormat(out, u8"local_%d", id) != MF_ERROR_OKAY)
		return MFG_ERROR_FAILED_TO_WRITE;
	return MF_ERROR_OKAY;
}

mfError mfgV2XOGL4Assemble(const mfmU8* bytecode, mfmU64 bytecodeSize, const mfgMetaData* metaData, mfsStream* outputStream)
{
	mfgAssemblerData assemblerData;
	assemblerData.metaData = metaData;
	for (mfmU8 i = 0; i < 128; ++i)
		assemblerData.references[i].active = MFM_FALSE;

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
	if (bytecode[5] > mfgOGL4AssemblerMinorVersion) // Check if minor version is supported
		return MFG_ERROR_UNSUPPORTED_MINOR_VER;

	// Check shader type
	if (metaData->shaderType == MFG_VERTEX_SHADER)
	{
		const mfsUTF8CodeUnit str[] =
			u8"// This GLSL shader was automatically generated from binary bytecode by the mfgOGL4Assemble function\n"
			u8"// Vertex shader\n"
			u8"// DO NOT MODIFY THIS FILE BY HAND\n\n#version 410 core\n\n";
		mfError err = mfsWrite(outputStream, str, sizeof(str) - 1, NULL);
		if (err != MF_ERROR_OKAY)
			return MFG_ERROR_FAILED_TO_WRITE;
	}
	else if (metaData->shaderType == MFG_PIXEL_SHADER)
	{
		const mfsUTF8CodeUnit str[] =
			u8"// This GLSL shader was automatically generated from binary bytecode by the mfgOGL4Assemble function\n"
			u8"// Pixel shader\n"
			u8"// DO NOT MODIFY THIS FILE BY HAND\n\n#version 410 core\n\n";
		mfError err = mfsWrite(outputStream, str, sizeof(str) - 1, NULL);
		if (err != MF_ERROR_OKAY)
			return MFG_ERROR_FAILED_TO_WRITE;
	}
	else return MFG_ERROR_INVALID_DATA;

	// Add binding points
	{
		mfgMetaDataBindingPoint* bp = metaData->firstBindingPoint;
		while (bp != NULL)
		{
			if (bp->type == MFG_CONSTANT_BUFFER)
			{
				mfgMetaDataConstantBuffer* cb = bp;
				if (mfsPrintFormat(outputStream, u8"layout (std140) uniform buf_%s\n{\n", bp->name) != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;

				mfgMetaDataConstantBufferVariable* var = cb->firstVariable;
				while (var != NULL)
				{
					if (mfsPutByte(outputStream, '\t') != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;

					mfError err = mfgOGL4WriteType(var->type, outputStream);
					if (err != MF_ERROR_OKAY)
						return err;

					if (var->arraySize == 0)
					{
						if (mfsPrintFormat(outputStream, u8" buf_%s_%d;\n", bp->name, var->id) != MF_ERROR_OKAY)
							return MFG_ERROR_FAILED_TO_WRITE;
					}
					else
					{
						if (mfsPrintFormat(outputStream, u8" buf_%s_%d[%d];\n", bp->name, var->id, var->arraySize) != MF_ERROR_OKAY)
							return MFG_ERROR_FAILED_TO_WRITE;
					}

					var = var->next;
				}

				if (mfsPrintFormat(outputStream, u8"};\n\n") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
			}
			else if (bp->type == MFG_TEXTURE_1D)
			{
				mfgMetaDataTexture1D* tex = bp;
				if (mfsPrintFormat(outputStream, u8"uniform sampler1D tex1d_%d;\n\n", bp->id) != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
			}
			else if (bp->type == MFG_TEXTURE_2D)
			{
				mfgMetaDataTexture2D* tex = bp;
				if (mfsPrintFormat(outputStream, u8"uniform sampler2D tex2d_%d;\n\n", bp->id) != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
			}
			else if (bp->type == MFG_TEXTURE_3D)
			{
				mfgMetaDataTexture3D* tex = bp;
				if (mfsPrintFormat(outputStream, u8"uniform sampler3D tex3d_%d;\n\n", bp->id) != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
			}
			else
			{
				mfError err = mfsPrintFormat(outputStream, u8"// UNSUPPORTED BINDING POINT TYPE '%x'\n\n", bp->type);
				if (err != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
			}

			bp = bp->next;
		}
	}

	// Add input variables
	{
		mfgMetaDataInputVariable* var = metaData->firstInputVar;
		while (var != NULL)
		{
			if (metaData->shaderType == MFG_VERTEX_SHADER)
			{
				if (!strcmp(var->name, u8"_vertexID"))
				{
					if (mfsPutString(outputStream, u8"in ") != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
					mfError err = mfgOGL4WriteType(var->type, outputStream);
					if (err != MF_ERROR_OKAY)
						return err;
					if (mfsPutString(outputStream, u8" gl_VertexID;\n\n") != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				}
				else if (!strcmp(var->name, u8"_instanceID"))
				{
					if (mfsPutString(outputStream, u8"in ") != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
					mfError err = mfgOGL4WriteType(var->type, outputStream);
					if (err != MF_ERROR_OKAY)
						return err;
					if (mfsPutString(outputStream, u8" gl_InstanceID;\n\n") != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				}
				else
				{
					if (mfsPrintFormat(outputStream, u8"layout (location = %d) in ", var->id) != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
					mfError err = mfgOGL4WriteType(var->type, outputStream);
					if (err != MF_ERROR_OKAY)
						return err;
					if (mfsPrintFormat(outputStream, u8" in_%d;\n\n", var->id) != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				}
			}
			else if (metaData->shaderType == MFG_PIXEL_SHADER)
			{
				if (!strcmp(var->name, u8"_position"))
				{
					if (mfsPutString(outputStream, u8"in ") != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
					mfError err = mfgOGL4WriteType(var->type, outputStream);
					if (err != MF_ERROR_OKAY)
						return err;
					if (mfsPutString(outputStream, u8" gl_FragCoord;\n\n") != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				}
				else if (!strcmp(var->name, u8"_in0"))
				{
					if (mfsPutString(outputStream, u8"layout (location = 0) in ") != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
					mfError err = mfgOGL4WriteType(var->type, outputStream);
					if (err != MF_ERROR_OKAY)
						return err;
					if (mfsPrintFormat(outputStream, u8" in_%d;\n\n", var->id) != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				}
				else if (!strcmp(var->name, u8"_in1"))
				{
					if (mfsPutString(outputStream, u8"layout (location = 1) in ") != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
					mfError err = mfgOGL4WriteType(var->type, outputStream);
					if (err != MF_ERROR_OKAY)
						return err;
					if (mfsPrintFormat(outputStream, u8" in_%d;\n\n", var->id) != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				}
				else if (!strcmp(var->name, u8"_in2"))
				{
					if (mfsPutString(outputStream, u8"layout (location = 2) in ") != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
					mfError err = mfgOGL4WriteType(var->type, outputStream);
					if (err != MF_ERROR_OKAY)
						return err;
					if (mfsPrintFormat(outputStream, u8" in_%d;\n\n", var->id) != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				}
				else if (!strcmp(var->name, u8"_in3"))
				{
					if (mfsPutString(outputStream, u8"layout (location = 3) in ") != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
					mfError err = mfgOGL4WriteType(var->type, outputStream);
					if (err != MF_ERROR_OKAY)
						return err;
					if (mfsPrintFormat(outputStream, u8" in_%d;\n\n", var->id) != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				}
				else if (!strcmp(var->name, u8"_in4"))
				{
					if (mfsPutString(outputStream, u8"layout (location = 4) in ") != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
					mfError err = mfgOGL4WriteType(var->type, outputStream);
					if (err != MF_ERROR_OKAY)
						return err;
					if (mfsPrintFormat(outputStream, u8" in_%d;\n\n", var->id) != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				}
				else if (!strcmp(var->name, u8"_in5"))
				{
					if (mfsPutString(outputStream, u8"layout (location = 5) in ") != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
					mfError err = mfgOGL4WriteType(var->type, outputStream);
					if (err != MF_ERROR_OKAY)
						return err;
					if (mfsPrintFormat(outputStream, u8" in_%d;\n\n", var->id) != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				}
				else if (!strcmp(var->name, u8"_in6"))
				{
					if (mfsPutString(outputStream, u8"layout (location = 6) in ") != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
					mfError err = mfgOGL4WriteType(var->type, outputStream);
					if (err != MF_ERROR_OKAY)
						return err;
					if (mfsPrintFormat(outputStream, u8" in_%d;\n\n", var->id) != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				}
				else if (!strcmp(var->name, u8"_in7"))
				{
					if (mfsPutString(outputStream, u8"layout (location = 7) in ") != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
					mfError err = mfgOGL4WriteType(var->type, outputStream);
					if (err != MF_ERROR_OKAY)
						return err;
					if (mfsPrintFormat(outputStream, u8" in_%d;\n\n", var->id) != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				}
				else return MFG_ERROR_INVALID_ARGUMENTS;
			}

			var = var->next;
		}
	}

	// Add output variables
	{
		mfgMetaDataOutputVariable* var = metaData->firstOutputVar;
		while (var != NULL)
		{
			if (metaData->shaderType == MFG_VERTEX_SHADER)
			{
				if (!strcmp(var->name, u8"_position"))
				{
					if (mfsPrintFormat(outputStream, u8"out gl_PerVertex\n{\n\tvec4 gl_Position;\n};\n\n") != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				}
				else if (!strcmp(var->name, u8"_out0"))
				{
					if (mfsPutString(outputStream, u8"layout (location = 0) out ") != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
					mfError err = mfgOGL4WriteType(var->type, outputStream);
					if (err != MF_ERROR_OKAY)
						return err;
					if (mfsPrintFormat(outputStream, u8" out_0;\n\n", var->id) != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				}
				else if (!strcmp(var->name, u8"_out1"))
				{
					if (mfsPutString(outputStream, u8"layout (location = 1) out ") != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
					mfError err = mfgOGL4WriteType(var->type, outputStream);
					if (err != MF_ERROR_OKAY)
						return err;
					if (mfsPrintFormat(outputStream, u8" out_1;\n\n", var->id) != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				}
				else if (!strcmp(var->name, u8"_out2"))
				{
					if (mfsPutString(outputStream, u8"layout (location = 2) out ") != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
					mfError err = mfgOGL4WriteType(var->type, outputStream);
					if (err != MF_ERROR_OKAY)
						return err;
					if (mfsPrintFormat(outputStream, u8" out_2;\n\n", var->id) != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				}
				else if (!strcmp(var->name, u8"_out3"))
				{
					if (mfsPutString(outputStream, u8"layout (location = 3) out ") != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
					mfError err = mfgOGL4WriteType(var->type, outputStream);
					if (err != MF_ERROR_OKAY)
						return err;
					if (mfsPrintFormat(outputStream, u8" out_3;\n\n", var->id) != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				}
				else if (!strcmp(var->name, u8"_out4"))
				{
					if (mfsPutString(outputStream, u8"layout (location = 4) out ") != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
					mfError err = mfgOGL4WriteType(var->type, outputStream);
					if (err != MF_ERROR_OKAY)
						return err;
					if (mfsPrintFormat(outputStream, u8" out_4;\n\n", var->id) != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				}
				else if (!strcmp(var->name, u8"_out5"))
				{
					if (mfsPutString(outputStream, u8"layout (location = 5) out ") != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
					mfError err = mfgOGL4WriteType(var->type, outputStream);
					if (err != MF_ERROR_OKAY)
						return err;
					if (mfsPrintFormat(outputStream, u8" out_5;\n\n", var->id) != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				}
				else if (!strcmp(var->name, u8"_out6"))
				{
					if (mfsPutString(outputStream, u8"layout (location = 6) out ") != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
					mfError err = mfgOGL4WriteType(var->type, outputStream);
					if (err != MF_ERROR_OKAY)
						return err;
					if (mfsPrintFormat(outputStream, u8" out_6;\n\n", var->id) != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				}
				else if (!strcmp(var->name, u8"_out7"))
				{
					if (mfsPutString(outputStream, u8"layout (location = 7) out ") != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
					mfError err = mfgOGL4WriteType(var->type, outputStream);
					if (err != MF_ERROR_OKAY)
						return err;
					if (mfsPrintFormat(outputStream, u8" out_7;\n\n", var->id) != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				}
				else return MFG_ERROR_INVALID_ARGUMENTS;
			}
			else if (metaData->shaderType == MFG_PIXEL_SHADER)
			{
				if (!strcmp(var->name, u8"_depth"))
				{
					if (mfsPutString(outputStream, u8"out float gl_FragDepth;\n\n") != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				}
				else if (!strcmp(var->name, u8"_target0"))
				{
					if (mfsPutString(outputStream, u8"layout (location = 0) out ") != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
					mfError err = mfgOGL4WriteType(var->type, outputStream);
					if (err != MF_ERROR_OKAY)
						return err;
					if (mfsPrintFormat(outputStream, u8" target_0;\n\n", var->id) != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				}
				else if (!strcmp(var->name, u8"_target1"))
				{
					if (mfsPutString(outputStream, u8"layout (location = 1) out ") != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
					mfError err = mfgOGL4WriteType(var->type, outputStream);
					if (err != MF_ERROR_OKAY)
						return err;
					if (mfsPrintFormat(outputStream, u8" target_1;\n\n", var->id) != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				}
				else if (!strcmp(var->name, u8"_target2"))
				{
					if (mfsPutString(outputStream, u8"layout (location = 2) out ") != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
					mfError err = mfgOGL4WriteType(var->type, outputStream);
					if (err != MF_ERROR_OKAY)
						return err;
					if (mfsPrintFormat(outputStream, u8" target_2;\n\n", var->id) != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				}
				else if (!strcmp(var->name, u8"_target3"))
				{
					if (mfsPutString(outputStream, u8"layout (location = 3) out ") != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
					mfError err = mfgOGL4WriteType(var->type, outputStream);
					if (err != MF_ERROR_OKAY)
						return err;
					if (mfsPrintFormat(outputStream, u8" target_3;\n\n", var->id) != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				}
				else if (!strcmp(var->name, u8"_target4"))
				{
					if (mfsPutString(outputStream, u8"layout (location = 4) out ") != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
					mfError err = mfgOGL4WriteType(var->type, outputStream);
					if (err != MF_ERROR_OKAY)
						return err;
					if (mfsPrintFormat(outputStream, u8" target_4;\n\n", var->id) != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				}
				else if (!strcmp(var->name, u8"_target5"))
				{
					if (mfsPutString(outputStream, u8"layout (location = 5) out ") != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
					mfError err = mfgOGL4WriteType(var->type, outputStream);
					if (err != MF_ERROR_OKAY)
						return err;
					if (mfsPrintFormat(outputStream, u8" target_5;\n\n", var->id) != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				}
				else if (!strcmp(var->name, u8"_target6"))
				{
					if (mfsPutString(outputStream, u8"layout (location = 6) out ") != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
					mfError err = mfgOGL4WriteType(var->type, outputStream);
					if (err != MF_ERROR_OKAY)
						return err;
					if (mfsPrintFormat(outputStream, u8" target_6;\n\n", var->id) != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				}
				else if (!strcmp(var->name, u8"_target7"))
				{
					if (mfsPutString(outputStream, u8"layout (location = 7) out ") != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
					mfError err = mfgOGL4WriteType(var->type, outputStream);
					if (err != MF_ERROR_OKAY)
						return err;
					if (mfsPrintFormat(outputStream, u8" target_7;\n\n", var->id) != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				}
				else return MFG_ERROR_INVALID_ARGUMENTS;
			}

			var = var->next;
		}
	}

	// Write entry function
	{
		const mfsUTF8CodeUnit str[] =
			u8"void main()\n"
			u8"{\n";
		mfError err = mfsWrite(outputStream, str, sizeof(str) - 1, NULL);
		if (err != MF_ERROR_OKAY)
			return MFG_ERROR_FAILED_TO_WRITE;
	}

	// Convert bytecode to HLSL
	const mfmU8* it = bytecode + 6;
	mfmU64 tabs = 1;
	while (it < bytecode + bytecodeSize)
	{
		switch (*it)
		{
			case MFG_BYTECODE_DECLB1:
			{
				for (mfmU64 i = 0; i < tabs; ++i)
					if (mfsPutByte(outputStream, '\t') != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				mfmU16 id = 0;
				mfmFromBigEndian2(it + 1, &id);
				if (mfsPrintFormat(outputStream, u8"bool local_%d;\n", id) != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				it += 3;
			} break;

			case MFG_BYTECODE_DECLI1:
			{
				for (mfmU64 i = 0; i < tabs; ++i)
					if (mfsPutByte(outputStream, '\t') != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				mfmU16 id = 0;
				mfmFromBigEndian2(it + 1, &id);
				if (mfsPrintFormat(outputStream, u8"int local_%d;\n", id) != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				it += 3;
			} break;
			case MFG_BYTECODE_DECLI2:
			{
				for (mfmU64 i = 0; i < tabs; ++i)
					if (mfsPutByte(outputStream, '\t') != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				mfmU16 id = 0;
				mfmFromBigEndian2(it + 1, &id);
				if (mfsPrintFormat(outputStream, u8"ivec2 local_%d;\n", id) != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				it += 3;
			} break;
			case MFG_BYTECODE_DECLI3:
			{
				for (mfmU64 i = 0; i < tabs; ++i)
					if (mfsPutByte(outputStream, '\t') != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				mfmU16 id = 0;
				mfmFromBigEndian2(it + 1, &id);
				if (mfsPrintFormat(outputStream, u8"ivec3 local_%d;\n", id) != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				it += 3;
			} break;
			case MFG_BYTECODE_DECLI4:
			{
				for (mfmU64 i = 0; i < tabs; ++i)
					if (mfsPutByte(outputStream, '\t') != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				mfmU16 id = 0;
				mfmFromBigEndian2(it + 1, &id);
				if (mfsPrintFormat(outputStream, u8"ivec4 local_%d;\n", id) != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				it += 3;
			} break;
			case MFG_BYTECODE_DECLI22:
			{
				for (mfmU64 i = 0; i < tabs; ++i)
					if (mfsPutByte(outputStream, '\t') != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				mfmU16 id = 0;
				mfmFromBigEndian2(it + 1, &id);
				if (mfsPrintFormat(outputStream, u8"imat2 local_%d;\n", id) != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				it += 3;
			} break;
			case MFG_BYTECODE_DECLI33:
			{
				for (mfmU64 i = 0; i < tabs; ++i)
					if (mfsPutByte(outputStream, '\t') != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				mfmU16 id = 0;
				mfmFromBigEndian2(it + 1, &id);
				if (mfsPrintFormat(outputStream, u8"imat3 local_%d;\n", id) != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				it += 3;
			} break;
			case MFG_BYTECODE_DECLI44:
			{
				for (mfmU64 i = 0; i < tabs; ++i)
					if (mfsPutByte(outputStream, '\t') != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				mfmU16 id = 0;
				mfmFromBigEndian2(it + 1, &id);
				if (mfsPrintFormat(outputStream, u8"imat4 local_%d;\n", id) != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				it += 3;
			} break;

			case MFG_BYTECODE_DECLI1A:
			{
				for (mfmU64 i = 0; i < tabs; ++i)
					if (mfsPutByte(outputStream, '\t') != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				mfmU16 id = 0;
				mfmFromBigEndian2(it + 1, &id);
				mfmU16 count = 0;
				mfmFromBigEndian2(it + 3, &count);
				if (mfsPrintFormat(outputStream, u8"int local_%d[%d];\n", id++, count) != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				it += 5;
			} break;
			case MFG_BYTECODE_DECLI2A:
			{
				for (mfmU64 i = 0; i < tabs; ++i)
					if (mfsPutByte(outputStream, '\t') != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				mfmU16 id = 0;
				mfmFromBigEndian2(it + 1, &id);
				mfmU16 count = 0;
				mfmFromBigEndian2(it + 3, &count);
				if (mfsPrintFormat(outputStream, u8"ivec2 local_%d[%d];\n", id++, count) != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				it += 5;
			} break;
			case MFG_BYTECODE_DECLI3A:
			{
				for (mfmU64 i = 0; i < tabs; ++i)
					if (mfsPutByte(outputStream, '\t') != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				mfmU16 id = 0;
				mfmFromBigEndian2(it + 1, &id);
				mfmU16 count = 0;
				mfmFromBigEndian2(it + 3, &count);
				if (mfsPrintFormat(outputStream, u8"ivec3 local_%d[%d];\n", id++, count) != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				it += 5;
			} break;
			case MFG_BYTECODE_DECLI4A:
			{
				for (mfmU64 i = 0; i < tabs; ++i)
					if (mfsPutByte(outputStream, '\t') != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				mfmU16 id = 0;
				mfmFromBigEndian2(it + 1, &id);
				mfmU16 count = 0;
				mfmFromBigEndian2(it + 3, &count);
				if (mfsPrintFormat(outputStream, u8"ivec4 local_%d[%d];\n", id++, count) != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				it += 5;
			} break;
			case MFG_BYTECODE_DECLI22A:
			{
				for (mfmU64 i = 0; i < tabs; ++i)
					if (mfsPutByte(outputStream, '\t') != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				mfmU16 id = 0;
				mfmFromBigEndian2(it + 1, &id);
				mfmU16 count = 0;
				mfmFromBigEndian2(it + 3, &count);
				if (mfsPrintFormat(outputStream, u8"imat2 local_%d[%d];\n", id++, count) != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				it += 5;
			} break;
			case MFG_BYTECODE_DECLI33A:
			{
				for (mfmU64 i = 0; i < tabs; ++i)
					if (mfsPutByte(outputStream, '\t') != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				mfmU16 id = 0;
				mfmFromBigEndian2(it + 1, &id);
				mfmU16 count = 0;
				mfmFromBigEndian2(it + 3, &count);
				if (mfsPrintFormat(outputStream, u8"imat3 local_%d[%d];\n", id++, count) != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				it += 5;
			} break;
			case MFG_BYTECODE_DECLI44A:
			{
				for (mfmU64 i = 0; i < tabs; ++i)
					if (mfsPutByte(outputStream, '\t') != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				mfmU16 id = 0;
				mfmFromBigEndian2(it + 1, &id);
				mfmU16 count = 0;
				mfmFromBigEndian2(it + 3, &count);
				if (mfsPrintFormat(outputStream, u8"imat4 local_%d[%d];\n", id++, count) != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				it += 5;
			} break;

			case MFG_BYTECODE_DECLF1:
			{
				for (mfmU64 i = 0; i < tabs; ++i)
					if (mfsPutByte(outputStream, '\t') != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				mfmU16 id = 0;
				mfmFromBigEndian2(it + 1, &id);
				if (mfsPrintFormat(outputStream, u8"float local_%d;\n", id) != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				it += 3;
			} break;
			case MFG_BYTECODE_DECLF2:
			{
				for (mfmU64 i = 0; i < tabs; ++i)
					if (mfsPutByte(outputStream, '\t') != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				mfmU16 id = 0;
				mfmFromBigEndian2(it + 1, &id);
				if (mfsPrintFormat(outputStream, u8"vec2 local_%d;\n", id) != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				it += 3;
			} break;
			case MFG_BYTECODE_DECLF3:
			{
				for (mfmU64 i = 0; i < tabs; ++i)
					if (mfsPutByte(outputStream, '\t') != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				mfmU16 id = 0;
				mfmFromBigEndian2(it + 1, &id);
				if (mfsPrintFormat(outputStream, u8"vec3 local_%d;\n", id) != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				it += 3;
			} break;
			case MFG_BYTECODE_DECLF4:
			{
				for (mfmU64 i = 0; i < tabs; ++i)
					if (mfsPutByte(outputStream, '\t') != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				mfmU16 id = 0;
				mfmFromBigEndian2(it + 1, &id);
				if (mfsPrintFormat(outputStream, u8"vec4 local_%d;\n", id) != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				it += 3;
			} break;
			case MFG_BYTECODE_DECLF22:
			{
				for (mfmU64 i = 0; i < tabs; ++i)
					if (mfsPutByte(outputStream, '\t') != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				mfmU16 id = 0;
				mfmFromBigEndian2(it + 1, &id);
				if (mfsPrintFormat(outputStream, u8"mat2 local_%d;\n", id) != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				it += 3;
			} break;
			case MFG_BYTECODE_DECLF33:
			{
				for (mfmU64 i = 0; i < tabs; ++i)
					if (mfsPutByte(outputStream, '\t') != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				mfmU16 id = 0;
				mfmFromBigEndian2(it + 1, &id);
				if (mfsPrintFormat(outputStream, u8"mat3 local_%d;\n", id) != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				it += 3;
			} break;
			case MFG_BYTECODE_DECLF44:
			{
				for (mfmU64 i = 0; i < tabs; ++i)
					if (mfsPutByte(outputStream, '\t') != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				mfmU16 id = 0;
				mfmFromBigEndian2(it + 1, &id);
				if (mfsPrintFormat(outputStream, u8"mat4 local_%d;\n", id) != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				it += 3;
			} break;

			case MFG_BYTECODE_DECLF1A:
			{
				for (mfmU64 i = 0; i < tabs; ++i)
					if (mfsPutByte(outputStream, '\t') != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				mfmU16 id = 0;
				mfmFromBigEndian2(it + 1, &id);
				mfmU16 count = 0;
				mfmFromBigEndian2(it + 3, &count);
				if (mfsPrintFormat(outputStream, u8"float local_%d[%d];\n", id++, count) != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				it += 5;
			} break;
			case MFG_BYTECODE_DECLF2A:
			{
				for (mfmU64 i = 0; i < tabs; ++i)
					if (mfsPutByte(outputStream, '\t') != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				mfmU16 id = 0;
				mfmFromBigEndian2(it + 1, &id);
				mfmU16 count = 0;
				mfmFromBigEndian2(it + 3, &count);
				if (mfsPrintFormat(outputStream, u8"vec2 local_%d[%d];\n", id++, count) != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				it += 5;
			} break;
			case MFG_BYTECODE_DECLF3A:
			{
				for (mfmU64 i = 0; i < tabs; ++i)
					if (mfsPutByte(outputStream, '\t') != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				mfmU16 id = 0;
				mfmFromBigEndian2(it + 1, &id);
				mfmU16 count = 0;
				mfmFromBigEndian2(it + 3, &count);
				if (mfsPrintFormat(outputStream, u8"vec3 local_%d[%d];\n", id++, count) != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				it += 5;
			} break;
			case MFG_BYTECODE_DECLF4A:
			{
				for (mfmU64 i = 0; i < tabs; ++i)
					if (mfsPutByte(outputStream, '\t') != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				mfmU16 id = 0;
				mfmFromBigEndian2(it + 1, &id);
				mfmU16 count = 0;
				mfmFromBigEndian2(it + 3, &count);
				if (mfsPrintFormat(outputStream, u8"vec4 local_%d[%d];\n", id++, count) != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				it += 5;
			} break;
			case MFG_BYTECODE_DECLF22A:
			{
				for (mfmU64 i = 0; i < tabs; ++i)
					if (mfsPutByte(outputStream, '\t') != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				mfmU16 id = 0;
				mfmFromBigEndian2(it + 1, &id);
				mfmU16 count = 0;
				mfmFromBigEndian2(it + 3, &count);
				if (mfsPrintFormat(outputStream, u8"mat2 local_%d[%d];\n", id++, count) != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				it += 5;
			} break;
			case MFG_BYTECODE_DECLF33A:
			{
				for (mfmU64 i = 0; i < tabs; ++i)
					if (mfsPutByte(outputStream, '\t') != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				mfmU16 id = 0;
				mfmFromBigEndian2(it + 1, &id);
				mfmU16 count = 0;
				mfmFromBigEndian2(it + 3, &count);
				if (mfsPrintFormat(outputStream, u8"mat3 local_%d[%d];\n", id++, count) != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				it += 5;
			} break;
			case MFG_BYTECODE_DECLF44A:
			{
				for (mfmU64 i = 0; i < tabs; ++i)
					if (mfsPutByte(outputStream, '\t') != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				mfmU16 id = 0;
				mfmFromBigEndian2(it + 1, &id);
				mfmU16 count = 0;
				mfmFromBigEndian2(it + 3, &count);
				if (mfsPrintFormat(outputStream, u8"mat4 local_%d[%d];\n", id++, count) != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				it += 5;
			} break;

			case MFG_BYTECODE_ASSIGN:
			{
				for (mfmU64 i = 0; i < tabs; ++i)
					if (mfsPutByte(outputStream, '\t') != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				mfmU16 id1 = 0;
				mfmFromBigEndian2(it + 1, &id1);
				mfmU16 id2 = 0;
				mfmFromBigEndian2(it + 3, &id2);
				mfError err = mfgOGL4PutID(id1, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8" = ") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				err = mfgOGL4PutID(id2, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8";\n") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				it += 5;
				break;
			}

			case MFG_BYTECODE_ADD:
			{
				for (mfmU64 i = 0; i < tabs; ++i)
					if (mfsPutByte(outputStream, '\t') != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				mfmU16 id1 = 0;
				mfmFromBigEndian2(it + 1, &id1);
				mfmU16 id2 = 0;
				mfmFromBigEndian2(it + 3, &id2);
				mfmU16 id3 = 0;
				mfmFromBigEndian2(it + 5, &id3);
				mfError err = mfgOGL4PutID(id3, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8" = ") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				err = mfgOGL4PutID(id1, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8" + ") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				err = mfgOGL4PutID(id2, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8";\n") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				it += 7;
				break;
			}

			case MFG_BYTECODE_SUBTRACT:
			{
				for (mfmU64 i = 0; i < tabs; ++i)
					if (mfsPutByte(outputStream, '\t') != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				mfmU16 id1 = 0;
				mfmFromBigEndian2(it + 1, &id1);
				mfmU16 id2 = 0;
				mfmFromBigEndian2(it + 3, &id2);
				mfmU16 id3 = 0;
				mfmFromBigEndian2(it + 5, &id3);
				mfError err = mfgOGL4PutID(id3, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8" = ") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				err = mfgOGL4PutID(id1, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8" - ") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				err = mfgOGL4PutID(id2, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8";\n") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				it += 7;
				break;
			}

			case MFG_BYTECODE_MULTIPLY:
			{
				for (mfmU64 i = 0; i < tabs; ++i)
					if (mfsPutByte(outputStream, '\t') != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				mfmU16 id1 = 0;
				mfmFromBigEndian2(it + 1, &id1);
				mfmU16 id2 = 0;
				mfmFromBigEndian2(it + 3, &id2);
				mfmU16 id3 = 0;
				mfmFromBigEndian2(it + 5, &id3);
				mfError err = mfgOGL4PutID(id3, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8" = ") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				err = mfgOGL4PutID(id1, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8" * ") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				err = mfgOGL4PutID(id2, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8";\n") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				it += 7;
				break;
			}

			case MFG_BYTECODE_DIVIDE:
			{
				for (mfmU64 i = 0; i < tabs; ++i)
					if (mfsPutByte(outputStream, '\t') != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				mfmU16 id1 = 0;
				mfmFromBigEndian2(it + 1, &id1);
				mfmU16 id2 = 0;
				mfmFromBigEndian2(it + 3, &id2);
				mfmU16 id3 = 0;
				mfmFromBigEndian2(it + 5, &id3);
				mfError err = mfgOGL4PutID(id3, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8" = ") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				err = mfgOGL4PutID(id1, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8" / ") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				err = mfgOGL4PutID(id2, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8";\n") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				it += 7;
				break;
			}

			case MFG_BYTECODE_AND:
			{
				for (mfmU64 i = 0; i < tabs; ++i)
					if (mfsPutByte(outputStream, '\t') != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				mfmU16 id1 = 0;
				mfmFromBigEndian2(it + 1, &id1);
				mfmU16 id2 = 0;
				mfmFromBigEndian2(it + 3, &id2);
				mfmU16 id3 = 0;
				mfmFromBigEndian2(it + 5, &id3);
				mfError err = mfgOGL4PutID(id3, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8" = ") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				err = mfgOGL4PutID(id1, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8" && ") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				err = mfgOGL4PutID(id2, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8";\n") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				it += 7;
				break;
			}

			case MFG_BYTECODE_OR:
			{
				for (mfmU64 i = 0; i < tabs; ++i)
					if (mfsPutByte(outputStream, '\t') != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				mfmU16 id1 = 0;
				mfmFromBigEndian2(it + 1, &id1);
				mfmU16 id2 = 0;
				mfmFromBigEndian2(it + 3, &id2);
				mfmU16 id3 = 0;
				mfmFromBigEndian2(it + 5, &id3);
				mfError err = mfgOGL4PutID(id3, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8" = ") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				err = mfgOGL4PutID(id1, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8" || ") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				err = mfgOGL4PutID(id2, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8";\n") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				it += 7;
				break;
			}

			case MFG_BYTECODE_NOT:
			{
				for (mfmU64 i = 0; i < tabs; ++i)
					if (mfsPutByte(outputStream, '\t') != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				mfmU16 id1 = 0;
				mfmFromBigEndian2(it + 1, &id1);
				mfmU16 id2 = 0;
				mfmFromBigEndian2(it + 3, &id2);
				mfError err = mfgOGL4PutID(id1, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8" = !") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				err = mfgOGL4PutID(id2, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8";\n") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				it += 5;
				break;
			}

			case MFG_BYTECODE_NEGATE:
			{
				for (mfmU64 i = 0; i < tabs; ++i)
					if (mfsPutByte(outputStream, '\t') != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				mfmU16 id1 = 0;
				mfmFromBigEndian2(it + 1, &id1);
				mfmU16 id2 = 0;
				mfmFromBigEndian2(it + 3, &id2);
				mfError err = mfgOGL4PutID(id1, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8" = -") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				err = mfgOGL4PutID(id2, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8";\n") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				it += 5;
				break;
			}

			case MFG_BYTECODE_GREATER:
			{
				for (mfmU64 i = 0; i < tabs; ++i)
					if (mfsPutByte(outputStream, '\t') != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				mfmU16 id1 = 0;
				mfmFromBigEndian2(it + 1, &id1);
				mfmU16 id2 = 0;
				mfmFromBigEndian2(it + 3, &id2);
				mfmU16 id3 = 0;
				mfmFromBigEndian2(it + 5, &id3);
				mfError err = mfgOGL4PutID(id3, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8" = ") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				err = mfgOGL4PutID(id1, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8" > ") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				err = mfgOGL4PutID(id2, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8";\n") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				it += 7;
				break;
			}

			case MFG_BYTECODE_LESS:
			{
				for (mfmU64 i = 0; i < tabs; ++i)
					if (mfsPutByte(outputStream, '\t') != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				mfmU16 id1 = 0;
				mfmFromBigEndian2(it + 1, &id1);
				mfmU16 id2 = 0;
				mfmFromBigEndian2(it + 3, &id2);
				mfmU16 id3 = 0;
				mfmFromBigEndian2(it + 5, &id3);
				mfError err = mfgOGL4PutID(id3, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8" = ") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				err = mfgOGL4PutID(id1, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8" < ") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				err = mfgOGL4PutID(id2, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8";\n") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				it += 7;
				break;
			}

			case MFG_BYTECODE_GEQUAL:
			{
				for (mfmU64 i = 0; i < tabs; ++i)
					if (mfsPutByte(outputStream, '\t') != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				mfmU16 id1 = 0;
				mfmFromBigEndian2(it + 1, &id1);
				mfmU16 id2 = 0;
				mfmFromBigEndian2(it + 3, &id2);
				mfmU16 id3 = 0;
				mfmFromBigEndian2(it + 5, &id3);
				mfError err = mfgOGL4PutID(id3, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8" = ") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				err = mfgOGL4PutID(id1, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8" >= ") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				err = mfgOGL4PutID(id2, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8";\n") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				it += 7;
				break;
			}

			case MFG_BYTECODE_LEQUAL:
			{
				for (mfmU64 i = 0; i < tabs; ++i)
					if (mfsPutByte(outputStream, '\t') != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				mfmU16 id1 = 0;
				mfmFromBigEndian2(it + 1, &id1);
				mfmU16 id2 = 0;
				mfmFromBigEndian2(it + 3, &id2);
				mfmU16 id3 = 0;
				mfmFromBigEndian2(it + 5, &id3);
				mfError err = mfgOGL4PutID(id3, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8" = ") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				err = mfgOGL4PutID(id1, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8" <= ") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				err = mfgOGL4PutID(id2, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8";\n") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				it += 7;
				break;
			}

			case MFG_BYTECODE_EQUAL:
			{
				for (mfmU64 i = 0; i < tabs; ++i)
					if (mfsPutByte(outputStream, '\t') != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				mfmU16 id1 = 0;
				mfmFromBigEndian2(it + 1, &id1);
				mfmU16 id2 = 0;
				mfmFromBigEndian2(it + 3, &id2);
				mfmU16 id3 = 0;
				mfmFromBigEndian2(it + 5, &id3);
				mfError err = mfgOGL4PutID(id3, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8" = ") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				err = mfgOGL4PutID(id1, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8" == ") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				err = mfgOGL4PutID(id2, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8";\n") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				it += 7;
				break;
			}

			case MFG_BYTECODE_DIFFERENT:
			{
				for (mfmU64 i = 0; i < tabs; ++i)
					if (mfsPutByte(outputStream, '\t') != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				mfmU16 id1 = 0;
				mfmFromBigEndian2(it + 1, &id1);
				mfmU16 id2 = 0;
				mfmFromBigEndian2(it + 3, &id2);
				mfmU16 id3 = 0;
				mfmFromBigEndian2(it + 5, &id3);
				mfError err = mfgOGL4PutID(id3, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8" = ") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				err = mfgOGL4PutID(id1, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8" != ") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				err = mfgOGL4PutID(id2, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8";\n") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				it += 7;
				break;
			}

			case MFG_BYTECODE_LITB1TRUE:
			{
				for (mfmU64 i = 0; i < tabs; ++i)
					if (mfsPutByte(outputStream, '\t') != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				mfmU16 id1 = 0;
				mfmFromBigEndian2(it + 1, &id1);
				mfError err = mfgOGL4PutID(id1, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8" = true;\n") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				it += 3;
				break;
			}

			case MFG_BYTECODE_LITB1FALSE:
			{
				for (mfmU64 i = 0; i < tabs; ++i)
					if (mfsPutByte(outputStream, '\t') != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				mfmU16 id1 = 0;
				mfmFromBigEndian2(it + 1, &id1);
				mfError err = mfgOGL4PutID(id1, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8" = false;\n") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				it += 3;
				break;
			}

			case MFG_BYTECODE_LITI1:
			{
				for (mfmU64 i = 0; i < tabs; ++i)
					if (mfsPutByte(outputStream, '\t') != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				mfmU16 id1 = 0;
				mfmFromBigEndian2(it + 1, &id1);
				mfmI32 value = 0;
				mfmFromBigEndian4(it + 3, &value);
				mfError err = mfgOGL4PutID(id1, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8" = %d;\n", value) != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				it += 3 + 4;
				break;
			}

			case MFG_BYTECODE_LITI2:
			{
				for (mfmU64 i = 0; i < tabs; ++i)
					if (mfsPutByte(outputStream, '\t') != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				mfmU16 id1 = 0;
				mfmFromBigEndian2(it + 1, &id1);
				mfmI32 values[2];
				mfmFromBigEndian4(it + 3, &values[0]);
				mfmFromBigEndian4(it + 3 + 4, &values[1]);
				mfError err = mfgOGL4PutID(id1, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8" = ivec2(%d, %d);\n", values[0], values[1]) != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				it += 3 + 8;
				break;
			}

			case MFG_BYTECODE_LITI3:
			{
				for (mfmU64 i = 0; i < tabs; ++i)
					if (mfsPutByte(outputStream, '\t') != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				mfmU16 id1 = 0;
				mfmFromBigEndian2(it + 1, &id1);
				mfmI32 values[3];
				mfmFromBigEndian4(it + 3, &values[0]);
				mfmFromBigEndian4(it + 3 + 4, &values[1]);
				mfmFromBigEndian4(it + 3 + 8, &values[2]);
				mfError err = mfgOGL4PutID(id1, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8" = ivec3(%d, %d, %d);\n", values[0], values[1], values[2]) != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				it += 3 + 12;
				break;
			}

			case MFG_BYTECODE_LITI4:
			{
				for (mfmU64 i = 0; i < tabs; ++i)
					if (mfsPutByte(outputStream, '\t') != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				mfmU16 id1 = 0;
				mfmFromBigEndian2(it + 1, &id1);
				mfmI32 values[4];
				mfmFromBigEndian4(it + 3, &values[0]);
				mfmFromBigEndian4(it + 3 + 4, &values[1]);
				mfmFromBigEndian4(it + 3 + 8, &values[2]);
				mfmFromBigEndian4(it + 3 + 12, &values[3]);
				mfError err = mfgOGL4PutID(id1, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8" = ivec4(%d, %d, %d, %d);\n", values[0], values[1], values[2], values[3]) != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				it += 3 + 16;
				break;
			}

			case MFG_BYTECODE_LITF1:
			{
				for (mfmU64 i = 0; i < tabs; ++i)
					if (mfsPutByte(outputStream, '\t') != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				mfmU16 id1 = 0;
				mfmFromBigEndian2(it + 1, &id1);
				mfmF32 value = 0;
				mfmFromBigEndian4(it + 3, &value);
				mfError err = mfgOGL4PutID(id1, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8" = %f;\n", value) != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				it += 3 + 4;
				break;
			}

			case MFG_BYTECODE_LITF2:
			{
				for (mfmU64 i = 0; i < tabs; ++i)
					if (mfsPutByte(outputStream, '\t') != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				mfmU16 id1 = 0;
				mfmFromBigEndian2(it + 1, &id1);
				mfmF32 values[2];
				mfmFromBigEndian4(it + 3, &values[0]);
				mfmFromBigEndian4(it + 3 + 4, &values[1]);
				mfError err = mfgOGL4PutID(id1, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8" = vec2(%f, %f);\n", values[0], values[1]) != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				it += 3 + 8;
				break;
			}

			case MFG_BYTECODE_LITF3:
			{
				for (mfmU64 i = 0; i < tabs; ++i)
					if (mfsPutByte(outputStream, '\t') != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				mfmU16 id1 = 0;
				mfmFromBigEndian2(it + 1, &id1);
				mfmF32 values[3];
				mfmFromBigEndian4(it + 3, &values[0]);
				mfmFromBigEndian4(it + 3 + 4, &values[1]);
				mfmFromBigEndian4(it + 3 + 8, &values[2]);
				mfError err = mfgOGL4PutID(id1, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8" = vec3(%f, %f, %f);\n", values[0], values[1], values[2]) != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				it += 3 + 12;
				break;
			}

			case MFG_BYTECODE_LITF4:
			{
				for (mfmU64 i = 0; i < tabs; ++i)
					if (mfsPutByte(outputStream, '\t') != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				mfmU16 id1 = 0;
				mfmFromBigEndian2(it + 1, &id1);
				mfmF32 values[4];
				mfmFromBigEndian4(it + 3, &values[0]);
				mfmFromBigEndian4(it + 3 + 4, &values[1]);
				mfmFromBigEndian4(it + 3 + 8, &values[2]);
				mfmFromBigEndian4(it + 3 + 12, &values[3]);
				mfError err = mfgOGL4PutID(id1, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8" = vec4(%f, %f, %f, %f);\n", values[0], values[1], values[2], values[3]) != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				it += 3 + 16;
				break;
			}
			case MFG_BYTECODE_GET2CMP:
			{
				mfmU16 id1 = 0;
				mfmFromBigEndian2(it + 1, &id1);

				mfmU16 id2 = 0;
				mfmFromBigEndian2(it + 3, &id2);

				mfmU8 index = it[5];

				for (mfmU8 i = 0; i < 128; ++i)
					if (assemblerData.references[i].active == MFM_FALSE || assemblerData.references[i].id == id1)
					{
						assemblerData.references[i].varIndex = id2;
						assemblerData.references[i].rows = 2;
						assemblerData.references[i].cols = 1;
						assemblerData.references[i].index = index;
						assemblerData.references[i].active = MFM_TRUE;
						assemblerData.references[i].id = id1;
						break;
					}

				it += 6;
				break;
			}

			case MFG_BYTECODE_GET3CMP:
			{
				mfmU16 id1 = 0;
				mfmFromBigEndian2(it + 1, &id1);

				mfmU16 id2 = 0;
				mfmFromBigEndian2(it + 3, &id2);

				mfmU8 index = it[5];

				for (mfmU8 i = 0; i < 128; ++i)
					if (assemblerData.references[i].active == MFM_FALSE || assemblerData.references[i].id == id1)
					{
						assemblerData.references[i].varIndex = id2;
						assemblerData.references[i].rows = 3;
						assemblerData.references[i].cols = 1;
						assemblerData.references[i].index = index;
						assemblerData.references[i].active = MFM_TRUE;
						assemblerData.references[i].id = id1;
						break;
					}

				it += 6;
				break;
			}

			case MFG_BYTECODE_GET4CMP:
			{
				mfmU16 id1 = 0;
				mfmFromBigEndian2(it + 1, &id1);

				mfmU16 id2 = 0;
				mfmFromBigEndian2(it + 3, &id2);

				mfmU8 index = it[5];

				for (mfmU8 i = 0; i < 128; ++i)
					if (assemblerData.references[i].active == MFM_FALSE || assemblerData.references[i].id == id1)
					{
						assemblerData.references[i].varIndex = id2;
						assemblerData.references[i].rows = 4;
						assemblerData.references[i].cols = 1;
						assemblerData.references[i].index = index;
						assemblerData.references[i].active = MFM_TRUE;
						assemblerData.references[i].id = id1;
						break;
					}

				it += 6;
				break;
			}

			case MFG_BYTECODE_GET22CMP:
			{
				mfmU16 id1 = 0;
				mfmFromBigEndian2(it + 1, &id1);

				mfmU16 id2 = 0;
				mfmFromBigEndian2(it + 3, &id2);

				mfmU8 index = it[5];

				for (mfmU8 i = 0; i < 128; ++i)
					if (assemblerData.references[i].active == MFM_FALSE || assemblerData.references[i].id == id1)
					{
						assemblerData.references[i].varIndex = id2;
						assemblerData.references[i].rows = 2;
						assemblerData.references[i].cols = 2;
						assemblerData.references[i].index = index;
						assemblerData.references[i].active = MFM_TRUE;
						assemblerData.references[i].id = id1;
						break;
					}

				it += 6;
				break;
			}

			case MFG_BYTECODE_GET33CMP:
			{
				mfmU16 id1 = 0;
				mfmFromBigEndian2(it + 1, &id1);

				mfmU16 id2 = 0;
				mfmFromBigEndian2(it + 3, &id2);

				mfmU8 index = it[5];

				for (mfmU8 i = 0; i < 128; ++i)
					if (assemblerData.references[i].active == MFM_FALSE || assemblerData.references[i].id == id1)
					{
						assemblerData.references[i].varIndex = id2;
						assemblerData.references[i].rows = 3;
						assemblerData.references[i].cols = 3;
						assemblerData.references[i].index = index;
						assemblerData.references[i].active = MFM_TRUE;
						assemblerData.references[i].id = id1;
						break;
					}

				it += 6;
				break;
			}

			case MFG_BYTECODE_GET44CMP:
			{
				mfmU16 id1 = 0;
				mfmFromBigEndian2(it + 1, &id1);

				mfmU16 id2 = 0;
				mfmFromBigEndian2(it + 3, &id2);

				mfmU8 index = it[5];

				for (mfmU8 i = 0; i < 128; ++i)
					if (assemblerData.references[i].active == MFM_FALSE || assemblerData.references[i].id == id1)
					{
						assemblerData.references[i].varIndex = id2;
						assemblerData.references[i].rows = 4;
						assemblerData.references[i].cols = 4;
						assemblerData.references[i].index = index;
						assemblerData.references[i].active = MFM_TRUE;
						assemblerData.references[i].id = id1;
						break;
					}

				it += 6;
				break;
			}

			case MFG_BYTECODE_GETACMP:
			{
				mfmU16 id1 = 0;
				mfmFromBigEndian2(it + 1, &id1);

				mfmU16 id2 = 0;
				mfmFromBigEndian2(it + 3, &id2);

				mfmU16 id3 = 0;
				mfmFromBigEndian2(it + 5, &id3);

				for (mfmU8 i = 0; i < 128; ++i)
					if (assemblerData.references[i].active == MFM_FALSE)
					{
						assemblerData.references[i].varIndex = id2;
						assemblerData.references[i].rows = 0;
						assemblerData.references[i].cols = 0;
						assemblerData.references[i].index = 0;
						assemblerData.references[i].active = MFM_TRUE;
						assemblerData.references[i].isArray = MFM_TRUE;
						assemblerData.references[i].id = id1;
						assemblerData.references[i].accessID = id3;
						break;
					}

				it += 7;
				break;
			}

			case MFG_BYTECODE_OPSCOPE:
			{
				for (mfmU64 i = 0; i < tabs; ++i)
					if (mfsPutByte(outputStream, '\t') != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				if (mfsPrintFormat(outputStream, u8"{\n") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				++tabs;
				++it;
				break;
			}

			case MFG_BYTECODE_CLSCOPE:
			{
				--tabs;
				for (mfmU64 i = 0; i < tabs; ++i)
					if (mfsPutByte(outputStream, '\t') != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				if (mfsPrintFormat(outputStream, u8"}\n") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				++it;
				break;
			}

			case MFG_BYTECODE_DISCARD:
			{
				for (mfmU64 i = 0; i < tabs; ++i)
					if (mfsPutByte(outputStream, '\t') != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				if (mfsPrintFormat(outputStream, u8"discard;\n") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				++it;
				break;
			}

			case MFG_BYTECODE_RETURN:
			{
				for (mfmU64 i = 0; i < tabs; ++i)
					if (mfsPutByte(outputStream, '\t') != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				if (mfsPrintFormat(outputStream, u8"return;\n") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				++it;
				break;
			}

			case MFG_BYTECODE_WHILE:
			{
				mfmU16 id1 = 0;
				mfmFromBigEndian2(it + 1, &id1);

				for (mfmU64 i = 0; i < tabs; ++i)
					if (mfsPutByte(outputStream, '\t') != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				if (mfsPrintFormat(outputStream, u8"while (") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				mfError err = mfgOGL4PutID(id1, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8")\n") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				it += 3;
				break;
			}

			case MFG_BYTECODE_IF:
			{
				mfmU16 id1 = 0;
				mfmFromBigEndian2(it + 1, &id1);

				for (mfmU64 i = 0; i < tabs; ++i)
					if (mfsPutByte(outputStream, '\t') != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				if (mfsPrintFormat(outputStream, u8"if (") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				mfError err = mfgOGL4PutID(id1, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8")\n") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				it += 3;
				break;
			}

			case MFG_BYTECODE_ELSE:
			{
				for (mfmU64 i = 0; i < tabs; ++i)
					if (mfsPutByte(outputStream, '\t') != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				if (mfsPrintFormat(outputStream, u8"else\n") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				++it;
				break;
			}

			case MFG_BYTECODE_MULMAT:
			{
				for (mfmU64 i = 0; i < tabs; ++i)
					if (mfsPutByte(outputStream, '\t') != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				mfmU16 id1 = 0;
				mfmFromBigEndian2(it + 1, &id1);
				mfmU16 id2 = 0;
				mfmFromBigEndian2(it + 3, &id2);
				mfmU16 id3 = 0;
				mfmFromBigEndian2(it + 5, &id3);
				mfError err = mfgOGL4PutID(id3, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8" = ") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				err = mfgOGL4PutID(id1, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8" * ") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				err = mfgOGL4PutID(id2, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8";\n") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				it += 7;
				break;
			}

			case MFG_BYTECODE_SAMPLE1D:
			case MFG_BYTECODE_SAMPLE2D:
			case MFG_BYTECODE_SAMPLE3D:
			{
				for (mfmU64 i = 0; i < tabs; ++i)
					if (mfsPutByte(outputStream, '\t') != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				mfmU16 id1 = 0;
				mfmFromBigEndian2(it + 1, &id1);
				mfmU16 id2 = 0;
				mfmFromBigEndian2(it + 3, &id2);
				mfmU16 id3 = 0;
				mfmFromBigEndian2(it + 5, &id3);
				mfError err = mfgOGL4PutID(id3, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8" = texture(") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				err = mfgOGL4PutID(id1, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8" , ") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				err = mfgOGL4PutID(id2, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8");\n") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				it += 7;
				break;
			}

			case MFG_BYTECODE_COS:
			{
				for (mfmU64 i = 0; i < tabs; ++i)
					if (mfsPutByte(outputStream, '\t') != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				mfmU16 id1 = 0;
				mfmFromBigEndian2(it + 1, &id1);
				mfmU16 id2 = 0;
				mfmFromBigEndian2(it + 3, &id2);
				mfError err = mfgOGL4PutID(id1, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8" = cos(") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				err = mfgOGL4PutID(id2, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8");\n") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				it += 5;
				break;
			}

			case MFG_BYTECODE_SIN:
			{
				for (mfmU64 i = 0; i < tabs; ++i)
					if (mfsPutByte(outputStream, '\t') != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				mfmU16 id1 = 0;
				mfmFromBigEndian2(it + 1, &id1);
				mfmU16 id2 = 0;
				mfmFromBigEndian2(it + 3, &id2);
				mfError err = mfgOGL4PutID(id1, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8" = sin(") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				err = mfgOGL4PutID(id2, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8");\n") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				it += 5;
				break;
			}

			case MFG_BYTECODE_TAN:
			{
				for (mfmU64 i = 0; i < tabs; ++i)
					if (mfsPutByte(outputStream, '\t') != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				mfmU16 id1 = 0;
				mfmFromBigEndian2(it + 1, &id1);
				mfmU16 id2 = 0;
				mfmFromBigEndian2(it + 3, &id2);
				mfError err = mfgOGL4PutID(id1, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8" = tan(") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				err = mfgOGL4PutID(id2, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8");\n") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				it += 5;
				break;
			}

			case MFG_BYTECODE_ACOS:
			{
				for (mfmU64 i = 0; i < tabs; ++i)
					if (mfsPutByte(outputStream, '\t') != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				mfmU16 id1 = 0;
				mfmFromBigEndian2(it + 1, &id1);
				mfmU16 id2 = 0;
				mfmFromBigEndian2(it + 3, &id2);
				mfError err = mfgOGL4PutID(id1, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8" = acos(") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				err = mfgOGL4PutID(id2, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8");\n") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				it += 5;
				break;
			}

			case MFG_BYTECODE_ASIN:
			{
				for (mfmU64 i = 0; i < tabs; ++i)
					if (mfsPutByte(outputStream, '\t') != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				mfmU16 id1 = 0;
				mfmFromBigEndian2(it + 1, &id1);
				mfmU16 id2 = 0;
				mfmFromBigEndian2(it + 3, &id2);
				mfError err = mfgOGL4PutID(id1, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8" = asin(") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				err = mfgOGL4PutID(id2, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8");\n") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				it += 5;
				break;
			}

			case MFG_BYTECODE_ATAN:
			{
				for (mfmU64 i = 0; i < tabs; ++i)
					if (mfsPutByte(outputStream, '\t') != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				mfmU16 id1 = 0;
				mfmFromBigEndian2(it + 1, &id1);
				mfmU16 id2 = 0;
				mfmFromBigEndian2(it + 3, &id2);
				mfError err = mfgOGL4PutID(id1, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8" = atan(") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				err = mfgOGL4PutID(id2, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8");\n") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				it += 5;
				break;
			}

			case MFG_BYTECODE_DEGREES:
			{
				for (mfmU64 i = 0; i < tabs; ++i)
					if (mfsPutByte(outputStream, '\t') != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				mfmU16 id1 = 0;
				mfmFromBigEndian2(it + 1, &id1);
				mfmU16 id2 = 0;
				mfmFromBigEndian2(it + 3, &id2);
				mfError err = mfgOGL4PutID(id1, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8" = degrees(") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				err = mfgOGL4PutID(id2, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8");\n") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				it += 5;
				break;
			}

			case MFG_BYTECODE_RADIANS:
			{
				for (mfmU64 i = 0; i < tabs; ++i)
					if (mfsPutByte(outputStream, '\t') != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				mfmU16 id1 = 0;
				mfmFromBigEndian2(it + 1, &id1);
				mfmU16 id2 = 0;
				mfmFromBigEndian2(it + 3, &id2);
				mfError err = mfgOGL4PutID(id1, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8" = radians(") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				err = mfgOGL4PutID(id2, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8");\n") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				it += 5;
				break;
			}

			case MFG_BYTECODE_EXP:
			{
				for (mfmU64 i = 0; i < tabs; ++i)
					if (mfsPutByte(outputStream, '\t') != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				mfmU16 id1 = 0;
				mfmFromBigEndian2(it + 1, &id1);
				mfmU16 id2 = 0;
				mfmFromBigEndian2(it + 3, &id2);
				mfError err = mfgOGL4PutID(id1, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8" = exp(") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				err = mfgOGL4PutID(id2, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8");\n") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				it += 5;
				break;
			}

			case MFG_BYTECODE_LOG:
			{
				for (mfmU64 i = 0; i < tabs; ++i)
					if (mfsPutByte(outputStream, '\t') != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				mfmU16 id1 = 0;
				mfmFromBigEndian2(it + 1, &id1);
				mfmU16 id2 = 0;
				mfmFromBigEndian2(it + 3, &id2);
				mfError err = mfgOGL4PutID(id1, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8" = log(") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				err = mfgOGL4PutID(id2, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8");\n") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				it += 5;
				break;
			}

			case MFG_BYTECODE_EXP2:
			{
				for (mfmU64 i = 0; i < tabs; ++i)
					if (mfsPutByte(outputStream, '\t') != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				mfmU16 id1 = 0;
				mfmFromBigEndian2(it + 1, &id1);
				mfmU16 id2 = 0;
				mfmFromBigEndian2(it + 3, &id2);
				mfError err = mfgOGL4PutID(id1, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8" = exp2(") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				err = mfgOGL4PutID(id2, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8");\n") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				it += 5;
				break;
			}

			case MFG_BYTECODE_LOG2:
			{
				for (mfmU64 i = 0; i < tabs; ++i)
					if (mfsPutByte(outputStream, '\t') != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				mfmU16 id1 = 0;
				mfmFromBigEndian2(it + 1, &id1);
				mfmU16 id2 = 0;
				mfmFromBigEndian2(it + 3, &id2);
				mfError err = mfgOGL4PutID(id1, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8" = log2(") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				err = mfgOGL4PutID(id2, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8");\n") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				it += 5;
				break;
			}

			case MFG_BYTECODE_POW:
			{
				for (mfmU64 i = 0; i < tabs; ++i)
					if (mfsPutByte(outputStream, '\t') != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				mfmU16 id1 = 0;
				mfmFromBigEndian2(it + 1, &id1);
				mfmU16 id2 = 0;
				mfmFromBigEndian2(it + 3, &id2);
				mfmU16 id3 = 0;
				mfmFromBigEndian2(it + 5, &id3);
				mfError err = mfgOGL4PutID(id1, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8" = pow(") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				err = mfgOGL4PutID(id2, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8" , ") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				err = mfgOGL4PutID(id3, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8");\n") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				it += 7;
				break;
			}

			case MFG_BYTECODE_SQRT:
			{
				for (mfmU64 i = 0; i < tabs; ++i)
					if (mfsPutByte(outputStream, '\t') != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				mfmU16 id1 = 0;
				mfmFromBigEndian2(it + 1, &id1);
				mfmU16 id2 = 0;
				mfmFromBigEndian2(it + 3, &id2);
				mfError err = mfgOGL4PutID(id1, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8" = sqrt(") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				err = mfgOGL4PutID(id2, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8");\n") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				it += 5;
				break;
			}

			case MFG_BYTECODE_ISQRT:
			{
				for (mfmU64 i = 0; i < tabs; ++i)
					if (mfsPutByte(outputStream, '\t') != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				mfmU16 id1 = 0;
				mfmFromBigEndian2(it + 1, &id1);
				mfmU16 id2 = 0;
				mfmFromBigEndian2(it + 3, &id2);
				mfError err = mfgOGL4PutID(id1, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8" = inversesqrt(") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				err = mfgOGL4PutID(id2, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8");\n") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				it += 5;
				break;
			}

			case MFG_BYTECODE_ABS:
			{
				for (mfmU64 i = 0; i < tabs; ++i)
					if (mfsPutByte(outputStream, '\t') != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				mfmU16 id1 = 0;
				mfmFromBigEndian2(it + 1, &id1);
				mfmU16 id2 = 0;
				mfmFromBigEndian2(it + 3, &id2);
				mfError err = mfgOGL4PutID(id1, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8" = abs(") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				err = mfgOGL4PutID(id2, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8");\n") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				it += 5;
				break;
			}

			case MFG_BYTECODE_SIGN:
			{
				for (mfmU64 i = 0; i < tabs; ++i)
					if (mfsPutByte(outputStream, '\t') != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				mfmU16 id1 = 0;
				mfmFromBigEndian2(it + 1, &id1);
				mfmU16 id2 = 0;
				mfmFromBigEndian2(it + 3, &id2);
				mfError err = mfgOGL4PutID(id1, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8" = sign(") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				err = mfgOGL4PutID(id2, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8");\n") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				it += 5;
				break;
			}

			case MFG_BYTECODE_FLOOR:
			{
				for (mfmU64 i = 0; i < tabs; ++i)
					if (mfsPutByte(outputStream, '\t') != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				mfmU16 id1 = 0;
				mfmFromBigEndian2(it + 1, &id1);
				mfmU16 id2 = 0;
				mfmFromBigEndian2(it + 3, &id2);
				mfError err = mfgOGL4PutID(id1, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8" = floor(") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				err = mfgOGL4PutID(id2, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8");\n") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				it += 5;
				break;
			}

			case MFG_BYTECODE_CEIL:
			{
				for (mfmU64 i = 0; i < tabs; ++i)
					if (mfsPutByte(outputStream, '\t') != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				mfmU16 id1 = 0;
				mfmFromBigEndian2(it + 1, &id1);
				mfmU16 id2 = 0;
				mfmFromBigEndian2(it + 3, &id2);
				mfError err = mfgOGL4PutID(id1, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8" = ceil(") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				err = mfgOGL4PutID(id2, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8");\n") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				it += 5;
				break;
			}

			case MFG_BYTECODE_ROUND:
			{
				for (mfmU64 i = 0; i < tabs; ++i)
					if (mfsPutByte(outputStream, '\t') != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				mfmU16 id1 = 0;
				mfmFromBigEndian2(it + 1, &id1);
				mfmU16 id2 = 0;
				mfmFromBigEndian2(it + 3, &id2);
				mfError err = mfgOGL4PutID(id1, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8" = round(") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				err = mfgOGL4PutID(id2, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8");\n") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				it += 5;
				break;
			}

			case MFG_BYTECODE_FRACT:
			{
				for (mfmU64 i = 0; i < tabs; ++i)
					if (mfsPutByte(outputStream, '\t') != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				mfmU16 id1 = 0;
				mfmFromBigEndian2(it + 1, &id1);
				mfmU16 id2 = 0;
				mfmFromBigEndian2(it + 3, &id2);
				mfError err = mfgOGL4PutID(id1, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8" = fract(") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				err = mfgOGL4PutID(id2, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8");\n") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				it += 5;
				break;
			}

			case MFG_BYTECODE_LERP:
			{
				for (mfmU64 i = 0; i < tabs; ++i)
					if (mfsPutByte(outputStream, '\t') != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				mfmU16 id1 = 0;
				mfmFromBigEndian2(it + 1, &id1);
				mfmU16 id2 = 0;
				mfmFromBigEndian2(it + 3, &id2);
				mfmU16 id3 = 0;
				mfmFromBigEndian2(it + 5, &id3);
				mfmU16 id4 = 0;
				mfmFromBigEndian2(it + 7, &id4);
				mfError err = mfgOGL4PutID(id1, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8" = lerp(") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				err = mfgOGL4PutID(id2, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8" , ") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				err = mfgOGL4PutID(id3, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8" , ") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				err = mfgOGL4PutID(id4, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8");\n") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				it += 9;
				break;
			}

			case MFG_BYTECODE_CLAMP:
			{
				for (mfmU64 i = 0; i < tabs; ++i)
					if (mfsPutByte(outputStream, '\t') != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				mfmU16 id1 = 0;
				mfmFromBigEndian2(it + 1, &id1);
				mfmU16 id2 = 0;
				mfmFromBigEndian2(it + 3, &id2);
				mfmU16 id3 = 0;
				mfmFromBigEndian2(it + 5, &id3);
				mfmU16 id4 = 0;
				mfmFromBigEndian2(it + 7, &id4);
				mfError err = mfgOGL4PutID(id1, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8" = clamp(") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				err = mfgOGL4PutID(id4, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8" , ") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				err = mfgOGL4PutID(id2, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8" , ") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				err = mfgOGL4PutID(id3, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8");\n") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				it += 9;
				break;
			}

			case MFG_BYTECODE_DOT:
			{
				for (mfmU64 i = 0; i < tabs; ++i)
					if (mfsPutByte(outputStream, '\t') != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				mfmU16 id1 = 0;
				mfmFromBigEndian2(it + 1, &id1);
				mfmU16 id2 = 0;
				mfmFromBigEndian2(it + 3, &id2);
				mfmU16 id3 = 0;
				mfmFromBigEndian2(it + 5, &id3);
				mfError err = mfgOGL4PutID(id1, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8" = dot(") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				err = mfgOGL4PutID(id2, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8" , ") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				err = mfgOGL4PutID(id3, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8");\n") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				it += 7;
				break;
			}

			case MFG_BYTECODE_CROSS:
			{
				for (mfmU64 i = 0; i < tabs; ++i)
					if (mfsPutByte(outputStream, '\t') != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				mfmU16 id1 = 0;
				mfmFromBigEndian2(it + 1, &id1);
				mfmU16 id2 = 0;
				mfmFromBigEndian2(it + 3, &id2);
				mfmU16 id3 = 0;
				mfmFromBigEndian2(it + 5, &id3);
				mfError err = mfgOGL4PutID(id1, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8" = cross(") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				err = mfgOGL4PutID(id2, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8" , ") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				err = mfgOGL4PutID(id3, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8");\n") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				it += 7;
				break;
			}

			case MFG_BYTECODE_REFLECT:
			{
				for (mfmU64 i = 0; i < tabs; ++i)
					if (mfsPutByte(outputStream, '\t') != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				mfmU16 id1 = 0;
				mfmFromBigEndian2(it + 1, &id1);
				mfmU16 id2 = 0;
				mfmFromBigEndian2(it + 3, &id2);
				mfmU16 id3 = 0;
				mfmFromBigEndian2(it + 5, &id3);
				mfError err = mfgOGL4PutID(id1, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8" = reflect(") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				err = mfgOGL4PutID(id2, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8" , ") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				err = mfgOGL4PutID(id3, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8");\n") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				it += 7;
				break;
			}

			case MFG_BYTECODE_MIN:
			{
				for (mfmU64 i = 0; i < tabs; ++i)
					if (mfsPutByte(outputStream, '\t') != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				mfmU16 id1 = 0;
				mfmFromBigEndian2(it + 1, &id1);
				mfmU16 id2 = 0;
				mfmFromBigEndian2(it + 3, &id2);
				mfmU16 id3 = 0;
				mfmFromBigEndian2(it + 5, &id3);
				mfError err = mfgOGL4PutID(id1, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8" = min(") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				err = mfgOGL4PutID(id2, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8" , ") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				err = mfgOGL4PutID(id3, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8");\n") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				it += 7;
				break;
			}

			case MFG_BYTECODE_MAX:
			{
				for (mfmU64 i = 0; i < tabs; ++i)
					if (mfsPutByte(outputStream, '\t') != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				mfmU16 id1 = 0;
				mfmFromBigEndian2(it + 1, &id1);
				mfmU16 id2 = 0;
				mfmFromBigEndian2(it + 3, &id2);
				mfmU16 id3 = 0;
				mfmFromBigEndian2(it + 5, &id3);
				mfError err = mfgOGL4PutID(id1, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8" = max(") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				err = mfgOGL4PutID(id2, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8" , ") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				err = mfgOGL4PutID(id3, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8");\n") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				it += 7;
				break;
			}

			case MFG_BYTECODE_FETCH1D:
			case MFG_BYTECODE_FETCH2D:
			case MFG_BYTECODE_FETCH3D:
			{
				for (mfmU64 i = 0; i < tabs; ++i)
					if (mfsPutByte(outputStream, '\t') != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				mfmU16 id1 = 0;
				mfmFromBigEndian2(it + 1, &id1);
				mfmU16 id2 = 0;
				mfmFromBigEndian2(it + 3, &id2);
				mfmU16 id3 = 0;
				mfmFromBigEndian2(it + 5, &id3);
				mfError err = mfgOGL4PutID(id3, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8" = texelFetch(") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				err = mfgOGL4PutID(id1, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8" , ") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				err = mfgOGL4PutID(id2, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8" , 0);\n") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				it += 7;
				break;
			}

			case MFG_BYTECODE_I1TOF1:
			{
				for (mfmU64 i = 0; i < tabs; ++i)
					if (mfsPutByte(outputStream, '\t') != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				mfmU16 id1 = 0;
				mfmFromBigEndian2(it + 1, &id1);
				mfmU16 id2 = 0;
				mfmFromBigEndian2(it + 3, &id2);
				mfError err = mfgOGL4PutID(id1, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8" = float(") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				err = mfgOGL4PutID(id2, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8");\n") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				it += 5;
				break;
			}

			case MFG_BYTECODE_I2TOF2:
			{
				for (mfmU64 i = 0; i < tabs; ++i)
					if (mfsPutByte(outputStream, '\t') != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				mfmU16 id1 = 0;
				mfmFromBigEndian2(it + 1, &id1);
				mfmU16 id2 = 0;
				mfmFromBigEndian2(it + 3, &id2);
				mfError err = mfgOGL4PutID(id1, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8" = vec2(") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				err = mfgOGL4PutID(id2, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8");\n") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				it += 5;
				break;
			}

			case MFG_BYTECODE_I3TOF3:
			{
				for (mfmU64 i = 0; i < tabs; ++i)
					if (mfsPutByte(outputStream, '\t') != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				mfmU16 id1 = 0;
				mfmFromBigEndian2(it + 1, &id1);
				mfmU16 id2 = 0;
				mfmFromBigEndian2(it + 3, &id2);
				mfError err = mfgOGL4PutID(id1, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8" = vec3(") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				err = mfgOGL4PutID(id2, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8");\n") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				it += 5;
				break;
			}

			case MFG_BYTECODE_I4TOF4:
			{
				for (mfmU64 i = 0; i < tabs; ++i)
					if (mfsPutByte(outputStream, '\t') != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				mfmU16 id1 = 0;
				mfmFromBigEndian2(it + 1, &id1);
				mfmU16 id2 = 0;
				mfmFromBigEndian2(it + 3, &id2);
				mfError err = mfgOGL4PutID(id1, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8" = vec4(") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				err = mfgOGL4PutID(id2, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8");\n") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				it += 5;
				break;
			}

			case MFG_BYTECODE_F1TOI1:
			{
				for (mfmU64 i = 0; i < tabs; ++i)
					if (mfsPutByte(outputStream, '\t') != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				mfmU16 id1 = 0;
				mfmFromBigEndian2(it + 1, &id1);
				mfmU16 id2 = 0;
				mfmFromBigEndian2(it + 3, &id2);
				mfError err = mfgOGL4PutID(id1, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8" = int(") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				err = mfgOGL4PutID(id2, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8");\n") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				it += 5;
				break;
			}

			case MFG_BYTECODE_F2TOI2:
			{
				for (mfmU64 i = 0; i < tabs; ++i)
					if (mfsPutByte(outputStream, '\t') != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				mfmU16 id1 = 0;
				mfmFromBigEndian2(it + 1, &id1);
				mfmU16 id2 = 0;
				mfmFromBigEndian2(it + 3, &id2);
				mfError err = mfgOGL4PutID(id1, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8" = ivec2(") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				err = mfgOGL4PutID(id2, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8");\n") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				it += 5;
				break;
			}

			case MFG_BYTECODE_F3TOI3:
			{
				for (mfmU64 i = 0; i < tabs; ++i)
					if (mfsPutByte(outputStream, '\t') != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				mfmU16 id1 = 0;
				mfmFromBigEndian2(it + 1, &id1);
				mfmU16 id2 = 0;
				mfmFromBigEndian2(it + 3, &id2);
				mfError err = mfgOGL4PutID(id1, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8" = ivec3(") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				err = mfgOGL4PutID(id2, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8");\n") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				it += 5;
				break;
			}

			case MFG_BYTECODE_F4TOI4:
			{
				for (mfmU64 i = 0; i < tabs; ++i)
					if (mfsPutByte(outputStream, '\t') != MF_ERROR_OKAY)
						return MFG_ERROR_FAILED_TO_WRITE;
				mfmU16 id1 = 0;
				mfmFromBigEndian2(it + 1, &id1);
				mfmU16 id2 = 0;
				mfmFromBigEndian2(it + 3, &id2);
				mfError err = mfgOGL4PutID(id1, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8" = ivec4(") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				err = mfgOGL4PutID(id2, &assemblerData, outputStream);
				if (err != MF_ERROR_OKAY)
					return err;
				if (mfsPrintFormat(outputStream, u8");\n") != MF_ERROR_OKAY)
					return MFG_ERROR_FAILED_TO_WRITE;
				it += 5;
				break;
			}

			default:
				return MFG_ERROR_INVALID_DATA;
				break;
		}
	}

	if (mfsPrintFormat(outputStream, u8"}\n") != MF_ERROR_OKAY)
		return MFG_ERROR_FAILED_TO_WRITE;

	return MF_ERROR_OKAY;
}
