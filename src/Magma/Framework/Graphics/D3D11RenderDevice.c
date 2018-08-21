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
#include <d3d11_1.h>
#include <d3dcompiler.h>

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")
#pragma comment (lib, "dxguid.lib")

#define MFG_POOL_32_ELEMENT_COUNT 2048
#define MFG_POOL_64_ELEMENT_COUNT 2048
#define MFG_POOL_256_ELEMENT_COUNT 2048
#define MFG_POOL_512_ELEMENT_COUNT 2048

typedef struct mfgD3D11VertexShader mfgD3D11VertexShader;
typedef struct mfgD3D11PixelShader mfgD3D11PixelShader;

typedef struct
{
	const mfgMetaDataBindingPoint* bp;
	mfgEnum shaderType;
	UINT index;
	BOOL active;
	void* shader;
} mfgD3D11BindingPoint;

struct mfgD3D11VertexShader
{
	mfgRenderDeviceObject base;
	
	const mfgMetaData* md;
	ID3D11VertexShader* shader;
	ID3DBlob* blob;
	mfgD3D11BindingPoint bps[16];
};

struct mfgD3D11PixelShader
{
	mfgRenderDeviceObject base;
	const mfgMetaData* md;
	ID3D11PixelShader* shader;
	ID3DBlob* blob;
	mfgD3D11BindingPoint bps[16];
};

typedef struct
{
	mfgRenderDeviceObject base;
	
	mfgD3D11VertexShader* vertex;
	mfgD3D11PixelShader* pixel;
} mfgD3D11Pipeline;

typedef struct
{
	mfgRenderDeviceObject base;
	ID3D11Buffer* buffer;
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
	ID3D11Buffer* buffer;
} mfgD3D11VertexBuffer;

typedef struct
{
	mfgRenderDeviceObject base;
	
	ID3D11InputLayout* inputLayout;
	UINT offsets[16];
	UINT strides[16];
	UINT elementCount;
} mfgD3D11VertexLayout;

typedef struct
{
	mfgRenderDeviceObject base;
	ID3D11Buffer* buffers[16];
	UINT bufferCount;
	mfgD3D11VertexLayout* layout;
} mfgD3D11VertexArray;

typedef struct
{
	mfgRenderDeviceObject base;
	ID3D11Buffer* buffer;
	DXGI_FORMAT format;
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

	IDXGISwapChain* swapChain;
	ID3D11Device1* device;
	ID3D11DeviceContext1* deviceContext;

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

	mfgD3D11VertexShader* d3dVS = vs;
	d3dVS->shader->lpVtbl->Release(d3dVS->shader);
	if (mfmDeallocate(((mfgD3D11RenderDevice*)d3dVS->base.renderDevice)->pool512, d3dVS) != MFM_ERROR_OKAY)
		abort();
}

mfgError mfgD3D11CreateVertexShader(mfgRenderDevice* rd, mfgVertexShader** vs, const mfmU8* bytecode, mfmU64 bytecodeSize, const mfgMetaData* metaData)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{ if (rd == NULL || vs == NULL || bytecode == NULL || metaData == NULL) return MFG_ERROR_INVALID_ARGUMENTS; }
#endif

	mfgD3D11RenderDevice* d3dRD = (mfgD3D11RenderDevice*)rd;

	// Allocate vertex shader
	mfgD3D11VertexShader* d3dVS = NULL;
	if (mfmAllocate(d3dRD->pool512, &d3dVS, sizeof(mfgD3D11VertexShader)) != MFM_ERROR_OKAY)
		MFG_RETURN_ERROR(MFG_ERROR_ALLOCATION_FAILED, u8"Failed to allocate vertex shader on pool");

	// Init object
	d3dVS->base.object.destructorFunc = &mfgD3D11DestroyVertexShader;
	d3dVS->base.object.referenceCount = 0;
	d3dVS->base.renderDevice = rd;

	// Create string stream
	mfsStream* ss;
	char buffer[4096];
	if (mfsCreateStringStream(&ss, buffer, sizeof(buffer), d3dRD->stack) != MFM_ERROR_OKAY)
		MFG_RETURN_ERROR(MFG_ERROR_INTERNAL, u8"Failed to create string stream for mfgD3D11Assemble");
	mfgError err = mfgD3D11Assemble(bytecode, bytecodeSize, metaData, ss);
	if (err != MFG_ERROR_OKAY)
		MFG_RETURN_ERROR(MFG_ERROR_INTERNAL, u8"mfgD3D11Assemble failed");
	if (mfsPutByte(ss, '\0') != MFS_ERROR_OKAY)
	{
		mfsDestroyStringStream(ss);
		MFG_RETURN_ERROR(MFG_ERROR_INTERNAL, u8"mfsPutByte returned error");
	}

	mfsDestroyStringStream(ss);
	mfsPutString(mfsOutStream, buffer);

	// Compile shader
	{
		ID3DBlob* errorMessages;
		HRESULT hr = D3DCompile(buffer, sizeof(buffer), NULL, NULL, NULL, "VS", "vs_4_0", D3D10_SHADER_PACK_MATRIX_COLUMN_MAJOR, 0, &d3dVS->blob, &errorMessages);
		if (FAILED(hr))
		{
			mfsPutString(mfsErrStream, errorMessages->lpVtbl->GetBufferPointer(errorMessages));
			MFG_RETURN_ERROR(MFG_ERROR_INTERNAL, u8"D3DCompile failed");
		}
	}

	// Create shader
	{
		HRESULT hr = d3dRD->device->lpVtbl->CreateVertexShader(d3dRD->device, d3dVS->blob->lpVtbl->GetBufferPointer(d3dVS->blob), d3dVS->blob->lpVtbl->GetBufferSize(d3dVS->blob), NULL, &d3dVS->shader);
		if (FAILED(hr))
			MFG_RETURN_ERROR(MFG_ERROR_INTERNAL, u8"CreateVertexShader failed");
	}

	d3dVS->md = metaData;

	// Get binding points
	for (mfmU16 i = 0; i < 16; ++i)
	{
		d3dVS->bps[i].active = FALSE;
		d3dVS->bps[i].shader = d3dVS;
		d3dVS->bps[i].shaderType = MFG_VERTEX_SHADER;
	}

	{
		mfgMetaDataBindingPoint* bp = d3dVS->md->firstBindingPoint;
		for (mfmU16 i = 0; i < 16 && bp != NULL; ++i)
		{
			if (bp->type == MFG_CONSTANT_BUFFER)
			{
				d3dVS->bps[i].bp = bp;
				d3dVS->bps[i].index = bp->id;
				d3dVS->bps[i].active = TRUE;
			}
			else if (bp->type == MFG_TEXTURE_1D)
			{
				d3dVS->bps[i].bp = bp;
				d3dVS->bps[i].index = bp->id;
				d3dVS->bps[i].active = TRUE;
			}
			else if (bp->type == MFG_TEXTURE_2D)
			{
				d3dVS->bps[i].bp = bp;
				d3dVS->bps[i].index = bp->id;
				d3dVS->bps[i].active = TRUE;
			}
			else if (bp->type == MFG_TEXTURE_3D)
			{
				d3dVS->bps[i].bp = bp;
				d3dVS->bps[i].index = bp->id;
				d3dVS->bps[i].active = TRUE;
			}

			bp = bp->next;
		}
	}

	*vs = d3dVS;

	return MFG_ERROR_OKAY;
}

mfgError mfgD3D11GetVertexShaderBindingPoint(mfgRenderDevice* rd, mfgBindingPoint** bp, mfgVertexShader* vs, const mfsUTF8CodeUnit* name)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (rd == NULL || bp == NULL || vs == NULL || name == NULL) return MFG_ERROR_INVALID_ARGUMENTS;
#endif
	mfgD3D11VertexShader* d3dVS = vs;

	for (mfmU64 i = 0; i < 16; ++i)
		if (d3dVS->bps[i].active == TRUE)
		{
			if (strcmp(name, d3dVS->bps[i].bp->name) == 0)
			{
				*bp = &d3dVS->bps[i];
				return MFG_ERROR_OKAY;
			}
		}

	return MFG_ERROR_NOT_FOUND;
}

