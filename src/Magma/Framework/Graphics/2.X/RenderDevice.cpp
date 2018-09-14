#include "RenderDevice.hpp"
#include "Config.h"

#include "../Exception.hpp"
#include "../../String/StringStream.hpp"
#include "../../ErrorString.h"

#ifdef MAGMA_FRAMEWORK_DEBUG
#define CHECK_ERROR(rd, err) do { if (err != MF_ERROR_OKAY) { mfsStringStream ss;\
	mfsUTF8CodeUnit msg[256];\
	mfError e = mfsCreateLocalStringStream(&ss, (mfmU8*)msg, sizeof(msg));\
	if (e != MF_ERROR_OKAY)\
		throw Magma::Framework::Graphics::RenderDeviceError(mfErrorToString(err));\
	mfsUTF8CodeUnit internalErrorMsg[256] = { '\0' };\
	mfgV2XGetErrorString(rd, internalErrorMsg, sizeof(internalErrorMsg));\
	e = mfsPrintFormat((mfsStream*)&ss, u8"Error on function " __FUNCTION__ ": %s\n%s\n", mfErrorToString(err), internalErrorMsg);\
	if (e != MF_ERROR_OKAY)\
		throw Magma::Framework::Graphics::RenderDeviceError(mfErrorToString(err));\
	mfsDestroyLocalStringStream(&ss);\
	throw Magma::Framework::Graphics::RenderDeviceError(msg);\
 } } while(false)
#else
#define CHECK_ERROR(err) do {} while(false)
#endif

void * Magma::Framework::Graphics::V2X::HVertexShader::GetBindingPoint(const mfsUTF8CodeUnit * name)
{
	auto vs = (mfgV2XVertexShader*)&this->Get();
	mfgV2XBindingPoint* bp = NULL;
	mfError err = mfgV2XGetVertexShaderBindingPoint(vs->renderDevice, &bp, vs, name);
	if (err != MF_ERROR_OKAY)
		throw RenderDeviceError(mfErrorToString(err));
	return bp;
}

void * Magma::Framework::Graphics::V2X::HPixelShader::GetBindingPoint(const mfsUTF8CodeUnit * name)
{
	auto ps = (mfgV2XPixelShader*)&this->Get();
	mfgV2XBindingPoint* bp = NULL;
	mfError err = mfgV2XGetPixelShaderBindingPoint(ps->renderDevice, &bp, ps, name);
	if (err != MF_ERROR_OKAY)
		throw RenderDeviceError(mfErrorToString(err));
	return bp;
}

void * Magma::Framework::Graphics::V2X::HConstantBuffer::Map()
{
	auto cb = (mfgV2XConstantBuffer*)&this->Get();
	void* mem;
	mfError err = mfgV2XMapConstantBuffer(cb->renderDevice, cb, &mem);
	CHECK_ERROR(cb->renderDevice, err);
	return mem;
}

void Magma::Framework::Graphics::V2X::HConstantBuffer::Unmap()
{
	auto cb = (mfgV2XConstantBuffer*)&this->Get();
	mfError err = mfgV2XUnmapConstantBuffer(cb->renderDevice, cb);
	CHECK_ERROR(cb->renderDevice, err);
}

void Magma::Framework::Graphics::V2X::HTexture1D::Update(mfmU32 dstX, mfmU32 width, const void * data)
{
	auto tex = (mfgV2XTexture1D*)&this->Get();
	mfError err = mfgV2XUpdateTexture1D(tex->renderDevice, tex, dstX, width, data);
	CHECK_ERROR(tex->renderDevice, err);
}

void Magma::Framework::Graphics::V2X::HTexture1D::GenerateMipmaps()
{
	auto tex = (mfgV2XTexture1D*)&this->Get();
	mfError err = mfgV2XGenerateTexture1DMipmaps(tex->renderDevice, tex);
	CHECK_ERROR(tex->renderDevice, err);
}

void Magma::Framework::Graphics::V2X::HTexture2D::Update(mfmU32 dstX, mfmU32 dstY, mfmU32 width, mfmU32 height, const void * data)
{
	auto tex = (mfgV2XTexture2D*)&this->Get();
	mfError err = mfgV2XUpdateTexture2D(tex->renderDevice, tex, dstX, dstY, width, height, data);
	CHECK_ERROR(tex->renderDevice, err);
}

