#include "RenderDevice.h"
#include <string.h>

void mfgDefaultRenderDeviceDesc(mfgRenderDeviceDesc * desc)
{
	desc->vsyncEnabled = MFM_FALSE;
}

void mfgDefaultVertexElement(mfgVertexElement * element)
{
	memset(element->name, '\0', sizeof(element->name));
	element->stride = 0;
	element->offset = 0;
	element->bufferIndex = 0;
	element->type = 0;
	element->size = 0;
}

void mfgDefaultSamplerDesc(mfgSamplerDesc * desc)
{
	desc->borderColor[0] = 0.0f;
	desc->borderColor[1] = 0.0f;
	desc->borderColor[2] = 0.0f;
	desc->borderColor[3] = 0.0f;
	desc->maxAnisotropy = 1;
	desc->minFilter = MFG_NEAREST;
	desc->magFilter = MFG_NEAREST;
	desc->mipmapFilter = MFG_NONE;
	desc->adressU = MFG_REPEAT;
	desc->adressV = MFG_REPEAT;
	desc->adressW = MFG_REPEAT;
}

void mfgDefaultRasterStateDesc(mfgRasterStateDesc * desc)
{
	desc->cullEnabled = MFM_TRUE;
	desc->frontFace = MFG_CCW;
	desc->cullFace = MFG_BACK;
	desc->rasterMode = MFG_FILL;
}

