#include "D3D11RenderDevice.h"
#include "D3D11Assembler.h"
#include "Config.h"

#include "../Memory/StackAllocator.h"
#include "../Memory/PoolAllocator.h"
#include "../String/StringStream.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef MAGMA_FRAMEWORK_USE_DIRECTX

#include <windows.h>
#include <windowsx.h>
#include <d3d11.h>
#include <d3dcompiler.h>

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")
#pragma comment (lib, "dxguid.lib")

#define MFG_POOL_32_ELEMENT_COUNT 2048
#define MFG_POOL_64_ELEMENT_COUNT 2048
#define MFG_POOL_256_ELEMENT_COUNT 2048
#define MFG_POOL_512_ELEMENT_COUNT 2048

typedef struct mfgD3D11Shader mfgD3D11Shader;

typedef struct
{
	const mfgMetaDataBindingPoint* bp;



	mfgD3D11Shader* shader;
} mfgD3D11BindingPoint;

struct mfgD3D11Shader
{
	mfgRenderDeviceObject base;
	
	const mfgMetaData* md;
	mfgD3D11BindingPoint bps[16];
};

typedef struct
{
	mfgRenderDeviceObject base;
	
} mfgD3D11Pipeline;

typedef struct
{
	mfgRenderDeviceObject base;
	
} mfgD3D11ConstantBuffer;

typedef struct
{
	mfgRenderDeviceObject base;

} mfgD3D11Texture1D;

typedef struct
{
	mfgRenderDeviceObject base;

} mfgD3D11Texture2D;

typedef struct
{
	mfgRenderDeviceObject base;

} mfgD3D11Texture3D;

typedef struct
{
	mfgRenderDeviceObject base;
	
} mfgD3D11Sampler;

typedef struct
{
	mfgRenderDeviceObject base;

} mfgD3D11RenderTexture;

typedef struct
{
	mfgRenderDeviceObject base;

} mfgD3D11DepthStencilTexture;

typedef struct
{
	mfgRenderDeviceObject base;

} mfgD3D11Framebuffer;

typedef struct
{
	mfgRenderDeviceObject base;
	
} mfgD3D11VertexBuffer;

typedef struct
{
	mfgRenderDeviceObject base;
	

} mfgD3D11VertexLayout;

typedef struct
{
	mfgRenderDeviceObject base;
	
} mfgD3D11VertexArray;

typedef struct
{
	mfgRenderDeviceObject base;
	
} mfgD3D11IndexBuffer;

typedef struct
{
	mfgRenderDeviceObject base;
	ID3D11RasterizerState* state;
} mfgD3D11RasterState;

typedef struct
{
	mfgRenderDeviceObject base;
	ID3D11DepthStencilState* state;
	mfmU32 stencilRef;
} mfgD3D11DepthStencilState;

typedef struct
{
	mfgRenderDeviceObject base;
	ID3D11BlendState* state;
} mfgD3D11BlendState;

typedef struct
{
	mfgRenderDevice base;

	mfiWindow* window;

	void* allocator;

	mfsUTF8CodeUnit errorString[256];
	mfmU64 errorStringSize;

	mfmPoolAllocator* pool32;
	mfmU8 pool32Memory[MFM_POOL_ALLOCATOR_SIZE(MFG_POOL_32_ELEMENT_COUNT, 32)];

	mfmPoolAllocator* pool64;
	mfmU8 pool64Memory[MFM_POOL_ALLOCATOR_SIZE(MFG_POOL_64_ELEMENT_COUNT, 64)];

	mfmPoolAllocator* pool256;
	mfmU8 pool256Memory[MFM_POOL_ALLOCATOR_SIZE(MFG_POOL_256_ELEMENT_COUNT, 256)];

	mfmPoolAllocator* pool512;
	mfmU8 pool512Memory[MFM_POOL_ALLOCATOR_SIZE(MFG_POOL_512_ELEMENT_COUNT, 512)];

	mfmStackAllocator* stack;
	mfmU8 stackMemory[MFM_STACK_ALLOCATOR_SIZE(2048)];

	mfgRasterState* defaultRasterState;
	mfgDepthStencilState* defaultDepthStencilState;
	mfgBlendState* defaultBlendState;

	mfgD3D11IndexBuffer* currentIndexBuffer;

	IDXGISwapChain* swapChain;
	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;

	ID3D11Texture2D* backBuffer;
	ID3D11RenderTargetView* defaultRenderTargetView;
	ID3D11RenderTargetView** renderTargetViews;
	mfmU64 renderTargetCount;

	ID3D11Texture2D* depthStencilBuffer;
	ID3D11DepthStencilView* defaultDepthStencilView;
	ID3D11DepthStencilView* depthStencilView;

} mfgD3D11RenderDevice;

#define MFG_RETURN_ERROR(code, msg) {\
	mfgD3D11RenderDevice* d3dRD = (mfgD3D11RenderDevice*)rd;\
	d3dRD->errorStringSize = snprintf(d3dRD->errorString, sizeof(d3dRD->errorString),\
									 msg) + 1;\
	if (d3dRD->errorStringSize > sizeof(d3dRD->errorString))\
	d3dRD->errorStringSize = sizeof(d3dRD->errorString);\
	return code; }

#ifdef MAGMA_FRAMEWORK_DEBUG
#define MFG_CHECK_D3D_ERROR(hr) do {\
if (FAILED(hr)) {\
	MFG_RETURN_ERROR(MFG_ERROR_INTERNAL, u8"DirectX error on " __FUNCTION__)\
}} while (0)
#else
#define MFG_CHECK_GL_ERROR()
#endif

void mfgD3D11DestroyVertexShader(void* vs)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (vs == NULL) abort();
#endif

}

mfgError mfgD3D11CreateVertexShader(mfgRenderDevice* rd, mfgVertexShader** vs, const mfmU8* bytecode, mfmU64 bytecodeSize, const mfgMetaData* metaData)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{ if (rd == NULL || vs == NULL || bytecode == NULL || metaData == NULL) return MFG_ERROR_INVALID_ARGUMENTS; }
#endif

	mfgD3D11RenderDevice* d3dRD = (mfgD3D11RenderDevice*)rd;

	return MFG_ERROR_OKAY;
}

