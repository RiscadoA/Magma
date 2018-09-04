#include <Magma/Framework/Graphics/2.X/D3D11RenderDevice.h>
#include <Magma/Framework/Graphics/2.X/OGL4RenderDevice.h>
#include <Magma/Framework/Graphics/2.X/MSL/Compiler.h>
#include <Magma/Framework/String/UTF8.h>
#include <Magma/Framework/String/Stream.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

mfiWindow* window = NULL;
mfgV2XRenderDevice* renderDevice = NULL;

mfmBool windowOpen = MFM_TRUE;

mfgV2XVertexShader* vs = NULL;
mfgV2XPixelShader* ps = NULL;
mfgV2XPipeline* pp = NULL;
mfgV2XVertexBuffer* vb = NULL;
mfgV2XVertexLayout* vl = NULL;
mfgV2XVertexArray* va = NULL;
mfgV2XIndexBuffer* ib = NULL;
mfgV2XConstantBuffer* cb = NULL;
mfgV2XBindingPoint* cbBP = NULL;
mfgV2XTexture2D* tex = NULL;
mfgV2XSampler* sampler = NULL;
mfgV2XBindingPoint* texBP = NULL;
mfgV2XRenderTexture* rt = NULL;
mfgV2XDepthStencilTexture* dst = NULL;
mfgV2XFramebuffer* fb = NULL;

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