void Magma::Framework::Graphics::V2X::HTexture2D::GenerateMipmaps()
{
	auto tex = (mfgV2XTexture2D*)&this->Get();
	mfError err = mfgV2XGenerateTexture2DMipmaps(tex->renderDevice, tex);
	CHECK_ERROR(tex->renderDevice, err);
}

void Magma::Framework::Graphics::V2X::HTexture3D::Update(mfmU32 dstX, mfmU32 dstY, mfmU32 dstZ, mfmU32 width, mfmU32 height, mfmU32 depth, const void * data)
{
	auto tex = (mfgV2XTexture3D*)&this->Get();
	mfError err = mfgV2XUpdateTexture3D(tex->renderDevice, tex, dstX, dstY, dstZ, width, height, depth, data);
	CHECK_ERROR(tex->renderDevice, err);
}

void Magma::Framework::Graphics::V2X::HTexture3D::GenerateMipmaps()
{
	auto tex = (mfgV2XTexture3D*)&this->Get();
	mfError err = mfgV2XGenerateTexture3DMipmaps(tex->renderDevice, tex);
	CHECK_ERROR(tex->renderDevice, err);
}

void * Magma::Framework::Graphics::V2X::HVertexBuffer::Map()
{
	auto vb = (mfgV2XVertexBuffer*)&this->Get();
	void* mem;
	mfError err = mfgV2XMapVertexBuffer(vb->renderDevice, vb, &mem);
	CHECK_ERROR(vb->renderDevice, err);
	return mem;
}

void Magma::Framework::Graphics::V2X::HVertexBuffer::Unmap()
{
	auto vb = (mfgV2XVertexBuffer*)&this->Get();
	mfError err = mfgV2XUnmapVertexBuffer(vb->renderDevice, vb);
	CHECK_ERROR(vb->renderDevice, err);
}

void * Magma::Framework::Graphics::V2X::HIndexBuffer::Map()
{
	auto ib = (mfgV2XIndexBuffer*)&this->Get();
	void* mem;
	mfError err = mfgV2XMapIndexBuffer(ib->renderDevice, ib, &mem);
	CHECK_ERROR(ib->renderDevice, err);
	return mem;
}

void Magma::Framework::Graphics::V2X::HIndexBuffer::Unmap()
{
	auto ib = (mfgV2XIndexBuffer*)&this->Get();
	mfError err = mfgV2XUnmapIndexBuffer(ib->renderDevice, ib);
	CHECK_ERROR(ib->renderDevice, err);
}

Magma::Framework::Graphics::V2X::HVertexShader Magma::Framework::Graphics::V2X::HRenderDevice::CreateVertexShader(const mfmU8 * bytecode, mfmU64 bytecodeSize, HMetaData metaData)
{
	auto rd = (mfgV2XRenderDevice*)&this->Get();
	mfgV2XVertexShader* vs = NULL;
	mfError err = mfgV2XCreateVertexShader(rd, &vs, bytecode, bytecodeSize, (mfgMetaData*)&metaData.Get());
	CHECK_ERROR(rd, err);
	return vs;
}

Magma::Framework::Graphics::V2X::HPixelShader Magma::Framework::Graphics::V2X::HRenderDevice::CreatePixelShader(const mfmU8 * bytecode, mfmU64 bytecodeSize, HMetaData metaData)
{
	auto rd = (mfgV2XRenderDevice*)&this->Get();
	mfgV2XVertexShader* ps = NULL;
	mfError err = mfgV2XCreatePixelShader(rd, &ps, bytecode, bytecodeSize, (mfgMetaData*)&metaData.Get());
	CHECK_ERROR(rd, err);
	return ps;
}

Magma::Framework::Graphics::V2X::HPipeline Magma::Framework::Graphics::V2X::HRenderDevice::CreatePipeline(HVertexShader vs, HPixelShader ps)
{
	auto rd = (mfgV2XRenderDevice*)&this->Get();
	mfgV2XPipeline* pp = NULL;
	mfError err = mfgV2XCreatePipeline(rd, &pp, (mfgV2XVertexShader*)&vs.Get(), (mfgV2XPixelShader*)&ps.Get());
	CHECK_ERROR(rd, err);
	return pp;
}