mfgError mfgD3D11GetVertexShaderBindingPoint(mfgRenderDevice* rd, mfgBindingPoint** bp, mfgVertexShader* vs, const mfsUTF8CodeUnit* name)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (rd == NULL || bp == NULL || vs == NULL || name == NULL) return MFG_ERROR_INVALID_ARGUMENTS;
#endif
	mfgD3D11Shader* d3dVS = vs;

	/*for (mfmU64 i = 0; i < 16; ++i)
		if (d3dVS->bps[i].active == GL_TRUE)
		{
			if (strcmp(name, d3dVS->bps[i].bp->name) == 0)
			{
				*bp = &d3dVS->bps[i];
				return MFG_ERROR_OKAY;
			}
		}*/

	return MFG_ERROR_NOT_FOUND;
}

void mfgD3D11DestroyPixelShader(void* ps)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (ps == NULL) abort();
#endif

}


mfgError mfgD3D11CreatePixelShader(mfgRenderDevice* rd, mfgPixelShader** ps, const mfmU8* bytecode, mfmU64 bytecodeSize, const mfgMetaData* metaData)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{ if (rd == NULL || ps == NULL || bytecode == NULL || metaData == NULL) return MFG_ERROR_INVALID_ARGUMENTS; }
#endif

	mfgD3D11RenderDevice* d3dRD = (mfgD3D11RenderDevice*)rd;

	return MFG_ERROR_OKAY;
}

mfgError mfgD3D11GetPixelShaderBindingPoint(mfgRenderDevice* rd, mfgBindingPoint** bp, mfgPixelShader* ps, const mfsUTF8CodeUnit* name)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (rd == NULL || bp == NULL || ps == NULL || name == NULL) return MFG_ERROR_INVALID_ARGUMENTS;
#endif

	return MFG_ERROR_NOT_FOUND;
}

mfgError mfgD3D11BindConstantBuffer(mfgRenderDevice* rd, mfgBindingPoint* bp, mfgConstantBuffer* cb)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (rd == NULL || bp == NULL) return MFG_ERROR_INVALID_ARGUMENTS;
#endif
	
	return MFG_ERROR_OKAY;
}

mfgError mfgD3D11BindConstantBufferRange(mfgRenderDevice* rd, mfgBindingPoint* bp, mfgConstantBuffer* cb, mfmU64 offset, mfmU64 size)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (rd == NULL || bp == NULL || cb == NULL) return MFG_ERROR_INVALID_ARGUMENTS;
#endif
	
	return MFG_ERROR_OKAY;
}

mfgError mfgD3D11BindTexture1D(mfgRenderDevice* rd, mfgBindingPoint* bp, mfgTexture1D* tex)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (rd == NULL || bp == NULL) return MFG_ERROR_INVALID_ARGUMENTS;
#endif
	
	return MFG_ERROR_OKAY;
}

mfgError mfgD3D11BindTexture2D(mfgRenderDevice* rd, mfgBindingPoint* bp, mfgTexture2D* tex)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (rd == NULL || bp == NULL) return MFG_ERROR_INVALID_ARGUMENTS;
#endif
	
	return MFG_ERROR_OKAY;
}

mfgError mfgD3D11BindTexture3D(mfgRenderDevice* rd, mfgBindingPoint* bp, mfgTexture3D* tex)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (rd == NULL || bp == NULL) return MFG_ERROR_INVALID_ARGUMENTS;
#endif
	
	return MFG_ERROR_OKAY;
}

mfgError mfgD3D11BindRenderTexture(mfgRenderDevice* rd, mfgBindingPoint* bp, mfgRenderTexture* tex)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (rd == NULL || bp == NULL) return MFG_ERROR_INVALID_ARGUMENTS;
#endif
	
	return MFG_ERROR_OKAY;
}

mfgError mfgD3D11BindSampler(mfgRenderDevice* rd, mfgBindingPoint* bp, mfgSampler* sampler)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (rd == NULL || bp == NULL) return MFG_ERROR_INVALID_ARGUMENTS;
#endif
	
	return MFG_ERROR_OKAY;
}

void mfgD3D11DestroyPipeline(void* pp)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (pp == NULL) abort();
#endif
	
}

mfgError mfgD3D11CreatePipeline(mfgRenderDevice* rd, mfgPipeline** pp, mfgVertexShader* vs, mfgPixelShader* ps)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{ if (rd == NULL || pp == NULL || vs == NULL || ps == NULL) return MFG_ERROR_INVALID_ARGUMENTS; }
#endif

	mfgD3D11RenderDevice* d3dRD = (mfgD3D11RenderDevice*)rd;



	return MFG_ERROR_OKAY;
}

mfgError mfgD3D11SetPipeline(mfgRenderDevice* rd, mfgPipeline* pp)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{ if (rd == NULL) return MFG_ERROR_INVALID_ARGUMENTS; }
#endif

	mfgD3D11RenderDevice* d3dRD = (mfgD3D11RenderDevice*)rd;

	
	return MFG_ERROR_OKAY;
}

void mfgD3D11DestroyConstantBuffer(void* buffer)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (buffer == NULL) abort();
#endif
	
}

mfgError mfgD3D11CreateConstantBuffer(mfgRenderDevice* rd, mfgConstantBuffer** cb, mfmU64 size, const void* data, mfgEnum usage)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{ if (rd == NULL || cb == NULL) return MFG_ERROR_INVALID_ARGUMENTS; }
#endif

	mfgD3D11RenderDevice* d3dRD = (mfgD3D11RenderDevice*)rd;


	return MFG_ERROR_OKAY;
}

mfgError mfgD3D11MapConstantBuffer(mfgRenderDevice* rd, mfgConstantBuffer* cb, void** memory)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{ if (rd == NULL || cb == NULL || memory == NULL) return MFG_ERROR_INVALID_ARGUMENTS; }
#endif

	mfgD3D11RenderDevice* d3dRD = (mfgD3D11RenderDevice*)rd;

	
	return MFG_ERROR_OKAY;
}