void mfgD3D11DestroyPixelShader(void* ps)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (ps == NULL) abort();
#endif

	mfgD3D11PixelShader* d3dPS = ps;
	d3dPS->shader->lpVtbl->Release(d3dPS->shader);
	if (mfmDeallocate(((mfgD3D11RenderDevice*)d3dPS->base.renderDevice)->pool512, d3dPS) != MFM_ERROR_OKAY)
		abort();
}


mfgError mfgD3D11CreatePixelShader(mfgRenderDevice* rd, mfgPixelShader** ps, const mfmU8* bytecode, mfmU64 bytecodeSize, const mfgMetaData* metaData)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{ if (rd == NULL || ps == NULL || bytecode == NULL || metaData == NULL) return MFG_ERROR_INVALID_ARGUMENTS; }
#endif

	mfgD3D11RenderDevice* d3dRD = (mfgD3D11RenderDevice*)rd;

	// Allocate pixel shader
	mfgD3D11PixelShader* d3dPS = NULL;
	if (mfmAllocate(d3dRD->pool512, &d3dPS, sizeof(mfgD3D11PixelShader)) != MFM_ERROR_OKAY)
		MFG_RETURN_ERROR(MFG_ERROR_ALLOCATION_FAILED, u8"Failed to allocate pixel shader on pool");

	// Init object
	d3dPS->base.object.destructorFunc = &mfgD3D11DestroyPixelShader;
	d3dPS->base.object.referenceCount = 0;
	d3dPS->base.renderDevice = rd;

	// Create string stream
	mfsStream* ss;
	char buffer[4096];
	if (mfsCreateStringStream(&ss, buffer, sizeof(buffer), d3dRD->stack) != MFM_ERROR_OKAY)
		MFG_RETURN_ERROR(MFG_ERROR_INTERNAL, u8"Failed to create string stream for mfgD3D11Assemble");
	mfgError err = mfgD3D11Assemble(bytecode, bytecodeSize, metaData, ss);
	if (err != MFG_ERROR_OKAY)
		MFG_RETURN_ERROR(MFG_ERROR_INTERNAL, u8"mfgD3D11Assemble failed");
	if (mfsPutByte(ss, '\0') != MFS_ERROR_OKAY)
	{
		mfsDestroyStringStream(ss);
		MFG_RETURN_ERROR(MFG_ERROR_INTERNAL, u8"mfsPutByte returned error");
	}

	mfsDestroyStringStream(ss);
	mfsPutString(mfsOutStream, buffer);

	// Compile shader
	{
		ID3DBlob* errorMessages;
		HRESULT hr = D3DCompile(buffer, sizeof(buffer), NULL, NULL, NULL, "PS", "ps_4_0", D3D10_SHADER_PACK_MATRIX_COLUMN_MAJOR, 0, &d3dPS->blob, &errorMessages);
		if (FAILED(hr))
		{
			mfsPutString(mfsErrStream, errorMessages->lpVtbl->GetBufferPointer(errorMessages));
			MFG_RETURN_ERROR(MFG_ERROR_INTERNAL, u8"D3DCompile failed");
		}
	}

	// Create shader
	{
		HRESULT hr = d3dRD->device->lpVtbl->CreatePixelShader(d3dRD->device, d3dPS->blob->lpVtbl->GetBufferPointer(d3dPS->blob), d3dPS->blob->lpVtbl->GetBufferSize(d3dPS->blob), NULL, &d3dPS->shader);
		if (FAILED(hr))
			MFG_RETURN_ERROR(MFG_ERROR_INTERNAL, u8"CreatePixelShader failed");
	}

	d3dPS->md = metaData;

	// Get binding points
	for (mfmU16 i = 0; i < 16; ++i)
	{
		d3dPS->bps[i].active = FALSE;
		d3dPS->bps[i].shader = d3dPS;
		d3dPS->bps[i].shaderType = MFG_PIXEL_SHADER;
	}

	{
		mfgMetaDataBindingPoint* bp = d3dPS->md->firstBindingPoint;
		for (mfmU16 i = 0; i < 16 && bp != NULL; ++i)
		{
			if (bp->type == MFG_CONSTANT_BUFFER)
			{
				d3dPS->bps[i].bp = bp;
				d3dPS->bps[i].index = bp->id;
				d3dPS->bps[i].active = TRUE;
			}
			else if (bp->type == MFG_TEXTURE_1D)
			{
				d3dPS->bps[i].bp = bp;
				d3dPS->bps[i].index = bp->id;
				d3dPS->bps[i].active = TRUE;
			}
			else if (bp->type == MFG_TEXTURE_2D)
			{
				d3dPS->bps[i].bp = bp;
				d3dPS->bps[i].index = bp->id;
				d3dPS->bps[i].active = TRUE;
			}
			else if (bp->type == MFG_TEXTURE_3D)
			{
				d3dPS->bps[i].bp = bp;
				d3dPS->bps[i].index = bp->id;
				d3dPS->bps[i].active = TRUE;
			}

			bp = bp->next;
		}
	}

	*ps = d3dPS;

	return MFG_ERROR_OKAY;
}

mfgError mfgD3D11GetPixelShaderBindingPoint(mfgRenderDevice* rd, mfgBindingPoint** bp, mfgPixelShader* ps, const mfsUTF8CodeUnit* name)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (rd == NULL || bp == NULL || ps == NULL || name == NULL) return MFG_ERROR_INVALID_ARGUMENTS;
#endif
	mfgD3D11PixelShader* d3dPS = ps;

	for (mfmU64 i = 0; i < 16; ++i)
		if (d3dPS->bps[i].active == TRUE)
		{
			if (strcmp(name, d3dPS->bps[i].bp->name) == 0)
			{
				*bp = &d3dPS->bps[i];
				return MFG_ERROR_OKAY;
			}
		}

	return MFG_ERROR_NOT_FOUND;
}

mfgError mfgD3D11BindConstantBuffer(mfgRenderDevice* rd, mfgBindingPoint* bp, mfgConstantBuffer* cb)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (rd == NULL || bp == NULL) return MFG_ERROR_INVALID_ARGUMENTS;
#endif
	
	mfgD3D11RenderDevice* d3dRD = rd;
	mfgD3D11BindingPoint* d3dBP = bp;
	mfgD3D11ConstantBuffer* d3dCB = cb;

	if (d3dBP->shaderType == MFG_VERTEX_SHADER)
	{
		if (d3dCB == NULL)
			d3dRD->deviceContext->lpVtbl->VSSetConstantBuffers(d3dRD->deviceContext, d3dBP->index, 1, NULL);
		else
			d3dRD->deviceContext->lpVtbl->VSSetConstantBuffers(d3dRD->deviceContext, d3dBP->index, 1, &d3dCB->buffer);
	}
	else if (d3dBP->shaderType == MFG_PIXEL_SHADER)
	{
		if (d3dCB == NULL)
			d3dRD->deviceContext->lpVtbl->PSSetConstantBuffers(d3dRD->deviceContext, d3dBP->index, 1, NULL);
		else
			d3dRD->deviceContext->lpVtbl->PSSetConstantBuffers(d3dRD->deviceContext, d3dBP->index, 1, &d3dCB->buffer);
	}

	return MFG_ERROR_OKAY;
}

