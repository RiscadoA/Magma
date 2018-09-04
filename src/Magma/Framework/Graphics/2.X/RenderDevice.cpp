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
	mfsDestroyLocalStringStream(&ss);\
\
 } } while(false)
#else
#define CHECK_ERROR(err) do {} while(false)
#endif

void * Magma::Framework::Graphics::V2X::VertexShader::GetBindingPoint(const mfsUTF8CodeUnit * name)
{
	auto vs = (mfgV2XVertexShader*)&this->Get();
	mfgV2XBindingPoint* bp = NULL;
	mfError err = mfgV2XGetVertexShaderBindingPoint(vs->renderDevice, &bp, vs, name);
	if (err != MF_ERROR_OKAY)
		throw RenderDeviceError(mfErrorToString(err));
	return bp;
}

void * Magma::Framework::Graphics::V2X::PixelShader::GetBindingPoint(const mfsUTF8CodeUnit * name)
{
	auto ps = (mfgV2XPixelShader*)&this->Get();
	mfgV2XBindingPoint* bp = NULL;
	mfError err = mfgV2XGetPixelShaderBindingPoint(ps->renderDevice, &bp, ps, name);
	if (err != MF_ERROR_OKAY)
		throw RenderDeviceError(mfErrorToString(err));
	return bp;
}

void * Magma::Framework::Graphics::V2X::ConstantBuffer::Map()
{
	auto cb = (mfgV2XConstantBuffer*)&this->Get();
	void* mem;
	mfError err = mfgV2XMapConstantBuffer(cb->renderDevice, cb, &mem);
	CHECK_ERROR(cb->renderDevice, err);
	return mem;
}

void Magma::Framework::Graphics::V2X::ConstantBuffer::Unmap()
{
	auto cb = (mfgV2XConstantBuffer*)&this->Get();
	mfError err = mfgV2XUnmapConstantBuffer(cb->renderDevice, cb);
	CHECK_ERROR(cb->renderDevice, err);
}

void Magma::Framework::Graphics::V2X::Texture1D::Update(mfmU32 dstX, mfmU32 width, const void * data)
{
	auto tex = (mfgV2XTexture1D*)&this->Get();
	mfError err = mfgV2XUpdateTexture1D(tex->renderDevice, tex, dstX, width, data);
	CHECK_ERROR(tex->renderDevice, err);
}

void Magma::Framework::Graphics::V2X::Texture1D::GenerateMipmaps()
{
	auto tex = (mfgV2XTexture1D*)&this->Get();
	mfError err = mfgV2XGenerateTexture1DMipmaps(tex->renderDevice, tex);
	CHECK_ERROR(tex->renderDevice, err);
}

void Magma::Framework::Graphics::V2X::Texture2D::Update(mfmU32 dstX, mfmU32 dstY, mfmU32 width, mfmU32 height, const void * data)
{
	auto tex = (mfgV2XTexture2D*)&this->Get();
	mfError err = mfgV2XUpdateTexture2D(tex->renderDevice, tex, dstX, dstY, width, height, data);
	CHECK_ERROR(tex->renderDevice, err);
}

void Magma::Framework::Graphics::V2X::Texture2D::GenerateMipmaps()
{
	auto tex = (mfgV2XTexture2D*)&this->Get();
	mfError err = mfgV2XGenerateTexture2DMipmaps(tex->renderDevice, tex);
	CHECK_ERROR(tex->renderDevice, err);
}

void Magma::Framework::Graphics::V2X::Texture3D::Update(mfmU32 dstX, mfmU32 dstY, mfmU32 dstZ, mfmU32 width, mfmU32 height, mfmU32 depth, const void * data)
{
	auto tex = (mfgV2XTexture3D*)&this->Get();
	mfError err = mfgV2XUpdateTexture3D(tex->renderDevice, tex, dstX, dstY, dstZ, width, height, depth, data);
	CHECK_ERROR(tex->renderDevice, err);
}

void Magma::Framework::Graphics::V2X::Texture3D::GenerateMipmaps()
{
	auto tex = (mfgV2XTexture3D*)&this->Get();
	mfError err = mfgV2XGenerateTexture3DMipmaps(tex->renderDevice, tex);
	CHECK_ERROR(tex->renderDevice, err);
}

void * Magma::Framework::Graphics::V2X::VertexBuffer::Map()
{
	auto vb = (mfgV2XVertexBuffer*)&this->Get();
	void* mem;
	mfError err = mfgV2XMapVertexBuffer(vb->renderDevice, vb, &mem);
	CHECK_ERROR(vb->renderDevice, err);
	return mem;
}

void Magma::Framework::Graphics::V2X::VertexBuffer::Unmap()
{
	auto vb = (mfgV2XVertexBuffer*)&this->Get();
	mfError err = mfgV2XUnmapVertexBuffer(vb->renderDevice, vb);
	CHECK_ERROR(vb->renderDevice, err);
}

void * Magma::Framework::Graphics::V2X::IndexBuffer::Map()
{
	auto ib = (mfgV2XIndexBuffer*)&this->Get();
	void* mem;
	mfError err = mfgV2XMapIndexBuffer(ib->renderDevice, ib, &mem);
	CHECK_ERROR(ib->renderDevice, err);
	return mem;
}

void Magma::Framework::Graphics::V2X::IndexBuffer::Unmap()
{
	auto ib = (mfgV2XIndexBuffer*)&this->Get();
	mfError err = mfgV2XUnmapIndexBuffer(ib->renderDevice, ib);
	CHECK_ERROR(ib->renderDevice, err);
}

void Magma::Framework::Graphics::V2X::RenderDevice::ClearColor(mfmF32 r, mfmF32 g, mfmF32 b, mfmF32 a)
{
	mfError err = mfgV2XClearColor(reinterpret_cast<mfgV2XRenderDevice*>(&this->Get()), r, g, b, a);
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (err != MF_ERROR_OKAY)
		throw RenderDeviceError(mfErrorToString(err));
#endif
}

void Magma::Framework::Graphics::V2X::RenderDevice::SwapBuffers()
{
	mfError err = mfgV2XSwapBuffers(reinterpret_cast<mfgV2XRenderDevice*>(&this->Get()));
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (err != MF_ERROR_OKAY)
		throw RenderDeviceError(mfErrorToString(err));
#endif
}

Magma::Framework::Graphics::V2X::RenderDevice Magma::Framework::Graphics::V2X::CreateRenderDevice(const mfsUTF8CodeUnit * type, Input::WindowHandle window, const RenderDeviceDesc * desc, Memory::AllocatorHandle allocator)
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