mfgError mfgD3D11UnmapConstantBuffer(mfgRenderDevice* rd, mfgConstantBuffer* cb)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{ if (rd == NULL || cb == NULL) return MFG_ERROR_INVALID_ARGUMENTS; }
#endif

	mfgD3D11RenderDevice* d3dRD = (mfgD3D11RenderDevice*)rd;

	return MFG_ERROR_OKAY;
}

void mfgD3D11DestroyVertexBuffer(void* buffer)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (buffer == NULL) abort();
#endif
	
}

mfgError mfgD3D11CreateVertexBuffer(mfgRenderDevice* rd, mfgVertexBuffer** vb, mfmU64 size, const void* data, mfgEnum usage)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (rd == NULL || vb == NULL) return MFG_ERROR_INVALID_ARGUMENTS;
#endif

	return MFG_ERROR_OKAY;
}


mfgError mfgD3D11MapVertexBuffer(mfgRenderDevice* rd, mfgVertexBuffer* vb, void** memory)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
		if (rd == NULL || vb == NULL || memory == NULL) return MFG_ERROR_INVALID_ARGUMENTS;
#endif

	mfgD3D11RenderDevice* d3dRD = (mfgD3D11RenderDevice*)rd;

	return MFG_ERROR_OKAY;
}

mfgError mfgD3D11UnmapVertexBuffer(mfgRenderDevice* rd, mfgVertexBuffer* vb)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (rd == NULL || vb == NULL) return MFG_ERROR_INVALID_ARGUMENTS;
#endif

	mfgD3D11RenderDevice* d3dRD = (mfgD3D11RenderDevice*)rd;

	return MFG_ERROR_OKAY;
}

void mfgD3D11DestroyIndexBuffer(void* buffer)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (buffer == NULL) abort();
#endif
	
}

mfgError mfgD3D11CreateIndexBuffer(mfgRenderDevice* rd, mfgIndexBuffer** ib, mfmU64 size, const void* data, mfgEnum format, mfgEnum usage)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (rd == NULL || ib == NULL) return MFG_ERROR_INVALID_ARGUMENTS;
#endif

	mfgD3D11RenderDevice* d3dRD = (mfgD3D11RenderDevice*)rd;

	return MFG_ERROR_OKAY;
}


mfgError mfgD3D11MapIndexBuffer(mfgRenderDevice* rd, mfgIndexBuffer* ib, void** memory)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (rd == NULL || ib == NULL || memory == NULL) return MFG_ERROR_INVALID_ARGUMENTS;
#endif

	mfgD3D11RenderDevice* d3dRD = (mfgD3D11RenderDevice*)rd;

	return MFG_ERROR_OKAY;
}

mfgError mfgD3D11UnmapIndexBuffer(mfgRenderDevice* rd, mfgIndexBuffer* ib)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (rd == NULL || ib == NULL) return MFG_ERROR_INVALID_ARGUMENTS;
#endif

	mfgD3D11RenderDevice* d3dRD = (mfgD3D11RenderDevice*)rd;

	return MFG_ERROR_OKAY;
}

mfgError mfgD3D11SetIndexBuffer(mfgRenderDevice* rd, mfgIndexBuffer* ib)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (rd == NULL) return MFG_ERROR_INVALID_ARGUMENTS;
#endif

	mfgD3D11RenderDevice* d3dRD = (mfgD3D11RenderDevice*)rd;

	return MFG_ERROR_OKAY;
}

void mfgD3D11DestroyVertexLayout(void* vl)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (vl == NULL) abort();
#endif

}

mfgError mfgD3D11CreateVertexLayout(mfgRenderDevice* rd, mfgVertexLayout** vl, mfmU64 elementCount, const mfgVertexElement* elements, mfgVertexShader* vs)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{ if (rd == NULL || vl == NULL || elementCount == 0 || elementCount > 14 || vs == NULL || elements == NULL) return MFG_ERROR_INVALID_ARGUMENTS; }
#endif

	mfgD3D11RenderDevice* d3dRD = (mfgD3D11RenderDevice*)rd;

	return MFG_ERROR_OKAY;
}

void mfgD3D11DestroyVertexArray(void* va)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (va == NULL) abort();
#endif
}

mfgError mfgD3D11CreateVertexArray(mfgRenderDevice* rd, mfgVertexArray** va, mfmU64 bufferCount, mfgVertexBuffer** buffers, mfgVertexLayout* vl)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{ if (rd == NULL || va == NULL || bufferCount == 0 || vl == NULL || buffers == NULL) return MFG_ERROR_INVALID_ARGUMENTS; }
#endif

	mfgD3D11RenderDevice* d3dRD = (mfgD3D11RenderDevice*)rd;

	return MFG_ERROR_OKAY;
}

mfgError mfgD3D11SetVertexArray(mfgRenderDevice* rd, mfgVertexArray* va)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{ if (rd == NULL || va == NULL) return MFG_ERROR_INVALID_ARGUMENTS; }
#endif

	mfgD3D11RenderDevice* d3dRD = (mfgD3D11RenderDevice*)rd;

	return MFG_ERROR_OKAY;
}


void mfgD3D11DestroyTexture1D(void* tex)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (tex == NULL) abort();
#endif
}

mfgError mfgD3D11CreateTexture1D(mfgRenderDevice* rd, mfgTexture1D** tex, mfmU64 width, mfgEnum format, const void* data, mfgEnum usage)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{ if (rd == NULL || tex == NULL) return MFG_ERROR_INVALID_ARGUMENTS; }
#endif

	return MFG_ERROR_OKAY;
}

mfgError mfgD3D11UpdateTexture1D(mfgRenderDevice* rd, mfgTexture1D* tex, mfmU64 dstX, mfmU64 width, const void* data)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{ if (rd == NULL || tex == NULL || data == NULL) return MFG_ERROR_INVALID_ARGUMENTS; }
#endif

	return MFG_ERROR_OKAY;
}

