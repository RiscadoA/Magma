#include <Magma/Framework/Input/GLWindow.h>
#include <Magma/Framework/Graphics/OGL4RenderDevice.h>
#include <Magma/Framework/String/UTF8.h>
#include <Magma/Framework/String/Stream.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

mfiWindow* window = NULL;
mfgRenderDevice* renderDevice = NULL;

mfmBool windowOpen = MFM_TRUE;

mfgVertexShader* vs = NULL;
mfgPixelShader* ps = NULL;
mfgPipeline* pp = NULL;
mfgVertexBuffer* vb = NULL;
mfgVertexLayout* vl = NULL;
mfgVertexArray* va = NULL;
mfgIndexBuffer* ib = NULL;

struct Vertex
{
	mfmF32 x;
	mfmF32 y;
	mfmF32 z;
	mfmF32 w;
};

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
			strcpy(metaDataB + ptr, u8"position");
			ptr += 16;
			metaDataB[ptr++] = 0x00;
			metaDataB[ptr++] = 0x00;	// ID 0
			metaDataB[ptr++] = MFG_FLOAT4;

			// Output var 1
			strcpy(metaDataB + ptr, u8"_position");
			ptr += 16;
			metaDataB[ptr++] = 0x00;
			metaDataB[ptr++] = 0x01;	// ID 1
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
			MFG_BYTECODE_ASSIGN, 0x00, 0x01, 0x00, 0x00,
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

	// Load pixel shader
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
			metaDataB[ptr++] = MFG_PIXEL_SHADER;
			metaDataB[ptr++] = 0;	// Input var count
			metaDataB[ptr++] = 1;	// Output var count
			metaDataB[ptr++] = 0;	// Binding point count

			// Output var 1
			strcpy(metaDataB + ptr, u8"_target0");
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
			MFG_BYTECODE_LITI4, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x01,
			0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x01,
		};

		// Create shader
		if (mfgCreatePixelShader(renderDevice, &ps, bytecode, sizeof(bytecode), metaData) != MFG_ERROR_OKAY)
		{
			mfsUTF8CodeUnit err[512];
			mfgGetErrorString(renderDevice, err, sizeof(err));
			mfsPrintFormatUTF8(mfsErrStream, err);
			mfsFlush(mfsErrStream);
			abort();
		}
	}

	// Create pipeline
	if (mfgCreatePipeline(renderDevice, &pp, vs, ps) != MFG_ERROR_OKAY)
	{
		mfsUTF8CodeUnit err[512];
		mfgGetErrorString(renderDevice, err, sizeof(err));
		mfsPrintFormatUTF8(mfsErrStream, err);
		mfsFlush(mfsErrStream);
		abort();
	}

	// Create vertex buffer
	if (mfgCreateVertexBuffer(renderDevice, &vb, sizeof(struct Vertex) * 4, NULL, MFG_USAGE_DYNAMIC) != MFG_ERROR_OKAY)
	{
		mfsUTF8CodeUnit err[512];
		mfgGetErrorString(renderDevice, err, sizeof(err));
		mfsPrintFormatUTF8(mfsErrStream, err);
		mfsFlush(mfsErrStream);
		abort();
	}

	// Create vertex layout
	{
		mfgVertexElement elements[1];
		
		mfgDefaultVertexElement(&elements[0]);

		elements[0].bufferIndex = 0;
		strcpy(elements[0].name, u8"position");
		elements[0].offset = offsetof(struct Vertex, x);
		elements[0].stride = sizeof(struct Vertex);
		elements[0].size = 4;
		elements[0].type = MFG_FLOAT;

		// Create layout
		if (mfgCreateVertexLayout(renderDevice, &vl, 1, elements, vs) != MFG_ERROR_OKAY)
		{
			mfsUTF8CodeUnit err[512];
			mfgGetErrorString(renderDevice, err, sizeof(err));
			mfsPrintFormatUTF8(mfsErrStream, err);
			mfsFlush(mfsErrStream);
			abort();
		}
	}

	// Create vertex array
	if (mfgCreateVertexArray(renderDevice, &va, 1, &vb, vl) != MFG_ERROR_OKAY)
	{
		mfsUTF8CodeUnit err[512];
		mfgGetErrorString(renderDevice, err, sizeof(err));
		mfsPrintFormatUTF8(mfsErrStream, err);
		mfsFlush(mfsErrStream);
		abort();
	}

	// Create index buffer
	{
		mfmU8 indices[] =
		{
			2, 1, 0,
			0, 3, 2,
		};

		if (mfgCreateIndexBuffer(renderDevice, &ib, sizeof(indices), indices, MFG_UBYTE, MFG_USAGE_DYNAMIC) != MFG_ERROR_OKAY)
		{
			mfsUTF8CodeUnit err[512];
			mfgGetErrorString(renderDevice, err, sizeof(err));
			mfsPrintFormatUTF8(mfsErrStream, err);
			mfsFlush(mfsErrStream);
			abort();
		}
	}

	mfmF32 x = 0.0f;

	while (windowOpen == MFM_TRUE)
	{
		window->pollEvents(window);

		x += 0.001f;

		// Clear

		if (mfgClearColor(renderDevice, 0.0f, 0.0f, 0.2f, 1.0f) != MFG_ERROR_OKAY)
			abort();
		if (mfgClearDepth(renderDevice, 1.0f) != MFG_ERROR_OKAY)
			abort();
		if (mfgClearStencil(renderDevice, 0) != MFG_ERROR_OKAY)
			abort();

		// Update vertex buffer
		{
			struct Vertex* vertexes = NULL;

			if (mfgMapVertexBuffer(renderDevice, vb, &vertexes) != MFG_ERROR_OKAY)
			{
				mfsUTF8CodeUnit err[512];
				mfgGetErrorString(renderDevice, err, sizeof(err));
				mfsPrintFormatUTF8(mfsErrStream, err);
				mfsFlush(mfsErrStream);
				abort();
			}

			vertexes[0].x = -0.5f + x;
			vertexes[0].y = -0.5f;
			vertexes[0].z = 0.0f;
			vertexes[0].w = 1.0f;

			vertexes[1].x = -0.5f + x;
			vertexes[1].y = +0.5f;
			vertexes[1].z = 0.0f;
			vertexes[1].w = 1.0f;

			vertexes[2].x = +0.5f + x;
			vertexes[2].y = +0.5f;
			vertexes[2].z = 0.0f;
			vertexes[2].w = 1.0f;

			vertexes[3].x = +0.5f + x;
			vertexes[3].y = -0.5f;
			vertexes[3].z = 0.0f;
			vertexes[3].w = 1.0f;

			if (mfgUnmapVertexBuffer(renderDevice, vb) != MFG_ERROR_OKAY)
			{
				mfsUTF8CodeUnit err[512];
				mfgGetErrorString(renderDevice, err, sizeof(err));
				mfsPrintFormatUTF8(mfsErrStream, err);
				mfsFlush(mfsErrStream);
				abort();
			}
		}

		// Draw vertex array
		if (mfgSetPipeline(renderDevice, pp) != MFG_ERROR_OKAY)
			abort();
		if (mfgSetVertexArray(renderDevice, va) != MFG_ERROR_OKAY)
			abort();
		if (mfgSetIndexBuffer(renderDevice, ib) != MFG_ERROR_OKAY)
			abort();
		if (mfgDrawTrianglesIndexed(renderDevice, 0, 6) != MFG_ERROR_OKAY)
			abort();

		if (mfgSwapBuffers(renderDevice) != MFG_ERROR_OKAY)
			abort();
	}

	mfgDestroyVertexArray(va);
	mfgDestroyVertexLayout(vl);
	mfgDestroyVertexBuffer(vb);

	mfgDestroyPipeline(pp);
	mfgDestroyPixelShader(ps);
	mfgDestroyVertexShader(vs);

	mfgDestroyOGL4RenderDevice(renderDevice);
	mfiDestroyGLWindow(window);
}