void mfgDefaultDepthStencilStateDesc(mfgDepthStencilStateDesc * desc)
{
	desc->depthEnabled = MFM_TRUE;
	desc->depthWriteEnabled = MFM_TRUE;
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

void mfgDefaultBlendStateDesc(mfgBlendStateDesc * desc)
{
	desc->blendEnabled = MFM_FALSE;
	desc->sourceFactor = MFG_ONE;
	desc->destinationFactor = MFG_ZERO;
	desc->blendOperation = MFG_ADD;
	desc->sourceAlphaFactor = MFG_ONE;
	desc->destinationAlphaFactor = MFG_ZERO;
	desc->blendAlphaOperation = MFG_ADD;
}

mfgError mfgCreateVertexShader(mfgRenderDevice * rd, mfgVertexShader ** vertexShader, const mfmU8 * bytecode, mfmU64 bytecodeSize, const mfgMetaData * metaData)
{
	return rd->createVertexShader(rd, vertexShader, bytecode, bytecodeSize, metaData);
}

void mfgDestroyVertexShader(void * vertexShader)
{
	((mfgRenderDeviceObject*)vertexShader)->renderDevice->destroyVertexShader(vertexShader);
}

mfgError mfgCreatePixelShader(mfgRenderDevice * rd, mfgPixelShader ** pixelShader, const mfmU8 * bytecode, mfmU64 bytecodeSize, const mfgMetaData * metaData)
{
	return rd->createPixelShader(rd, pixelShader, bytecode, bytecodeSize, metaData);
}

void mfgDestroyPixelShader(void * pixelShader)
{
	((mfgRenderDeviceObject*)pixelShader)->renderDevice->destroyPixelShader(pixelShader);
}

mfgError mfgCreatePipeline(mfgRenderDevice * rd, mfgPipeline ** pipeline, mfgVertexShader * vs, mfgPixelShader * ps)
{
	return rd->createPipeline(rd, pipeline, vs, ps);
}

void mfgDestroyPipeline(void * pipeline)
{
	((mfgRenderDeviceObject*)pipeline)->renderDevice->destroyPipeline(pipeline);
}

mfgError mfgSetPipeline(mfgRenderDevice * rd, mfgPipeline * pipeline)
{
	return rd->setPipeline(rd, pipeline);
}

mfgError mfgGetVertexShaderBindingPoint(mfgRenderDevice * rd, mfgBindingPoint ** bp, mfgVertexShader * vertexShader, const mfsUTF8CodeUnit * name)
{
	return rd->getVertexShaderBindingPoint(rd, bp, vertexShader, name);
}

mfgError mfgGetPixelShaderBindingPoint(mfgRenderDevice * rd, mfgBindingPoint ** bp, mfgPixelShader * pixelShader, const mfsUTF8CodeUnit * name)
{
	return rd->getPixelShaderBindingPoint(rd, bp, pixelShader, name);
}

mfgError mfgBindConstantBuffer(mfgRenderDevice * rd, mfgBindingPoint * bp, mfgConstantBuffer * cb)
{
	return rd->bindConstantBuffer(rd, bp, cb);
}

mfgError mfgBindConstantBufferRange(mfgRenderDevice * rd, mfgBindingPoint * bp, mfgConstantBuffer * cb, mfmU64 offset, mfmU64 size)
{
	return rd->bindConstantBufferRange(rd, bp, cb, offset, size);
}

mfgError mfgBindTexture1D(mfgRenderDevice * rd, mfgBindingPoint * bp, mfgTexture1D * tex)
{
	return rd->bindTexture1D(rd, bp, tex);
}

mfgError mfgBindTexture2D(mfgRenderDevice * rd, mfgBindingPoint * bp, mfgTexture2D * tex)
{
	return rd->bindTexture2D(rd, bp, tex);
}

mfgError mfgBindTexture3D(mfgRenderDevice * rd, mfgBindingPoint * bp, mfgTexture3D * tex)
{
	return rd->bindTexture3D(rd, bp, tex);
}

mfgError mfgBindSampler(mfgRenderDevice * rd, mfgBindingPoint * bp, mfgSampler * sampler)
{
	return rd->bindSampler(rd, bp, sampler);
}

mfgError mfgCreateConstantBuffer(mfgRenderDevice * rd, mfgConstantBuffer ** cb, mfmU64 size, const void * data, mfgEnum usage)
{
	return rd->createConstantBuffer(rd, cb, size, data, usage);
}

void mfgDestroyConstantBuffer(void * cb)
{
	((mfgRenderDeviceObject*)cb)->renderDevice->destroyConstantBuffer(cb);
}

mfgError mfgMapConstantBuffer(mfgRenderDevice * rd, mfgConstantBuffer * cb, void ** memory)
{
	return rd->mapConstantBuffer(rd, cb, memory);
}

mfgError mfgUnmapConstantBuffer(mfgRenderDevice * rd, mfgConstantBuffer * cb)
{
	return rd->unmapConstantBuffer(rd, cb);
}

mfgError mfgCreateVertexBuffer(mfgRenderDevice * rd, mfgVertexBuffer ** vb, mfmU64 size, const void * data, mfgEnum usage)
{
	return rd->createVertexBuffer(rd, vb, size, data, usage);
}

void mfgDestroyVertexBuffer(void * vb)
{
	((mfgRenderDeviceObject*)vb)->renderDevice->destroyVertexBuffer(vb);
}

mfgError mfgMapVertexBuffer(mfgRenderDevice * rd, mfgVertexBuffer * vb, void ** memory)
{
	return rd->mapVertexBuffer(rd, vb, memory);
}

mfgError mfgUnmapVertexBuffer(mfgRenderDevice * rd, mfgVertexBuffer * vb)
{
	return rd->unmapVertexBuffer(rd, vb);
}

mfgError mfgCreateVertexLayout(mfgRenderDevice * rd, mfgVertexLayout ** vl, mfmU64 elementCount, const mfgVertexElement * elements, mfgVertexShader * vs)
{
	return rd->createVertexLayout(rd, vl, elementCount, elements, vs);
}

void mfgDestroyVertexLayout(void * vl)
{
	((mfgRenderDeviceObject*)vl)->renderDevice->destroyVertexLayout(vl);
}

mfgError mfgCreateVertexArray(mfgRenderDevice * rd, mfgVertexArray ** va, mfmU64 vbCount, mfgVertexBuffer ** vbs, mfgVertexLayout * vl)
{
	return rd->createVertexArray(rd, va, vbCount, vbs, vl);
}

void mfgDestroyVertexArray(void * va)
{
	((mfgRenderDeviceObject*)va)->renderDevice->destroyVertexArray(va);
}

mfgError mfgSetVertexArray(mfgRenderDevice * rd, mfgVertexArray * va)
{
	return rd->setVertexArray(rd, va);
}

mfgError mfgCreateIndexBuffer(mfgRenderDevice * rd, mfgIndexBuffer ** ib, mfmU64 size, const void * data, mfgEnum format, mfgEnum usage)
{
	return rd->createIndexBuffer(rd, ib, size, data, format, usage);
}

void mfgDestroyIndexBuffer(void * ib)
{
	((mfgRenderDeviceObject*)ib)->renderDevice->destroyIndexBuffer(ib);
}

mfgError mfgMapIndexBuffer(mfgRenderDevice * rd, mfgIndexBuffer * ib, void ** memory)
{
	return rd->mapIndexBuffer(rd, ib, memory);
}

mfgError mfgUnmapIndexBuffer(mfgRenderDevice * rd, mfgIndexBuffer * ib)
{
	return rd->unmapIndexBuffer(rd, ib);
}

mfgError mfgSetIndexBuffer(mfgRenderDevice * rd, mfgIndexBuffer * ib)
{
	return rd->setIndexBuffer(rd, ib);
}

mfgError mfgCreateTexture1D(mfgRenderDevice * rd, mfgTexture1D ** tex, mfmU64 width, mfgEnum format, const void * data, mfgEnum usage)
{
	return rd->createTexture1D(rd, tex, width, format, data, usage);
}

void mfgDestroyTexture1D(void * tex)
{
	((mfgRenderDeviceObject*)tex)->renderDevice->destroyTexture1D(tex);
}

mfgError mfgUpdateTexture1D(mfgRenderDevice * rd, mfgTexture1D * tex, mfmU64 dstX, mfmU64 width, const void * data)
{
	return rd->updateTexture1D(rd, tex, dstX, width, data);
}

mfgError mfgGenerateTexture1DMipmaps(mfgRenderDevice * rd, mfgTexture1D * tex)
{
	return rd->generateTexture1DMipmaps(rd, tex);
}

mfgError mfgCreateTexture2D(mfgRenderDevice * rd, mfgTexture2D ** tex, mfmU64 width, mfmU64 height, mfgEnum format, const void * data, mfgEnum usage)
{
	return rd->createTexture2D(rd, tex, width, height, format, data, usage);
}

void mfgDestroyTexture2D(void * tex)
{
	((mfgRenderDeviceObject*)tex)->renderDevice->destroyTexture2D(tex);
}

mfgError mfgUpdateTexture2D(mfgRenderDevice * rd, mfgTexture2D * tex, mfmU64 dstX, mfmU64 dstY, mfmU64 width, mfmU64 height, const void * data)
{
	return rd->updateTexture2D(rd, tex, dstX, dstY, width, height, data);
}

mfgError mfgGenerateTexture2DMipmaps(mfgRenderDevice * rd, mfgTexture2D * tex)
{
	return rd->generateTexture2DMipmaps(rd, tex);
}

mfgError mfgCreateTexture3D(mfgRenderDevice * rd, mfgTexture3D ** tex, mfmU64 width, mfmU64 height, mfmU64 depth, mfgEnum format, const void * data, mfgEnum usage)
{
	return rd->createTexture3D(rd, tex, width, height, depth, format, data, usage);
}

void mfgDestroyTexture3D(void * tex)
{
	((mfgRenderDeviceObject*)tex)->renderDevice->destroyTexture3D(tex);
}

mfgError mfgUpdateTexture3D(mfgRenderDevice * rd, mfgTexture3D * tex, mfmU64 dstX, mfmU64 dstY, mfmU64 dstZ, mfmU64 width, mfmU64 height, mfmU64 depth, const void * data)
{
	return rd->updateTexture3D(rd, tex, dstX, dstY, dstZ, width, height, depth, data);
}

mfgError mfgGenerateTexture3DMipmaps(mfgRenderDevice * rd, mfgTexture3D * tex)
{
	return rd->generateTexture3DMipmaps(rd, tex);
}

mfgError mfgCreateSampler(mfgRenderDevice * rd, mfgSampler ** sampler, const mfgSamplerDesc * desc)
{
	return rd->createSampler(rd, sampler, desc);
}

void mfgDestroySampler(void * sampler)
{
	((mfgRenderDeviceObject*)sampler)->renderDevice->destroySampler(sampler);
}

mfgError mfgCreateRasterState(mfgRenderDevice * rd, mfgRasterState ** state, const mfgRasterStateDesc * desc)
{
	return rd->createRasterState(rd, state, desc);
}

void mfgDestroyRasterState(void * state)
{
	((mfgRenderDeviceObject*)state)->renderDevice->destroyRasterState(state);
}

mfgError mfgSetRasterState(mfgRenderDevice * rd, mfgRasterState * state)
{
	return rd->setRasterState(rd, state);
}

mfgError mfgCreateDepthStencilState(mfgRenderDevice * rd, mfgDepthStencilState ** state, const mfgDepthStencilStateDesc * desc)
{
	return rd->createDepthStencilState(rd, state, desc);
}

void mfgDestroyDepthStencilState(void * state)
{
	((mfgRenderDeviceObject*)state)->renderDevice->destroyDepthStencilState(state);
}

mfgError mfgSetDepthStencilState(mfgRenderDevice * rd, mfgDepthStencilState * state)
{
	return rd->setDepthStencilState(rd, state);
}

mfgError mfgCreateBlendState(mfgRenderDevice * rd, mfgBlendState ** state, const mfgBlendStateDesc * desc)
{
	return rd->createBlendState(rd, state, desc);
}

void mfgDestroyBlendState(void * state)
{
	((mfgRenderDeviceObject*)state)->renderDevice->destroyBlendState(state);
}

mfgError mfgSetBlendState(mfgRenderDevice * rd, mfgBlendState * state)
{
	return rd->setBlendState(rd, state);
}

mfgError mfgCreateRenderTexture(mfgRenderDevice * rd, mfgRenderTexture ** tex, mfmU64 width, mfmU64 height, mfgEnum format)
{
	return rd->createRenderTexture(rd, tex, width, height, format);
}

void mfgDestroyRenderTexture(void * tex)
{
	((mfgRenderDeviceObject*)tex)->renderDevice->destroyRenderTexture(tex);
}

mfgError mfgCreateDepthStencilTexture(mfgRenderDevice * rd, mfgDepthStencilTexture ** tex, mfmU64 width, mfmU64 height, mfgEnum format)
{
	return rd->createDepthStencilTexture(rd, tex, width, height, format);
}

void mfgDestroyDepthStencilTexture(void * tex)
{
	((mfgRenderDeviceObject*)tex)->renderDevice->destroyDepthStencilTexture(tex);
}

mfgError mfgCreateFramebuffer(mfgRenderDevice * rd, mfgFramebuffer ** fb, mfmU64 textureCount, mfgRenderTexture ** textures, mfgDepthStencilTexture * depthStencilTexture)
{
	return rd->createFramebuffer(rd, fb, textureCount, textures, depthStencilTexture);
}

void mfgDestroyFramebuffer(void * fb)
{
	((mfgRenderDeviceObject*)fb)->renderDevice->destroyFramebuffer(fb);
}

mfgError mfgSetFramebuffer(mfgRenderDevice * rd, mfgFramebuffer * fb)
{
	return rd->setFramebuffer(rd, fb);
}

mfgError mfgClearColor(mfgRenderDevice * rd, mfmF32 r, mfmF32 g, mfmF32 b, mfmF32 a)
{
	return rd->clearColor(rd, r, g, b, a);
}

mfgError mfgClearDepth(mfgRenderDevice * rd, mfmF32 depth)
{
	return rd->clearDepth(rd, depth);
}

mfgError mfgClearStencil(mfgRenderDevice * rd, mfmI32 stencil)
{
	return rd->clearStencil(rd, stencil);
}

mfgError mfgDrawTriangles(mfgRenderDevice * rd, mfmU64 offset, mfmU64 count)
{
	return rd->drawTriangles(rd, offset, count);
}

mfgError mfgDrawTrianglesIndexed(mfgRenderDevice * rd, mfmU64 offset, mfmU64 count)
{
	return rd->drawTrianglesIndexed(rd, offset, count);
}

mfgError mfgSwapBuffers(mfgRenderDevice * rd)
{
	return rd->swapBuffers(rd);
}

mfgError mfgGetPropertyI(mfgRenderDevice * rd, mfgEnum id, mfmI32 * value)
{
	return rd->getPropertyI(rd, id, value);
}

mfgError mfgGetPropertyF(mfgRenderDevice * rd, mfgEnum id, mfmF32 * value)
{
	return rd->getPropertyF(rd, id, value);
}

mfmBool mfgGetErrorString(mfgRenderDevice * rd, mfsUTF8CodeUnit * str, mfmU64 maxSize)
{
	return rd->getErrorString(rd, str, maxSize);
}