mfgError mfgD3D11GenerateTexture1DMipmaps(mfgRenderDevice* rd, mfgTexture1D* tex)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{ if (rd == NULL || tex == NULL) return MFG_ERROR_INVALID_ARGUMENTS; }
#endif

	return MFG_ERROR_OKAY;
}

void mfgD3D11DestroyTexture2D(void* tex)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (tex == NULL) abort();
#endif
}

mfgError mfgD3D11CreateTexture2D(mfgRenderDevice* rd, mfgTexture2D** tex, mfmU64 width, mfmU64 height, mfgEnum format, const void* data, mfgEnum usage)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{ if (rd == NULL || tex == NULL) return MFG_ERROR_INVALID_ARGUMENTS; }
#endif

	return MFG_ERROR_OKAY;
}

mfgError mfgD3D11UpdateTexture2D(mfgRenderDevice* rd, mfgTexture2D* tex, mfmU64 dstX, mfmU64 dstY, mfmU64 width, mfmU64 height, const void* data)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{ if (rd == NULL || tex == NULL || data == NULL) return MFG_ERROR_INVALID_ARGUMENTS; }
#endif

	mfgD3D11RenderDevice* d3dRD = (mfgD3D11RenderDevice*)rd;

	return MFG_ERROR_OKAY;
}

mfgError mfgD3D11GenerateTexture2DMipmaps(mfgRenderDevice* rd, mfgTexture2D* tex)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{ if (rd == NULL || tex == NULL) return MFG_ERROR_INVALID_ARGUMENTS; }
#endif

	mfgD3D11RenderDevice* d3dRD = (mfgD3D11RenderDevice*)rd;

	return MFG_ERROR_OKAY;
}

void mfgD3D11DestroyTexture3D(void* tex)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (tex == NULL) abort();
#endif

}

mfgError mfgD3D11CreateTexture3D(mfgRenderDevice* rd, mfgTexture3D** tex, mfmU64 width, mfmU64 height, mfmU64 depth, mfgEnum format, const void* data, mfgEnum usage)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{ if (rd == NULL || tex == NULL) return MFG_ERROR_INVALID_ARGUMENTS; }
#endif

	mfgD3D11RenderDevice* d3dRD = (mfgD3D11RenderDevice*)rd;

	return MFG_ERROR_OKAY;
}

mfgError mfgD3D11UpdateTexture3D(mfgRenderDevice* rd, mfgTexture3D* tex, mfmU64 dstX, mfmU64 dstY, mfmU64 dstZ, mfmU64 width, mfmU64 height, mfmU64 depth, const void* data)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{ if (rd == NULL || tex == NULL || data == NULL) return MFG_ERROR_INVALID_ARGUMENTS; }
#endif

	mfgD3D11RenderDevice* d3dRD = (mfgD3D11RenderDevice*)rd;

	return MFG_ERROR_OKAY;
}

mfgError mfgD3D11GenerateTexture3DMipmaps(mfgRenderDevice* rd, mfgTexture3D* tex)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{ if (rd == NULL || tex == NULL) return MFG_ERROR_INVALID_ARGUMENTS; }
#endif

	mfgD3D11RenderDevice* d3dRD = (mfgD3D11RenderDevice*)rd;

	return MFG_ERROR_OKAY;
}

void mfgD3D11DestroySampler(void* sampler)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (sampler == NULL) abort();
#endif
}

mfgError mfgD3D11CreateSampler(mfgRenderDevice* rd, mfgSampler** sampler, const mfgSamplerDesc* desc)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{ if (rd == NULL || sampler == NULL || desc == NULL) return MFG_ERROR_INVALID_ARGUMENTS; }
#endif

	mfgD3D11RenderDevice* d3dRD = (mfgD3D11RenderDevice*)rd;

	return MFG_ERROR_OKAY;
}

void mfgD3D11DestroyRenderTexture(void* tex)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (tex == NULL) abort();
#endif
}

mfgError mfgD3D11CreateRenderTexture(mfgRenderDevice* rd, mfgRenderTexture** tex, mfmU64 width, mfmU64 height, mfgEnum format)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{ if (rd == NULL || tex == NULL) return MFG_ERROR_INVALID_ARGUMENTS; }
#endif

	mfgD3D11RenderDevice* d3dRD = (mfgD3D11RenderDevice*)rd;

	return MFG_ERROR_OKAY;
}

void mfgD3D11DestroyDepthStencilTexture(void* tex)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (tex == NULL) abort();
#endif
}

mfgError mfgD3D11CreateDepthStencilTexture(mfgRenderDevice* rd, mfgDepthStencilTexture** tex, mfmU64 width, mfmU64 height, mfgEnum format)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{ if (rd == NULL || tex == NULL) return MFG_ERROR_INVALID_ARGUMENTS; }
#endif

	return MFG_ERROR_OKAY;
}

void mfgD3D11DestroyFramebuffer(void* fb)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (fb == NULL) abort();
#endif

}

mfgError mfgD3D11CreateFramebuffer(mfgRenderDevice* rd, mfgFramebuffer** fb, mfmU64 textureCount, mfgRenderTexture** textures, mfgDepthStencilTexture* depthStencilTexture)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{ if (rd == NULL || fb == NULL || textures == NULL || textureCount == 0) return MFG_ERROR_INVALID_ARGUMENTS; }
#endif

	mfgD3D11RenderDevice* d3dRD = (mfgD3D11RenderDevice*)rd;

	return MFG_ERROR_OKAY;
}

mfgError mfgD3D11SetFramebuffer(mfgRenderDevice* rd, mfgFramebuffer* fb)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{ if (rd == NULL) return MFG_ERROR_INVALID_ARGUMENTS; }
#endif

	return MFG_ERROR_OKAY;
}

mfgError mfgD3D11ClearColor(mfgRenderDevice* rd, mfmF32 r, mfmF32 g, mfmF32 b, mfmF32 a)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{ if (rd == NULL) return MFG_ERROR_INVALID_ARGUMENTS; }
#endif

	return MFG_ERROR_OKAY;
}