mfgError mfgD3D11BindConstantBufferRange(mfgRenderDevice* rd, mfgBindingPoint* bp, mfgConstantBuffer* cb, mfmU64 offset, mfmU64 size)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (rd == NULL || bp == NULL || cb == NULL) return MFG_ERROR_INVALID_ARGUMENTS;
#endif
	
	mfgD3D11RenderDevice* d3dRD = rd;
	mfgD3D11BindingPoint* d3dBP = bp;
	mfgD3D11ConstantBuffer* d3dCB = cb;

	UINT d3dOffset = offset;
	UINT d3dSize = size;

	if (d3dBP->shaderType == MFG_VERTEX_SHADER)
	{
		if (d3dCB == NULL)
			d3dRD->deviceContext->lpVtbl->VSSetConstantBuffers1(d3dRD->deviceContext, d3dBP->index, 1, NULL, NULL, NULL);
		else
			d3dRD->deviceContext->lpVtbl->VSSetConstantBuffers1(d3dRD->deviceContext, d3dBP->index, 1, d3dCB->buffer, &d3dOffset, &d3dSize);
	}
	else if (d3dBP->shaderType == MFG_PIXEL_SHADER)
	{
		if (d3dCB == NULL)
			d3dRD->deviceContext->lpVtbl->PSSetConstantBuffers1(d3dRD->deviceContext, d3dBP->index, 1, NULL, NULL, NULL);
		else
			d3dRD->deviceContext->lpVtbl->PSSetConstantBuffers1(d3dRD->deviceContext, d3dBP->index, 1, d3dCB->buffer, &d3dOffset, &d3dSize);
	}

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

	mfgD3D11Pipeline* d3dPP = pp;
	--d3dPP->vertex->base.object.referenceCount;
	--d3dPP->pixel->base.object.referenceCount;
	if (mfmDeallocate(((mfgD3D11RenderDevice*)d3dPP->base.renderDevice)->pool32, d3dPP) != MFM_ERROR_OKAY)
		abort();
}

mfgError mfgD3D11CreatePipeline(mfgRenderDevice* rd, mfgPipeline** pp, mfgVertexShader* vs, mfgPixelShader* ps)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{ if (rd == NULL || pp == NULL || vs == NULL || ps == NULL) return MFG_ERROR_INVALID_ARGUMENTS; }
#endif

	mfgD3D11RenderDevice* d3dRD = (mfgD3D11RenderDevice*)rd;

	// Allocate pixel shader
	mfgD3D11Pipeline* d3dPP = NULL;
	if (mfmAllocate(d3dRD->pool32, &d3dPP, sizeof(mfgD3D11Pipeline)) != MFM_ERROR_OKAY)
		MFG_RETURN_ERROR(MFG_ERROR_ALLOCATION_FAILED, u8"Failed to allocate pipeline on pool");

	// Init object
	d3dPP->base.object.destructorFunc = &mfgD3D11DestroyPipeline;
	d3dPP->base.object.referenceCount = 0;
	d3dPP->base.renderDevice = rd;

	// Init pipeline
	d3dPP->vertex = vs;
	d3dPP->pixel = ps;
	++vs->object.referenceCount;
	++ps->object.referenceCount;

	return MFG_ERROR_OKAY;
}

mfgError mfgD3D11SetPipeline(mfgRenderDevice* rd, mfgPipeline* pp)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{ if (rd == NULL) return MFG_ERROR_INVALID_ARGUMENTS; }
#endif

	mfgD3D11RenderDevice* d3dRD = (mfgD3D11RenderDevice*)rd;
	mfgD3D11Pipeline* d3dPP = pp;

	if (d3dPP == NULL)
	{
		d3dRD->deviceContext->lpVtbl->VSSetShader(d3dRD->deviceContext, NULL, NULL, 0);
		d3dRD->deviceContext->lpVtbl->PSSetShader(d3dRD->deviceContext, NULL, NULL, 0);
	}
	else
	{
		d3dRD->deviceContext->lpVtbl->VSSetShader(d3dRD->deviceContext, d3dPP->vertex->shader, NULL, 0);
		d3dRD->deviceContext->lpVtbl->PSSetShader(d3dRD->deviceContext, d3dPP->pixel->shader, NULL, 0);
	}
	
	return MFG_ERROR_OKAY;
}

void mfgD3D11DestroyConstantBuffer(void* buffer)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (buffer == NULL) abort();
#endif
	
	mfgD3D11ConstantBuffer* d3dCB = buffer;
	d3dCB->buffer->lpVtbl->Release(d3dCB->buffer);
	if (mfmDeallocate(((mfgD3D11RenderDevice*)d3dCB->base.renderDevice)->pool32, d3dCB) != MFM_ERROR_OKAY)
		abort();
}

mfgError mfgD3D11CreateConstantBuffer(mfgRenderDevice* rd, mfgConstantBuffer** cb, mfmU64 size, const void* data, mfgEnum usage)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{ if (rd == NULL || cb == NULL) return MFG_ERROR_INVALID_ARGUMENTS; }
#endif

	mfgD3D11RenderDevice* d3dRD = (mfgD3D11RenderDevice*)rd;


	// Allocate vertex buffer
	mfgD3D11ConstantBuffer* d3dCB = NULL;
	if (mfmAllocate(d3dRD->pool32, &d3dCB, sizeof(mfgD3D11ConstantBuffer)) != MFM_ERROR_OKAY)
		MFG_RETURN_ERROR(MFG_ERROR_ALLOCATION_FAILED, u8"Failed to allocate constant buffer on pool");

	// Init object
	d3dCB->base.object.destructorFunc = &mfgD3D11DestroyConstantBuffer;
	d3dCB->base.object.referenceCount = 0;
	d3dCB->base.renderDevice = rd;

	// Create buffer
	D3D11_BUFFER_DESC desc;

	switch (usage)
	{
		case MFG_USAGE_DEFAULT: desc.Usage = D3D11_USAGE_DEFAULT; desc.CPUAccessFlags = 0; break;
		case MFG_USAGE_STATIC: desc.Usage = D3D11_USAGE_IMMUTABLE; desc.CPUAccessFlags = 0; break;
		case MFG_USAGE_DYNAMIC: desc.Usage = D3D11_USAGE_DYNAMIC; desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; break;
		default: MFG_RETURN_ERROR(MFG_ERROR_INVALID_ARGUMENTS, u8"Unsupported usage mode");
	}

	desc.ByteWidth = ceil(((mfmF32)size) / 16) * 16;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.MiscFlags = 0;

	if (data == NULL)
	{
		if (usage != MFG_USAGE_DYNAMIC)
			MFG_RETURN_ERROR(MFG_ERROR_INVALID_ARGUMENTS, u8"Non dynamic buffers must have initial data");

		HRESULT hr = d3dRD->device->lpVtbl->CreateBuffer(d3dRD->device, &desc, NULL, &d3dCB->buffer);
		if (FAILED(hr))
			MFG_RETURN_ERROR(MFG_ERROR_INTERNAL, u8"CreateBuffer failed");
	}
	else
	{
		D3D11_SUBRESOURCE_DATA initData;

		initData.pSysMem = data;
		initData.SysMemPitch = 0;
		initData.SysMemSlicePitch = 0;

		HRESULT hr = d3dRD->device->lpVtbl->CreateBuffer(d3dRD->device, &desc, &initData, &d3dCB->buffer);
		if (FAILED(hr))
			MFG_RETURN_ERROR(MFG_ERROR_INTERNAL, u8"CreateBuffer failed");
	}

	*cb = d3dCB;

	return MFG_ERROR_OKAY;
}

mfgError mfgD3D11MapConstantBuffer(mfgRenderDevice* rd, mfgConstantBuffer* cb, void** memory)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{ if (rd == NULL || cb == NULL || memory == NULL) return MFG_ERROR_INVALID_ARGUMENTS; }
#endif

	mfgD3D11RenderDevice* d3dRD = (mfgD3D11RenderDevice*)rd;
	mfgD3D11ConstantBuffer* d3dCB = cb;

	D3D11_MAPPED_SUBRESOURCE map;
	HRESULT hr = d3dRD->deviceContext->lpVtbl->Map(d3dRD->deviceContext, d3dCB->buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
	if (FAILED(hr))
		MFG_RETURN_ERROR(MFG_ERROR_INTERNAL, u8"Map failed");

	*memory = map.pData;

	return MFG_ERROR_OKAY;
}

