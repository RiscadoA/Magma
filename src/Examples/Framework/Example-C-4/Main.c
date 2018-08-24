#include <Magma/Framework/Entry.h>
#include <Magma/Framework/Graphics/2.X/Bytecode.h>
#include <Magma/Framework/Graphics/2.X/D3D11Assembler.h>
#include <Magma/Framework/String/Stream.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, const char** argv)
{
	if (mfInit(argc, argv) != MF_ERROR_OKAY)
		abort();

	mfError err = MF_ERROR_OKAY;

	mfmU8 metaData[256];
	mfmU64 ptr = 0;
	metaData[ptr++] = MFG_METADATA_HEADER_MARKER_0;
	metaData[ptr++] = MFG_METADATA_HEADER_MARKER_1;
	metaData[ptr++] = MFG_METADATA_HEADER_MARKER_2;
	metaData[ptr++] = MFG_METADATA_HEADER_MARKER_3;
	metaData[ptr++] = MFG_VERTEX_SHADER;
	metaData[ptr++] = 2;	// Input var count
	metaData[ptr++] = 1;	// Output var count
	metaData[ptr++] = 2;	// Binding point count

	// Input var 1
	memcpy(metaData + ptr, "test1", 6);
	ptr += 16;
	metaData[ptr++] = 0x00;
	metaData[ptr++] = 0x01;
	metaData[ptr++] = MFG_FLOAT4;

	// Input var 2
	memcpy(metaData + ptr, "test2", 6);
	ptr += 16;
	metaData[ptr++] = 0x00;
	metaData[ptr++] = 0x02;
	metaData[ptr++] = MFG_FLOAT1;

	// Output var 1
	memcpy(metaData + ptr, "out1", 5);
	ptr += 16;
	metaData[ptr++] = 0x00;
	metaData[ptr++] = 0x03;
	metaData[ptr++] = MFG_FLOAT1;

	// Constant buffer
	memcpy(metaData + ptr, "buf1", 5);
	ptr += 16;
	metaData[ptr++] = MFG_CONSTANT_BUFFER;
	metaData[ptr++] = 0x00; // 2 elements
	metaData[ptr++] = 0x02;
	// Elements:
	{
		metaData[ptr++] = 0x00; // Var index 4
		metaData[ptr++] = 0x04; 
		metaData[ptr++] = MFG_FLOAT44;

		metaData[ptr++] = 0x00; // Var index 5
		metaData[ptr++] = 0x05;
		metaData[ptr++] = MFG_FLOAT22;
	}

	// 3D texture
	memcpy(metaData + ptr, "texture", 8);
	ptr += 16;
	metaData[ptr++] = MFG_TEXTURE_3D;
	metaData[ptr++] = 0x00; // Var index 6
	metaData[ptr++] = 0x06;

	// Load
	mfgMetaData* loadedData = NULL;
	err = mfgLoadMetaData(metaData, sizeof(metaData), &loadedData, NULL);
	if (err != MF_ERROR_OKAY)
		abort();

	// Get param (test1)
	const mfgMetaDataInputVariable* inputVar;
	err = mfgGetMetaDataInput(loadedData, u8"test1", &inputVar);
	if (err != MF_ERROR_OKAY)
		abort();
	mfsPrintFormatUTF8(mfsOutStream, u8"Successfully got input variable from the loaded meta data:\n- Type: %d\nIndex: %d\n", inputVar->type, inputVar->id);

	// Get param (test2)
	err = mfgGetMetaDataInput(loadedData, u8"test2", &inputVar);
	if (err != MF_ERROR_OKAY)
		abort();
	mfsPrintFormatUTF8(mfsOutStream, u8"Successfully got input variable from the loaded meta data:\n- Type: %d\nIndex: %d\n", inputVar->type, inputVar->id);

	// Get param (out1)
	const mfgMetaDataOutputVariable* outputVar;
	err = mfgGetMetaDataOutput(loadedData, u8"out1", &outputVar);
	if (err != MF_ERROR_OKAY)
		abort();
	mfsPrintFormatUTF8(mfsOutStream, u8"Successfully got output variable from the loaded meta data:\n- Type: %d\nIndex: %d\n", outputVar->type, outputVar->id);

	// Get constant buffer
	const mfgMetaDataBindingPoint* bp;
	err = mfgGetMetaDataBindingPoint(loadedData, u8"buf1", &bp);
	if (err != MF_ERROR_OKAY)
		abort();
	mfsPrintFormatUTF8(mfsOutStream, u8"Successfully got binding point from the loaded meta data:\n- Type: %d\nName: \"%s\"\n", bp->type, bp->name);

	// Get texture
	err = mfgGetMetaDataBindingPoint(loadedData, u8"texture", &bp);
	if (err != MF_ERROR_OKAY)
		abort();
	mfsPrintFormatUTF8(mfsOutStream, u8"Successfully got binding point from the loaded meta data:\n- Type: %d\nName: \"%s\"\n", bp->type, bp->name);

	// Unload
	mfgUnloadMetaData(loadedData);

	for (;;);

	mfTerminate();
	return 0;
}