void Magma::Framework::Graphics::V2X::HRenderDevice::SetPipeline(HPipeline pipeline)
{
	auto rd = (mfgV2XRenderDevice*)&this->Get();
	mfError err = mfgV2XSetPipeline(rd, (mfgV2XPipeline*)pipeline.GetNoChecks());
	CHECK_ERROR(rd, err);
}

void Magma::Framework::Graphics::V2X::HRenderDevice::BindConstantBuffer(void * bp, HConstantBuffer cb)
{
	auto rd = (mfgV2XRenderDevice*)&this->Get();
	mfError err = mfgV2XBindConstantBuffer(rd, (mfgV2XBindingPoint*)bp, (mfgV2XConstantBuffer*)&cb.Get());
	CHECK_ERROR(rd, err);
}

void Magma::Framework::Graphics::V2X::HRenderDevice::BindConstantBufferRange(void * bp, HConstantBuffer cb, mfmU64 offset, mfmU64 size)
{
	auto rd = (mfgV2XRenderDevice*)&this->Get();
	mfError err = mfgV2XBindConstantBufferRange(rd, (mfgV2XBindingPoint*)bp, (mfgV2XConstantBuffer*)&cb.Get(), offset, size);
	CHECK_ERROR(rd, err);
}

void Magma::Framework::Graphics::V2X::HRenderDevice::BindTexture1D(void * bp, HTexture1D tex)
{
	auto rd = (mfgV2XRenderDevice*)&this->Get();
	mfError err = mfgV2XBindTexture1D(rd, (mfgV2XBindingPoint*)bp, (mfgV2XTexture1D*)&tex.Get());
	CHECK_ERROR(rd, err);
}

void Magma::Framework::Graphics::V2X::HRenderDevice::BindTexture2D(void * bp, HTexture2D tex)
{
	auto rd = (mfgV2XRenderDevice*)&this->Get();
	mfError err = mfgV2XBindTexture2D(rd, (mfgV2XBindingPoint*)bp, (mfgV2XTexture2D*)tex.GetNoChecks());
	CHECK_ERROR(rd, err);
}

void Magma::Framework::Graphics::V2X::HRenderDevice::BindTexture3D(void * bp, HTexture3D tex)
{
	auto rd = (mfgV2XRenderDevice*)&this->Get();
	mfError err = mfgV2XBindTexture3D(rd, (mfgV2XBindingPoint*)bp, (mfgV2XTexture3D*)tex.GetNoChecks());
	CHECK_ERROR(rd, err);
}

void Magma::Framework::Graphics::V2X::HRenderDevice::BindRenderTexture(void * bp, HRenderTexture tex)
{
	auto rd = (mfgV2XRenderDevice*)&this->Get();
	mfError err = mfgV2XBindRenderTexture(rd, (mfgV2XBindingPoint*)bp, (mfgV2XRenderTexture*)tex.GetNoChecks());
	CHECK_ERROR(rd, err);
}

void Magma::Framework::Graphics::V2X::HRenderDevice::BindSampler(void * bp, HSampler sampler)
{
	auto rd = (mfgV2XRenderDevice*)&this->Get();
	mfError err = mfgV2XBindSampler(rd, (mfgV2XBindingPoint*)bp, (mfgV2XSampler*)sampler.GetNoChecks());
	CHECK_ERROR(rd, err);
}

Magma::Framework::Graphics::V2X::HConstantBuffer Magma::Framework::Graphics::V2X::HRenderDevice::CreateConstantBuffer(mfmU64 size, const void * data, Usage usage)
{
	auto rd = (mfgV2XRenderDevice*)&this->Get();
	mfgV2XConstantBuffer* cb = NULL;
	mfError err = mfgV2XCreateConstantBuffer(rd, &cb, size, data, static_cast<mfgEnum>(usage));
	CHECK_ERROR(rd, err);
	return cb;
}

Magma::Framework::Graphics::V2X::HVertexBuffer Magma::Framework::Graphics::V2X::HRenderDevice::CreateVertexBuffer(mfmU64 size, const void * data, Usage usage)
{
	auto rd = (mfgV2XRenderDevice*)&this->Get();
	mfgV2XVertexBuffer* vb = NULL;
	mfError err = mfgV2XCreateVertexBuffer(rd, &vb, size, data, static_cast<mfgEnum>(usage));
	CHECK_ERROR(rd, err);
	return vb;
}