mfgError mfgD3D11ClearDepth(mfgRenderDevice* rd, mfmF32 depth)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{ if (rd == NULL) return MFG_ERROR_INVALID_ARGUMENTS; }
#endif

	return MFG_ERROR_OKAY;
}

mfgError mfgD3D11ClearStencil(mfgRenderDevice* rd, mfmI32 stencil)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{ if (rd == NULL) return MFG_ERROR_INVALID_ARGUMENTS; }
#endif

	return MFG_ERROR_OKAY;
}

mfgError mfgD3D11SwapBuffers(mfgRenderDevice* rd)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{ if (rd == NULL) return MFG_ERROR_INVALID_ARGUMENTS; }
#endif

	return MFG_ERROR_OKAY;
}

void mfgD3D11DestroyRasterState(void* state)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (state == NULL) abort();
#endif
	mfgD3D11RasterState* rs = state;
	rs->state->lpVtbl->Release(rs->state);
	if (mfmDeallocate(((mfgD3D11RenderDevice*)rs->base.renderDevice)->pool32, rs) != MFM_ERROR_OKAY)
		abort();
}

mfgError mfgD3D11CreateRasterState(mfgRenderDevice* rd, mfgRasterState** state, const mfgRasterStateDesc* desc)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{ if (rd == NULL || state == NULL || desc == NULL) return MFG_ERROR_INVALID_ARGUMENTS; }
#endif

	mfgD3D11RenderDevice* d3dRD = (mfgD3D11RenderDevice*)rd;

	mfgD3D11RasterState* rs = NULL;
	if (mfmAllocate(d3dRD->pool32, &rs, sizeof(mfgD3D11RasterState)) != MFM_ERROR_OKAY)
		MFG_RETURN_ERROR(MFG_ERROR_ALLOCATION_FAILED, u8"Failed to allocate raster state on pool");

	D3D11_RASTERIZER_DESC rDesc;
	rDesc.DepthBias = 0.0f;
	rDesc.SlopeScaledDepthBias = 0.0f;
	rDesc.DepthBiasClamp = 0.0f;
	rDesc.DepthClipEnable = TRUE;
	rDesc.ScissorEnable = TRUE;
	rDesc.MultisampleEnable = FALSE;
	rDesc.AntialiasedLineEnable = FALSE;

	if (desc->cullEnabled == MFM_TRUE)
	{
		switch (desc->cullFace)
		{
			case MFG_FRONT:	rDesc.CullMode = D3D11_CULL_FRONT; break;
			case MFG_BACK: rDesc.CullMode = D3D11_CULL_BACK; break;
			default: MFG_RETURN_ERROR(MFG_ERROR_INVALID_ARGUMENTS, u8"Unsupported cull face");
		}
	}
	else rDesc.CullMode = D3D11_CULL_NONE;

	switch (desc->frontFace)
	{
		case MFG_CW: rDesc.FrontCounterClockwise = FALSE; break;
		case MFG_CCW: rDesc.FrontCounterClockwise = TRUE; break;
		default: MFG_RETURN_ERROR(MFG_ERROR_INVALID_ARGUMENTS, u8"Unsupported front face winding order");
	}

	switch (desc->rasterMode)
	{
		case MFG_WIREFRAME: rDesc.FillMode = D3D11_FILL_WIREFRAME; break;
		case MFG_FILL: rDesc.FillMode = D3D11_FILL_SOLID; break;
		default: MFG_RETURN_ERROR(MFG_ERROR_INVALID_ARGUMENTS, u8"Unsupported rasterizer mode");
	}

	HRESULT hr = d3dRD->device->lpVtbl->CreateRasterizerState(d3dRD->device, &rDesc, &rs->state);
	if (FAILED(hr))
		MFG_RETURN_ERROR(MFG_ERROR_INTERNAL, u8"CreateRasterizerState failed");

	*state = rs;

	return MFG_ERROR_OKAY;
}

mfgError mfgD3D11SetRasterState(mfgRenderDevice* rd, mfgRasterState* state)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{ if (rd == NULL) return MFG_ERROR_INVALID_ARGUMENTS; }
#endif
	mfgD3D11RenderDevice* d3dRD = (mfgD3D11RenderDevice*)rd;

	if (state == NULL)
		return mfgD3D11SetRasterState(rd, ((mfgD3D11RenderDevice*)rd)->defaultRasterState);
	else
		d3dRD->deviceContext->lpVtbl->RSSetState(d3dRD->deviceContext, ((mfgD3D11RasterState*)state)->state);

	return MFG_ERROR_OKAY;
}

void mfgD3D11DestroyDepthStencilState(void* state)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (state == NULL) abort();
#endif

}

mfgError mfgD3D11CreateDepthStencilState(mfgRenderDevice* rd, mfgDepthStencilState** state, const mfgDepthStencilStateDesc* desc)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{ if (rd == NULL || state == NULL || desc == NULL) return MFG_ERROR_INVALID_ARGUMENTS; }
#endif

	mfgD3D11RenderDevice* d3dRD = (mfgD3D11RenderDevice*)rd;

	return MFG_ERROR_OKAY;
}

mfgError mfgD3D11SetDepthStencilState(mfgRenderDevice* rd, mfgDepthStencilState* state)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{ if (rd == NULL) return MFG_ERROR_INVALID_ARGUMENTS; }
#endif
	mfgD3D11RenderDevice* d3dRD = (mfgD3D11RenderDevice*)rd;
	
	if (state == NULL)
		return mfgD3D11SetDepthStencilState(rd, d3dRD->defaultDepthStencilState);
	else
		d3dRD->deviceContext->lpVtbl->OMSetDepthStencilState(d3dRD->deviceContext, ((mfgD3D11DepthStencilState*)state)->state, ((mfgD3D11DepthStencilState*)state)->stencilRef);

	return MFG_ERROR_OKAY;
}

void mfgD3D11DestroyBlendState(void* state)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (state == NULL) abort();
#endif
}