mfgError mfgD3D11UnmapConstantBuffer(mfgRenderDevice* rd, mfgConstantBuffer* cb)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{ if (rd == NULL || cb == NULL) return MFG_ERROR_INVALID_ARGUMENTS; }
#endif

	mfgD3D11RenderDevice* d3dRD = (mfgD3D11RenderDevice*)rd;
	mfgD3D11ConstantBuffer* d3dCB = cb;
	d3dRD->deviceContext->lpVtbl->Unmap(d3dRD->deviceContext, d3dCB->buffer, 0);

	return MFG_ERROR_OKAY;
}

void mfgD3D11DestroyVertexBuffer(void* buffer)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (buffer == NULL) abort();
#endif

	mfgD3D11VertexBuffer* d3dVB = buffer;
	d3dVB->buffer->lpVtbl->Release(d3dVB->buffer);
	if (mfmDeallocate(((mfgD3D11RenderDevice*)d3dVB->base.renderDevice)->pool32, d3dVB) != MFM_ERROR_OKAY)
		abort();
}

mfgError mfgD3D11CreateVertexBuffer(mfgRenderDevice* rd, mfgVertexBuffer** vb, mfmU64 size, const void* data, mfgEnum usage)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (rd == NULL || vb == NULL) return MFG_ERROR_INVALID_ARGUMENTS;
#endif

	mfgD3D11RenderDevice* d3dRD = (mfgD3D11RenderDevice*)rd;

	// Allocate vertex buffer
	mfgD3D11VertexBuffer* d3dVB = NULL;
	if (mfmAllocate(d3dRD->pool32, &d3dVB, sizeof(mfgD3D11VertexBuffer)) != MFM_ERROR_OKAY)
		MFG_RETURN_ERROR(MFG_ERROR_ALLOCATION_FAILED, u8"Failed to allocate vertex buffer on pool");

	// Init object
	d3dVB->base.object.destructorFunc = &mfgD3D11DestroyVertexBuffer;
	d3dVB->base.object.referenceCount = 0;
	d3dVB->base.renderDevice = rd;

	// Create buffer
	D3D11_BUFFER_DESC desc;

	switch (usage)
	{
		case MFG_USAGE_DEFAULT: desc.Usage = D3D11_USAGE_DEFAULT; desc.CPUAccessFlags = 0; break;
		case MFG_USAGE_STATIC: desc.Usage = D3D11_USAGE_IMMUTABLE; desc.CPUAccessFlags = 0; break;
		case MFG_USAGE_DYNAMIC: desc.Usage = D3D11_USAGE_DYNAMIC; desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; break;
		default: MFG_RETURN_ERROR(MFG_ERROR_INVALID_ARGUMENTS, u8"Unsupported usage mode");
	}

	desc.ByteWidth = size;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.MiscFlags = 0;

	if (data == NULL)
	{
		if (usage != MFG_USAGE_DYNAMIC)
			MFG_RETURN_ERROR(MFG_ERROR_INVALID_ARGUMENTS, u8"Non dynamic buffers must have initial data");

		HRESULT hr = d3dRD->device->lpVtbl->CreateBuffer(d3dRD->device, &desc, NULL, &d3dVB->buffer);
		if (FAILED(hr))
			MFG_RETURN_ERROR(MFG_ERROR_INTERNAL, u8"CreateBuffer failed");
	}
	else
	{
		D3D11_SUBRESOURCE_DATA initData;

		initData.pSysMem = data;
		initData.SysMemPitch = 0;
		initData.SysMemSlicePitch = 0;

		HRESULT hr = d3dRD->device->lpVtbl->CreateBuffer(d3dRD->device, &desc, &initData, &d3dVB->buffer);
		if (FAILED(hr))
			MFG_RETURN_ERROR(MFG_ERROR_INTERNAL, u8"CreateBuffer failed");
	}

	*vb = d3dVB;

	return MFG_ERROR_OKAY;
}


mfgError mfgD3D11MapVertexBuffer(mfgRenderDevice* rd, mfgVertexBuffer* vb, void** memory)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
		if (rd == NULL || vb == NULL || memory == NULL) return MFG_ERROR_INVALID_ARGUMENTS;
#endif

	mfgD3D11RenderDevice* d3dRD = (mfgD3D11RenderDevice*)rd;
	mfgD3D11VertexBuffer* d3dVB = vb;

	D3D11_MAPPED_SUBRESOURCE map;
	HRESULT hr = d3dRD->deviceContext->lpVtbl->Map(d3dRD->deviceContext, d3dVB->buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
	if (FAILED(hr))
		MFG_RETURN_ERROR(MFG_ERROR_INTERNAL, u8"Map failed");

	*memory = map.pData;

	return MFG_ERROR_OKAY;
}

mfgError mfgD3D11UnmapVertexBuffer(mfgRenderDevice* rd, mfgVertexBuffer* vb)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (rd == NULL || vb == NULL) return MFG_ERROR_INVALID_ARGUMENTS;
#endif

	mfgD3D11RenderDevice* d3dRD = (mfgD3D11RenderDevice*)rd;
	mfgD3D11VertexBuffer* d3dVB = vb;
	d3dRD->deviceContext->lpVtbl->Unmap(d3dRD->deviceContext, d3dVB->buffer, 0);

	return MFG_ERROR_OKAY;
}

void mfgD3D11DestroyIndexBuffer(void* buffer)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (buffer == NULL) abort();
#endif
	
	mfgD3D11IndexBuffer* d3dIB = buffer;
	d3dIB->buffer->lpVtbl->Release(d3dIB->buffer);
	if (mfmDeallocate(((mfgD3D11RenderDevice*)d3dIB->base.renderDevice)->pool32, d3dIB) != MFM_ERROR_OKAY)
		abort();
}

mfgError mfgD3D11CreateIndexBuffer(mfgRenderDevice* rd, mfgIndexBuffer** ib, mfmU64 size, const void* data, mfgEnum format, mfgEnum usage)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (rd == NULL || ib == NULL) return MFG_ERROR_INVALID_ARGUMENTS;
#endif

	mfgD3D11RenderDevice* d3dRD = (mfgD3D11RenderDevice*)rd;

	// Allocate index buffer
	mfgD3D11IndexBuffer* d3dIB = NULL;
	if (mfmAllocate(d3dRD->pool32, &d3dIB, sizeof(mfgD3D11VertexBuffer)) != MFM_ERROR_OKAY)
		MFG_RETURN_ERROR(MFG_ERROR_ALLOCATION_FAILED, u8"Failed to allocate index buffer on pool");

	// Init object
	d3dIB->base.object.destructorFunc = &mfgD3D11DestroyIndexBuffer;
	d3dIB->base.object.referenceCount = 0;
	d3dIB->base.renderDevice = rd;

	// Create buffer
	D3D11_BUFFER_DESC desc;

	switch (usage)
	{
		case MFG_USAGE_DEFAULT: desc.Usage = D3D11_USAGE_DEFAULT; desc.CPUAccessFlags = 0; break;
		case MFG_USAGE_STATIC: desc.Usage = D3D11_USAGE_IMMUTABLE; desc.CPUAccessFlags = 0; break;
		case MFG_USAGE_DYNAMIC: desc.Usage = D3D11_USAGE_DYNAMIC; desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; break;
		default: MFG_RETURN_ERROR(MFG_ERROR_INVALID_ARGUMENTS, u8"Unsupported usage mode");
	}

	switch (format)
	{
		case MFG_USHORT: d3dIB->format = DXGI_FORMAT_R16_UINT; break;
		case MFG_UINT: d3dIB->format = DXGI_FORMAT_R32_UINT; break;
		default: MFG_RETURN_ERROR(MFG_ERROR_INVALID_ARGUMENTS, u8"Unsupported index format");
	}

	desc.ByteWidth = size;
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	desc.MiscFlags = 0;

	if (data == NULL)
	{
		if (usage != MFG_USAGE_DYNAMIC)
			MFG_RETURN_ERROR(MFG_ERROR_INVALID_ARGUMENTS, u8"Non dynamic buffers must have initial data");

		HRESULT hr = d3dRD->device->lpVtbl->CreateBuffer(d3dRD->device, &desc, NULL, &d3dIB->buffer);
		if (FAILED(hr))
			MFG_RETURN_ERROR(MFG_ERROR_INTERNAL, u8"CreateBuffer failed");
	}
	else
	{
		D3D11_SUBRESOURCE_DATA initData;

		initData.pSysMem = data;
		initData.SysMemPitch = 0;
		initData.SysMemSlicePitch = 0;

		HRESULT hr = d3dRD->device->lpVtbl->CreateBuffer(d3dRD->device, &desc, &initData, &d3dIB->buffer);
		if (FAILED(hr))
			MFG_RETURN_ERROR(MFG_ERROR_INTERNAL, u8"CreateBuffer failed");
	}

	*ib = d3dIB;
	return MFG_ERROR_OKAY;
}


