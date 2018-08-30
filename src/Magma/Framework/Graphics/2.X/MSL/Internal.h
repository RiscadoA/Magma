#pragma once

#include "../../../String/UTF8.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define MFG_V2X_TOKEN_ATTRIBUTE_SIZE 32
#define MFG_V2X_MAX_ERROR_MESSAGE_SIZE 512
#define MFG_V2X_MAX_INPUT_VARS 32
#define MFG_V2X_MAX_OUTPUT_VARS 32
#define MFG_V2X_MAX_CONSTANT_BUFFER_VARS 32
#define MFG_V2X_MAX_CONSTANT_BUFFERS 4
#define MFG_V2X_MAX_TEXTURE_1DS 8
#define MFG_V2X_MAX_TEXTURE_2DS 8
#define MFG_V2X_MAX_TEXTURE_3DS 8

	typedef mfmU32 mfgV2XEnum;

	typedef struct
	{
		mfsUTF8CodeUnit id[MFG_V2X_TOKEN_ATTRIBUTE_SIZE];
		mfsUTF8CodeUnit name[MFG_V2X_TOKEN_ATTRIBUTE_SIZE];
		mfmU64 arraySize; // 0 if not an array
		mfgV2XEnum type;
		mfmU16 index;
		mfmBool active;
	} mfgV2XCompilerMSLVariable;

	typedef struct
	{
		mfgV2XCompilerMSLVariable variables[MFG_V2X_MAX_INPUT_VARS];
	} mfgV2XCompilerMSLInput;

	typedef struct
	{
		mfgV2XCompilerMSLVariable variables[MFG_V2X_MAX_OUTPUT_VARS];
	} mfgV2XCompilerMSLOutput;

	typedef struct
	{
		mfgV2XCompilerMSLVariable variables[MFG_V2X_MAX_CONSTANT_BUFFER_VARS];
		mfsUTF8CodeUnit id[MFG_V2X_TOKEN_ATTRIBUTE_SIZE];
		mfsUTF8CodeUnit name[MFG_V2X_TOKEN_ATTRIBUTE_SIZE];
		mfmBool active;
		mfmU16 index;
	} mfgV2XCompilerMSLConstantBuffer;

	typedef struct
	{
		mfsUTF8CodeUnit id[MFG_V2X_TOKEN_ATTRIBUTE_SIZE];
		mfsUTF8CodeUnit name[MFG_V2X_TOKEN_ATTRIBUTE_SIZE];
		mfmBool active;
		mfmU16 index;
	} mfgV2XCompilerMSLTexture1D;

	typedef struct
	{
		mfsUTF8CodeUnit id[MFG_V2X_TOKEN_ATTRIBUTE_SIZE];
		mfsUTF8CodeUnit name[MFG_V2X_TOKEN_ATTRIBUTE_SIZE];
		mfmBool active;
		mfmU16 index;
	} mfgV2XCompilerMSLTexture2D;

	typedef struct
	{
		mfsUTF8CodeUnit id[MFG_V2X_TOKEN_ATTRIBUTE_SIZE];
		mfsUTF8CodeUnit name[MFG_V2X_TOKEN_ATTRIBUTE_SIZE];
		mfmBool active;
		mfmU16 index;
	} mfgV2XCompilerMSLTexture3D;

	typedef struct
	{
		mfgV2XCompilerMSLInput input;
		mfgV2XCompilerMSLOutput output;
		mfgV2XCompilerMSLConstantBuffer constantBuffers[MFG_V2X_MAX_CONSTANT_BUFFERS];
		mfgV2XCompilerMSLTexture1D texture1Ds[MFG_V2X_MAX_TEXTURE_1DS];
		mfgV2XCompilerMSLTexture2D texture2Ds[MFG_V2X_MAX_TEXTURE_2DS];
		mfgV2XCompilerMSLTexture3D texture3Ds[MFG_V2X_MAX_TEXTURE_3DS];
	} mfgV2XCompilerState;

#ifdef __cplusplus
}
#endif
