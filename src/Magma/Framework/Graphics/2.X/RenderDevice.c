#include "RenderDevice.h"
#include <string.h>

struct
{
	mfmBool active;
	mfgV2XRenderDeviceCreatorFunction func;
	mfsUTF8CodeUnit type[16];
} mfgRenderDeviceCreatorRegisterEntries[MFG_MAX_RENDER_DEVICE_CREATOR_REGISTER_ENTRIES];

mfError mfgV2XInitRenderDevices()
{
	// Init all entries

	for (mfmU64 i = 0; i < MFG_MAX_RENDER_DEVICE_CREATOR_REGISTER_ENTRIES; ++i)
	{
		mfgRenderDeviceCreatorRegisterEntries[i].active = MFM_FALSE;
		mfgRenderDeviceCreatorRegisterEntries[i].func = NULL;
	}

	return MF_ERROR_OKAY;
}

void mfgV2XTerminateRenderDevices()
{
	// Reset all entries

	for (mfmU64 i = 0; i < MFG_MAX_RENDER_DEVICE_CREATOR_REGISTER_ENTRIES; ++i)
	{
		mfgRenderDeviceCreatorRegisterEntries[i].active = MFM_FALSE;
		mfgRenderDeviceCreatorRegisterEntries[i].func = NULL;
	}
}

mfError mfgV2XRegisterRenderDeviceCreator(const mfsUTF8CodeUnit * type, mfgV2XRenderDeviceCreatorFunction func)
{
	for (mfmU64 i = 0; i < MFG_MAX_RENDER_DEVICE_CREATOR_REGISTER_ENTRIES; ++i)
		if (mfgRenderDeviceCreatorRegisterEntries[i].active == MFM_FALSE)
		{
			mfgRenderDeviceCreatorRegisterEntries[i].active = MFM_TRUE;
			mfgRenderDeviceCreatorRegisterEntries[i].func = func;

			for (mfmU32 j = 0; j < 16; ++j)
			{
				mfgRenderDeviceCreatorRegisterEntries[i].type[j] = type[j];
				if (type[j] == '\0')
					break;
			}

			return MF_ERROR_OKAY;
		}

	return MFG_ERROR_NO_REGISTER_ENTRIES;
}

mfError mfgV2XCreateRenderDevice(const mfsUTF8CodeUnit * type, mfgV2XRenderDevice ** renderDevice, mfiWindow * window, const mfgV2XRenderDeviceDesc * desc, void * allocator)
{
	for (mfmU64 i = 0; i < MFG_MAX_RENDER_DEVICE_CREATOR_REGISTER_ENTRIES; ++i)
		if (mfgRenderDeviceCreatorRegisterEntries[i].active == MFM_TRUE &&
			strcmp(type, mfgRenderDeviceCreatorRegisterEntries[i].type) == 0)
			return mfgRenderDeviceCreatorRegisterEntries[i].func(renderDevice, window, desc, allocator);
	return MFG_ERROR_TYPE_NOT_REGISTERED;
}

void mfgV2XDestroyRenderDevice(void * renderDevice)
{
	((mfmObject*)renderDevice)->destructorFunc(renderDevice);
}

void mfgV2XDefaultRenderDeviceDesc(mfgV2XRenderDeviceDesc * desc)
{
	desc->vsyncEnabled = MFM_FALSE;
}

void mfgV2XDefaultVertexElement(mfgV2XVertexElement * element)
{
	memset(element->name, '\0', sizeof(element->name));
	element->stride = 0;
	element->offset = 0;
	element->bufferIndex = 0;
	element->type = 0;
	element->size = 0;
}

void mfgV2XDefaultSamplerDesc(mfgV2XSamplerDesc * desc)
{
	desc->borderColor[0] = 0.0f;
	desc->borderColor[1] = 0.0f;
	desc->borderColor[2] = 0.0f;
	desc->borderColor[3] = 1.0f;
	desc->maxAnisotropy = 1;
	desc->minFilter = MFG_NEAREST;
	desc->magFilter = MFG_NEAREST;
	desc->mipmapFilter = MFG_NONE;
	desc->addressU = MFG_CLAMP;
	desc->addressV = MFG_CLAMP;
	desc->addressW = MFG_CLAMP;
}

void mfgV2XDefaultRasterStateDesc(mfgV2XRasterStateDesc * desc)
{
	desc->cullEnabled = MFM_FALSE;
	desc->frontFace = MFG_CCW;
	desc->cullFace = MFG_BACK;
	desc->rasterMode = MFG_FILL;
}