mfgError mfgD3D11MapIndexBuffer(mfgRenderDevice* rd, mfgIndexBuffer* ib, void** memory)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (rd == NULL || ib == NULL || memory == NULL) return MFG_ERROR_INVALID_ARGUMENTS;
#endif

	mfgD3D11RenderDevice* d3dRD = (mfgD3D11RenderDevice*)rd;
	mfgD3D11IndexBuffer* d3dIB = ib;

	D3D11_MAPPED_SUBRESOURCE map;
	HRESULT hr = d3dRD->deviceContext->lpVtbl->Map(d3dRD->deviceContext, d3dIB->buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
	if (FAILED(hr))
		MFG_RETURN_ERROR(MFG_ERROR_INTERNAL, u8"Map failed");

	*memory = map.pData;

	return MFG_ERROR_OKAY;
}

mfgError mfgD3D11UnmapIndexBuffer(mfgRenderDevice* rd, mfgIndexBuffer* ib)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (rd == NULL || ib == NULL) return MFG_ERROR_INVALID_ARGUMENTS;
#endif

	mfgD3D11RenderDevice* d3dRD = (mfgD3D11RenderDevice*)rd;
	mfgD3D11IndexBuffer* d3dIB = ib;
	d3dRD->deviceContext->lpVtbl->Unmap(d3dRD->deviceContext, d3dIB->buffer, 0);

	return MFG_ERROR_OKAY;
}

mfgError mfgD3D11SetIndexBuffer(mfgRenderDevice* rd, mfgIndexBuffer* ib)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (rd == NULL) return MFG_ERROR_INVALID_ARGUMENTS;
#endif

	mfgD3D11RenderDevice* d3dRD = (mfgD3D11RenderDevice*)rd;
	mfgD3D11IndexBuffer* d3dIB = ib;
	d3dRD->deviceContext->lpVtbl->IASetIndexBuffer(d3dRD->deviceContext, d3dIB->buffer, d3dIB->format, 0);

	return MFG_ERROR_OKAY;
}

void mfgD3D11DestroyVertexLayout(void* vl)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (vl == NULL) abort();
#endif

	mfgD3D11VertexLayout* d3dVL = vl;
	d3dVL->inputLayout->lpVtbl->Release(d3dVL->inputLayout);
	if (mfmDeallocate(((mfgD3D11RenderDevice*)d3dVL->base.renderDevice)->pool256, d3dVL) != MFM_ERROR_OKAY)
		abort();
}

mfgError mfgD3D11CreateVertexLayout(mfgRenderDevice* rd, mfgVertexLayout** vl, mfmU64 elementCount, const mfgVertexElement* elements, mfgVertexShader* vs)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{ if (rd == NULL || vl == NULL || elementCount == 0 || elementCount > 14 || vs == NULL || elements == NULL) return MFG_ERROR_INVALID_ARGUMENTS; }
#endif

	mfgD3D11RenderDevice* d3dRD = (mfgD3D11RenderDevice*)rd;

	// Allocate vertex layout
	mfgD3D11VertexLayout* d3dVL = NULL;
	if (mfmAllocate(d3dRD->pool256, &d3dVL, sizeof(mfgD3D11VertexLayout)) != MFM_ERROR_OKAY)
		MFG_RETURN_ERROR(MFG_ERROR_ALLOCATION_FAILED, u8"Failed to allocate vertex layout on pool");

	// Init object
	d3dVL->base.object.destructorFunc = &mfgD3D11DestroyVertexLayout;
	d3dVL->base.object.referenceCount = 0;
	d3dVL->base.renderDevice = rd;

	// Create input layout
	if (elementCount > 16)
		MFG_RETURN_ERROR(MFG_ERROR_INVALID_ARGUMENTS, u8"Only up to 16 vertex elements are supported");
	d3dVL->elementCount = elementCount;

	D3D11_INPUT_ELEMENT_DESC inputDesc[16];
	char names[16][16];

	for (mfmU64 i = 0; i < elementCount; ++i)
	{
		ZeroMemory(&inputDesc[i], sizeof(inputDesc[i]));

		// Get semantic name
		{
			mfgMetaDataInputVariable* var = ((mfgD3D11VertexShader*)vs)->md->firstInputVar;
			while (var != NULL)
			{
				if (strcmp(var->name, elements[i].name) == 0)
					break;
				var = var->next;
			}

			if (var == NULL)
				MFG_RETURN_ERROR(MFG_ERROR_NOT_FOUND, u8"Couldn't find vertex element with matching name");
			snprintf(names[i], 16, u8"IN%dIN", var->id);
			inputDesc[i].SemanticName = names[i];
		}

		inputDesc[i].SemanticIndex = 0;
		inputDesc[i].InputSlot = elements[i].bufferIndex;
		inputDesc[i].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		inputDesc[i].AlignedByteOffset = elements[i].offset;
		inputDesc[i].InstanceDataStepRate = 0;
		d3dVL->offsets[i] = elements[i].offset;
		d3dVL->strides[i] = elements[i].stride;

		// Get format
		switch (elements[i].size)
		{
			case 1:
				switch (elements[i].type)
				{
					case MFG_BYTE: inputDesc[i].Format = DXGI_FORMAT_R8_SINT; break;
					case MFG_SHORT: inputDesc[i].Format = DXGI_FORMAT_R16_SINT; break;
					case MFG_INT: inputDesc[i].Format = DXGI_FORMAT_R32_SINT; break;
					case MFG_UBYTE: inputDesc[i].Format = DXGI_FORMAT_R8_UINT; break;
					case MFG_USHORT: inputDesc[i].Format = DXGI_FORMAT_R16_UINT; break;
					case MFG_UINT: inputDesc[i].Format = DXGI_FORMAT_R32_UINT; break;
					case MFG_NBYTE: inputDesc[i].Format = DXGI_FORMAT_R8_SNORM; break;
					case MFG_NSHORT: inputDesc[i].Format = DXGI_FORMAT_R16_SNORM; break;
					case MFG_NUBYTE: inputDesc[i].Format = DXGI_FORMAT_R8_UNORM; break;
					case MFG_NUSHORT: inputDesc[i].Format = DXGI_FORMAT_R16_UNORM; break;
					case MFG_FLOAT: inputDesc[i].Format = DXGI_FORMAT_R32_FLOAT; break;
					default: MFG_RETURN_ERROR(MFG_ERROR_INVALID_ARGUMENTS, u8"Unsupported element type");
				}
				break;
			case 2:
				switch (elements[i].type)
				{
					case MFG_BYTE: inputDesc[i].Format = DXGI_FORMAT_R8G8_SINT; break;
					case MFG_SHORT: inputDesc[i].Format = DXGI_FORMAT_R16G16_SINT; break;
					case MFG_INT: inputDesc[i].Format = DXGI_FORMAT_R32G32_SINT; break;
					case MFG_UBYTE: inputDesc[i].Format = DXGI_FORMAT_R8G8_UINT; break;
					case MFG_USHORT: inputDesc[i].Format = DXGI_FORMAT_R16G16_UINT; break;
					case MFG_UINT: inputDesc[i].Format = DXGI_FORMAT_R32G32_UINT; break;
					case MFG_NBYTE: inputDesc[i].Format = DXGI_FORMAT_R8G8_SNORM; break;
					case MFG_NSHORT: inputDesc[i].Format = DXGI_FORMAT_R16G16_SNORM; break;
					case MFG_NUBYTE: inputDesc[i].Format = DXGI_FORMAT_R8G8_UNORM; break;
					case MFG_NUSHORT: inputDesc[i].Format = DXGI_FORMAT_R16G16_UNORM; break;
					case MFG_FLOAT: inputDesc[i].Format = DXGI_FORMAT_R32G32_FLOAT; break;
					default: MFG_RETURN_ERROR(MFG_ERROR_INVALID_ARGUMENTS, u8"Unsupported element type");
				}
				break;
			case 3:
				switch (elements[i].type)
				{
					case MFG_INT: inputDesc[i].Format = DXGI_FORMAT_R32G32B32_SINT; break;
					case MFG_UINT: inputDesc[i].Format = DXGI_FORMAT_R32G32B32_UINT; break;
					case MFG_FLOAT: inputDesc[i].Format = DXGI_FORMAT_R32G32B32_FLOAT; break;
					default: MFG_RETURN_ERROR(MFG_ERROR_INVALID_ARGUMENTS, u8"Unsupported element type");
				}
				break;
			case 4:
				switch (elements[i].type)
				{
					case MFG_BYTE: inputDesc[i].Format = DXGI_FORMAT_R8G8B8A8_SINT; break;
					case MFG_SHORT: inputDesc[i].Format = DXGI_FORMAT_R16G16B16A16_SINT; break;
					case MFG_INT: inputDesc[i].Format = DXGI_FORMAT_R32G32B32A32_SINT; break;
					case MFG_UBYTE: inputDesc[i].Format = DXGI_FORMAT_R8G8B8A8_UINT; break;
					case MFG_USHORT: inputDesc[i].Format = DXGI_FORMAT_R16G16B16A16_UINT; break;
					case MFG_UINT: inputDesc[i].Format = DXGI_FORMAT_R32G32B32A32_UINT; break;
					case MFG_NBYTE: inputDesc[i].Format = DXGI_FORMAT_R8G8B8A8_SNORM; break;
					case MFG_NSHORT: inputDesc[i].Format = DXGI_FORMAT_R16G16B16A16_SNORM; break;
					case MFG_NUBYTE: inputDesc[i].Format = DXGI_FORMAT_R8G8B8A8_UNORM; break;
					case MFG_NUSHORT: inputDesc[i].Format = DXGI_FORMAT_R16G16B16A16_UNORM; break;
					case MFG_FLOAT: inputDesc[i].Format = DXGI_FORMAT_R32G32B32A32_FLOAT; break;
					default: MFG_RETURN_ERROR(MFG_ERROR_INVALID_ARGUMENTS, u8"Unsupported element type");
				}
				break;
			default: MFG_RETURN_ERROR(MFG_ERROR_INVALID_ARGUMENTS, u8"Unsupported element size");
		}
	}

	HRESULT hr = d3dRD->device->lpVtbl->CreateInputLayout(
		d3dRD->device,
		inputDesc,
		elementCount,
		((mfgD3D11VertexShader*)vs)->blob->lpVtbl->GetBufferPointer(((mfgD3D11VertexShader*)vs)->blob),
		((mfgD3D11VertexShader*)vs)->blob->lpVtbl->GetBufferSize(((mfgD3D11VertexShader*)vs)->blob),
		&d3dVL->inputLayout);
	if (FAILED(hr))
		MFG_RETURN_ERROR(MFG_ERROR_INTERNAL, u8"CreateInputLayout failed");

	*vl = d3dVL;

	return MFG_ERROR_OKAY;
}

