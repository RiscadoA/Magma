#include <Magma/Framework/Graphics/D3D11RenderDevice.h>
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
mfgConstantBuffer* cb = NULL;
mfgBindingPoint* cbBP = NULL;
mfgTexture2D* tex = NULL;
mfgSampler* sampler = NULL;
mfgBindingPoint* texBP = NULL;
mfgRenderTexture* rt = NULL;
mfgDepthStencilTexture* dst = NULL;
mfgFramebuffer* fb = NULL;

struct Vertex
{
	mfmF32 x;
	mfmF32 y;
	mfmF32 z;
	mfmF32 w;
	mfmF32 u;
	mfmF32 v;
};

void OnClose(void* window)
{
	windowOpen = MFM_FALSE;
}

//#define USE_GL

void Main(int argc, char** argv)
{
	// Open window
	{
#ifdef USE_GL
		if (mfiCreateGLWindow(&window, 800, 600, MFI_WINDOWED, u8"Example-C-6") != MFI_ERROR_OKAY)
			abort();
#else
		if (mfiCreateD3DWindow(&window, 800, 600, MFI_WINDOWED, u8"Example-C-6") != MFI_ERROR_OKAY)
			abort();
#endif
		window->onClose = &OnClose;
	}

	// Create render device
	{
		mfgRenderDeviceDesc desc;
		desc.vsyncEnabled = MFM_TRUE;
#ifdef USE_GL
		if (mfgCreateOGL4RenderDevice(&renderDevice, window, &desc, NULL) != MFG_ERROR_OKAY)
			abort();
#else
		if(mfgCreateD3D11RenderDevice(&renderDevice, window, &desc, NULL) != MFG_ERROR_OKAY)
			abort();
#endif
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
			metaDataB[ptr++] = 2;	// Input var count
			metaDataB[ptr++] = 2;	// Output var count
			metaDataB[ptr++] = 1;	// Binding point count

			// Input var 1
			strcpy(metaDataB + ptr, u8"position");
			ptr += 16;
			metaDataB[ptr++] = 0x00;
			metaDataB[ptr++] = 0x00;	// ID 0
			metaDataB[ptr++] = MFG_FLOAT4;

			// Input var 2
			strcpy(metaDataB + ptr, u8"uvs");
			ptr += 16;
			metaDataB[ptr++] = 0x00;
			metaDataB[ptr++] = 0x03;	// ID 2
			metaDataB[ptr++] = MFG_FLOAT2;

			// Output var 1
			strcpy(metaDataB + ptr, u8"_out0");
			ptr += 16;
			metaDataB[ptr++] = 0x00;
			metaDataB[ptr++] = 0x04;	// ID 4
			metaDataB[ptr++] = MFG_FLOAT2;

			// Output var 2
			strcpy(metaDataB + ptr, u8"_position");
			ptr += 16;
			metaDataB[ptr++] = 0x00;
			metaDataB[ptr++] = 0x01;	// ID 1
			metaDataB[ptr++] = MFG_FLOAT4;


			// Constant buffer
			strcpy(metaDataB + ptr, u8"transform");
			ptr += 16;
			metaDataB[ptr++] = MFG_CONSTANT_BUFFER;
			metaDataB[ptr++] = 0x00;
			metaDataB[ptr++] = 0x00;	// ID 0
			metaDataB[ptr++] = 0x00; // 1 element
			metaDataB[ptr++] = 0x01;
			// Elements:
			{
				metaDataB[ptr++] = 0x00; // Var index 1
				metaDataB[ptr++] = 0x02;
				metaDataB[ptr++] = MFG_FLOAT4;
			}

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
			MFG_BYTECODE_ADD, 0x00, 0x00, 0x00, 0x02, 0x00, 0x01,
			MFG_BYTECODE_GET4CMP, 0x00, 0xFF, 0x00, 0x01, 0x03,
			MFG_BYTECODE_LITI1, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x01,
			MFG_BYTECODE_ASSIGN, 0x00, 0x04, 0x00, 0x03,
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
			metaDataB[ptr++] = 1;	// Input var count
			metaDataB[ptr++] = 1;	// Output var count
			metaDataB[ptr++] = 1;	// Binding point count

			// Input var 1
			strcpy(metaDataB + ptr, u8"_in0");
			ptr += 16;
			metaDataB[ptr++] = 0x00;
			metaDataB[ptr++] = 0x00;	// ID 0
			metaDataB[ptr++] = MFG_FLOAT2;

			// Output var 1
			strcpy(metaDataB + ptr, u8"_target0");
			ptr += 16;
			metaDataB[ptr++] = 0x00;
			metaDataB[ptr++] = 0x01;	// ID 1
			metaDataB[ptr++] = MFG_FLOAT4;

			// Texture 2D
			strcpy(metaDataB + ptr, u8"texture");
			ptr += 16;
			metaDataB[ptr++] = MFG_TEXTURE_2D;
			metaDataB[ptr++] = 0x00;
			metaDataB[ptr++] = 0x02;	// ID 2

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
			
			MFG_BYTECODE_SAMPLE2D, 0x00, 0x02, 0x00, 0x00, 0x00, 0x01,
			/*MFG_BYTECODE_LITI4, 0x00, 0x01,
			0x00, 0x00, 0x00, 0x01,
			0x00, 0x00, 0x00, 0x01,
			0x00, 0x00, 0x00, 0x01,
			0x00, 0x00, 0x00, 0x01,*/
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
	{
		struct Vertex vertexes[4];

		vertexes[0].x = -0.5f;
		vertexes[0].y = -0.5f;
		vertexes[0].z = 0.0f;
		vertexes[0].w = 1.0f;
		vertexes[0].u = 0.0f;
		vertexes[0].v = 0.0f;

		vertexes[1].x = -0.5f;
		vertexes[1].y = +0.5f;
		vertexes[1].z = 0.0f;
		vertexes[1].w = 1.0f;
		vertexes[1].u = 0.0f;
		vertexes[1].v = 1.0f;

		vertexes[2].x = +0.5f;
		vertexes[2].y = +0.5f;
		vertexes[2].z = 0.0f;
		vertexes[2].w = 1.0f;
		vertexes[2].u = 1.0f;
		vertexes[2].v = 1.0f;

		vertexes[3].x = +0.5f;
		vertexes[3].y = -0.5f;
		vertexes[3].z = 0.0f;
		vertexes[3].w = 1.0f;
		vertexes[3].u = 1.0f;
		vertexes[3].v = 0.0f;

		if (mfgCreateVertexBuffer(renderDevice, &vb, sizeof(vertexes), vertexes, MFG_USAGE_STATIC) != MFG_ERROR_OKAY)
		{
			mfsUTF8CodeUnit err[512];
			mfgGetErrorString(renderDevice, err, sizeof(err));
			mfsPrintFormatUTF8(mfsErrStream, err);
			mfsFlush(mfsErrStream);
			abort();
		}
	}

	// Create vertex layout
	{
		mfgVertexElement elements[2];
		
		mfgDefaultVertexElement(&elements[0]);
		mfgDefaultVertexElement(&elements[1]);

		elements[0].bufferIndex = 0;
		strcpy(elements[0].name, u8"position");
		elements[0].offset = offsetof(struct Vertex, x);
		elements[0].stride = sizeof(struct Vertex);
		elements[0].size = 4;
		elements[0].type = MFG_FLOAT;

		elements[1].bufferIndex = 0;
		strcpy(elements[1].name, u8"uvs");
		elements[1].offset = offsetof(struct Vertex, u);
		elements[1].stride = sizeof(struct Vertex);
		elements[1].size = 2;
		elements[1].type = MFG_FLOAT;

		// Create layout
		if (mfgCreateVertexLayout(renderDevice, &vl, 2, elements, vs) != MFG_ERROR_OKAY)
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
		mfmU16 indices[] =
		{
			2, 1, 0,
			0, 3, 2,
		};

		if (mfgCreateIndexBuffer(renderDevice, &ib, sizeof(indices), indices, MFG_USHORT, MFG_USAGE_STATIC) != MFG_ERROR_OKAY)
		{
			mfsUTF8CodeUnit err[512];
			mfgGetErrorString(renderDevice, err, sizeof(err));
			mfsPrintFormatUTF8(mfsErrStream, err);
			mfsFlush(mfsErrStream);
			abort();
		}
	}

	// Create constant buffer
	if (mfgCreateConstantBuffer(renderDevice, &cb, sizeof(float[4]), NULL, MFG_USAGE_DYNAMIC) != MFG_ERROR_OKAY)
	{
		mfsUTF8CodeUnit err[512];
		mfgGetErrorString(renderDevice, err, sizeof(err));
		mfsPrintFormatUTF8(mfsErrStream, err);
		mfsFlush(mfsErrStream);
		abort();
	}

	// Get binding point
	if (mfgGetVertexShaderBindingPoint(renderDevice, &cbBP, vs, u8"transform") != MFG_ERROR_OKAY)
	{
		mfsUTF8CodeUnit err[512];
		mfgGetErrorString(renderDevice, err, sizeof(err));
		mfsPrintFormatUTF8(mfsErrStream, err);
		mfsFlush(mfsErrStream);
		abort();
	}

	// Create texture
	{
		mfmF32 data[] =
		{
			1.0f, 0.0f, 0.0f, 1.0f,		0.0f, 1.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 1.0f, 1.0f,		1.0f, 1.0f, 1.0f, 1.0f,
		};
		
		if (mfgCreateTexture2D(renderDevice, &tex, 2, 2, MFG_RGBA32FLOAT, data, MFG_USAGE_STATIC) != MFG_ERROR_OKAY)
		{
			mfsUTF8CodeUnit err[512];
			mfgGetErrorString(renderDevice, err, sizeof(err));
			mfsPrintFormatUTF8(mfsErrStream, err);
			mfsFlush(mfsErrStream);
			abort();
		}
	}

	// Create sampler
	{
		mfgSamplerDesc desc;
		mfgDefaultSamplerDesc(&desc);

		desc.minFilter = MFG_LINEAR;
		desc.magFilter = MFG_LINEAR;

		if (mfgCreateSampler(renderDevice, &sampler, &desc) != MFG_ERROR_OKAY)
		{
			mfsUTF8CodeUnit err[512];
			mfgGetErrorString(renderDevice, err, sizeof(err));
			mfsPrintFormatUTF8(mfsErrStream, err);
			mfsFlush(mfsErrStream);
			abort();
		}
	}

	// Get binding point
	if (mfgGetPixelShaderBindingPoint(renderDevice, &texBP, ps, u8"texture") != MFG_ERROR_OKAY)
	{
		mfsUTF8CodeUnit err[512];
		mfgGetErrorString(renderDevice, err, sizeof(err));
		mfsPrintFormatUTF8(mfsErrStream, err);
		mfsFlush(mfsErrStream);
		abort();
	}

	// Create render texture
	if (mfgCreateRenderTexture(renderDevice, &rt, 800, 600, MFG_RGBA8UNORM) != MFG_ERROR_OKAY)
	{
		mfsUTF8CodeUnit err[512];
		mfgGetErrorString(renderDevice, err, sizeof(err));
		mfsPrintFormatUTF8(mfsErrStream, err);
		mfsFlush(mfsErrStream);
		abort();
	}

	// Create depth stencil texture
	if (mfgCreateDepthStencilTexture(renderDevice, &dst, 800, 600, MFG_DEPTH24STENCIL8) != MFG_ERROR_OKAY)
	{
		mfsUTF8CodeUnit err[512];
		mfgGetErrorString(renderDevice, err, sizeof(err));
		mfsPrintFormatUTF8(mfsErrStream, err);
		mfsFlush(mfsErrStream);
		abort();
	}
	
	// Create framebuffer
	if (mfgCreateFramebuffer(renderDevice, &fb, 1, &rt, dst) != MFG_ERROR_OKAY)
	{
		mfsUTF8CodeUnit err[512];
		mfgGetErrorString(renderDevice, err, sizeof(err));
		mfsPrintFormatUTF8(mfsErrStream, err);
		mfsFlush(mfsErrStream);
		abort();
	}

	float x = 0.0f;

	while (windowOpen == MFM_TRUE)
	{
		window->pollEvents(window);

		if (mfgSetFramebuffer(renderDevice, fb) != MFG_ERROR_OKAY)
			abort();
		
		x += 0.0001f;

		// Clear

		if (mfgClearColor(renderDevice, 1.0f, 0.0f, 1.0f, 1.0f) != MFG_ERROR_OKAY)
			abort();

		if (mfgSetFramebuffer(renderDevice, NULL) != MFG_ERROR_OKAY)
			abort();

		if (mfgClearColor(renderDevice, 0.0f, 0.0f, 0.2f, 1.0f) != MFG_ERROR_OKAY)
			abort();

		// Render rectangle
		{
			// Update constant buffer
			{
				float* cbData;

				if (mfgMapConstantBuffer(renderDevice, cb, &cbData) != MFG_ERROR_OKAY)
				{
					mfsUTF8CodeUnit err[512];
					mfgGetErrorString(renderDevice, err, sizeof(err));
					mfsPrintFormatUTF8(mfsErrStream, err);
					mfsFlush(mfsErrStream);
					abort();
				}

				cbData[0] = 0.0f + x;
				cbData[1] = 0.0f;
				cbData[2] = 0.0f;
				cbData[3] = 1.0f;
	
				if (mfgUnmapConstantBuffer(renderDevice, cb) != MFG_ERROR_OKAY)
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
			if (mfgBindConstantBuffer(renderDevice, cbBP, cb) != MFG_ERROR_OKAY)
				abort();
			if (mfgBindRenderTexture(renderDevice, texBP, rt) != MFG_ERROR_OKAY)
				abort();
			if (mfgBindSampler(renderDevice, texBP, sampler) != MFG_ERROR_OKAY)
				abort();
			if (mfgSetVertexArray(renderDevice, va) != MFG_ERROR_OKAY)
				abort();
			if (mfgSetIndexBuffer(renderDevice, ib) != MFG_ERROR_OKAY)
				abort();
			if (mfgDrawTrianglesIndexed(renderDevice, 0, 6) != MFG_ERROR_OKAY)
				abort();
			if (mfgBindRenderTexture(renderDevice, texBP, NULL) != MFG_ERROR_OKAY)
				abort();
		}

		if (mfgSwapBuffers(renderDevice) != MFG_ERROR_OKAY)
			abort();
	}

	mfgDestroyFramebuffer(fb);
	mfgDestroyDepthStencilTexture(dst);
	mfgDestroyRenderTexture(rt);

	mfgDestroySampler(sampler);
	mfgDestroyTexture2D(tex);

	mfgDestroyConstantBuffer(cb);

	mfgDestroyIndexBuffer(ib);
	mfgDestroyVertexArray(va);
	mfgDestroyVertexLayout(vl);
	mfgDestroyVertexBuffer(vb);

	mfgDestroyPipeline(pp);
	mfgDestroyPixelShader(ps);
	mfgDestroyVertexShader(vs);

#ifdef USE_GL
	mfgDestroyOGL4RenderDevice(renderDevice);
	mfiDestroyGLWindow(window);
#else
	mfgDestroyD3D11RenderDevice(renderDevice);
	mfiDestroyD3DWindow(window);
#endif
}