int main(int argc, const char** argv)
{
	if (mfInit(argc, argv) != MF_ERROR_OKAY)
		abort();

	// Open window
	{
#ifdef USE_GL
		if (mfiCreateOGLWindow(&window, 800, 600, MFI_WINDOWED, u8"Example-C-6") != MF_ERROR_OKAY)
			abort();
#else
		if (mfiCreateD3DWindow(&window, 800, 600, MFI_WINDOWED, u8"Example-C-6") != MF_ERROR_OKAY)
			abort();
#endif
		window->onClose = &OnClose;
	}

	// Create render device
	{
		mfgV2XRenderDeviceDesc desc;
		desc.vsyncEnabled = MFM_TRUE;
#ifdef USE_GL
		if (mfgV2XCreateOGL4RenderDevice(&renderDevice, window, &desc, NULL) != MF_ERROR_OKAY)
			abort();
#else
		if(mfgV2XCreateD3D11RenderDevice(&renderDevice, window, &desc, NULL) != MF_ERROR_OKAY)
			abort();
#endif
	}

	// Load vertex shader
	{
		mfgMetaData* metaData = NULL;

		const mfmU8* src =
			u8"Input"
			u8"{"
			u8"		float4 position : position;"
			u8"		float2 uvs : uvs;"
			u8"};"

			u8"Output"
			u8"{"
			u8"		float2 uvs : _out0;"
			u8"		float4 position : _position;"
			u8"};"

			u8"ConstantBuffer buffer : transform"
			u8"{"
			u8"		float4 offset;"
			u8"};"

			u8"void main()"
			u8"{"
			u8"		Output.uvs = Input.uvs;"
			u8"		Output.position = Input.position + buffer.offset;"
			u8"		Output.position.w = 1.0f;"
			u8"}"
			;


		mfmU8 bytecode[256];
		mfmU64 bytecodeSize = 0;
		mfmU8 metaDataB[256];
		mfmU64 metaDataSize = 0;

		{
			mfgV2XMVLCompilerInfo info;
			if (mfgV2XRunMVLCompiler(src, bytecode, sizeof(bytecode), metaDataB, sizeof(metaDataB), MFG_VERTEX_SHADER, &info) != MF_ERROR_OKAY)
			{
				mfsPutString(mfsErrStream, info.errorMsg);
				abort();
			}
			bytecodeSize = info.bytecodeSize;
			metaDataSize = info.metaDataSize;
		}

		if (mfgLoadMetaData(metaDataB, sizeof(metaDataB), &metaData, NULL) != MF_ERROR_OKAY)
			abort();

		// Create shader
		if (mfgV2XCreateVertexShader(renderDevice, &vs, bytecode, bytecodeSize, metaData) != MF_ERROR_OKAY)
		{
			mfsUTF8CodeUnit err[512];
			mfgV2XGetErrorString(renderDevice, err, sizeof(err));
			mfsPrintFormat(mfsErrStream, err);
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
			if (mfgLoadMetaData(metaDataB, sizeof(metaDataB), &metaData, NULL) != MF_ERROR_OKAY)
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
		if (mfgV2XCreatePixelShader(renderDevice, &ps, bytecode, sizeof(bytecode), metaData) != MF_ERROR_OKAY)
		{
			mfsUTF8CodeUnit err[512];
			mfgV2XGetErrorString(renderDevice, err, sizeof(err));
			mfsPrintFormat(mfsErrStream, err);
			mfsFlush(mfsErrStream);
			abort();
		}
	}

	// Create pipeline
	if (mfgV2XCreatePipeline(renderDevice, &pp, vs, ps) != MF_ERROR_OKAY)
	{
		mfsUTF8CodeUnit err[512];
		mfgV2XGetErrorString(renderDevice, err, sizeof(err));
		mfsPrintFormat(mfsErrStream, err);
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

		if (mfgV2XCreateVertexBuffer(renderDevice, &vb, sizeof(vertexes), vertexes, MFG_USAGE_STATIC) != MF_ERROR_OKAY)
		{
			mfsUTF8CodeUnit err[512];
			mfgV2XGetErrorString(renderDevice, err, sizeof(err));
			mfsPrintFormat(mfsErrStream, err);
			mfsFlush(mfsErrStream);
			abort();
		}
	}

	// Create vertex layout
	{
		mfgV2XVertexElement elements[2];
		
		mfgV2XDefaultVertexElement(&elements[0]);
		mfgV2XDefaultVertexElement(&elements[1]);

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
		if (mfgV2XCreateVertexLayout(renderDevice, &vl, 2, elements, vs) != MF_ERROR_OKAY)
		{
			mfsUTF8CodeUnit err[512];
			mfgV2XGetErrorString(renderDevice, err, sizeof(err));
			mfsPrintFormat(mfsErrStream, err);
			mfsFlush(mfsErrStream);
			abort();
		}
	}

	// Create vertex array
	if (mfgV2XCreateVertexArray(renderDevice, &va, 1, &vb, vl) != MF_ERROR_OKAY)
	{
		mfsUTF8CodeUnit err[512];
		mfgV2XGetErrorString(renderDevice, err, sizeof(err));
		mfsPrintFormat(mfsErrStream, err);
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

		if (mfgV2XCreateIndexBuffer(renderDevice, &ib, sizeof(indices), indices, MFG_USHORT, MFG_USAGE_STATIC) != MF_ERROR_OKAY)
		{
			mfsUTF8CodeUnit err[512];
			mfgV2XGetErrorString(renderDevice, err, sizeof(err));
			mfsPrintFormat(mfsErrStream, err);
			mfsFlush(mfsErrStream);
			abort();
		}
	}

	// Create constant buffer
	if (mfgV2XCreateConstantBuffer(renderDevice, &cb, sizeof(float[4]), NULL, MFG_USAGE_DYNAMIC) != MF_ERROR_OKAY)
	{
		mfsUTF8CodeUnit err[512];
		mfgV2XGetErrorString(renderDevice, err, sizeof(err));
		mfsPrintFormat(mfsErrStream, err);
		mfsFlush(mfsErrStream);
		abort();
	}

	// Get binding point
	if (mfgV2XGetVertexShaderBindingPoint(renderDevice, &cbBP, vs, u8"transform") != MF_ERROR_OKAY)
	{
		mfsUTF8CodeUnit err[512];
		mfgV2XGetErrorString(renderDevice, err, sizeof(err));
		mfsPrintFormat(mfsErrStream, err);
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
		
		if (mfgV2XCreateTexture2D(renderDevice, &tex, 2, 2, MFG_RGBA32FLOAT, data, MFG_USAGE_STATIC) != MF_ERROR_OKAY)
		{
			mfsUTF8CodeUnit err[512];
			mfgV2XGetErrorString(renderDevice, err, sizeof(err));
			mfsPrintFormat(mfsErrStream, err);
			mfsFlush(mfsErrStream);
			abort();
		}
	}

	// Create sampler
	{
		mfgV2XSamplerDesc desc;
		mfgV2XDefaultSamplerDesc(&desc);

		desc.minFilter = MFG_LINEAR;
		desc.magFilter = MFG_LINEAR;

		if (mfgV2XCreateSampler(renderDevice, &sampler, &desc) != MF_ERROR_OKAY)
		{
			mfsUTF8CodeUnit err[512];
			mfgV2XGetErrorString(renderDevice, err, sizeof(err));
			mfsPrintFormat(mfsErrStream, err);
			mfsFlush(mfsErrStream);
			abort();
		}
	}

	// Get binding point
	if (mfgV2XGetPixelShaderBindingPoint(renderDevice, &texBP, ps, u8"texture") != MF_ERROR_OKAY)
	{
		mfsUTF8CodeUnit err[512];
		mfgV2XGetErrorString(renderDevice, err, sizeof(err));
		mfsPrintFormat(mfsErrStream, err);
		mfsFlush(mfsErrStream);
		abort();
	}

	// Create render texture
	if (mfgV2XCreateRenderTexture(renderDevice, &rt, 800, 600, MFG_RGBA8UNORM) != MF_ERROR_OKAY)
	{
		mfsUTF8CodeUnit err[512];
		mfgV2XGetErrorString(renderDevice, err, sizeof(err));
		mfsPrintFormat(mfsErrStream, err);
		mfsFlush(mfsErrStream);
		abort();
	}

	// Create depth stencil texture
	if (mfgV2XCreateDepthStencilTexture(renderDevice, &dst, 800, 600, MFG_DEPTH24STENCIL8) != MF_ERROR_OKAY)
	{
		mfsUTF8CodeUnit err[512];
		mfgV2XGetErrorString(renderDevice, err, sizeof(err));
		mfsPrintFormat(mfsErrStream, err);
		mfsFlush(mfsErrStream);
		abort();
	}
	
	// Create framebuffer
	if (mfgV2XCreateFramebuffer(renderDevice, &fb, 1, &rt, dst) != MF_ERROR_OKAY)
	{
		mfsUTF8CodeUnit err[512];
		mfgV2XGetErrorString(renderDevice, err, sizeof(err));
		mfsPrintFormat(mfsErrStream, err);
		mfsFlush(mfsErrStream);
		abort();
	}

	float x = 0.0f;

	while (windowOpen == MFM_TRUE)
	{
		window->pollEvents(window);

		if (mfgV2XSetFramebuffer(renderDevice, fb) != MF_ERROR_OKAY)
			abort();
		
		x += 0.0001f;

		// Clear

		if (mfgV2XClearColor(renderDevice, 1.0f, 0.0f, 1.0f, 1.0f) != MF_ERROR_OKAY)
			abort();

		if (mfgV2XSetFramebuffer(renderDevice, NULL) != MF_ERROR_OKAY)
			abort();

		if (mfgV2XClearColor(renderDevice, 0.0f, 0.0f, 0.2f, 1.0f) != MF_ERROR_OKAY)
			abort();

		// Render rectangle
		{
			// Update constant buffer
			{
				float* cbData;

				if (mfgV2XMapConstantBuffer(renderDevice, cb, &cbData) != MF_ERROR_OKAY)
				{
					mfsUTF8CodeUnit err[512];
					mfgV2XGetErrorString(renderDevice, err, sizeof(err));
					mfsPrintFormat(mfsErrStream, err);
					mfsFlush(mfsErrStream);
					abort();
				}

				cbData[0] = 0.0f + x;
				cbData[1] = 0.0f;
				cbData[2] = 0.0f;
				cbData[3] = 1.0f;
	
				if (mfgV2XUnmapConstantBuffer(renderDevice, cb) != MF_ERROR_OKAY)
				{
					mfsUTF8CodeUnit err[512];
					mfgV2XGetErrorString(renderDevice, err, sizeof(err));
					mfsPrintFormat(mfsErrStream, err);
					mfsFlush(mfsErrStream);
					abort();
				}
			}

			// Draw vertex array
			if (mfgV2XSetPipeline(renderDevice, pp) != MF_ERROR_OKAY)
				abort();
			if (mfgV2XBindConstantBuffer(renderDevice, cbBP, cb) != MF_ERROR_OKAY)
				abort();
			if (mfgV2XBindRenderTexture(renderDevice, texBP, rt) != MF_ERROR_OKAY)
				abort();
			if (mfgV2XBindSampler(renderDevice, texBP, sampler) != MF_ERROR_OKAY)
				abort();
			if (mfgV2XSetVertexArray(renderDevice, va) != MF_ERROR_OKAY)
				abort();
			if (mfgV2XSetIndexBuffer(renderDevice, ib) != MF_ERROR_OKAY)
				abort();
			if (mfgV2XDrawTrianglesIndexed(renderDevice, 0, 6) != MF_ERROR_OKAY)
				abort();
			if (mfgV2XBindRenderTexture(renderDevice, texBP, NULL) != MF_ERROR_OKAY)
				abort();
		}

		if (mfgV2XSwapBuffers(renderDevice) != MF_ERROR_OKAY)
			abort();
	}

	if (mfgV2XBindConstantBuffer(renderDevice, cbBP, NULL) != MF_ERROR_OKAY)
		abort();
	if (mfgV2XSetVertexArray(renderDevice, NULL) != MF_ERROR_OKAY)
		abort();
	if (mfgV2XSetIndexBuffer(renderDevice, NULL) != MF_ERROR_OKAY)
		abort();
	if (mfgV2XSetPipeline(renderDevice, NULL) != MF_ERROR_OKAY)
		abort();

	mfgV2XDestroyFramebuffer(fb);
	mfgV2XDestroyDepthStencilTexture(dst);
	mfgV2XDestroyRenderTexture(rt);

	mfgV2XDestroySampler(sampler);
	mfgV2XDestroyTexture2D(tex);

	mfgV2XDestroyConstantBuffer(cb);

	mfgV2XDestroyIndexBuffer(ib);
	mfgV2XDestroyVertexArray(va);
	mfgV2XDestroyVertexLayout(vl);
	mfgV2XDestroyVertexBuffer(vb);

	mfgV2XDestroyPipeline(pp);
	mfgV2XDestroyPixelShader(ps);
	mfgV2XDestroyVertexShader(vs);

#ifdef USE_GL
	mfgV2XDestroyOGL4RenderDevice(renderDevice);
	mfiDestroyGLWindow(window);
#else
	mfgV2XDestroyD3D11RenderDevice(renderDevice);
	mfiDestroyD3DWindow(window);
#endif

	mfTerminate();
	return 0;
}