void mfgD3D11DestroyVertexArray(void* va)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (va == NULL) abort();
#endif

	mfgD3D11VertexArray* d3dVA = va;
	if (mfmDeallocate(((mfgD3D11RenderDevice*)d3dVA->base.renderDevice)->pool256, d3dVA) != MFM_ERROR_OKAY)
		abort();
}

mfgError mfgD3D11CreateVertexArray(mfgRenderDevice* rd, mfgVertexArray** va, mfmU64 bufferCount, mfgVertexBuffer** buffers, mfgVertexLayout* vl)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{ if (rd == NULL || va == NULL || bufferCount == 0 || vl == NULL || buffers == NULL) return MFG_ERROR_INVALID_ARGUMENTS; }
#endif

	mfgD3D11RenderDevice* d3dRD = (mfgD3D11RenderDevice*)rd;

	// Allocate vertex array
	mfgD3D11VertexArray* d3dVA = NULL;
	if (mfmAllocate(d3dRD->pool256, &d3dVA, sizeof(mfgD3D11VertexArray)) != MFM_ERROR_OKAY)
		MFG_RETURN_ERROR(MFG_ERROR_ALLOCATION_FAILED, u8"Failed to allocate vertex array on pool");

	// Init object
	d3dVA->base.object.destructorFunc = &mfgD3D11DestroyVertexArray;
	d3dVA->base.object.referenceCount = 0;
	d3dVA->base.renderDevice = rd;

	if (bufferCount > 16)
		MFG_RETURN_ERROR(MFG_ERROR_INVALID_ARGUMENTS, u8"Vertex arrays can only have up to 16 vertex buffers");
	d3dVA->bufferCount = bufferCount;
	for (mfmU64 i = 0; i < bufferCount; ++i)
		d3dVA->buffers[i] = ((mfgD3D11VertexBuffer*)buffers[i])->buffer;
	d3dVA->layout = vl;

	*va = d3dVA;

	return MFG_ERROR_OKAY;
}

mfgError mfgD3D11SetVertexArray(mfgRenderDevice* rd, mfgVertexArray* va)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{ if (rd == NULL || va == NULL) return MFG_ERROR_INVALID_ARGUMENTS; }
#endif

	mfgD3D11RenderDevice* d3dRD = (mfgD3D11RenderDevice*)rd;
	mfgD3D11VertexArray* d3dVA = va;

	d3dRD->deviceContext->lpVtbl->IASetInputLayout(d3dRD->deviceContext, d3dVA->layout->inputLayout);
	d3dRD->deviceContext->lpVtbl->IASetVertexBuffers(d3dRD->deviceContext, 0, d3dVA->bufferCount, d3dVA->buffers, d3dVA->layout->strides, d3dVA->layout->offsets);

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

	rs->base.object.destructorFunc = &mfgD3D11DestroyRasterState;
	rs->base.object.referenceCount = 0;
	rs->base.renderDevice = rd;

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
	mfgD3D11DepthStencilState* dss = state;
	dss->state->lpVtbl->Release(dss->state);
	if (mfmDeallocate(((mfgD3D11RenderDevice*)dss->base.renderDevice)->pool32, dss) != MFM_ERROR_OKAY)
		abort();
}