void mfgV2XDefaultDepthStencilStateDesc(mfgV2XDepthStencilStateDesc * desc)
{
	desc->depthEnabled = MFM_FALSE;
	desc->depthWriteEnabled = MFM_FALSE;
	desc->depthNear = 0.0f;
	desc->depthFar = 1.0f;
	desc->depthCompare = MFG_LESS;
	desc->stencilRef = 0;
	desc->stencilEnabled = MFM_FALSE;
	desc->stencilReadMask = 0xFF;
	desc->stencilWriteMask = 0xFF;
	desc->frontFaceStencilCompare = MFG_ALWAYS;
	desc->frontFaceStencilFail = MFG_KEEP;
	desc->frontFaceStencilPass = MFG_KEEP;
	desc->frontFaceDepthFail = MFG_KEEP;
	desc->backFaceStencilCompare = MFG_ALWAYS;
	desc->backFaceStencilFail = MFG_KEEP;
	desc->backFaceStencilPass = MFG_KEEP;
	desc->backFaceDepthFail = MFG_KEEP;
}

void mfgV2XDefaultBlendStateDesc(mfgV2XBlendStateDesc * desc)
{
	desc->blendEnabled = MFM_FALSE;
	desc->sourceFactor = MFG_ONE;
	desc->destinationFactor = MFG_ZERO;
	desc->blendOperation = MFG_ADD;
	desc->sourceAlphaFactor = MFG_ONE;
	desc->destinationAlphaFactor = MFG_ZERO;
	desc->blendAlphaOperation = MFG_ADD;
}

mfError mfgV2XCreateVertexShader(mfgV2XRenderDevice * rd, mfgV2XVertexShader ** vertexShader, const mfmU8 * bytecode, mfmU64 bytecodeSize, const mfgMetaData * metaData)
{
	return rd->createVertexShader(rd, vertexShader, bytecode, bytecodeSize, metaData);
}

void mfgV2XDestroyVertexShader(void * vertexShader)
{
	((mfgV2XRenderDeviceObject*)vertexShader)->renderDevice->destroyVertexShader(vertexShader);
}

mfError mfgV2XCreatePixelShader(mfgV2XRenderDevice * rd, mfgV2XPixelShader ** pixelShader, const mfmU8 * bytecode, mfmU64 bytecodeSize, const mfgMetaData * metaData)
{
	return rd->createPixelShader(rd, pixelShader, bytecode, bytecodeSize, metaData);
}

void mfgV2XDestroyPixelShader(void * pixelShader)
{
	((mfgV2XRenderDeviceObject*)pixelShader)->renderDevice->destroyPixelShader(pixelShader);
}

mfError mfgV2XCreatePipeline(mfgV2XRenderDevice * rd, mfgV2XPipeline ** pipeline, mfgV2XVertexShader * vs, mfgV2XPixelShader * ps)
{
	return rd->createPipeline(rd, pipeline, vs, ps);
}

void mfgV2XDestroyPipeline(void * pipeline)
{
	((mfgV2XRenderDeviceObject*)pipeline)->renderDevice->destroyPipeline(pipeline);
}

mfError mfgV2XSetPipeline(mfgV2XRenderDevice * rd, mfgV2XPipeline * pipeline)
{
	return rd->setPipeline(rd, pipeline);
}

mfError mfgV2XGetVertexShaderBindingPoint(mfgV2XRenderDevice * rd, mfgV2XBindingPoint ** bp, mfgV2XVertexShader * vertexShader, const mfsUTF8CodeUnit * name)
{
	return rd->getVertexShaderBindingPoint(rd, bp, vertexShader, name);
}

mfError mfgV2XGetPixelShaderBindingPoint(mfgV2XRenderDevice * rd, mfgV2XBindingPoint ** bp, mfgV2XPixelShader * pixelShader, const mfsUTF8CodeUnit * name)
{
	return rd->getPixelShaderBindingPoint(rd, bp, pixelShader, name);
}

mfError mfgV2XBindConstantBuffer(mfgV2XRenderDevice * rd, mfgV2XBindingPoint * bp, mfgV2XConstantBuffer * cb)
{
	return rd->bindConstantBuffer(rd, bp, cb);
}

mfError mfgV2XBindConstantBufferRange(mfgV2XRenderDevice * rd, mfgV2XBindingPoint * bp, mfgV2XConstantBuffer * cb, mfmU64 offset, mfmU64 size)
{
	return rd->bindConstantBufferRange(rd, bp, cb, offset, size);
}

