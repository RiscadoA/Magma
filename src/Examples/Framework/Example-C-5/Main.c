﻿#include <Magma/Framework/Input/Entry.h>
#include <Magma/Framework/Graphics/Bytecode.h>
#include <Magma/Framework/Graphics/D3D11Assembler.h>
#include <Magma/Framework/String/Stream.h>
#include <stdlib.h>
#include <string.h>

void Main(int argc, char** argv)
{
	mfgError err = MFG_ERROR_OKAY;


	mfgMetaData* metaData = NULL;

	// Load meta data
	{
		mfmU8 metaDataB[256];
		mfmU64 ptr = 0;
		metaDataB[ptr++] = MFG_METADATA_HEADER_MARKER_0;
		metaDataB[ptr++] = MFG_METADATA_HEADER_MARKER_1;
		metaDataB[ptr++] = MFG_METADATA_HEADER_MARKER_2;
		metaDataB[ptr++] = MFG_METADATA_HEADER_MARKER_3;
		metaDataB[ptr++] = MFG_VERTEX_SHADER;
		metaDataB[ptr++] = 1;	// Input var count
		metaDataB[ptr++] = 1;	// Output var count
		metaDataB[ptr++] = 0;	// Binding point count

		// Input var 1
		memcpy(metaDataB + ptr, "position", 9);
		ptr += 16;
		metaDataB[ptr++] = 0x00;
		metaDataB[ptr++] = 0x01;	// ID 1
		metaDataB[ptr++] = MFG_FLOAT4;

		// Output var 1
		memcpy(metaDataB + ptr, "position", 9);
		ptr += 16;
		metaDataB[ptr++] = 0x00;
		metaDataB[ptr++] = 0x00;	// ID 0
		metaDataB[ptr++] = MFG_FLOAT2;
		
		// Load
		err = mfgLoadMetaData(metaDataB, sizeof(metaDataB), &metaData, NULL);
		if (err != MFG_ERROR_OKAY)
			abort();
	}

	mfmU8 bytecode[] =
	{
		MFG_BYTECODE_HEADER_MARKER_0,
		MFG_BYTECODE_HEADER_MARKER_1,
		MFG_BYTECODE_HEADER_MARKER_2,
		MFG_BYTECODE_HEADER_MARKER_3,
		0x02, // Major version 2
		0x00, // Minor version 0
		MFG_BYTECODE_ASSIGN, 0x00, 0x00, 0x00, 0x01,
	};

	err = mfgD3D11Assemble(bytecode, sizeof(bytecode), metaData, mfsOut);
	if (err != MFG_ERROR_OKAY)
		abort();

	// Unload meta data
	mfgUnloadMetaData(metaData);
}