mfgError mfgD3D11CreateDepthStencilState(mfgRenderDevice* rd, mfgDepthStencilState** state, const mfgDepthStencilStateDesc* desc)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{ if (rd == NULL || state == NULL || desc == NULL) return MFG_ERROR_INVALID_ARGUMENTS; }
#endif

	mfgD3D11RenderDevice* d3dRD = (mfgD3D11RenderDevice*)rd;

	mfgD3D11DepthStencilState* dss = NULL;
	if (mfmAllocate(d3dRD->pool32, &dss, sizeof(mfgD3D11DepthStencilState)) != MFM_ERROR_OKAY)
		MFG_RETURN_ERROR(MFG_ERROR_ALLOCATION_FAILED, u8"Failed to allocate depth stencil state on pool");

	dss->base.object.destructorFunc = &mfgD3D11DestroyDepthStencilState;
	dss->base.object.referenceCount = 0;
	dss->base.renderDevice = rd;

	D3D11_DEPTH_STENCIL_DESC dsDesc;
	
	dsDesc.DepthEnable = (desc->depthEnabled == MFM_TRUE) ? TRUE : FALSE;
	dsDesc.DepthWriteMask = (desc->depthWriteEnabled == MFM_TRUE) ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
	switch (desc->depthCompare)
	{
		case MFG_NEVER: dsDesc.DepthFunc = D3D11_COMPARISON_NEVER; break;
		case MFG_LESS: dsDesc.DepthFunc = D3D11_COMPARISON_LESS; break;
		case MFG_LEQUAL: dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL; break;
		case MFG_GREATER: dsDesc.DepthFunc = D3D11_COMPARISON_GREATER; break;
		case MFG_GEQUAL: dsDesc.DepthFunc = D3D11_COMPARISON_GREATER_EQUAL; break;
		case MFG_EQUAL: dsDesc.DepthFunc = D3D11_COMPARISON_EQUAL; break;
		case MFG_NEQUAL: dsDesc.DepthFunc = D3D11_COMPARISON_NOT_EQUAL; break;
		case MFG_ALWAYS: dsDesc.DepthFunc = D3D11_COMPARISON_ALWAYS; break;
		default: MFG_RETURN_ERROR(MFG_ERROR_INVALID_ARGUMENTS, u8"Unsupported depth compare function");
	}

	dsDesc.StencilEnable = (desc->stencilEnabled == MFM_TRUE) ? TRUE : FALSE;
	dsDesc.StencilReadMask = desc->stencilReadMask;
	dsDesc.StencilWriteMask = desc->stencilWriteMask;
	dss->stencilRef = desc->stencilRef;

	switch (desc->frontFaceDepthFail)
	{
		case MFG_KEEP: dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP; break;
		case MFG_ZERO: dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_ZERO; break;
		case MFG_REPLACE: dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_REPLACE; break;
		case MFG_INCREMENT: dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR; break;
		case MFG_INCREMENT_WRAP: dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR_SAT; break;
		case MFG_DECREMENT: dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR; break;
		case MFG_DECREMENT_WRAP: dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR_SAT; break;
		case MFG_INVERT: dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INVERT; break;
		default: MFG_RETURN_ERROR(MFG_ERROR_INVALID_ARGUMENTS, u8"Unsupported front face depth fail action");
	}

	switch (desc->frontFaceStencilFail)
	{
		case MFG_KEEP: dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP; break;
		case MFG_ZERO: dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_ZERO; break;
		case MFG_REPLACE: dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_REPLACE; break;
		case MFG_INCREMENT: dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_INCR; break;
		case MFG_INCREMENT_WRAP: dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_INCR_SAT; break;
		case MFG_DECREMENT: dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_DECR; break;
		case MFG_DECREMENT_WRAP: dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_DECR_SAT; break;
		case MFG_INVERT: dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_INVERT; break;
		default: MFG_RETURN_ERROR(MFG_ERROR_INVALID_ARGUMENTS, u8"Unsupported front face stencil fail action");
	}

	switch (desc->frontFaceStencilPass)
	{
		case MFG_KEEP: dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP; break;
		case MFG_ZERO: dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_ZERO; break;
		case MFG_REPLACE: dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE; break;
		case MFG_INCREMENT: dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR; break;
		case MFG_INCREMENT_WRAP: dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR_SAT; break;
		case MFG_DECREMENT: dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_DECR; break;
		case MFG_DECREMENT_WRAP: dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_DECR_SAT; break;
		case MFG_INVERT: dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INVERT; break;
		default: MFG_RETURN_ERROR(MFG_ERROR_INVALID_ARGUMENTS, u8"Unsupported front face stencil pass action");
	}

	switch (desc->frontFaceStencilCompare)
	{
		case MFG_NEVER: dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_NEVER; break;
		case MFG_LESS: dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_LESS; break;
		case MFG_LEQUAL: dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_LESS_EQUAL; break;
		case MFG_GREATER: dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_GREATER; break;
		case MFG_GEQUAL: dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_GREATER_EQUAL; break;
		case MFG_EQUAL: dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL; break;
		case MFG_NEQUAL: dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_NOT_EQUAL; break;
		case MFG_ALWAYS: dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS; break;
		default: MFG_RETURN_ERROR(MFG_ERROR_INVALID_ARGUMENTS, u8"Unsupported front face stencil compare function");
	}

	switch (desc->backFaceDepthFail)
	{
		case MFG_KEEP: dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP; break;
		case MFG_ZERO: dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_ZERO; break;
		case MFG_REPLACE: dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_REPLACE; break;
		case MFG_INCREMENT: dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR; break;
		case MFG_INCREMENT_WRAP: dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR_SAT; break;
		case MFG_DECREMENT: dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR; break;
		case MFG_DECREMENT_WRAP: dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR_SAT; break;
		case MFG_INVERT: dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_INVERT; break;
		default: MFG_RETURN_ERROR(MFG_ERROR_INVALID_ARGUMENTS, u8"Unsupported back face depth fail action");
	}

	switch (desc->backFaceStencilFail)
	{
		case MFG_KEEP: dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP; break;
		case MFG_ZERO: dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_ZERO; break;
		case MFG_REPLACE: dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_REPLACE; break;
		case MFG_INCREMENT: dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_INCR; break;
		case MFG_INCREMENT_WRAP: dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_INCR_SAT; break;
		case MFG_DECREMENT: dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_DECR; break;
		case MFG_DECREMENT_WRAP: dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_DECR_SAT; break;
		case MFG_INVERT: dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_INVERT; break;
		default: MFG_RETURN_ERROR(MFG_ERROR_INVALID_ARGUMENTS, u8"Unsupported back face stencil fail action");
	}

	switch (desc->backFaceStencilPass)
	{
		case MFG_KEEP: dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP; break;
		case MFG_ZERO: dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_ZERO; break;
		case MFG_REPLACE: dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE; break;
		case MFG_INCREMENT: dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_INCR; break;
		case MFG_INCREMENT_WRAP: dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_INCR_SAT; break;
		case MFG_DECREMENT: dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_DECR; break;
		case MFG_DECREMENT_WRAP: dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_DECR_SAT; break;
		case MFG_INVERT: dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_INVERT; break;
		default: MFG_RETURN_ERROR(MFG_ERROR_INVALID_ARGUMENTS, u8"Unsupported back face stencil pass action");
	}

	switch (desc->backFaceStencilCompare)
	{
		case MFG_NEVER: dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_NEVER; break;
		case MFG_LESS: dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_LESS; break;
		case MFG_LEQUAL: dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_LESS_EQUAL; break;
		case MFG_GREATER: dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_GREATER; break;
		case MFG_GEQUAL: dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_GREATER_EQUAL; break;
		case MFG_EQUAL: dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_EQUAL; break;
		case MFG_NEQUAL: dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_NOT_EQUAL; break;
		case MFG_ALWAYS: dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS; break;
		default: MFG_RETURN_ERROR(MFG_ERROR_INVALID_ARGUMENTS, u8"Unsupported back face stencil compare function");
	}

	HRESULT hr = d3dRD->device->lpVtbl->CreateDepthStencilState(d3dRD->device, &dsDesc, &dss->state);
	if (FAILED(hr))
		MFG_RETURN_ERROR(MFG_ERROR_INTERNAL, u8"CreateDepthStencilState failed");

	*state = dss;

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
	mfgD3D11BlendState* bs = state;
	bs->state->lpVtbl->Release(bs->state);
	if (mfmDeallocate(((mfgD3D11RenderDevice*)bs->base.renderDevice)->pool32, bs) != MFM_ERROR_OKAY)
		abort();
}

