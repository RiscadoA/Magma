﻿#include <Magma/Framework/Input/GLWindow.h>
#include <Magma/Framework/Graphics/OGL4RenderDevice.h>
#include <Magma/Framework/String/UTF8.h>
#include <Magma/Framework/String/Stream.h>
#include <stdio.h>
#include <stdlib.h>

mfiWindow* window = NULL;
mfgRenderDevice* renderDevice = NULL;

mfmBool windowOpen = MFM_TRUE;

mfgVertexShader* vs = NULL;

void OnClose(void* window)
{
	windowOpen = MFM_FALSE;
}

void Main(int argc, char** argv)
{
	// Open window
	{
		if (mfiCreateGLWindow(&window, 800, 600, MFI_WINDOWED, u8"Example-C-2") != MFI_ERROR_OKAY)
			abort();
		window->onClose = &OnClose;
	}

	// Create render device
	{
		mfgRenderDeviceDesc desc;
		desc.vsyncEnabled = MFM_TRUE;
		if(mfgCreateOGL4RenderDevice(&renderDevice, window, &desc, NULL) != MFG_ERROR_OKAY)
			abort();
	}

	// Load vertex shader
	{
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
			metaDataB[ptr++] = MFG_FLOAT4;

			// Load
			if (mfgLoadMetaData(metaDataB, sizeof(metaDataB), &metaData, NULL) != MFG_ERROR_OKAY)
				abort();
		}

		// Load bytecode
		mfmU8 bytecode[] =
		{
			MFG_BYTECODE_HEADER_MARKER_0,
			MFG_BYTECODE_HEADER_MARKER_1,
			MFG_BYTECODE_HEADER_MARKER_2,
			MFG_BYTECODE_HEADER_MARKER_3,
			0x02, // Major version 2
			0x00, // Minor version 0
			MFG_BYTECODE_DECLF4A, 0x00, 0x02, 0x00, 0x04,
			MFG_BYTECODE_ASSIGN, 0x00, 0x02, 0x00, 0x01,
			MFG_BYTECODE_ASSIGN, 0x00, 0x00, 0x00, 0x02,
			MFG_BYTECODE_GET4CMP, 0x00, 0xF0, 0x00, 0x00, 0x01,
			MFG_BYTECODE_MULMAT, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00,
		};

		// Create shader
		if (mfgCreateVertexShader(renderDevice, &vs, bytecode, sizeof(bytecode), metaData) != MFG_ERROR_OKAY)
		{
			mfsUTF8CodeUnit err[512];
			mfgGetErrorString(renderDevice, err, sizeof(err));
			mfsPrintFormatUTF8(mfsErrStream, err);
			mfsFlush(mfsErrStream);
			abort();
		}
	}

	while (windowOpen == MFM_TRUE)
	{
		window->pollEvents(window);

		if (mfgClearColor(renderDevice, 0.0f, 0.0f, 0.2f, 1.0f) != MFG_ERROR_OKAY)
			abort();
		if (mfgClearDepth(renderDevice, 1.0f) != MFG_ERROR_OKAY)
			abort();
		if (mfgClearStencil(renderDevice, 0) != MFG_ERROR_OKAY)
			abort();
		if (mfgSwapBuffers(renderDevice) != MFG_ERROR_OKAY)
			abort();
	}

	if (mfgDestroyVertexShader(renderDevice, vs) != MFG_ERROR_OKAY)
		abort();

	mfiDestroyGLWindow(window);
	mfgDestroyOGL4RenderDevice(renderDevice);
}
