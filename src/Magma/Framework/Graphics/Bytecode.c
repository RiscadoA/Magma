#include "Bytecode.h"
#include "../Memory/Allocator.h"
#include "../Memory/Endianness.h"

#include <string.h>
#include <stdlib.h>

mfError mfgLoadMetaData(const mfmU8 * metaData, mfmU64 size, mfgMetaData ** outData, void * allocator)
{
	if (metaData == 0 || outData == NULL)
		return MFG_ERROR_INVALID_ARGUMENTS;

	if (size < 8)
		return MFG_ERROR_INVALID_DATA;

	if (metaData[0] != MFG_METADATA_HEADER_MARKER_0 ||
		metaData[1] != MFG_METADATA_HEADER_MARKER_1 ||
		metaData[2] != MFG_METADATA_HEADER_MARKER_2 ||
		metaData[3] != MFG_METADATA_HEADER_MARKER_3)
		return MFG_ERROR_INVALID_DATA;

	mfmU8 shaderType = metaData[4];
	mfmU8 inputVarCount = metaData[5];
	mfmU8 outputVarCount = metaData[6];
	mfmU8 bindingPointCount = metaData[7];

	mfmU8* memory = NULL;

	// Check size again
	if (size < 8 + inputVarCount * (16 + 2 + 1) + outputVarCount * (16 + 2 + 1))
		return MFG_ERROR_INVALID_DATA;

	// If it has no binding points, theres no need to check further
	if (bindingPointCount == 0)
	{
		// Allocate the memory needed
		mfError err = mfmAllocate(
			allocator,
			&memory,
			sizeof(mfgMetaData) +
			sizeof(mfgMetaDataInputVariable) * inputVarCount +
			sizeof(mfgMetaDataOutputVariable) * outputVarCount);

		if (err != MFM_ERROR_OKAY)
			return MFG_ERROR_ALLOCATION_FAILED;
	}
	// If it has binding points, check their size
	else
	{
		mfmU64 readPtr = 8 + inputVarCount * (16 + 2 + 1) + outputVarCount * (16 + 2 + 1);
		mfmU64 bpAllocSize = 0;

		// Get binding point allocation size
		for (mfmU64 i = 0; i < bindingPointCount; ++i)
		{
			readPtr += 16;
			mfmU8 type = metaData[readPtr++];
			readPtr += 2;
			if (type == MFG_CONSTANT_BUFFER)
			{
				bpAllocSize += sizeof(mfgMetaDataConstantBuffer);

				mfmU16 elementCount = 0;
				mfmFromBigEndian2(metaData + readPtr, &elementCount);
				readPtr += 2;

				// Get constant buffer size
				bpAllocSize += sizeof(mfgMetaDataConstantBufferVariable) * elementCount;
				readPtr += elementCount * (2 + 1);
			}
			else if (type == MFG_TEXTURE_1D)
			{
				bpAllocSize += sizeof(mfgMetaDataTexture1D);
				readPtr += 2;
			}
			else if (type == MFG_TEXTURE_2D)
			{
				bpAllocSize += sizeof(mfgMetaDataTexture2D);
				readPtr += 2;
			}
			else if (type == MFG_TEXTURE_3D)
			{
				bpAllocSize += sizeof(mfgMetaDataTexture3D);
				readPtr += 2;
			}
			else
				return MFG_ERROR_INVALID_DATA;
		}

		// Allocate the memory needed
		mfError err = mfmAllocate(
			allocator,
			&memory,
			sizeof(mfgMetaData) +
			sizeof(mfgMetaDataInputVariable) * inputVarCount +
			sizeof(mfgMetaDataOutputVariable) * outputVarCount +
			bpAllocSize);

		if (err != MFM_ERROR_OKAY)
			return MFG_ERROR_ALLOCATION_FAILED;
	}

	// Init meta data struct
	(*outData) = (mfgMetaData*)memory;
	{
		mfError err = mfmInitObject(&(*outData)->object);
		if (err != MFM_ERROR_OKAY)
			return err;
	}	
	(*outData)->object.destructorFunc = &mfgUnloadMetaData;
	(*outData)->allocator = allocator;
	(*outData)->inputVarCount = inputVarCount;
	(*outData)->outputVarCount = outputVarCount;
	(*outData)->bindingPointCount = bindingPointCount;
	(*outData)->shaderType = shaderType;
	(*outData)->firstInputVar = NULL;
	(*outData)->firstOutputVar = NULL;
	(*outData)->firstBindingPoint = NULL;

	mfmU64 readPtr = 8;
	mfmU64 writePtr = sizeof(mfgMetaData);

	// Get input variables
	{
		mfgMetaDataInputVariable* prevVar = NULL;
		for (mfmU64 i = 0; i < inputVarCount; ++i)
		{
			// Get var
			mfgMetaDataInputVariable* var = (mfgMetaDataInputVariable*)(memory + writePtr);
			writePtr += sizeof(mfgMetaDataInputVariable);
			var->next = NULL;
			if (prevVar != NULL)
				prevVar->next = var;
			else
				(*outData)->firstInputVar = var;
			prevVar = var;

			// Get variable name
			memcpy(var->name, metaData + readPtr, 16);
			readPtr += 16;

			// Get index
			mfmFromBigEndian2(metaData + readPtr, &var->id);
			readPtr += 2;

			// Get type
			var->type = metaData[readPtr++];		
		}
	}

	// Get output variables
	{
		mfgMetaDataOutputVariable* prevVar = NULL;
		for (mfmU64 i = 0; i < outputVarCount; ++i)
		{
			// Get var
			mfgMetaDataOutputVariable* var = (mfgMetaDataOutputVariable*)(memory + writePtr);
			writePtr += sizeof(mfgMetaDataOutputVariable);
			var->next = NULL;
			if (prevVar != NULL)
				prevVar->next = var;
			else
				(*outData)->firstOutputVar = var;
			prevVar = var;

			// Get variable name
			memcpy(var->name, metaData + readPtr, 16);
			readPtr += 16;

			// Get index
			mfmFromBigEndian2(metaData + readPtr, &var->id);
			readPtr += 2;

			// Get type
			var->type = metaData[readPtr++];
		}
	}

	// Get binding points
	{
		mfgMetaDataBindingPoint* prevBP = NULL;
		for (mfmU64 i = 0; i < bindingPointCount; ++i)
		{
			// Get binding point
			mfgMetaDataBindingPoint* bp = (mfgMetaDataBindingPoint*)(memory + writePtr);
			bp->next = NULL;
			if (prevBP != NULL)
				prevBP->next = bp;
			else
				(*outData)->firstBindingPoint = bp;
			prevBP = bp;

			// Get binding point name
			memcpy(bp->name, metaData + readPtr, 16);
			readPtr += 16;

			// Get type
			bp->type = metaData[readPtr++];

			// Get index
			mfmFromBigEndian2(metaData + readPtr, &bp->id);
			readPtr += 2;

			if (bp->type == MFG_CONSTANT_BUFFER)
			{
				mfgMetaDataConstantBuffer* cb = (mfgMetaDataConstantBuffer*)bp;
				writePtr += sizeof(mfgMetaDataConstantBuffer);

				// Get var count
				mfmFromBigEndian2(metaData + readPtr, &cb->variableCount);
				readPtr += 2;
				cb->firstVariable = NULL;

				mfgMetaDataConstantBufferVariable* prevVar = NULL;

				for (mfmU8 i = 0; i < cb->variableCount; ++i)
				{
					mfgMetaDataConstantBufferVariable* var = (mfgMetaDataConstantBufferVariable*)(memory + writePtr);
					writePtr += sizeof(mfgMetaDataConstantBufferVariable);
					var->next = NULL;
					if (prevVar != NULL)
						prevVar->next = var;
					else
						cb->firstVariable = var;
					prevVar = var;

					// Get index
					mfmFromBigEndian2(metaData + readPtr, &var->id);
					readPtr += 2;

					// Get type
					var->type = metaData[readPtr++];
				}
			}
			else if (bp->type == MFG_TEXTURE_1D)
			{
				mfgMetaDataTexture1D* tex = (mfgMetaDataTexture1D*)bp;
				writePtr += sizeof(mfgMetaDataTexture1D);
			}
			else if (bp->type == MFG_TEXTURE_2D)
			{
				mfgMetaDataTexture2D* tex = (mfgMetaDataTexture2D*)bp;
				writePtr += sizeof(mfgMetaDataTexture2D);
			}
			else if (bp->type == MFG_TEXTURE_3D)
			{
				mfgMetaDataTexture3D* tex = (mfgMetaDataTexture3D*)bp;
				writePtr += sizeof(mfgMetaDataTexture3D);
			}
			else
			{
				mfmDeallocate(allocator, memory);
				return MFG_ERROR_INVALID_DATA;
			}
		}
	}

	return MFG_ERROR_OKAY;
}