Magma::Framework::Graphics::V2X::HVertexLayout Magma::Framework::Graphics::V2X::HRenderDevice::CreateVertexLayout(mfmU64 elementCount, const VertexElement * elements, HVertexShader vs)
{
	auto rd = (mfgV2XRenderDevice*)&this->Get();

	auto c_elements = (mfgV2XVertexElement*)alloca(sizeof(mfgV2XVertexElement) * (size_t)elementCount);
	for (mfmU64 i = 0; i < elementCount; ++i)
	{
		c_elements[i].bufferIndex = elements[i].bufferIndex;
		memcpy(c_elements[i].name, elements[i].name, sizeof(c_elements[i].name));
		c_elements[i].offset = elements[i].offset;
		c_elements[i].stride = elements[i].stride;
		c_elements[i].size = elements[i].size;
		c_elements[i].type = static_cast<mfgEnum>(elements[i].type);
	}

	mfgV2XVertexLayout* vl = NULL;
	mfError err = mfgV2XCreateVertexLayout(rd, &vl, elementCount, c_elements, (mfgV2XVertexShader*)&vs.Get());
	CHECK_ERROR(rd, err);
	return vl;
}

Magma::Framework::Graphics::V2X::HVertexArray Magma::Framework::Graphics::V2X::HRenderDevice::CreateVertexArray(mfmU64 bufferCount, HVertexBuffer * buffers, HVertexLayout layout)
{
	auto rd = (mfgV2XRenderDevice*)&this->Get();

	auto c_buffers = (mfgV2XVertexBuffer**)alloca(sizeof(mfgV2XVertexBuffer*) * (size_t)bufferCount);
	for (mfmU64 i = 0; i < bufferCount; ++i)
		c_buffers[i] = (mfgV2XVertexBuffer*)&buffers[i].Get();

	mfgV2XVertexArray* va = NULL;
	mfError err = mfgV2XCreateVertexArray(rd, &va, bufferCount, c_buffers, (mfgV2XVertexLayout*)&layout.Get());
	CHECK_ERROR(rd, err);
	return va;
}

void Magma::Framework::Graphics::V2X::HRenderDevice::SetVertexArray(HVertexArray va)
{
	auto rd = (mfgV2XRenderDevice*)&this->Get();
	mfError err = mfgV2XSetVertexArray(rd, (mfgV2XVertexArray*)va.GetNoChecks());
	CHECK_ERROR(rd, err);
}

Magma::Framework::Graphics::V2X::HIndexBuffer Magma::Framework::Graphics::V2X::HRenderDevice::CreateIndexBuffer(mfmU64 size, const void * data, Type type, Usage usage)
{
	auto rd = (mfgV2XRenderDevice*)&this->Get();
	mfgV2XIndexBuffer* ib = NULL;
	mfError err = mfgV2XCreateIndexBuffer(rd, &ib, size, data, static_cast<mfgEnum>(type), static_cast<mfgEnum>(usage));
	CHECK_ERROR(rd, err);
	return ib;
}

void Magma::Framework::Graphics::V2X::HRenderDevice::SetIndexBuffer(HIndexBuffer ib)
{
	auto rd = (mfgV2XRenderDevice*)&this->Get();
	mfError err = mfgV2XSetIndexBuffer(rd, (mfgV2XIndexBuffer*)ib.GetNoChecks());
	CHECK_ERROR(rd, err);
}

Magma::Framework::Graphics::V2X::HTexture1D Magma::Framework::Graphics::V2X::HRenderDevice::CreateTexture1D(mfmU64 width, Format format, const void * data, Usage usage)
{
	auto rd = (mfgV2XRenderDevice*)&this->Get();
	mfgV2XTexture1D* tex = NULL;
	mfError err = mfgV2XCreateTexture1D(rd, &tex, width, static_cast<mfgEnum>(format), data, static_cast<mfgEnum>(usage));
	CHECK_ERROR(rd, err);
	return tex;
}

Magma::Framework::Graphics::V2X::HTexture2D Magma::Framework::Graphics::V2X::HRenderDevice::CreateTexture2D(mfmU64 width, mfmU64 height, Format format, const void * data, Usage usage)
{
	auto rd = (mfgV2XRenderDevice*)&this->Get();
	mfgV2XTexture2D* tex = NULL;
	mfError err = mfgV2XCreateTexture2D(rd, &tex, width, height, static_cast<mfgEnum>(format), data, static_cast<mfgEnum>(usage));
	CHECK_ERROR(rd, err);
	return tex;
}