mfgError mfgD3D11CreateBlendState(mfgRenderDevice* rd, mfgBlendState** state, const mfgBlendStateDesc* desc)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	{ if (rd == NULL || state == NULL || desc == NULL) return MFG_ERROR_INVALID_ARGUMENTS; }
#endif
	mfgD3D11RenderDevice* d3dRD = (mfgD3D11RenderDevice*)rd;

	mfgD3D11BlendState* bs = NULL;
	if (mfmAllocate(d3dRD->pool32, &bs, sizeof(mfgD3D11BlendState)) != MFM_ERROR_OKAY)
		MFG_RETURN_ERROR(MFG_ERROR_ALLOCATION_FAILED, u8"Failed to allocate blend state on pool");

	bs->base.object.destructorFunc = &mfgD3D11DestroyBlendState;
	bs->base.object.referenceCount = 0;
	bs->base.renderDevice = rd;

	D3D11_BLEND_DESC bDesc;

	bDesc.AlphaToCoverageEnable = FALSE;
	bDesc.IndependentBlendEnable = FALSE;

	bDesc.RenderTarget[0].BlendEnable = (desc->blendEnabled == MFM_TRUE) ? TRUE : FALSE;

	switch (desc->blendOperation)
	{
		case MFG_ADD: bDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD; break;
		case MFG_SUBTRACT: bDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_SUBTRACT; break;
		case MFG_REV_SUBTRACT: bDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_REV_SUBTRACT; break;
		case MFG_MIN: bDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_MIN; break;
		case MFG_MAX: bDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_MAX; break;
		default: MFG_RETURN_ERROR(MFG_ERROR_INVALID_ARGUMENTS, u8"Unsupported blend operation");
	}

	switch (desc->sourceFactor)
	{
		case MFG_ZERO: bDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ZERO; break;
		case MFG_ONE: bDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE; break;
		case MFG_SRC_COLOR: bDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_COLOR; break;
		case MFG_INV_SRC_COLOR: bDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_INV_SRC_COLOR; break;
		case MFG_DST_COLOR: bDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_DEST_COLOR; break;
		case MFG_INV_DST_COLOR: bDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_INV_DEST_COLOR; break;
		case MFG_SRC_ALPHA: bDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA; break;
		case MFG_INV_SRC_ALPHA: bDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_INV_SRC_ALPHA; break;
		case MFG_DST_ALPHA: bDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_DEST_ALPHA; break;
		case MFG_INV_DST_ALPHA: bDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_INV_DEST_ALPHA; break;

		default: MFG_RETURN_ERROR(MFG_ERROR_INVALID_ARGUMENTS, u8"Unsupported blend source factor");
	}

	switch (desc->destinationFactor)
	{
		case MFG_ZERO: bDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO; break;
		case MFG_ONE: bDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE; break;
		case MFG_SRC_COLOR: bDesc.RenderTarget[0].DestBlend = D3D11_BLEND_SRC_COLOR; break;
		case MFG_INV_SRC_COLOR: bDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_COLOR; break;
		case MFG_DST_COLOR: bDesc.RenderTarget[0].DestBlend = D3D11_BLEND_DEST_COLOR; break;
		case MFG_INV_DST_COLOR: bDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_DEST_COLOR; break;
		case MFG_SRC_ALPHA: bDesc.RenderTarget[0].DestBlend = D3D11_BLEND_SRC_ALPHA; break;
		case MFG_INV_SRC_ALPHA: bDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA; break;
		case MFG_DST_ALPHA: bDesc.RenderTarget[0].DestBlend = D3D11_BLEND_DEST_ALPHA; break;
		case MFG_INV_DST_ALPHA: bDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_DEST_ALPHA; break;

		default: MFG_RETURN_ERROR(MFG_ERROR_INVALID_ARGUMENTS, u8"Unsupported blend destination factor");
	}

	switch (desc->blendAlphaOperation)
	{
		case MFG_ADD: bDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD; break;
		case MFG_SUBTRACT: bDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_SUBTRACT; break;
		case MFG_REV_SUBTRACT: bDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_REV_SUBTRACT; break;
		case MFG_MIN: bDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_MIN; break;
		case MFG_MAX: bDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_MAX; break;
		default: MFG_RETURN_ERROR(MFG_ERROR_INVALID_ARGUMENTS, u8"Unsupported blend alpha operation");
	}

	switch (desc->sourceAlphaFactor)
	{
		case MFG_ZERO: bDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO; break;
		case MFG_ONE: bDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE; break;
		case MFG_SRC_COLOR: bDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_COLOR; break;
		case MFG_INV_SRC_COLOR: bDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_INV_SRC_COLOR; break;
		case MFG_DST_COLOR: bDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_DEST_COLOR; break;
		case MFG_INV_DST_COLOR: bDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_INV_DEST_COLOR; break;
		case MFG_SRC_ALPHA: bDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA; break;
		case MFG_INV_SRC_ALPHA: bDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA; break;
		case MFG_DST_ALPHA: bDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_DEST_ALPHA; break;
		case MFG_INV_DST_ALPHA: bDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_INV_DEST_ALPHA; break;

		default: MFG_RETURN_ERROR(MFG_ERROR_INVALID_ARGUMENTS, u8"Unsupported blend source alpha factor");
	}

	switch (desc->destinationAlphaFactor)
	{
		case MFG_ZERO: bDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO; break;
		case MFG_ONE: bDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE; break;
		case MFG_SRC_COLOR: bDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_SRC_COLOR; break;
		case MFG_INV_SRC_COLOR: bDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_COLOR; break;
		case MFG_DST_COLOR: bDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_DEST_COLOR; break;
		case MFG_INV_DST_COLOR: bDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_DEST_COLOR; break;
		case MFG_SRC_ALPHA: bDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_SRC_ALPHA; break;
		case MFG_INV_SRC_ALPHA: bDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA; break;
		case MFG_DST_ALPHA: bDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_DEST_ALPHA; break;
		case MFG_INV_DST_ALPHA: bDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_DEST_ALPHA; break;

		default: MFG_RETURN_ERROR(MFG_ERROR_INVALID_ARGUMENTS, u8"Unsupported blend destination alpha factor");
	}

	bDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	HRESULT hr = d3dRD->device->lpVtbl->CreateBlendState(d3dRD->device, &bDesc, &bs->state);
	if (FAILED(hr))
		MFG_RETURN_ERROR(MFG_ERROR_INTERNAL, u8"CreateBlendState failed");

	*state = bs;

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
	if (rd == NULL) return MFG_ERROR_INVALID_ARGUMENTS;
#endif
	


	return MFG_ERROR_OKAY;
}

mfgError mfgD3D11GetPropertyI(mfgRenderDevice* rd, mfgEnum id, mfmI32* value)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (rd == NULL || value == NULL) return MFG_ERROR_INVALID_ARGUMENTS;
#endif

	INT out;

	if (id == MFG_MAX_ANISOTROPY)
		out = 16;
	else if (id == MFG_CONSTANT_ALIGN)
		out = 16;
	else
		MFG_RETURN_ERROR(MFG_ERROR_INVALID_ARGUMENTS, u8"Unsupported property ID");

	*value = out;

	return MFG_ERROR_OKAY;
}

mfgError mfgD3D11GetPropertyF(mfgRenderDevice* rd, mfgEnum id, mfmF32* value)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (rd == NULL || value == NULL) return MFG_ERROR_INVALID_ARGUMENTS;
#endif

	FLOAT out;

	if (id == MFG_MAX_ANISOTROPY)
		out = 16.0f;
	else if (id == MFG_CONSTANT_ALIGN)
		out = 16.0f;
	else
		MFG_RETURN_ERROR(MFG_ERROR_INVALID_ARGUMENTS, u8"Unsupported property ID");

	*value = out;

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


			D3D11_SDK_VERSION;

			D3D_FEATURE_LEVEL levels[] =
			{
				D3D_FEATURE_LEVEL_11_1,
			};

			hr = D3D11CreateDeviceAndSwapChain(
				NULL,
				D3D_DRIVER_TYPE_HARDWARE,
				NULL,
				flags,
				levels,
				1,
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

	rd->base.getPropertyI = &mfgD3D11GetPropertyI;
	rd->base.getPropertyF = &mfgD3D11GetPropertyF;

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