mfError mfgV2XBindTexture1D(mfgV2XRenderDevice * rd, mfgV2XBindingPoint * bp, mfgV2XTexture1D * tex)
{
	return rd->bindTexture1D(rd, bp, tex);
}

mfError mfgV2XBindTexture2D(mfgV2XRenderDevice * rd, mfgV2XBindingPoint * bp, mfgV2XTexture2D * tex)
{
	return rd->bindTexture2D(rd, bp, tex);
}

mfError mfgV2XBindTexture3D(mfgV2XRenderDevice * rd, mfgV2XBindingPoint * bp, mfgV2XTexture3D * tex)
{
	return rd->bindTexture3D(rd, bp, tex);
}

mfError mfgV2XBindRenderTexture(mfgV2XRenderDevice * rd, mfgV2XBindingPoint * bp, mfgV2XRenderTexture * tex)
{
	return rd->bindRenderTexture(rd, bp, tex);
}

mfError mfgV2XBindSampler(mfgV2XRenderDevice * rd, mfgV2XBindingPoint * bp, mfgV2XSampler * sampler)
{
	return rd->bindSampler(rd, bp, sampler);
}

mfError mfgV2XCreateConstantBuffer(mfgV2XRenderDevice * rd, mfgV2XConstantBuffer ** cb, mfmU64 size, const void * data, mfgEnum usage)
{
	return rd->createConstantBuffer(rd, cb, size, data, usage);
}

void mfgV2XDestroyConstantBuffer(void * cb)
{
	((mfgV2XRenderDeviceObject*)cb)->renderDevice->destroyConstantBuffer(cb);
}

mfError mfgV2XMapConstantBuffer(mfgV2XRenderDevice * rd, mfgV2XConstantBuffer * cb, void ** memory)
{
	return rd->mapConstantBuffer(rd, cb, memory);
}

mfError mfgV2XUnmapConstantBuffer(mfgV2XRenderDevice * rd, mfgV2XConstantBuffer * cb)
{
	return rd->unmapConstantBuffer(rd, cb);
}

mfError mfgV2XCreateVertexBuffer(mfgV2XRenderDevice * rd, mfgV2XVertexBuffer ** vb, mfmU64 size, const void * data, mfgEnum usage)
{
	return rd->createVertexBuffer(rd, vb, size, data, usage);
}

void mfgV2XDestroyVertexBuffer(void * vb)
{
	((mfgV2XRenderDeviceObject*)vb)->renderDevice->destroyVertexBuffer(vb);
}

mfError mfgV2XMapVertexBuffer(mfgV2XRenderDevice * rd, mfgV2XVertexBuffer * vb, void ** memory)
{
	return rd->mapVertexBuffer(rd, vb, memory);
}

mfError mfgV2XUnmapVertexBuffer(mfgV2XRenderDevice * rd, mfgV2XVertexBuffer * vb)
{
	return rd->unmapVertexBuffer(rd, vb);
}

mfError mfgV2XCreateVertexLayout(mfgV2XRenderDevice * rd, mfgV2XVertexLayout ** vl, mfmU64 elementCount, const mfgV2XVertexElement * elements, mfgV2XVertexShader * vs)
{
	return rd->createVertexLayout(rd, vl, elementCount, elements, vs);
}

void mfgV2XDestroyVertexLayout(void * vl)
{
	((mfgV2XRenderDeviceObject*)vl)->renderDevice->destroyVertexLayout(vl);
}

mfError mfgV2XCreateVertexArray(mfgV2XRenderDevice * rd, mfgV2XVertexArray ** va, mfmU64 vbCount, mfgV2XVertexBuffer ** vbs, mfgV2XVertexLayout * vl)
{
	return rd->createVertexArray(rd, va, vbCount, vbs, vl);
}

void mfgV2XDestroyVertexArray(void * va)
{
	((mfgV2XRenderDeviceObject*)va)->renderDevice->destroyVertexArray(va);
}

mfError mfgV2XSetVertexArray(mfgV2XRenderDevice * rd, mfgV2XVertexArray * va)
{
	return rd->setVertexArray(rd, va);
}

mfError mfgV2XCreateIndexBuffer(mfgV2XRenderDevice * rd, mfgV2XIndexBuffer ** ib, mfmU64 size, const void * data, mfgEnum format, mfgEnum usage)
{
	return rd->createIndexBuffer(rd, ib, size, data, format, usage);
}

void mfgV2XDestroyIndexBuffer(void * ib)
{
	((mfgV2XRenderDeviceObject*)ib)->renderDevice->destroyIndexBuffer(ib);
}