mfgError mfgD3D11CreateBlendState(mfgRenderDevice* rd, mfgBlendState** state, const mfgBlendStateDesc* desc)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{ if (rd == NULL || state == NULL || desc == NULL) return MFG_ERROR_INVALID_ARGUMENTS; }
#endif

	return MFG_ERROR_OKAY;
}

mfgError mfgD3D11SetBlendState(mfgRenderDevice* rd, mfgBlendState* state)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{ if (rd == NULL) return MFG_ERROR_INVALID_ARGUMENTS; }
#endif
	mfgD3D11RenderDevice* d3dRD = (mfgD3D11RenderDevice*)rd;

	if (state == NULL)
		return mfgD3D11SetBlendState(rd, d3dRD->defaultBlendState);
	else
	{
		FLOAT color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
		d3dRD->deviceContext->lpVtbl->OMSetBlendState(d3dRD->deviceContext, ((mfgD3D11BlendState*)state)->state, color, 0xFFFFFFFF);
	}

	return MFG_ERROR_OKAY;
}

mfmBool mfgD3D11GetErrorString(mfgRenderDevice* rd, mfsUTF8CodeUnit* str, mfmU64 maxSize)
{
	if (rd == NULL || str == NULL || maxSize == 0)
		abort();

	mfgD3D11RenderDevice* d3dRD = (mfgD3D11RenderDevice*)rd;

	if (d3dRD->errorStringSize == 0)
	{
		str[0] = '\0';
		return MFM_FALSE;
	}

	if (maxSize >= d3dRD->errorStringSize)
	{
		memcpy(str, d3dRD->errorString, d3dRD->errorStringSize);
		str[d3dRD->errorStringSize] = '\0';
	}
	else
	{
		memcpy(str, d3dRD->errorString, maxSize - 1);
		str[maxSize] = '\0';
	}

	return MFM_TRUE;
}

mfgError mfgD3D11DrawTriangles(mfgRenderDevice* rd, mfmU64 offset, mfmU64 count)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{ if (rd == NULL) return MFG_ERROR_INVALID_ARGUMENTS; }
#endif
	
	return MFG_ERROR_OKAY;
}

mfgError mfgD3D11DrawTrianglesIndexed(mfgRenderDevice* rd, mfmU64 offset, mfmU64 count)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (rd == NULL || ((mfgD3D11RenderDevice*)rd)->currentIndexBuffer == NULL) return MFG_ERROR_INVALID_ARGUMENTS;
#endif
	


	return MFG_ERROR_OKAY;
}