void mfgUnloadMetaData(void * metaData)
{
	mfError err = mfmDestroyObject(&((mfgMetaData*)metaData)->object);
	if (err != MFM_ERROR_OKAY)
		abort();
	err = mfmDeallocate(((mfgMetaData*)metaData)->allocator, metaData);
	if (err != MFM_ERROR_OKAY)
		abort();
}

mfError mfgGetMetaDataInput(const mfgMetaData * metaData, const mfsUTF8CodeUnit * name, const mfgMetaDataInputVariable ** inputVar)
{
	if (metaData == NULL || name == NULL || inputVar == NULL)
		return MFG_ERROR_INVALID_ARGUMENTS;

	mfgMetaDataInputVariable* var = metaData->firstInputVar;
	while (var != NULL)
	{
		mfmBool found = MFM_TRUE;
		for (mfmU64 i = 0; i < 16; ++i)
		{
			if (var->name[i] != name[i])
			{
				found = MFM_FALSE;
				break;
			}

			if (var->name[i] == '\0')
				break;
		}

		if (found == MFM_TRUE)
		{
			*inputVar = var;
			return MFG_ERROR_OKAY;
		}
		else
			var = var->next;
	}

	return MFG_ERROR_NOT_FOUND;
}

mfError mfgGetMetaDataOutput(const mfgMetaData * metaData, const mfsUTF8CodeUnit * name, const mfgMetaDataOutputVariable ** outputVar)
{
	if (metaData == NULL || name == NULL || outputVar == NULL)
		return MFG_ERROR_INVALID_ARGUMENTS;

	mfgMetaDataOutputVariable* var = metaData->firstOutputVar;
	while (var != NULL)
	{
		mfmBool found = MFM_TRUE;
		for (mfmU64 i = 0; i < 16; ++i)
		{
			if (var->name[i] != name[i])
			{
				found = MFM_FALSE;
				break;
			}

			if (var->name[i] == '\0')
				break;
		}

		if (found == MFM_TRUE)
		{
			*outputVar = var;
			return MFG_ERROR_OKAY;
		}
		else
			var = var->next;
	}

	return MFG_ERROR_NOT_FOUND;
}

mfError mfgGetMetaDataBindingPoint(const mfgMetaData * metaData, const mfsUTF8CodeUnit * name, const mfgMetaDataBindingPoint ** bindingPoint)
{
	if (metaData == NULL || name == NULL || bindingPoint == NULL)
		return MFG_ERROR_INVALID_ARGUMENTS;

	mfgMetaDataBindingPoint* bp = metaData->firstBindingPoint;
	while (bp != NULL)
	{
		mfmBool found = MFM_TRUE;
		for (mfmU64 i = 0; i < 16; ++i)
		{
			if (bp->name[i] != name[i])
			{
				found = MFM_FALSE;
				break;
			}

			if (bp->name[i] == '\0')
				break;
		}

		if (found == MFM_TRUE)
		{
			*bindingPoint = bp;
			return MFG_ERROR_OKAY;
		}
		else
			bp = bp->next;
	}

	return MFG_ERROR_NOT_FOUND;
}