mfError mfgV2XMapIndexBuffer(mfgV2XRenderDevice * rd, mfgV2XIndexBuffer * ib, void ** memory)
{
	return rd->mapIndexBuffer(rd, ib, memory);
}

mfError mfgV2XUnmapIndexBuffer(mfgV2XRenderDevice * rd, mfgV2XIndexBuffer * ib)
{
	return rd->unmapIndexBuffer(rd, ib);
}

mfError mfgV2XSetIndexBuffer(mfgV2XRenderDevice * rd, mfgV2XIndexBuffer * ib)
{
	return rd->setIndexBuffer(rd, ib);
}

mfError mfgV2XCreateTexture1D(mfgV2XRenderDevice * rd, mfgV2XTexture1D ** tex, mfmU64 width, mfgEnum format, const void * data, mfgEnum usage)
{
	return rd->createTexture1D(rd, tex, width, format, data, usage);
}

void mfgV2XDestroyTexture1D(void * tex)
{
	((mfgV2XRenderDeviceObject*)tex)->renderDevice->destroyTexture1D(tex);
}

mfError mfgV2XUpdateTexture1D(mfgV2XRenderDevice * rd, mfgV2XTexture1D * tex, mfmU64 dstX, mfmU64 width, const void * data)
{
	return rd->updateTexture1D(rd, tex, dstX, width, data);
}

mfError mfgV2XGenerateTexture1DMipmaps(mfgV2XRenderDevice * rd, mfgV2XTexture1D * tex)
{
	return rd->generateTexture1DMipmaps(rd, tex);
}

mfError mfgV2XCreateTexture2D(mfgV2XRenderDevice * rd, mfgV2XTexture2D ** tex, mfmU64 width, mfmU64 height, mfgEnum format, const void * data, mfgEnum usage)
{
	return rd->createTexture2D(rd, tex, width, height, format, data, usage);
}

void mfgV2XDestroyTexture2D(void * tex)
{
	((mfgV2XRenderDeviceObject*)tex)->renderDevice->destroyTexture2D(tex);
}

mfError mfgV2XUpdateTexture2D(mfgV2XRenderDevice * rd, mfgV2XTexture2D * tex, mfmU64 dstX, mfmU64 dstY, mfmU64 width, mfmU64 height, const void * data)
{
	return rd->updateTexture2D(rd, tex, dstX, dstY, width, height, data);
}

mfError mfgV2XGenerateTexture2DMipmaps(mfgV2XRenderDevice * rd, mfgV2XTexture2D * tex)
{
	return rd->generateTexture2DMipmaps(rd, tex);
}

mfError mfgV2XCreateTexture3D(mfgV2XRenderDevice * rd, mfgV2XTexture3D ** tex, mfmU64 width, mfmU64 height, mfmU64 depth, mfgEnum format, const void * data, mfgEnum usage)
{
	return rd->createTexture3D(rd, tex, width, height, depth, format, data, usage);
}

void mfgV2XDestroyTexture3D(void * tex)
{
	((mfgV2XRenderDeviceObject*)tex)->renderDevice->destroyTexture3D(tex);
}

mfError mfgV2XUpdateTexture3D(mfgV2XRenderDevice * rd, mfgV2XTexture3D * tex, mfmU64 dstX, mfmU64 dstY, mfmU64 dstZ, mfmU64 width, mfmU64 height, mfmU64 depth, const void * data)
{
	return rd->updateTexture3D(rd, tex, dstX, dstY, dstZ, width, height, depth, data);
}

mfError mfgV2XGenerateTexture3DMipmaps(mfgV2XRenderDevice * rd, mfgV2XTexture3D * tex)
{
	return rd->generateTexture3DMipmaps(rd, tex);
}

mfError mfgV2XCreateSampler(mfgV2XRenderDevice * rd, mfgV2XSampler ** sampler, const mfgV2XSamplerDesc * desc)
{
	return rd->createSampler(rd, sampler, desc);
}

void mfgV2XDestroySampler(void * sampler)
{
	((mfgV2XRenderDeviceObject*)sampler)->renderDevice->destroySampler(sampler);
}

mfError mfgV2XCreateRasterState(mfgV2XRenderDevice * rd, mfgV2XRasterState ** state, const mfgV2XRasterStateDesc * desc)
{
	return rd->createRasterState(rd, state, desc);
}

void mfgV2XDestroyRasterState(void * state)
{
	((mfgV2XRenderDeviceObject*)state)->renderDevice->destroyRasterState(state);
}

mfError mfgV2XSetRasterState(mfgV2XRenderDevice * rd, mfgV2XRasterState * state)
{
	return rd->setRasterState(rd, state);
}