Magma::Framework::Graphics::V2X::HTexture3D Magma::Framework::Graphics::V2X::HRenderDevice::CreateTexture3D(mfmU64 width, mfmU64 height, mfmU64 depth, Format format, const void * data, Usage usage)
{
	auto rd = (mfgV2XRenderDevice*)&this->Get();
	mfgV2XTexture3D* tex = NULL;
	mfError err = mfgV2XCreateTexture3D(rd, &tex, width, height, depth, static_cast<mfgEnum>(format), data, static_cast<mfgEnum>(usage));
	CHECK_ERROR(rd, err);
	return tex;
}

Magma::Framework::Graphics::V2X::HSampler Magma::Framework::Graphics::V2X::HRenderDevice::CreateSampler(const SamplerDesc & desc)
{
	mfgV2XSamplerDesc c_desc;

	c_desc.addressU = static_cast<mfgEnum>(desc.addressU);
	c_desc.addressV = static_cast<mfgEnum>(desc.addressV);
	c_desc.addressW = static_cast<mfgEnum>(desc.addressW);
	c_desc.magFilter = static_cast<mfgEnum>(desc.magFilter);
	c_desc.minFilter = static_cast<mfgEnum>(desc.minFilter);
	c_desc.mipmapFilter = static_cast<mfgEnum>(desc.mipmapFilter);
	c_desc.maxAnisotropy = desc.maxAnisotropy;
	c_desc.borderColor[0] = desc.borderColor[0];
	c_desc.borderColor[1] = desc.borderColor[1];
	c_desc.borderColor[2] = desc.borderColor[2];
	c_desc.borderColor[3] = desc.borderColor[3];

	auto rd = (mfgV2XRenderDevice*)&this->Get();
	mfgV2XSampler* sampler = NULL;
	mfError err = mfgV2XCreateSampler(rd, &sampler, &c_desc);
	CHECK_ERROR(rd, err);
	return sampler;
}

Magma::Framework::Graphics::V2X::HRasterState Magma::Framework::Graphics::V2X::HRenderDevice::CreateRasterState(const RasterStateDesc & desc)
{
	mfgV2XRasterStateDesc c_desc;

	c_desc.cullEnabled = (desc.cullEnabled ? MFM_TRUE : MFM_FALSE);
	c_desc.cullFace = static_cast<mfgEnum>(desc.cullFace);
	c_desc.frontFace = static_cast<mfgEnum>(desc.frontFace);
	c_desc.rasterMode = static_cast<mfgEnum>(desc.rasterMode);

	auto rd = (mfgV2XRenderDevice*)&this->Get();
	mfgV2XRasterState* state = NULL;
	mfError err = mfgV2XCreateRasterState(rd, &state, &c_desc);
	CHECK_ERROR(rd, err);
	return state;
}

Magma::Framework::Graphics::V2X::HDepthStencilState Magma::Framework::Graphics::V2X::HRenderDevice::CreateDepthStencilState(const DepthStencilStateDesc & desc)
{
	mfgV2XDepthStencilStateDesc c_desc;

	c_desc.depthEnabled = (desc.depthEnabled ? MFM_TRUE : MFM_FALSE);
	c_desc.depthWriteEnabled = (desc.depthWriteEnabled ? MFM_TRUE : MFM_FALSE);
	c_desc.depthCompare = static_cast<mfgEnum>(desc.depthCompare);
	c_desc.depthNear = desc.depthNear;
	c_desc.depthFar = desc.depthFar;

	c_desc.stencilEnabled = (desc.stencilEnabled ? MFM_TRUE : MFM_FALSE);
	c_desc.stencilReadMask = desc.stencilReadMask;
	c_desc.stencilWriteMask = desc.stencilWriteMask;
	c_desc.stencilRef = desc.stencilRef;

	c_desc.backFaceStencilCompare = static_cast<mfgEnum>(desc.backFaceStencilCompare);
	c_desc.backFaceDepthFail = static_cast<mfgEnum>(desc.backFaceDepthFail);
	c_desc.backFaceStencilPass = static_cast<mfgEnum>(desc.backFaceStencilPass);
	c_desc.backFaceStencilFail = static_cast<mfgEnum>(desc.backFaceStencilFail);

	c_desc.frontFaceStencilCompare = static_cast<mfgEnum>(desc.frontFaceStencilCompare);
	c_desc.frontFaceDepthFail = static_cast<mfgEnum>(desc.frontFaceDepthFail);
	c_desc.frontFaceStencilPass = static_cast<mfgEnum>(desc.frontFaceStencilPass);
	c_desc.frontFaceStencilFail = static_cast<mfgEnum>(desc.frontFaceStencilFail);

	auto rd = (mfgV2XRenderDevice*)&this->Get();
	mfgV2XDepthStencilState* state = NULL;
	mfError err = mfgV2XCreateDepthStencilState(rd, &state, &c_desc);
	CHECK_ERROR(rd, err);
	return state;
}