mfgError mfgCreateD3D11RenderDevice(mfgRenderDevice ** renderDevice, mfiWindow* window, const mfgRenderDeviceDesc * desc, void * allocator)
{
	// Check if params are valid
	if (renderDevice == NULL || window == NULL || desc == NULL || window->type != MFI_D3DWINDOW)
		return MFG_ERROR_INVALID_ARGUMENTS;

	// Allocate render device
	mfgD3D11RenderDevice* rd;
	if (mfmAllocate(allocator, &rd, sizeof(mfgD3D11RenderDevice)) != MFM_ERROR_OKAY)
		return MFG_ERROR_ALLOCATION_FAILED;

	// Create 32 bytes pool
	{
		mfmPoolAllocatorDesc desc;
		desc.expandable = MFM_FALSE;
		desc.slotCount = MFG_POOL_32_ELEMENT_COUNT;
		desc.slotSize = 32;
		mfmError err = mfmCreatePoolAllocatorOnMemory(&rd->pool32, &desc, rd->pool32Memory, sizeof(rd->pool32Memory));
		if (err != MFM_ERROR_OKAY)
			return MFG_ERROR_ALLOCATION_FAILED;
	}

	// Create 64 bytes pool
	{
		mfmPoolAllocatorDesc desc;
		desc.expandable = MFM_FALSE;
		desc.slotCount = MFG_POOL_64_ELEMENT_COUNT;
		desc.slotSize = 64;
		mfmError err = mfmCreatePoolAllocatorOnMemory(&rd->pool64, &desc, rd->pool64Memory, sizeof(rd->pool64Memory));
		if (err != MFM_ERROR_OKAY)
			return MFG_ERROR_ALLOCATION_FAILED;
	}

	// Create 256 bytes pool
	{
		mfmPoolAllocatorDesc desc;
		desc.expandable = MFM_FALSE;
		desc.slotCount = MFG_POOL_256_ELEMENT_COUNT;
		desc.slotSize = 256;
		mfmError err = mfmCreatePoolAllocatorOnMemory(&rd->pool256, &desc, rd->pool256Memory, sizeof(rd->pool256Memory));
		if (err != MFM_ERROR_OKAY)
			return MFG_ERROR_ALLOCATION_FAILED;
	}

	// Create 512 bytes pool
	{
		mfmPoolAllocatorDesc desc;
		desc.expandable = MFM_FALSE;
		desc.slotCount = MFG_POOL_512_ELEMENT_COUNT;
		desc.slotSize = 512;
		mfmError err = mfmCreatePoolAllocatorOnMemory(&rd->pool512, &desc, rd->pool512Memory, sizeof(rd->pool512Memory));
		if (err != MFM_ERROR_OKAY)
			return MFG_ERROR_ALLOCATION_FAILED;
	}

	// Create stack
	{
		mfmError err = mfmCreateStackAllocatorOnMemory(&rd->stack, 2048, rd->stackMemory, sizeof(rd->stackMemory));
		if (err != MFM_ERROR_OKAY)
			return MFG_ERROR_ALLOCATION_FAILED;
	}

	// Initialize some properties
	rd->base.object.destructorFunc = &mfgDestroyD3D11RenderDevice;
	rd->base.object.referenceCount = 0;
	rd->window = window;
	rd->allocator = allocator;
	memset(rd->errorString, '\0', sizeof(rd->errorString));
	rd->errorStringSize = 0;

	rd->currentIndexBuffer = NULL;

	// Init D3D11 stuff
	{
		HRESULT hr;

		// Create device and swap chain
		{
			DXGI_SWAP_CHAIN_DESC desc;
			ZeroMemory(&desc, sizeof(desc));

			desc.BufferCount = 1;
			desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			desc.OutputWindow = (HWND)mfiGetD3DWindowHandle(window);
			desc.SampleDesc.Count = 1;
			desc.SampleDesc.Quality = 0;
			desc.Windowed = ((window->getMode(window)) == MFI_FULLSCREEN) ? FALSE : TRUE;

			UINT flags = 0;
#ifdef MAGMA_FRAMEWORK_DEBUG
			flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif


			hr = D3D11CreateDeviceAndSwapChain(
				NULL,
				D3D_DRIVER_TYPE_HARDWARE,
				NULL,
				flags,
				NULL,
				NULL,
				D3D11_SDK_VERSION,
				&desc,
				&rd->swapChain,
				&rd->device,
				NULL,
				&rd->deviceContext);

			if (FAILED(hr))
				return MFG_ERROR_INTERNAL;
		}

		// Get back buffer
		{
			
			hr = rd->swapChain->lpVtbl->GetBuffer(rd->swapChain, 0, &IID_ID3D11Texture2D, (LPVOID*)&rd->backBuffer);
			if (FAILED(hr))
				return MFG_ERROR_INTERNAL;
		}

		// Create render target view
		{
			
			hr = rd->device->lpVtbl->CreateRenderTargetView(rd->device, rd->backBuffer, NULL, &rd->defaultRenderTargetView);
			rd->backBuffer->lpVtbl->Release(rd->backBuffer);
		}

		// Create depth stencil buffer
		{
			D3D11_TEXTURE2D_DESC desc;
			ZeroMemory(&desc, sizeof(desc));
			desc.Width = window->getWidth(window);
			desc.Height = window->getHeight(window);
			desc.MipLevels = 1;
			desc.ArraySize = 1;
			desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			desc.SampleDesc.Count = 1;
			desc.SampleDesc.Quality = 0;
			desc.Usage = D3D11_USAGE_DEFAULT;
			desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
			desc.CPUAccessFlags = 0;
			desc.MiscFlags = 0;

			hr = rd->device->lpVtbl->CreateTexture2D(rd->device, &desc, NULL, &rd->depthStencilBuffer);

			if (FAILED(hr))
				return MFG_ERROR_INTERNAL;
		}

		// Create depth stencil view
		{
			hr = rd->device->lpVtbl->CreateDepthStencilView(rd->device, rd->depthStencilBuffer, NULL, &rd->defaultDepthStencilView);
			if (FAILED(hr))
				return MFG_ERROR_INTERNAL;
		}

		// Set default render target
		rd->deviceContext->lpVtbl->OMSetRenderTargets(rd->deviceContext, 1, &rd->defaultRenderTargetView, rd->defaultDepthStencilView);
		rd->renderTargetCount = 1;

		// Set default viewport
		{
			D3D11_VIEWPORT viewport;
			ZeroMemory(&viewport, sizeof(viewport));

			viewport.TopLeftX = 0;
			viewport.TopLeftY = 0;
			viewport.Width = window->getWidth(window);
			viewport.Height = window->getHeight(window);
			viewport.MinDepth = 0.0f;
			viewport.MaxDepth = 1.0f;

			rd->deviceContext->lpVtbl->RSSetViewports(rd->deviceContext, 1, &viewport);
		}
	}

	// Set functions
	rd->base.createVertexShader = &mfgD3D11CreateVertexShader;
	rd->base.destroyVertexShader = &mfgD3D11DestroyVertexShader;
	rd->base.createPixelShader = &mfgD3D11CreatePixelShader;
	rd->base.destroyPixelShader = &mfgD3D11DestroyPixelShader;
	rd->base.createPipeline = &mfgD3D11CreatePipeline;
	rd->base.destroyPipeline = &mfgD3D11DestroyPipeline;
	rd->base.setPipeline = &mfgD3D11SetPipeline;

	rd->base.getVertexShaderBindingPoint = &mfgD3D11GetVertexShaderBindingPoint;
	rd->base.getPixelShaderBindingPoint = &mfgD3D11GetPixelShaderBindingPoint;
	rd->base.bindConstantBuffer = &mfgD3D11BindConstantBuffer;
	rd->base.bindConstantBufferRange = &mfgD3D11BindConstantBufferRange;
	rd->base.bindTexture1D = &mfgD3D11BindTexture1D;
	rd->base.bindTexture2D = &mfgD3D11BindTexture2D;
	rd->base.bindTexture3D = &mfgD3D11BindTexture3D;
	rd->base.bindRenderTexture = &mfgD3D11BindRenderTexture;
	rd->base.bindSampler = &mfgD3D11BindSampler;

	rd->base.createConstantBuffer = &mfgD3D11CreateConstantBuffer;
	rd->base.destroyConstantBuffer = &mfgD3D11DestroyConstantBuffer;
	rd->base.mapConstantBuffer = &mfgD3D11MapConstantBuffer;
	rd->base.unmapConstantBuffer = &mfgD3D11UnmapConstantBuffer;

	rd->base.createVertexBuffer = &mfgD3D11CreateVertexBuffer;
	rd->base.destroyVertexBuffer = &mfgD3D11DestroyVertexBuffer;
	rd->base.mapVertexBuffer = &mfgD3D11MapVertexBuffer;
	rd->base.unmapVertexBuffer = &mfgD3D11UnmapVertexBuffer;
	rd->base.createVertexLayout = &mfgD3D11CreateVertexLayout;
	rd->base.destroyVertexLayout = &mfgD3D11DestroyVertexLayout;
	rd->base.createVertexArray = &mfgD3D11CreateVertexArray;
	rd->base.destroyVertexArray = &mfgD3D11DestroyVertexArray;
	rd->base.setVertexArray = &mfgD3D11SetVertexArray;
	rd->base.createIndexBuffer = &mfgD3D11CreateIndexBuffer;
	rd->base.destroyIndexBuffer = &mfgD3D11DestroyIndexBuffer;
	rd->base.setIndexBuffer = &mfgD3D11SetIndexBuffer;
	rd->base.mapIndexBuffer = &mfgD3D11MapIndexBuffer;
	rd->base.unmapIndexBuffer = &mfgD3D11UnmapIndexBuffer;

	rd->base.createTexture1D = &mfgD3D11CreateTexture1D;
	rd->base.destroyTexture1D = &mfgD3D11DestroyTexture1D;
	rd->base.updateTexture1D = &mfgD3D11UpdateTexture1D;
	rd->base.generateTexture1DMipmaps = &mfgD3D11GenerateTexture1DMipmaps;

	rd->base.createTexture2D = &mfgD3D11CreateTexture2D;
	rd->base.destroyTexture2D = &mfgD3D11DestroyTexture2D;
	rd->base.updateTexture2D = &mfgD3D11UpdateTexture2D;
	rd->base.generateTexture2DMipmaps = &mfgD3D11GenerateTexture2DMipmaps;

	rd->base.createTexture3D = &mfgD3D11CreateTexture3D;
	rd->base.destroyTexture3D = &mfgD3D11DestroyTexture3D;
	rd->base.updateTexture3D = &mfgD3D11UpdateTexture3D;
	rd->base.generateTexture3DMipmaps = &mfgD3D11GenerateTexture3DMipmaps;

	rd->base.createSampler = &mfgD3D11CreateSampler;
	rd->base.destroySampler = &mfgD3D11DestroySampler;

	rd->base.createRasterState = &mfgD3D11CreateRasterState;
	rd->base.destroyRasterState = &mfgD3D11DestroyRasterState;
	rd->base.setRasterState = &mfgD3D11SetRasterState;
	rd->base.createDepthStencilState = &mfgD3D11CreateDepthStencilState;
	rd->base.destroyDepthStencilState = &mfgD3D11DestroyDepthStencilState;
	rd->base.setDepthStencilState = &mfgD3D11SetDepthStencilState;
	rd->base.createBlendState = &mfgD3D11CreateBlendState;
	rd->base.destroyBlendState = &mfgD3D11DestroyBlendState;
	rd->base.setBlendState = &mfgD3D11SetBlendState;

	rd->base.createRenderTexture = &mfgD3D11CreateRenderTexture;
	rd->base.destroyRenderTexture = &mfgD3D11DestroyRenderTexture;
	rd->base.createDepthStencilTexture = &mfgD3D11CreateDepthStencilTexture;
	rd->base.destroyDepthStencilTexture = &mfgD3D11DestroyDepthStencilTexture;
	rd->base.createFramebuffer = &mfgD3D11CreateFramebuffer;
	rd->base.destroyFramebuffer = &mfgD3D11DestroyFramebuffer;
	rd->base.setFramebuffer = &mfgD3D11SetFramebuffer;

	rd->base.clearColor = &mfgD3D11ClearColor;
	rd->base.clearDepth = &mfgD3D11ClearDepth;
	rd->base.clearStencil = &mfgD3D11ClearStencil;
	rd->base.drawTriangles = &mfgD3D11DrawTriangles;
	rd->base.drawTrianglesIndexed = &mfgD3D11DrawTrianglesIndexed;
	rd->base.swapBuffers = &mfgD3D11SwapBuffers;

	rd->base.getPropertyI = NULL;
	rd->base.getPropertyF = NULL;

	rd->base.getErrorString = &mfgD3D11GetErrorString;

	// Get and set the default raster state
	{
		mfgRasterStateDesc desc;
		mfgDefaultRasterStateDesc(&desc);
		mfgError err = mfgCreateRasterState(rd, &rd->defaultRasterState, &desc);
		if (err != MFG_ERROR_OKAY)
			return err;
		err = mfgSetRasterState(rd, rd->defaultRasterState);
		if (err != MFG_ERROR_OKAY)
			return err;
	}

	// Get and set the default depth stencil state
	{
		mfgDepthStencilStateDesc desc;
		mfgDefaultDepthStencilStateDesc(&desc);
		mfgError err = mfgCreateDepthStencilState(rd, &rd->defaultDepthStencilState, &desc);
		if (err != MFG_ERROR_OKAY)
			return err;
		err = mfgSetDepthStencilState(rd, rd->defaultDepthStencilState);
		if (err != MFG_ERROR_OKAY)
			return err;
	}

	// Get and set the default blend state
	{
		mfgBlendStateDesc desc;
		mfgDefaultBlendStateDesc(&desc);
		mfgError err = mfgCreateBlendState(rd, &rd->defaultBlendState, &desc);
		if (err != MFG_ERROR_OKAY)
			return err;
		err = mfgSetBlendState(rd, rd->defaultBlendState);
		if (err != MFG_ERROR_OKAY)
			return err;
	}

	// Successfully inited render device
	*renderDevice = rd;
	return MFG_ERROR_OKAY;
}

void mfgDestroyD3D11RenderDevice(void * renderDevice)
{
	if (renderDevice == NULL)
		abort();

	mfgD3D11RenderDevice* rd = (mfgD3D11RenderDevice*)renderDevice;

	// Destroy default states
	mfgDestroyRasterState(rd->defaultRasterState);
	mfgDestroyDepthStencilState(rd->defaultDepthStencilState);
	mfgDestroyBlendState(rd->defaultBlendState);

	// Destroy pools
	mfmDestroyStackAllocator(rd->stack);
	mfmDestroyPoolAllocator(rd->pool512);
	mfmDestroyPoolAllocator(rd->pool256);
	mfmDestroyPoolAllocator(rd->pool64);
	mfmDestroyPoolAllocator(rd->pool32);

	// Deallocate render device
	if (mfmDeallocate(rd->allocator, rd) != MFM_ERROR_OKAY)
		abort();
}

#else

mfgError mfgCreateD3D11RenderDevice(mfgRenderDevice ** renderDevice, mfiWindow* window, const mfgRenderDeviceDesc * desc, void * allocator)
{
	return MFG_ERROR_NOT_SUPPORTED;
}

void mfgDestroyD3D11RenderDevice(void * renderDevice)
{
	abort();
}

#endif