mfError mfgV2XCreateDepthStencilState(mfgV2XRenderDevice * rd, mfgV2XDepthStencilState ** state, const mfgV2XDepthStencilStateDesc * desc)
{
	return rd->createDepthStencilState(rd, state, desc);
}

void mfgV2XDestroyDepthStencilState(void * state)
{
	((mfgV2XRenderDeviceObject*)state)->renderDevice->destroyDepthStencilState(state);
}

mfError mfgV2XSetDepthStencilState(mfgV2XRenderDevice * rd, mfgV2XDepthStencilState * state)
{
	return rd->setDepthStencilState(rd, state);
}

mfError mfgV2XCreateBlendState(mfgV2XRenderDevice * rd, mfgV2XBlendState ** state, const mfgV2XBlendStateDesc * desc)
{
	return rd->createBlendState(rd, state, desc);
}

void mfgV2XDestroyBlendState(void * state)
{
	((mfgV2XRenderDeviceObject*)state)->renderDevice->destroyBlendState(state);
}

mfError mfgV2XSetBlendState(mfgV2XRenderDevice * rd, mfgV2XBlendState * state)
{
	return rd->setBlendState(rd, state);
}

mfError mfgV2XCreateRenderTexture(mfgV2XRenderDevice * rd, mfgV2XRenderTexture ** tex, mfmU64 width, mfmU64 height, mfgEnum format)
{
	return rd->createRenderTexture(rd, tex, width, height, format);
}

void mfgV2XDestroyRenderTexture(void * tex)
{
	((mfgV2XRenderDeviceObject*)tex)->renderDevice->destroyRenderTexture(tex);
}

mfError mfgV2XCreateDepthStencilTexture(mfgV2XRenderDevice * rd, mfgV2XDepthStencilTexture ** tex, mfmU64 width, mfmU64 height, mfgEnum format)
{
	return rd->createDepthStencilTexture(rd, tex, width, height, format);
}

void mfgV2XDestroyDepthStencilTexture(void * tex)
{
	((mfgV2XRenderDeviceObject*)tex)->renderDevice->destroyDepthStencilTexture(tex);
}

mfError mfgV2XCreateFramebuffer(mfgV2XRenderDevice * rd, mfgV2XFramebuffer ** fb, mfmU64 textureCount, mfgV2XRenderTexture ** textures, mfgV2XDepthStencilTexture * depthStencilTexture)
{
	return rd->createFramebuffer(rd, fb, textureCount, textures, depthStencilTexture);
}

void mfgV2XDestroyFramebuffer(void * fb)
{
	((mfgV2XRenderDeviceObject*)fb)->renderDevice->destroyFramebuffer(fb);
}

mfError mfgV2XSetFramebuffer(mfgV2XRenderDevice * rd, mfgV2XFramebuffer * fb)
{
	return rd->setFramebuffer(rd, fb);
}

mfError mfgV2XClearColor(mfgV2XRenderDevice * rd, mfmF32 r, mfmF32 g, mfmF32 b, mfmF32 a)
{
	return rd->clearColor(rd, r, g, b, a);
}

mfError mfgV2XClearDepth(mfgV2XRenderDevice * rd, mfmF32 depth)
{
	return rd->clearDepth(rd, depth);
}

mfError mfgV2XClearStencil(mfgV2XRenderDevice * rd, mfmI32 stencil)
{
	return rd->clearStencil(rd, stencil);
}

mfError mfgV2XDrawTriangles(mfgV2XRenderDevice * rd, mfmU64 offset, mfmU64 count)
{
	return rd->drawTriangles(rd, offset, count);
}

mfError mfgV2XDrawTrianglesIndexed(mfgV2XRenderDevice * rd, mfmU64 offset, mfmU64 count)
{
	return rd->drawTrianglesIndexed(rd, offset, count);
}

mfError mfgV2XSwapBuffers(mfgV2XRenderDevice * rd)
{
	return rd->swapBuffers(rd);
}

mfError mfgV2XGetPropertyI(mfgV2XRenderDevice * rd, mfgEnum id, mfmI32 * value)
{
	return rd->getPropertyI(rd, id, value);
}

mfError mfgV2XGetPropertyF(mfgV2XRenderDevice * rd, mfgEnum id, mfmF32 * value)
{
	return rd->getPropertyF(rd, id, value);
}

mfmBool mfgV2XGetErrorString(mfgV2XRenderDevice * rd, mfsUTF8CodeUnit * str, mfmU64 maxSize)
{
	return rd->getErrorString(rd, str, maxSize);
}