Magma::Framework::Graphics::V2X::HBlendState Magma::Framework::Graphics::V2X::HRenderDevice::CreateBlendState(const BlendStateDesc & desc)
{
	mfgV2XBlendStateDesc c_desc;

	c_desc.blendEnabled = (desc.blendEnabled ? MFM_TRUE : MFM_FALSE);
	c_desc.blendOperation = static_cast<mfgEnum>(desc.blendOperation);
	c_desc.sourceFactor = static_cast<mfgEnum>(desc.sourceFactor);
	c_desc.destinationFactor = static_cast<mfgEnum>(desc.destinationFactor);
	c_desc.blendAlphaOperation = static_cast<mfgEnum>(desc.blendAlphaOperation);
	c_desc.sourceAlphaFactor = static_cast<mfgEnum>(desc.sourceAlphaFactor);
	c_desc.destinationAlphaFactor = static_cast<mfgEnum>(desc.destinationAlphaFactor);

	auto rd = (mfgV2XRenderDevice*)&this->Get();
	mfgV2XBlendState* state = NULL;
	mfError err = mfgV2XCreateBlendState(rd, &state, &c_desc);
	CHECK_ERROR(rd, err);
	return state;
}

void Magma::Framework::Graphics::V2X::HRenderDevice::SetRasterState(HRasterState state)
{
	auto rd = (mfgV2XRenderDevice*)&this->Get();
	mfError err = mfgV2XSetRasterState(rd, (mfgV2XRasterState*)&state.Get());
	CHECK_ERROR(rd, err);
}

void Magma::Framework::Graphics::V2X::HRenderDevice::SetDepthStencilState(HDepthStencilState state)
{
	auto rd = (mfgV2XRenderDevice*)&this->Get();
	mfError err = mfgV2XSetDepthStencilState(rd, (mfgV2XDepthStencilState*)&state.Get());
	CHECK_ERROR(rd, err);
}

void Magma::Framework::Graphics::V2X::HRenderDevice::SetBlendState(HBlendState state)
{
	auto rd = (mfgV2XRenderDevice*)&this->Get();
	mfError err = mfgV2XSetBlendState(rd, (mfgV2XBlendState*)&state.Get());
	CHECK_ERROR(rd, err);
}

Magma::Framework::Graphics::V2X::HRenderTexture Magma::Framework::Graphics::V2X::HRenderDevice::CreateRenderTexture(mfmU64 width, mfmU64 height, Format format)
{
	auto rd = (mfgV2XRenderDevice*)&this->Get();
	mfgV2XRenderTexture* tex = NULL;
	mfError err = mfgV2XCreateRenderTexture(rd, &tex, width, height, static_cast<mfgEnum>(format));
	CHECK_ERROR(rd, err);
	return tex;
}

Magma::Framework::Graphics::V2X::HDepthStencilTexture Magma::Framework::Graphics::V2X::HRenderDevice::CreateDepthStencilTexture(mfmU64 width, mfmU64 height, Format format)
{
	auto rd = (mfgV2XRenderDevice*)&this->Get();
	mfgV2XDepthStencilTexture* tex = NULL;
	mfError err = mfgV2XCreateDepthStencilTexture(rd, &tex, width, height, static_cast<mfgEnum>(format));
	CHECK_ERROR(rd, err);
	return tex;
}

Magma::Framework::Graphics::V2X::HFramebuffer Magma::Framework::Graphics::V2X::HRenderDevice::CreateFramebuffer(mfmU64 textureCount, HRenderTexture * textures, HDepthStencilTexture depthStencilTexture)
{
	auto rd = (mfgV2XRenderDevice*)&this->Get();

	auto c_textures = (mfgV2XRenderTexture**)alloca(sizeof(mfgV2XRenderTexture*) * (size_t)textureCount);
	for (mfmU64 i = 0; i < textureCount; ++i)
		c_textures[i] = (mfgV2XRenderTexture*)&textures[i].Get();

	mfgV2XFramebuffer* fb = NULL;
	mfError err = mfgV2XCreateFramebuffer(rd, &fb, textureCount, c_textures, (mfgV2XDepthStencilTexture*)&depthStencilTexture.Get());
	CHECK_ERROR(rd, err);
	return fb;
}

void Magma::Framework::Graphics::V2X::HRenderDevice::SetFramebuffer(HFramebuffer framebuffer)
{
	auto rd = (mfgV2XRenderDevice*)&this->Get();
	mfError err = mfgV2XSetFramebuffer(rd, (mfgV2XFramebuffer*)&framebuffer.Get());
	CHECK_ERROR(rd, err);
}

void Magma::Framework::Graphics::V2X::HRenderDevice::ClearColor(mfmF32 r, mfmF32 g, mfmF32 b, mfmF32 a)
{
	auto rd = (mfgV2XRenderDevice*)&this->Get();
	mfError err = mfgV2XClearColor(rd, r, g, b, a);
	CHECK_ERROR(rd, err);
}

void Magma::Framework::Graphics::V2X::HRenderDevice::ClearDepth(mfmF32 depth)
{
	auto rd = (mfgV2XRenderDevice*)&this->Get();
	mfError err = mfgV2XClearDepth(rd, depth);
	CHECK_ERROR(rd, err);
}

void Magma::Framework::Graphics::V2X::HRenderDevice::ClearStencil(mfmI32 stencil)
{
	auto rd = (mfgV2XRenderDevice*)&this->Get();
	mfError err = mfgV2XClearStencil(rd, stencil);
	CHECK_ERROR(rd, err);
}

void Magma::Framework::Graphics::V2X::HRenderDevice::DrawTriangles(mfmU64 offset, mfmU64 count)
{
	auto rd = (mfgV2XRenderDevice*)&this->Get();
	mfError err = mfgV2XDrawTriangles(rd, offset, count);
	CHECK_ERROR(rd, err);
}

void Magma::Framework::Graphics::V2X::HRenderDevice::DrawTrianglesIndexed(mfmU64 offset, mfmU64 count)
{
	auto rd = (mfgV2XRenderDevice*)&this->Get();
	mfError err = mfgV2XDrawTrianglesIndexed(rd, offset, count);
	CHECK_ERROR(rd, err);
}

void Magma::Framework::Graphics::V2X::HRenderDevice::SwapBuffers()
{
	auto rd = (mfgV2XRenderDevice*)&this->Get();
	mfError err = mfgV2XSwapBuffers(rd);
	CHECK_ERROR(rd, err);
}

Magma::Framework::Graphics::V2X::HRenderDevice Magma::Framework::Graphics::V2X::CreateRenderDevice(const mfsUTF8CodeUnit * type, Input::HWindow window, const RenderDeviceDesc * desc, Memory::HAllocator allocator)
{
	if (desc == NULL)
		throw RenderDeviceError("Failed to create render device:\nThe render device description pointer is NULL");

	if (type == NULL)
	{
#if defined(MAGMA_FRAMEWORK_USE_DIRECTX)
		type = u8"d3d11";
#elif defined(MAGMA_FRAMEWORK_USE_OPENGL)
		type = u8"ogl4";
#endif
	}

	mfgV2XRenderDeviceDesc cDesc;
	mfgV2XDefaultRenderDeviceDesc(&cDesc);
	cDesc.vsyncEnabled = (desc->vsyncEnabled ? MFM_TRUE : MFM_FALSE);

	mfgV2XRenderDevice* rd = NULL;
	mfError err = mfgV2XCreateRenderDevice(type, &rd, reinterpret_cast<mfiWindow*>(&window.Get()), &cDesc, allocator.GetNoChecks());
	if (err != MF_ERROR_OKAY)
		throw RenderDeviceError(mfErrorToString(err));
	return rd;
}
