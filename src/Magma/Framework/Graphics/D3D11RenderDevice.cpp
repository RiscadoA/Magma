#include "D3D11RenderDevice.hpp"
#include "D3D11Assembler.hpp"

using namespace Magma::Framework::Graphics;
using namespace Magma::Framework;

#include <Config.hpp>
#include <sstream>
#include <string>
#include <map>

#if defined(MAGMA_FRAMEWORK_USE_DIRECTX)
#include <windows.h>
#include <windowsx.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <comdef.h>

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")
#pragma comment (lib, "dxguid.lib")
#endif

class D3D11Texture2D final : public Texture2D
{
public:
	D3D11Texture2D(D3D11RenderDevice* device, size_t width, size_t height, TextureFormat _format, const void* data, BufferUsage usage, bool isRenderTarget)
	{
		HRESULT hr;

		this->device = device;

		// Create texture
		D3D11_TEXTURE2D_DESC desc;

		desc.Width = width;
		desc.Height = height;
		
		desc.MipLevels = 1;
		desc.ArraySize = 1;

		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;

		switch (usage)
		{
			case BufferUsage::Default: desc.Usage = D3D11_USAGE_DEFAULT; desc.CPUAccessFlags = 0; break;
			case BufferUsage::Static: desc.Usage = D3D11_USAGE_IMMUTABLE; desc.CPUAccessFlags = 0; break;
			case BufferUsage::Dynamic: desc.Usage = D3D11_USAGE_DYNAMIC; desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; break;
			case BufferUsage::Invalid: throw RenderDeviceError("Failed to create D3D11Texture2D:\nInvalid buffer usage mode"); break;
			default: throw RenderDeviceError("Failed to create D3D11Texture2D:\nUnknown buffer usage mode"); break;
		}

		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		desc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

		switch (_format)
		{
			case TextureFormat::R8SNorm: desc.Format = DXGI_FORMAT_R8_SNORM; formatSize = 1; break;
			case TextureFormat::R16SNorm: desc.Format = DXGI_FORMAT_R16_SNORM; formatSize = 2; break;
			case TextureFormat::RG8SNorm: desc.Format = DXGI_FORMAT_R8G8_SNORM; formatSize = 2; break;
			case TextureFormat::RG16SNorm: desc.Format = DXGI_FORMAT_R16G16_SNORM; formatSize = 4; break;
			case TextureFormat::RGBA8SNorm: desc.Format = DXGI_FORMAT_R8G8B8A8_SNORM; formatSize = 4; break;
			case TextureFormat::RGBA16SNorm: desc.Format = DXGI_FORMAT_R16G16B16A16_SNORM; formatSize = 8; break;
			
			case TextureFormat::R8UNorm: desc.Format = DXGI_FORMAT_R8_UNORM; formatSize = 1; break;
			case TextureFormat::R16UNorm: desc.Format = DXGI_FORMAT_R16_UNORM; formatSize = 2; break;
			case TextureFormat::RG8UNorm: desc.Format = DXGI_FORMAT_R8G8_UNORM; formatSize = 2; break;
			case TextureFormat::RG16UNorm: desc.Format = DXGI_FORMAT_R16G16_UNORM; formatSize = 4; break;
			case TextureFormat::RGBA8UNorm: desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; formatSize = 4; break;
			case TextureFormat::RGBA16UNorm: desc.Format = DXGI_FORMAT_R16G16B16A16_UNORM; formatSize = 8; break;
			
			case TextureFormat::R8Int: desc.Format = DXGI_FORMAT_R8_SINT; formatSize = 1; break;
			case TextureFormat::R16Int: desc.Format = DXGI_FORMAT_R16_SINT; formatSize = 2; break;
			case TextureFormat::RG8Int: desc.Format = DXGI_FORMAT_R8G8_SINT; formatSize = 2; break;
			case TextureFormat::RG16Int: desc.Format = DXGI_FORMAT_R16G16_SINT; formatSize = 4; break;
			case TextureFormat::RGBA8Int: desc.Format = DXGI_FORMAT_R8G8B8A8_SINT; formatSize = 4; break;
			case TextureFormat::RGBA16Int: desc.Format = DXGI_FORMAT_R16G16B16A16_SINT; formatSize = 8; break;

			case TextureFormat::R8UInt: desc.Format = DXGI_FORMAT_R8_UINT; formatSize = 1; break;
			case TextureFormat::R16UInt: desc.Format = DXGI_FORMAT_R16_UINT; formatSize = 2; break;
			case TextureFormat::RG8UInt: desc.Format = DXGI_FORMAT_R8G8_UINT; formatSize = 2; break;
			case TextureFormat::RG16UInt: desc.Format = DXGI_FORMAT_R16G16_UINT; formatSize = 4; break;
			case TextureFormat::RGBA8UInt: desc.Format = DXGI_FORMAT_R8G8B8A8_UINT; formatSize = 4; break;
			case TextureFormat::RGBA16UInt: desc.Format = DXGI_FORMAT_R16G16B16A16_UINT; formatSize = 8; break;

			case TextureFormat::R32Float: desc.Format = DXGI_FORMAT_R32_FLOAT; formatSize = 4; break;
			case TextureFormat::RG32Float: desc.Format = DXGI_FORMAT_R32G32_FLOAT; formatSize = 8; break;
			case TextureFormat::RGB32Float: desc.Format = DXGI_FORMAT_R32G32B32_FLOAT; formatSize = 12; break;
			case TextureFormat::RGBA32Float: desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT; formatSize = 16; break;

			case TextureFormat::Invalid: throw std::runtime_error("Failed to create 2D texture on D3D11Context:\nInvalid format"); break;
			default: throw std::runtime_error("Failed to create 2D texture on D3D11Context:\nUnsupported format"); break;
		}

		format = desc.Format;

		if (data == nullptr)
		{
			if (usage != BufferUsage::Dynamic)
				throw RenderDeviceError("Failed to create D3D11Texture2D:\nOnly dynamic 2D textures can be initialized with null data");

			hr = ((ID3D11Device*)device->m_device)->CreateTexture2D(&desc, nullptr, &texture);
			if (FAILED(hr))
			{
				std::stringstream ss;
				ss << "Failed to create D3D11Texture2D:\nFailed to create 2D texture:" << std::endl;
				ss << "Error: " << _com_error(hr).ErrorMessage();
				throw RenderDeviceError(ss.str());
			}
		}
		else
		{
			D3D11_SUBRESOURCE_DATA initData;
			initData.pSysMem = data;
			initData.SysMemPitch = formatSize * width;
			initData.SysMemSlicePitch = 0;

			hr = ((ID3D11Device*)device->m_device)->CreateTexture2D(&desc, &initData, &texture);
			if (FAILED(hr))
			{
				std::stringstream ss;
				ss << "Failed to create D3D11Texture2D:\nFailed to create 2D texture:" << std::endl;
				ss << "Error: " << _com_error(hr).ErrorMessage();
				throw RenderDeviceError(ss.str());
			}
		}	

		// Create resource view
		hr = ((ID3D11Device*)device->m_device)->CreateShaderResourceView(texture, NULL, &resourceView);
		if (FAILED(hr))
		{
			std::stringstream ss;
			ss << "Failed to create D3D11Texture2D:\nFailed to create shader resource view:" << std::endl;
			ss << "Error: " << _com_error(hr).ErrorMessage();
			throw RenderDeviceError(ss.str());
		}

		// Create render target view
		renderTargetView = nullptr;
		if (isRenderTarget)
		{
			hr = ((ID3D11Device*)device->m_device)->CreateRenderTargetView(texture, NULL, &renderTargetView);
			if (FAILED(hr))
			{
				std::stringstream ss;
				ss << "Failed to create D3D11Texture2D:\nFailed to create render target view:" << std::endl;
				ss << "Error: " << _com_error(hr).ErrorMessage();
				throw RenderDeviceError(ss.str());
			}
		}
	}

	virtual ~D3D11Texture2D() final
	{
		resourceView->Release();
		if (renderTargetView != nullptr)
			renderTargetView->Release();
		texture->Release();
	}

	virtual void Update(size_t dstX, size_t dstY, size_t width, size_t height, void* data) final
	{
		D3D11_BOX dstBox;
		dstBox.back = 1;
		dstBox.front = 0;
		dstBox.left = dstX;
		dstBox.top = dstY;
		dstBox.right = dstX + width;
		dstBox.bottom = dstY + height;

		((ID3D11DeviceContext*)device->m_deviceContext)->UpdateSubresource(texture, 0, &dstBox, data, formatSize * width, 0);
	}

	virtual void GenerateMipmaps()
	{
		((ID3D11DeviceContext*)device->m_deviceContext)->GenerateMips(resourceView);
	}

	DXGI_FORMAT format;
	UINT formatSize;
	D3D11RenderDevice* device;
	ID3D11Texture2D* texture;
	ID3D11ShaderResourceView* resourceView;
	ID3D11RenderTargetView* renderTargetView;
};

class D3D11Sampler2D : public Sampler2D
{
public:
	D3D11Sampler2D(D3D11RenderDevice* device, const Sampler2DDesc& _desc)
	{
		HRESULT hr;

		this->device = device;

		D3D11_SAMPLER_DESC desc;

		if (_desc.maxAnisotropy > 1)
			desc.Filter = D3D11_FILTER_ANISOTROPIC;
		else if (_desc.minFilter == TextureFilter::Nearest && _desc.mipmapFilter != TextureFilter::Linear && _desc.magFilter == TextureFilter::Nearest)
			desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		else if (_desc.minFilter == TextureFilter::Linear && _desc.mipmapFilter != TextureFilter::Linear && _desc.magFilter == TextureFilter::Nearest)
			desc.Filter = D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT;
		else if (_desc.minFilter == TextureFilter::Linear && _desc.mipmapFilter != TextureFilter::Linear && _desc.magFilter == TextureFilter::Linear)
			desc.Filter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
		else if (_desc.minFilter == TextureFilter::Nearest && _desc.mipmapFilter != TextureFilter::Linear && _desc.magFilter == TextureFilter::Linear)
			desc.Filter = D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT;
		else if (_desc.minFilter == TextureFilter::Nearest && _desc.mipmapFilter == TextureFilter::Linear && _desc.magFilter == TextureFilter::Nearest)
			desc.Filter = D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR;
		else if (_desc.minFilter == TextureFilter::Linear && _desc.mipmapFilter == TextureFilter::Linear && _desc.magFilter == TextureFilter::Nearest)
			desc.Filter = D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
		else if (_desc.minFilter == TextureFilter::Linear && _desc.mipmapFilter == TextureFilter::Linear && _desc.magFilter == TextureFilter::Linear)
			desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		else if (_desc.minFilter == TextureFilter::Nearest && _desc.mipmapFilter == TextureFilter::Linear && _desc.magFilter == TextureFilter::Linear)
			desc.Filter = D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR;
		else
			throw RenderDeviceError("Failed to create 2D sampler:\nUnsupported/invalid texture filter");

		switch (_desc.addressU)
		{
			case TextureAdressMode::Repeat: desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP; break;
			case TextureAdressMode::Mirror: desc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR; break;
			case TextureAdressMode::Clamp: desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP; break;
			case TextureAdressMode::Border: desc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER; break;
			case TextureAdressMode::Invalid: throw std::runtime_error("Failed to create 2D sampler on D3D11Context:\nInvalid U adress mode"); break;
			default: throw std::runtime_error("Failed to create 2D sampler on D3D11Context:\nUnsupported U adress mode"); break;
		}

		switch (_desc.addressV)
		{
			case TextureAdressMode::Repeat: desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP; break;
			case TextureAdressMode::Mirror: desc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR; break;
			case TextureAdressMode::Clamp: desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP; break;
			case TextureAdressMode::Border: desc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER; break;
			case TextureAdressMode::Invalid: throw std::runtime_error("Failed to create 2D sampler on D3D11Context:\nInvalid V adress mode"); break;
			default: throw std::runtime_error("Failed to create 2D sampler on D3D11Context:\nUnsupported V adress mode"); break;
		}

		desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.BorderColor[0] = _desc.border[0];
		desc.BorderColor[1] = _desc.border[1];
		desc.BorderColor[2] = _desc.border[2];
		desc.BorderColor[3] = _desc.border[3];
		desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		desc.MaxAnisotropy = _desc.maxAnisotropy;
		desc.MinLOD = -FLT_MAX;
		desc.MaxLOD = +FLT_MAX;
		desc.MipLODBias = 0.0f;

		hr = ((ID3D11Device*)device->m_device)->CreateSamplerState(&desc, &sampler);
		if (FAILED(hr))
		{
			std::stringstream ss;
			ss << "Failed to create D3D11Sampler2D:\nFailed to create sampler state:" << std::endl;
			ss << "Error: " << _com_error(hr).ErrorMessage();
			throw RenderDeviceError(ss.str());
		}
	}

	virtual ~D3D11Sampler2D() final
	{
		sampler->Release();
	}

	D3D11RenderDevice* device;
	ID3D11SamplerState* sampler;
	ID3D11ShaderResourceView* resourceView;
};

class D3D11ConstantBuffer final : public ConstantBuffer
{
public:
	D3D11ConstantBuffer(D3D11RenderDevice* device, size_t size, const void* data, BufferUsage usage)
	{
		this->device = device;

		HRESULT hr;
		D3D11_BUFFER_DESC desc;

		switch (usage)
		{
			case BufferUsage::Default: desc.Usage = D3D11_USAGE_DEFAULT; desc.CPUAccessFlags = 0; break;
			case BufferUsage::Static: desc.Usage = D3D11_USAGE_IMMUTABLE; desc.CPUAccessFlags = 0; break;
			case BufferUsage::Dynamic: desc.Usage = D3D11_USAGE_DYNAMIC; desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; break;
			case BufferUsage::Invalid: throw RenderDeviceError("Failed to create D3D11ConstantBuffer:\nInvalid buffer usage mode"); break;
			default: throw RenderDeviceError("Failed to create D3D11ConstantBuffer:\nUnknown buffer usage mode"); break;
		}

		desc.ByteWidth = size;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.MiscFlags = 0;

		if (data == nullptr)
		{
			if (usage != BufferUsage::Dynamic)
				throw RenderDeviceError("Failed to create D3D11ConstantBuffer:\nOnly dynamic constant buffers can be initialized with null data");

			hr = ((ID3D11Device*)device->m_device)->CreateBuffer(&desc, NULL, &buffer);
			if (FAILED(hr))
			{
				std::stringstream ss;
				ss << "Failed to create D3D11ConstantBuffer:\nFailed to create buffer:" << std::endl;
				ss << "Error: " << _com_error(hr).ErrorMessage();
				throw RenderDeviceError(ss.str());
			}
		}
		else
		{
			D3D11_SUBRESOURCE_DATA initData;
			initData.pSysMem = data;
			initData.SysMemPitch = 0;
			initData.SysMemSlicePitch = 0;

			hr = ((ID3D11Device*)device->m_device)->CreateBuffer(&desc, &initData, &buffer);
			if (FAILED(hr))
			{
				std::stringstream ss;
				ss << "Failed to create D3D11ConstantBuffer:\nFailed to create buffer:" << std::endl;
				ss << "Error: " << _com_error(hr).ErrorMessage();
				throw RenderDeviceError(ss.str());
			}
		}
	}

	virtual ~D3D11ConstantBuffer() final
	{
		buffer->Release();
	}

	virtual void* Map() final
	{
		D3D11_MAPPED_SUBRESOURCE map;
		HRESULT hr = ((ID3D11DeviceContext*)device->m_deviceContext)->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
		if (FAILED(hr))
		{
			std::stringstream ss;
			ss << "Failed to map D3D11ConstantBuffer:\nFailed to map buffer:" << std::endl;
			ss << "Error: " << _com_error(hr).ErrorMessage();
			throw RenderDeviceError(ss.str());
		}
		return map.pData;
	}

	virtual void Unmap() final
	{
		((ID3D11DeviceContext*)device->m_deviceContext)->Unmap(buffer, 0);
	}

	D3D11RenderDevice* device;
	ID3D11Buffer* buffer;
};

class D3D11VertexBindingPoint;
class D3D11VertexShader : public VertexShader
{
public:
	D3D11VertexShader(D3D11RenderDevice* device, const ShaderData& data)
		: data(data)
	{
		this->device = device;

		HRESULT hr;
		std::string src;

		blob = nullptr;
		vs = nullptr;

		// Compile shader
		{
			D3D11Assembler::Assemble(data, src);
			ID3DBlob* errorMessages;
			hr = D3DCompile(src.c_str(), src.size(), nullptr, nullptr, nullptr, "VS", "vs_4_0", D3D10_SHADER_PACK_MATRIX_COLUMN_MAJOR, 0, &blob, &errorMessages);
			if (FAILED(hr))
			{
				std::stringstream ss;
				ss << "Failed to create shader on D3D11RenderDevice:\nFailed to compile vertex shader:\n";
				if (errorMessages)
					ss << (char*)errorMessages->GetBufferPointer() << std::endl;
				else
					ss << "Error: " << _com_error(hr).ErrorMessage() << " - no error messages";
				ss << "Assembled shader source:" << std::endl << src;
				throw RenderDeviceError(ss.str());
			}
		}

		// Create shader
		{
			hr = ((ID3D11Device*)device->m_device)->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), NULL, &vs);
			if (FAILED(hr))
			{
				std::stringstream ss;
				ss << "Failed to create shader on D3D11RenderDevice:\nFailed to create vertex shader:\n";
				ss << "Error: " << _com_error(hr).ErrorMessage() << std::endl;
				ss << "Assembled shader source:" << std::endl << src;
				throw RenderDeviceError(ss.str());
			}
		}
	}

	virtual ~D3D11VertexShader() final
	{
		if (vs != nullptr)
			vs->Release();
		if (blob != nullptr)
			blob->Release();
	}

	virtual VertexBindingPoint* GetBindingPoint(const char * name) final;

	ShaderData data;
	std::unordered_map<std::string, D3D11VertexBindingPoint*> bindingPoints;
	D3D11RenderDevice* device;
	ID3D11VertexShader* vs;
	ID3DBlob* blob;
};

class D3D11VertexBindingPoint final : public VertexBindingPoint
{
public:
	D3D11VertexBindingPoint(D3D11VertexShader* _shader, int _index, bool _isTexture)
		: shader(_shader), index(_index), isTexture(_isTexture)
	{
		// Empty
	}

	virtual ~D3D11VertexBindingPoint() final
	{

	}

	virtual void Bind(Texture2D* texture) final
	{
		auto tex = static_cast<D3D11Texture2D*>(texture);
		((ID3D11DeviceContext*)shader->device->m_deviceContext)->VSSetShaderResources(index, 1, &tex->resourceView);
	}

	virtual void Bind(Sampler2D* sampler) final
	{
		auto smplr = static_cast<D3D11Sampler2D*>(sampler);
		((ID3D11DeviceContext*)shader->device->m_deviceContext)->VSSetSamplers(index, 1, &smplr->sampler);
	}

	virtual void Bind(ConstantBuffer* buffer) final
	{
		auto buf = static_cast<D3D11ConstantBuffer*>(buffer);
		((ID3D11DeviceContext*)shader->device->m_deviceContext)->VSSetConstantBuffers(index, 1, &buf->buffer);
	}

	D3D11VertexShader* shader;
	bool isTexture;
	int index;
};

VertexBindingPoint * D3D11VertexShader::GetBindingPoint(const char * name)
{
	// Check current binding points
	for (auto& bp : bindingPoints)
		if (bp.first == name)
			return bp.second;

	// Check textures
	{
		const auto& vars = data.GetTexture2DVariables();
		for (auto& v : vars)
			if (v.name == name)
			{
				auto bp = new D3D11VertexBindingPoint(this, v.index, true);
				bindingPoints.insert(std::make_pair(std::string(name), bp));
				return bp;
			}
	}

	// Check constant buffers
	{
		const auto& vars = data.GetShaderConstantBuffers();
		for (auto& v : vars)
			if (v.name == name)
			{
				auto bp = new D3D11VertexBindingPoint(this, v.index, false);
				bindingPoints.insert(std::make_pair(std::string(name), bp));
				return bp;
			}
	}

	return nullptr;
}

class D3D11PixelBindingPoint;
class D3D11PixelShader : public PixelShader
{
public:
	D3D11PixelShader(D3D11RenderDevice* device, const ShaderData& data)
		: data(data)
	{
		this->device = device;

		HRESULT hr;
		std::string src;

		blob = nullptr;
		ps = nullptr;

		// Compile shader
		{			
			D3D11Assembler::Assemble(data, src);
			ID3DBlob* errorMessages;
			hr = D3DCompile(src.c_str(), src.size(), nullptr, nullptr, nullptr, "PS", "ps_4_0", D3D10_SHADER_PACK_MATRIX_COLUMN_MAJOR, 0, &blob, &errorMessages);
			if (FAILED(hr))
			{
				std::stringstream ss;
				ss << "Failed to create shader on D3D11RenderDevice:\nFailed to compile pixel shader:\n";
				if (errorMessages)
					ss << (char*)errorMessages->GetBufferPointer() << std::endl;
				else
					ss << "Error: " << _com_error(hr).ErrorMessage() << " - no error messages";
				ss << "Assembled shader source:" << std::endl << src;
				throw RenderDeviceError(ss.str());
			}
		}
			
		// Create shader
		{
			hr = ((ID3D11Device*)device->m_device)->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), NULL, &ps);
			if (FAILED(hr))
			{
				std::stringstream ss;
				ss << "Failed to create shader on D3D11RenderDevice:\nFailed to create pixel shader:\n";
				ss << "Error: " << _com_error(hr).ErrorMessage() << std::endl;
				ss << "Assembled shader source:" << std::endl << src;
				throw RenderDeviceError(ss.str());
			}
		}
	}

	virtual ~D3D11PixelShader() final
	{
		if (ps != nullptr)
			ps->Release();
		if (blob != nullptr)
			blob->Release();
	}

	virtual PixelBindingPoint * GetBindingPoint(const char * name) final;

	ShaderData data;
	std::unordered_map<std::string, D3D11PixelBindingPoint*> bindingPoints;
	D3D11RenderDevice* device;
	ID3D11PixelShader* ps;
	ID3DBlob* blob;
};

class D3D11PixelBindingPoint final : public PixelBindingPoint
{
public:
	D3D11PixelBindingPoint(D3D11PixelShader* _shader, int _index, bool _isTexture)
		: shader(_shader), index(_index), isTexture(_isTexture)
	{
		// Empty
	}

	virtual ~D3D11PixelBindingPoint() final
	{

	}

	virtual void Bind(Texture2D* texture) final
	{
		auto tex = static_cast<D3D11Texture2D*>(texture);
		((ID3D11DeviceContext*)shader->device->m_deviceContext)->PSSetShaderResources(index, 1, &tex->resourceView);
	}

	virtual void Bind(Sampler2D* sampler) final
	{
		auto smplr = static_cast<D3D11Sampler2D*>(sampler);
		((ID3D11DeviceContext*)shader->device->m_deviceContext)->PSSetSamplers(index, 1, &smplr->sampler);
	}

	virtual void Bind(ConstantBuffer* buffer) final
	{
		auto buf = static_cast<D3D11ConstantBuffer*>(buffer);
		((ID3D11DeviceContext*)shader->device->m_deviceContext)->PSSetConstantBuffers(index, 1, &buf->buffer);
	}

	D3D11PixelShader* shader;
	bool isTexture;
	int index;
};

PixelBindingPoint * D3D11PixelShader::GetBindingPoint(const char * name)
{
	// Check current binding points
	for (auto& bp : bindingPoints)
		if (bp.first == name)
			return bp.second;

	// Check textures
	{
		const auto& vars = data.GetTexture2DVariables();
		for (auto& v : vars)
			if (v.name == name)
			{
				auto bp = new D3D11PixelBindingPoint(this, v.index, true);
				bindingPoints.insert(std::make_pair(std::string(name), bp));
				return bp;
			}
	}

	// Check constant buffers
	{
		const auto& vars = data.GetShaderConstantBuffers();
		for (auto& v : vars)
			if (v.name == name)
			{
				auto bp = new D3D11PixelBindingPoint(this, v.index, false);
				bindingPoints.insert(std::make_pair(std::string(name), bp));
				return bp;
			}
	}

	return nullptr;
}

class D3D11Pipeline final : public Pipeline
{
public:
	D3D11Pipeline(D3D11VertexShader* vertexShader, D3D11PixelShader* pixelShader)
	{
		this->vertexShader = vertexShader;
		this->pixelShader = pixelShader;
	}

	virtual ~D3D11Pipeline() final
	{
		
	}

	D3D11VertexShader* vertexShader;
	D3D11PixelShader* pixelShader;
};

class D3D11VertexBuffer final : public VertexBuffer
{
public:
	D3D11VertexBuffer(D3D11RenderDevice* device, size_t size, const void* data, BufferUsage usage)
	{
		this->device = device;

		HRESULT hr;
		D3D11_BUFFER_DESC desc;
		
		switch (usage)
		{
			case BufferUsage::Default: desc.Usage = D3D11_USAGE_DEFAULT; desc.CPUAccessFlags = 0; break;
			case BufferUsage::Static: desc.Usage = D3D11_USAGE_IMMUTABLE; desc.CPUAccessFlags = 0; break;
			case BufferUsage::Dynamic: desc.Usage = D3D11_USAGE_DYNAMIC; desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; break;
			case BufferUsage::Invalid: throw RenderDeviceError("Failed to create D3D11VertexBuffer:\nInvalid buffer usage mode"); break;
			default: throw RenderDeviceError("Failed to create D3D11VertexBuffer:\nUnknown buffer usage mode"); break;
		}

		desc.ByteWidth = size;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.MiscFlags = 0;		

		if (data == nullptr)
		{
			if (usage != BufferUsage::Dynamic)
				throw RenderDeviceError("Failed to create D3D11VertexBuffer:\nOnly dynamic vertex buffers can be initialized with null data");

			hr = ((ID3D11Device*)device->m_device)->CreateBuffer(&desc, NULL, &buffer);
			if (FAILED(hr))
			{
				std::stringstream ss;
				ss << "Failed to create D3D11VertexBuffer:\nFailed to create buffer:" << std::endl;
				ss << "Error: " << _com_error(hr).ErrorMessage();
				throw RenderDeviceError(ss.str());
			}
		}
		else
		{
			D3D11_SUBRESOURCE_DATA initData;
			initData.pSysMem = data;
			initData.SysMemPitch = 0;
			initData.SysMemSlicePitch = 0;

			hr = ((ID3D11Device*)device->m_device)->CreateBuffer(&desc, &initData, &buffer);
			if (FAILED(hr))
			{
				std::stringstream ss;
				ss << "Failed to create D3D11VertexBuffer:\nFailed to create buffer:" << std::endl;
				ss << "Error: " << _com_error(hr).ErrorMessage();
				throw RenderDeviceError(ss.str());
			}
		}
	}

	virtual ~D3D11VertexBuffer() final
	{
		buffer->Release();
	}

	virtual void* Map() final
	{
		D3D11_MAPPED_SUBRESOURCE map;
		HRESULT hr = ((ID3D11DeviceContext*)device->m_deviceContext)->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
		if (FAILED(hr))
		{
			std::stringstream ss;
			ss << "Failed to map D3D11VertexBuffer:\nFailed to map buffer:" << std::endl;
			ss << "Error: " << _com_error(hr).ErrorMessage();
			throw RenderDeviceError(ss.str());
		}
		return map.pData;
	}

	virtual void Unmap() final
	{
		((ID3D11DeviceContext*)device->m_deviceContext)->Unmap(buffer, 0);
	}

	D3D11RenderDevice* device;
	ID3D11Buffer* buffer;
};

class D3D11VertexLayout final : public VertexLayout
{
public:
	D3D11VertexLayout(D3D11RenderDevice* device, size_t vertexElementCount, const VertexElement* vertexElements, D3D11VertexShader* vertexShader)
	{
		this->device = device;

		char** names = new char*[vertexElementCount];
		for (size_t i = 0; i < vertexElementCount; ++i)
			names[i] = nullptr;
		auto inputDesc = new D3D11_INPUT_ELEMENT_DESC[vertexElementCount];
		offsets = new UINT[vertexElementCount];
		strides = new UINT[vertexElementCount];

		try
		{
			for (size_t i = 0; i < vertexElementCount; ++i)
			{
				ZeroMemory(&inputDesc[i], sizeof(inputDesc[i]));

				// Get semantic name
				for (auto& v : vertexShader->data.GetInputVariables())
					if (v.name == vertexElements[i].name)
					{
						std::string str;
						str = "IN";
						str += std::to_string(v.index - 1);
						str += "IN";
						names[i] = new char[str.size() + 1];
						memcpy(names[i], str.c_str(), str.size());
						names[i][str.size()] = '\0';
						inputDesc[i].SemanticName = names[i];
						break;
					}

				inputDesc[i].SemanticIndex = 0;
				inputDesc[i].InputSlot = vertexElements[i].bufferIndex;
				inputDesc[i].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
				inputDesc[i].AlignedByteOffset = vertexElements[i].offset;
				inputDesc[i].InstanceDataStepRate = 0;
				offsets[i] = vertexElements[i].offset;
				strides[i] = vertexElements[i].stride;

				// Get format
				switch (vertexElements[i].size)
				{
					case 1:
						switch (vertexElements[i].type)
						{
							case VertexElementType::Byte: inputDesc[i].Format = DXGI_FORMAT_R8_SINT; break;
							case VertexElementType::Short: inputDesc[i].Format = DXGI_FORMAT_R16_SINT; break;
							case VertexElementType::Int: inputDesc[i].Format = DXGI_FORMAT_R32_SINT; break;
							case VertexElementType::UByte: inputDesc[i].Format = DXGI_FORMAT_R8_UINT; break;
							case VertexElementType::UShort: inputDesc[i].Format = DXGI_FORMAT_R16_UINT; break;
							case VertexElementType::UInt: inputDesc[i].Format = DXGI_FORMAT_R32_UINT; break;
							case VertexElementType::NByte: inputDesc[i].Format = DXGI_FORMAT_R8_SNORM; break;
							case VertexElementType::NShort: inputDesc[i].Format = DXGI_FORMAT_R16_SNORM; break;
							case VertexElementType::NUByte: inputDesc[i].Format = DXGI_FORMAT_R8_UNORM; break;
							case VertexElementType::NUShort: inputDesc[i].Format = DXGI_FORMAT_R16_UNORM; break;
							case VertexElementType::HalfFloat: inputDesc[i].Format = DXGI_FORMAT_R16_FLOAT; break;
							case VertexElementType::Float: inputDesc[i].Format = DXGI_FORMAT_R32_FLOAT; break;
							case VertexElementType::Invalid: throw RenderDeviceError("Faield to create D3D11VertexLayout:\nInvalid vertex element type"); break;
							default: throw RenderDeviceError("Faield to create D3D11VertexLayout:\nUnsupported vertex element type"); break;
						}
						break;
					case 2:
						switch (vertexElements[i].type)
						{
							case VertexElementType::Byte: inputDesc[i].Format = DXGI_FORMAT_R8G8_SINT; break;
							case VertexElementType::Short: inputDesc[i].Format = DXGI_FORMAT_R16G16_SINT; break;
							case VertexElementType::Int: inputDesc[i].Format = DXGI_FORMAT_R32G32_SINT; break;
							case VertexElementType::UByte: inputDesc[i].Format = DXGI_FORMAT_R8G8_UINT; break;
							case VertexElementType::UShort: inputDesc[i].Format = DXGI_FORMAT_R16G16_UINT; break;
							case VertexElementType::UInt: inputDesc[i].Format = DXGI_FORMAT_R32G32_UINT; break;
							case VertexElementType::NByte: inputDesc[i].Format = DXGI_FORMAT_R8G8_SNORM; break;
							case VertexElementType::NShort: inputDesc[i].Format = DXGI_FORMAT_R16G16_SNORM; break;
							case VertexElementType::NUByte: inputDesc[i].Format = DXGI_FORMAT_R8G8_UNORM; break;
							case VertexElementType::NUShort: inputDesc[i].Format = DXGI_FORMAT_R16G16_UNORM; break;
							case VertexElementType::HalfFloat: inputDesc[i].Format = DXGI_FORMAT_R16G16_FLOAT; break;
							case VertexElementType::Float: inputDesc[i].Format = DXGI_FORMAT_R32G32_FLOAT; break;
							case VertexElementType::Invalid: throw RenderDeviceError("Faield to create D3D11VertexLayout:\nInvalid vertex element type"); break;
							default: throw RenderDeviceError("Faield to create D3D11VertexLayout:\nUnsupported vertex element type"); break;
						}
						break;
					case 3:
						switch (vertexElements[i].type)
						{
							case VertexElementType::Int: inputDesc[i].Format = DXGI_FORMAT_R32G32B32_SINT; break;
							case VertexElementType::UInt: inputDesc[i].Format = DXGI_FORMAT_R32G32B32_UINT; break;
							case VertexElementType::Float: inputDesc[i].Format = DXGI_FORMAT_R32G32B32_FLOAT; break;
							case VertexElementType::Invalid: throw RenderDeviceError("Failed to create D3D11VertexLayout:\nInvalid vertex element type"); break;
							default: throw RenderDeviceError("Failed to create D3D11VertexLayout:\nUnsupported vertex element type"); break;
						}
						break;
					case 4:
						switch (vertexElements[i].type)
						{
							case VertexElementType::Byte: inputDesc[i].Format = DXGI_FORMAT_R8G8B8A8_SINT; break;
							case VertexElementType::Short: inputDesc[i].Format = DXGI_FORMAT_R16G16B16A16_SINT; break;
							case VertexElementType::Int: inputDesc[i].Format = DXGI_FORMAT_R32G32B32A32_SINT; break;
							case VertexElementType::UByte: inputDesc[i].Format = DXGI_FORMAT_R8G8B8A8_UINT; break;
							case VertexElementType::UShort: inputDesc[i].Format = DXGI_FORMAT_R16G16B16A16_UINT; break;
							case VertexElementType::UInt: inputDesc[i].Format = DXGI_FORMAT_R32G32B32A32_UINT; break;
							case VertexElementType::NByte: inputDesc[i].Format = DXGI_FORMAT_R8G8B8A8_SNORM; break;
							case VertexElementType::NShort: inputDesc[i].Format = DXGI_FORMAT_R16G16B16A16_SNORM; break;
							case VertexElementType::NUByte: inputDesc[i].Format = DXGI_FORMAT_R8G8B8A8_UNORM; break;
							case VertexElementType::NUShort: inputDesc[i].Format = DXGI_FORMAT_R16G16B16A16_UNORM; break;
							case VertexElementType::HalfFloat: inputDesc[i].Format = DXGI_FORMAT_R16G16B16A16_FLOAT; break;
							case VertexElementType::Float: inputDesc[i].Format = DXGI_FORMAT_R32G32B32A32_FLOAT; break;
							case VertexElementType::Invalid: throw RenderDeviceError("Faield to create D3D11VertexLayout:\nInvalid vertex element type"); break;
							default: throw RenderDeviceError("Faield to create D3D11VertexLayout:\nUnsupported vertex element type"); break;
						}
						break;
					default: throw RenderDeviceError("Faield to create D3D11VertexLayout:\nUnsupported vertex element component count"); break;
				}
			}
		}
		catch (...)
		{
			for (size_t i = 0; i < vertexElementCount; ++i)
				if (names[i] != nullptr)
					delete[] names[i];
			delete[] names;
			delete[] inputDesc;		
			throw;
		}

		HRESULT hr = ((ID3D11Device*)device->m_device)->CreateInputLayout(inputDesc, vertexElementCount, vertexShader->blob->GetBufferPointer(), vertexShader->blob->GetBufferSize(), &inputLayout);
		delete[] inputDesc;
		for (size_t i = 0; i < vertexElementCount; ++i)
			delete[] names[i];
		delete[] names;
		if (FAILED(hr))
		{
			std::stringstream ss;
			ss << "Failed to create D3D11VertexLayout:\nFailed to create input layout:" << std::endl;
			ss << "Error: " << _com_error(hr).ErrorMessage();
			throw RenderDeviceError(ss.str());
		}	
	}

	virtual ~D3D11VertexLayout() final
	{
		inputLayout->Release();
		delete[] offsets;
		delete[] strides;
	}

	D3D11RenderDevice* device;
	ID3D11InputLayout* inputLayout;
	UINT* offsets;
	UINT* strides;
};

class D3D11VertexArray final : public VertexArray
{
public:
	D3D11VertexArray(D3D11RenderDevice* device, size_t bufferCount, D3D11VertexBuffer** buffers, D3D11VertexLayout* layout)
	{
		this->device = device;
		this->buffers = new ID3D11Buffer*[bufferCount];
		for (size_t i = 0; i < bufferCount; ++i)
			this->buffers[i] = buffers[i]->buffer;
		this->bufferCount = bufferCount;
		this->layout = layout;
	}

	virtual ~D3D11VertexArray() final
	{
		
	}

	D3D11RenderDevice* device;
	ID3D11Buffer** buffers;
	D3D11VertexLayout* layout;
	size_t bufferCount;
};

class D3D11IndexBuffer final : public IndexBuffer
{
public:
	D3D11IndexBuffer(D3D11RenderDevice* device, IndexType type, size_t size, const void* data, BufferUsage usage)
	{
		this->device = device;

		HRESULT hr;
		D3D11_BUFFER_DESC desc;

		switch (usage)
		{
			case BufferUsage::Default: desc.Usage = D3D11_USAGE_DEFAULT; desc.CPUAccessFlags = 0; break;
			case BufferUsage::Static: desc.Usage = D3D11_USAGE_IMMUTABLE; desc.CPUAccessFlags = 0; break;
			case BufferUsage::Dynamic: desc.Usage = D3D11_USAGE_DYNAMIC; desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; break;
			case BufferUsage::Invalid: throw RenderDeviceError("Failed to create D3D11IndexBuffer:\nInvalid buffer usage mode"); break;
			default: throw RenderDeviceError("Failed to create D3D11IndexBuffer:\nUnknown buffer usage mode"); break;
		}

		switch (type)
		{
			case IndexType::UByte: format = DXGI_FORMAT_R8_UINT; break;
			case IndexType::UShort: format = DXGI_FORMAT_R16_UINT; break;
			case IndexType::UInt: format = DXGI_FORMAT_R32_UINT; break;
			case IndexType::Invalid: throw RenderDeviceError("Failed to create D3D11IndexBuffer:\nInvalid index type"); break;
			default: throw RenderDeviceError("Failed to create D3D11IndexBuffer:\nUnknown index type"); break;
		}

		desc.ByteWidth = size;
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		desc.MiscFlags = 0;

		if (data == nullptr)
		{
			if (usage != BufferUsage::Dynamic)
				throw RenderDeviceError("Failed to create D3D11IndexBuffer:\nOnly dynamic index buffers can be initialized with null data");

			hr = ((ID3D11Device*)device->m_device)->CreateBuffer(&desc, NULL, &buffer);
			if (FAILED(hr))
			{
				std::stringstream ss;
				ss << "Failed to create D3D11IndexBuffer:\nFailed to create buffer:" << std::endl;
				ss << "Error: " << _com_error(hr).ErrorMessage();
				throw RenderDeviceError(ss.str());
			}
		}
		else
		{
			D3D11_SUBRESOURCE_DATA initData;
			initData.pSysMem = data;
			initData.SysMemPitch = 0;
			initData.SysMemSlicePitch = 0;

			hr = ((ID3D11Device*)device->m_device)->CreateBuffer(&desc, &initData, &buffer);
			if (FAILED(hr))
			{
				std::stringstream ss;
				ss << "Failed to create D3D11IndexBuffer:\nFailed to create buffer:" << std::endl;
				ss << "Error: " << _com_error(hr).ErrorMessage();
				throw RenderDeviceError(ss.str());
			}
		}
	}

	virtual ~D3D11IndexBuffer() final
	{
		buffer->Release();
	}

	virtual void* Map() final
	{
		D3D11_MAPPED_SUBRESOURCE map;
		HRESULT hr = ((ID3D11DeviceContext*)device->m_deviceContext)->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
		if (FAILED(hr))
		{
			std::stringstream ss;
			ss << "Failed to map D3D11IndexBuffer:\nFailed to map buffer:" << std::endl;
			ss << "Error: " << _com_error(hr).ErrorMessage();
			throw RenderDeviceError(ss.str());
		}
		return map.pData;
	}

	virtual void Unmap() final
	{
		((ID3D11DeviceContext*)device->m_deviceContext)->Unmap(buffer, 0);
	}

	D3D11RenderDevice* device;
	ID3D11Buffer* buffer;
	DXGI_FORMAT format;
};

class D3D11RasterState final : public RasterState
{
public:
	D3D11RasterState(D3D11RenderDevice* device, const RasterStateDesc& _desc)
	{
		this->device = device;

		D3D11_RASTERIZER_DESC desc;

		desc.DepthBias = 0.0f;
		desc.SlopeScaledDepthBias = 0.0f;
		desc.DepthBiasClamp = 0.0f;
		desc.DepthClipEnable = true;
		desc.ScissorEnable = false;
		desc.MultisampleEnable = false;
		desc.AntialiasedLineEnable = false;

		if (_desc.cullEnabled)
		{
			switch (_desc.cullFace)
			{
				case Face::Front: desc.CullMode = D3D11_CULL_FRONT; break;
				case Face::Back: desc.CullMode = D3D11_CULL_BACK; break;
				case Face::Invalid: throw RenderDeviceError("Failed to create D3D11RasterState:\nInvalid cull face"); break;
				default: throw RenderDeviceError("Failed to create D3D11RasterState:\nUnsupported cull face"); break;
			}
		}
		else desc.CullMode = D3D11_CULL_NONE;
		
		switch (_desc.frontFace)
		{
			case Winding::CW: desc.FrontCounterClockwise = false; break;
			case Winding::CCW: desc.FrontCounterClockwise = true; break;
			case Winding::Invalid: throw RenderDeviceError("Failed to create D3D11RasterState:\nInvalid front face winding"); break;
			default: throw RenderDeviceError("Failed to create D3D11RasterState:\nUnsupported front face winding"); break;
		}

		switch (_desc.rasterMode)
		{
			case RasterMode::Wireframe: desc.FillMode = D3D11_FILL_WIREFRAME; break;
			case RasterMode::Fill: desc.FillMode = D3D11_FILL_SOLID; break;
			case RasterMode::Invalid: throw RenderDeviceError("Failed to create D3D11RasterState:\nInvalid raster mode"); break;
			default: throw RenderDeviceError("Failed to create D3D11RasterState:\nUnsupported raster mode"); break;
		}

		HRESULT hr = ((ID3D11Device*)device->m_device)->CreateRasterizerState(&desc, &state);
		if (FAILED(hr))
		{
			std::stringstream ss;
			ss << "Failed to create D3D11RasterState:\nFailed to create rasterizer state:" << std::endl;
			ss << "Error: " << _com_error(hr).ErrorMessage();
			throw RenderDeviceError(ss.str());
		}
	}

	virtual ~D3D11RasterState() final
	{
		state->Release();
	}

	D3D11RenderDevice* device;
	ID3D11RasterizerState* state;
};

class D3D11DepthStencilState final : public DepthStencilState
{
public:
	D3D11DepthStencilState(D3D11RenderDevice* device, const DepthStencilStateDesc& _desc)
	{
		D3D11_DEPTH_STENCIL_DESC desc;

		// Depth
		desc.DepthEnable = _desc.depthEnabled;
		desc.DepthWriteMask = _desc.depthWriteEnabled ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
		switch (_desc.depthCompare)
		{
			case Compare::Never: desc.DepthFunc = D3D11_COMPARISON_NEVER; break;
			case Compare::Less: desc.DepthFunc = D3D11_COMPARISON_LESS; break;
			case Compare::LEqual: desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL; break;
			case Compare::Greater: desc.DepthFunc = D3D11_COMPARISON_GREATER; break;
			case Compare::GEqual: desc.DepthFunc = D3D11_COMPARISON_GREATER_EQUAL; break;
			case Compare::Equal: desc.DepthFunc = D3D11_COMPARISON_EQUAL; break;
			case Compare::NotEqual: desc.DepthFunc = D3D11_COMPARISON_NOT_EQUAL; break;
			case Compare::Always: desc.DepthFunc = D3D11_COMPARISON_ALWAYS; break;
			case Compare::Invalid: throw RenderDeviceError("Failed to create D3D11DepthStencilState:\nInvalid depth compare function"); break;
			default: throw RenderDeviceError("Failed to create D3D11DepthStencilState:\nUnsupported depth compare function"); break;
		}

		// Stencil
		desc.StencilEnable = _desc.stencilEnabled;
		desc.StencilReadMask = _desc.stencilReadMask;
		desc.StencilWriteMask = _desc.stencilWriteMask;
		this->stencilRef = _desc.stencilRef;

		// Stencil front face
		switch (_desc.frontFaceDepthFail)
		{
			case StencilAction::Keep: desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP; break;
			case StencilAction::Zero: desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_ZERO; break;
			case StencilAction::Replace: desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_REPLACE; break;
			case StencilAction::Increment: desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR; break;
			case StencilAction::IncrementWrap: desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR_SAT; break;
			case StencilAction::Decrement: desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR; break;
			case StencilAction::DecrementWrap: desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR_SAT; break;
			case StencilAction::Invert: desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INVERT; break;
			case StencilAction::Invalid: throw RenderDeviceError("Failed to create D3D11DepthStencilState:\nInvalid front face depth stencil fail action"); break;
			default: throw RenderDeviceError("Failed to create D3D11DepthStencilState:\nUnsupported front face depth stencil fail action"); break;
		}

		switch (_desc.frontFaceStencilFail)
		{
			case StencilAction::Keep: desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP; break;
			case StencilAction::Zero: desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_ZERO; break;
			case StencilAction::Replace: desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_REPLACE; break;
			case StencilAction::Increment: desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_INCR; break;
			case StencilAction::IncrementWrap: desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_INCR_SAT; break;
			case StencilAction::Decrement: desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_DECR; break;
			case StencilAction::DecrementWrap: desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_DECR_SAT; break;
			case StencilAction::Invert: desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_INVERT; break;
			case StencilAction::Invalid: throw RenderDeviceError("Failed to create D3D11DepthStencilState:\nInvalid front face stencil fail action"); break;
			default: throw RenderDeviceError("Failed to create D3D11DepthStencilState:\nUnsupported front face stencil fail action"); break;
		}

		switch (_desc.frontFaceStencilPass)
		{
			case StencilAction::Keep: desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP; break;
			case StencilAction::Zero: desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_ZERO; break;
			case StencilAction::Replace: desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE; break;
			case StencilAction::Increment: desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR; break;
			case StencilAction::IncrementWrap: desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR_SAT; break;
			case StencilAction::Decrement: desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_DECR; break;
			case StencilAction::DecrementWrap: desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_DECR_SAT; break;
			case StencilAction::Invert: desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INVERT; break;
			case StencilAction::Invalid: throw RenderDeviceError("Failed to create D3D11DepthStencilState:\nInvalid front face stencil pass action"); break;
			default: throw RenderDeviceError("Failed to create D3D11DepthStencilState:\nUnsupported front face stencil pass action"); break;
		}

		switch (_desc.frontFaceStencilCompare)
		{
			case Compare::Never: desc.FrontFace.StencilFunc = D3D11_COMPARISON_NEVER; break;
			case Compare::Less: desc.FrontFace.StencilFunc = D3D11_COMPARISON_LESS; break;
			case Compare::LEqual: desc.FrontFace.StencilFunc = D3D11_COMPARISON_LESS_EQUAL; break;
			case Compare::Greater: desc.FrontFace.StencilFunc = D3D11_COMPARISON_GREATER; break;
			case Compare::GEqual: desc.FrontFace.StencilFunc = D3D11_COMPARISON_GREATER_EQUAL; break;
			case Compare::Equal: desc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL; break;
			case Compare::NotEqual: desc.FrontFace.StencilFunc = D3D11_COMPARISON_NOT_EQUAL; break;
			case Compare::Always: desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS; break;
			case Compare::Invalid: throw RenderDeviceError("Failed to create D3D11DepthStencilState:\nInvalid front face stencil compare function"); break;
			default: throw RenderDeviceError("Failed to create D3D11DepthStencilState:\nUnsupported front facil stencil compare function"); break;
		}

		// Stencil back face
		switch (_desc.backFaceDepthFail)
		{
			case StencilAction::Keep: desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP; break;
			case StencilAction::Zero: desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_ZERO; break;
			case StencilAction::Replace: desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_REPLACE; break;
			case StencilAction::Increment: desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR; break;
			case StencilAction::IncrementWrap: desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR_SAT; break;
			case StencilAction::Decrement: desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR; break;
			case StencilAction::DecrementWrap: desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR_SAT; break;
			case StencilAction::Invert: desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_INVERT; break;
			case StencilAction::Invalid: throw RenderDeviceError("Failed to create D3D11DepthStencilState:\nInvalid back face depth stencil fail action"); break;
			default: throw RenderDeviceError("Failed to create D3D11DepthStencilState:\nUnsupported front back depth stencil fail action"); break;
		}

		switch (_desc.backFaceStencilFail)
		{
			case StencilAction::Keep: desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP; break;
			case StencilAction::Zero: desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_ZERO; break;
			case StencilAction::Replace: desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_REPLACE; break;
			case StencilAction::Increment: desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_INCR; break;
			case StencilAction::IncrementWrap: desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_INCR_SAT; break;
			case StencilAction::Decrement: desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_DECR; break;
			case StencilAction::DecrementWrap: desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_DECR_SAT; break;
			case StencilAction::Invert: desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_INVERT; break;
			case StencilAction::Invalid: throw RenderDeviceError("Failed to create D3D11DepthStencilState:\nInvalid back face stencil fail action"); break;
			default: throw RenderDeviceError("Failed to create D3D11DepthStencilState:\nUnsupported back face stencil fail action"); break;
		}

		switch (_desc.backFaceStencilPass)
		{
			case StencilAction::Keep: desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP; break;
			case StencilAction::Zero: desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_ZERO; break;
			case StencilAction::Replace: desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE; break;
			case StencilAction::Increment: desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_INCR; break;
			case StencilAction::IncrementWrap: desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_INCR_SAT; break;
			case StencilAction::Decrement: desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_DECR; break;
			case StencilAction::DecrementWrap: desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_DECR_SAT; break;
			case StencilAction::Invert: desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_INVERT; break;
			case StencilAction::Invalid: throw RenderDeviceError("Failed to create D3D11DepthStencilState:\nInvalid back face stencil pass action"); break;
			default: throw RenderDeviceError("Failed to create D3D11DepthStencilState:\nUnsupported back face stencil pass action"); break;
		}

		switch (_desc.backFaceStencilCompare)
		{
			case Compare::Never: desc.BackFace.StencilFunc = D3D11_COMPARISON_NEVER; break;
			case Compare::Less: desc.BackFace.StencilFunc = D3D11_COMPARISON_LESS; break;
			case Compare::LEqual: desc.BackFace.StencilFunc = D3D11_COMPARISON_LESS_EQUAL; break;
			case Compare::Greater: desc.BackFace.StencilFunc = D3D11_COMPARISON_GREATER; break;
			case Compare::GEqual: desc.BackFace.StencilFunc = D3D11_COMPARISON_GREATER_EQUAL; break;
			case Compare::Equal: desc.BackFace.StencilFunc = D3D11_COMPARISON_EQUAL; break;
			case Compare::NotEqual: desc.BackFace.StencilFunc = D3D11_COMPARISON_NOT_EQUAL; break;
			case Compare::Always: desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS; break;
			case Compare::Invalid: throw RenderDeviceError("Failed to create D3D11DepthStencilState:\nInvalid back face stencil compare function"); break;
			default: throw RenderDeviceError("Failed to create D3D11DepthStencilState:\nUnsupported back facil stencil compare function"); break;
		}

		// Create state
		HRESULT hr = ((ID3D11Device*)device->m_device)->CreateDepthStencilState(&desc, &state);
		if (FAILED(hr))
		{
			std::stringstream ss;
			ss << "Failed to create D3D11DepthStencilState:\nFailed to create depth stencil state:" << std::endl;
			ss << "Error: " << _com_error(hr).ErrorMessage();
			throw RenderDeviceError(ss.str());
		}
	}

	virtual ~D3D11DepthStencilState() final
	{
		state->Release();
	}

	D3D11RenderDevice* device;
	ID3D11DepthStencilState* state;
	unsigned int stencilRef;
};

class D3D11BlendState final : public BlendState
{
public:
	D3D11BlendState(D3D11RenderDevice* device, const BlendStateDesc& _desc)
	{
		D3D11_BLEND_DESC desc;

		desc.AlphaToCoverageEnable = false;
		desc.IndependentBlendEnable = false;

		desc.RenderTarget[0].BlendEnable = _desc.blendEnabled;
		
		switch (_desc.blendOperation)
		{
			case BlendOperation::Add: desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD; break;
			case BlendOperation::Subtract: desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_SUBTRACT; break;
			case BlendOperation::ReverseSubtract: desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_REV_SUBTRACT; break;
			case BlendOperation::Min: desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_MIN; break;
			case BlendOperation::Max: desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_MAX; break;
			case BlendOperation::Invalid: throw RenderDeviceError("Failed to create D3D11BlendState:\nInvalid blend operation function"); break;
			default: throw RenderDeviceError("Failed to create D3D11BlendState:\nUnsupported blend operation function"); break;
		}

		switch (_desc.sourceFactor)
		{
			case BlendFactor::Zero: desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ZERO; break;
			case BlendFactor::One: desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE; break;

			case BlendFactor::SourceColor: desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_COLOR; break;
			case BlendFactor::InverseSourceColor: desc.RenderTarget[0].SrcBlend = D3D11_BLEND_INV_SRC_COLOR; break;
			case BlendFactor::DestinationColor: desc.RenderTarget[0].SrcBlend = D3D11_BLEND_DEST_COLOR; break;
			case BlendFactor::InverseDestinationColor: desc.RenderTarget[0].SrcBlend = D3D11_BLEND_INV_DEST_COLOR; break;

			case BlendFactor::SourceAlpha: desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA; break;
			case BlendFactor::InverseSourceAlpha: desc.RenderTarget[0].SrcBlend = D3D11_BLEND_INV_SRC_ALPHA; break;
			case BlendFactor::DestinationAlpha: desc.RenderTarget[0].SrcBlend = D3D11_BLEND_DEST_ALPHA; break;
			case BlendFactor::InverseDestinationAlpha: desc.RenderTarget[0].SrcBlend = D3D11_BLEND_INV_DEST_ALPHA; break;

			case BlendFactor::Invalid: throw RenderDeviceError("Failed to create D3D11BlendState:\nInvalid source factor"); break;
			default: throw RenderDeviceError("Failed to create D3D11BlendState:\nUnsupported source factor"); break;
		}

		switch (_desc.destinationFactor)
		{
			case BlendFactor::Zero: desc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO; break;
			case BlendFactor::One: desc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE; break;

			case BlendFactor::SourceColor: desc.RenderTarget[0].DestBlend = D3D11_BLEND_SRC_COLOR; break;
			case BlendFactor::InverseSourceColor: desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_COLOR; break;
			case BlendFactor::DestinationColor: desc.RenderTarget[0].DestBlend = D3D11_BLEND_DEST_COLOR; break;
			case BlendFactor::InverseDestinationColor: desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_DEST_COLOR; break;

			case BlendFactor::SourceAlpha: desc.RenderTarget[0].DestBlend = D3D11_BLEND_SRC_ALPHA; break;
			case BlendFactor::InverseSourceAlpha: desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA; break;
			case BlendFactor::DestinationAlpha: desc.RenderTarget[0].DestBlend = D3D11_BLEND_DEST_ALPHA; break;
			case BlendFactor::InverseDestinationAlpha: desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_DEST_ALPHA; break;

			case BlendFactor::Invalid: throw RenderDeviceError("Failed to create D3D11BlendState:\nInvalid destination factor"); break;
			default: throw RenderDeviceError("Failed to create D3D11BlendState:\nUnsupported destination factor"); break;
		}

		switch (_desc.alphaBlendOperation)
		{
			case BlendOperation::Add: desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD; break;
			case BlendOperation::Subtract: desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_SUBTRACT; break;
			case BlendOperation::ReverseSubtract: desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_REV_SUBTRACT; break;
			case BlendOperation::Min: desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_MIN; break;
			case BlendOperation::Max: desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_MAX; break;
			case BlendOperation::Invalid: throw RenderDeviceError("Failed to create D3D11BlendState:\nInvalid alpha blend operation function"); break;
			default: throw RenderDeviceError("Failed to create D3D11BlendState:\nUnsupported alpha blend operation function"); break;
		}

		switch (_desc.sourceAlphaFactor)
		{
			case BlendFactor::Zero: desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO; break;
			case BlendFactor::One: desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE; break;
			
			case BlendFactor::SourceColor: desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_COLOR; break;
			case BlendFactor::InverseSourceColor: desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_INV_SRC_COLOR; break;
			case BlendFactor::DestinationColor: desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_DEST_COLOR; break;
			case BlendFactor::InverseDestinationColor: desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_INV_DEST_COLOR; break;

			case BlendFactor::SourceAlpha: desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA; break;
			case BlendFactor::InverseSourceAlpha: desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA; break;
			case BlendFactor::DestinationAlpha: desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_DEST_ALPHA; break;
			case BlendFactor::InverseDestinationAlpha: desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_INV_DEST_ALPHA; break;
			
			case BlendFactor::Invalid: throw RenderDeviceError("Failed to create D3D11BlendState:\nInvalid source alpha factor"); break;
			default: throw RenderDeviceError("Failed to create D3D11BlendState:\nUnsupported source alpha factor"); break;
		}

		switch (_desc.destinationAlphaFactor)
		{
			case BlendFactor::Zero: desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO; break;
			case BlendFactor::One: desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE; break;

			case BlendFactor::SourceColor: desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_SRC_COLOR; break;
			case BlendFactor::InverseSourceColor: desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_COLOR; break;
			case BlendFactor::DestinationColor: desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_DEST_COLOR; break;
			case BlendFactor::InverseDestinationColor: desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_DEST_COLOR; break;

			case BlendFactor::SourceAlpha: desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_SRC_ALPHA; break;
			case BlendFactor::InverseSourceAlpha: desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA; break;
			case BlendFactor::DestinationAlpha: desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_DEST_ALPHA; break;
			case BlendFactor::InverseDestinationAlpha: desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_DEST_ALPHA; break;

			case BlendFactor::Invalid: throw RenderDeviceError("Failed to create D3D11BlendState:\nInvalid destination alpha factor"); break;
			default: throw RenderDeviceError("Failed to create D3D11BlendState:\nUnsupported destination alpha factor"); break;
		}	

		desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		
		// Create state
		HRESULT hr = ((ID3D11Device*)device->m_device)->CreateBlendState(&desc, &state);
		if (FAILED(hr))
		{
			std::stringstream ss;
			ss << "Failed to create D3D11BlendState:\nFailed to create blend state:" << std::endl;
			ss << "Error: " << _com_error(hr).ErrorMessage();
			throw RenderDeviceError(ss.str());
		}
	}

	virtual ~D3D11BlendState() final
	{
		state->Release();
	}

	D3D11RenderDevice* device;
	ID3D11BlendState* state;
};

void Magma::Framework::Graphics::D3D11RenderDevice::Init(Input::Window * window, const RenderDeviceSettings & settings)
{
#if defined(MAGMA_FRAMEWORK_USE_DIRECTX)
	m_settings = settings;
	m_window = dynamic_cast<Input::D3DWindow*>(window);
	if (m_window == nullptr)
		throw RenderDeviceError("Failed to init D3D11RenderDevice:\nCouldn't cast from Magma::Framework::Input::Window* to Magma::Framework::Input::D3DWindow*");

	HRESULT hr;
	IDXGISwapChain* swapChain = nullptr;
	ID3D11Device* device = nullptr;
	ID3D11DeviceContext* deviceContext = nullptr;

	// Create device and swap chain
	{
		DXGI_SWAP_CHAIN_DESC scd;
		ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

		scd.BufferCount = 1;
		scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		scd.OutputWindow = (HWND)m_window->GetHWND();
		scd.SampleDesc.Count = 1;
		scd.SampleDesc.Quality = 0;
		scd.Windowed = (window->GetMode() == Input::Window::Mode::Fullscreen) ? FALSE : TRUE;

		UINT flags = 0;

#ifdef MAGMA_FRAMEWORK_RENDER_DEVICE_DEBUG
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
				&scd,
				&swapChain,
				&device,
				NULL,
				&deviceContext);
		if (FAILED(hr))
		{
			std::stringstream ss;
			ss << "Failed to init D3D11RenderDevice:\nCouldn't create device and swap chain:\nError: " << _com_error(hr).ErrorMessage();
			throw RenderDeviceError(ss.str());
		}
	}
	
	// Get back buffer
	ID3D11Texture2D* backBuffer;
	{
		hr = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
		if (FAILED(hr))
		{
			swapChain->Release();
			device->Release();
			deviceContext->Release();
			std::stringstream ss;
			ss << "Failed to init D3D11RenderDevice:\nFailed to get back buffer:\nError: " << _com_error(hr).ErrorMessage();
			throw RenderDeviceError(ss.str());
		}
	}

	// Create render target view
	ID3D11RenderTargetView* renderTargetView;
	{
		hr = device->CreateRenderTargetView(backBuffer, NULL, &renderTargetView);
		backBuffer->Release();
		if (FAILED(hr))
		{
			swapChain->Release();
			device->Release();
			deviceContext->Release();
			std::stringstream ss;
			ss << "Failed to init D3D11RenderDevice:\nFailed to create render target view:\nError: " << _com_error(hr).ErrorMessage();
			throw RenderDeviceError(ss.str());
		}
	}

	// Create depth stencil buffer
	ID3D11Texture2D* depthStencilBuffer;
	{
		D3D11_TEXTURE2D_DESC depthStencilDesc;
		ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
		depthStencilDesc.Width = m_window->GetWidth();
		depthStencilDesc.Height = m_window->GetHeight();
		depthStencilDesc.MipLevels = 1;
		depthStencilDesc.ArraySize = 1;
		depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
		depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
		depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthStencilDesc.CPUAccessFlags = 0;
		depthStencilDesc.MiscFlags = 0;

		hr = device->CreateTexture2D(&depthStencilDesc, NULL, &depthStencilBuffer);
		if (FAILED(hr))
		{
			swapChain->Release();
			device->Release();
			deviceContext->Release();
			renderTargetView->Release();
			std::stringstream ss;
			ss << "Failed to init D3D11Context:\nFailed to create depth stencil texture:\nError: " << _com_error(hr).ErrorMessage();
			throw RenderDeviceError(ss.str());
		}
	}

	// Create depth stencil view
	ID3D11DepthStencilView* depthStencilView;
	{
		hr = device->CreateDepthStencilView(depthStencilBuffer, NULL, &depthStencilView);
		if (FAILED(hr))
		{
			depthStencilBuffer->Release();
			swapChain->Release();
			device->Release();
			deviceContext->Release();
			renderTargetView->Release();
			std::stringstream ss;
			ss << "Failed to init D3D11Context:\nFailed to create depth stencil view:\nError: " << _com_error(hr).ErrorMessage();
			throw RenderDeviceError(ss.str());
		}
	}

	// Set default render target
	deviceContext->OMSetRenderTargets(1, &renderTargetView, depthStencilView);

	// Set viewport
	{
		D3D11_VIEWPORT viewport;
		ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = m_window->GetWidth();
		viewport.Height = m_window->GetHeight();
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;

		deviceContext->RSSetViewports(1, &viewport);
	}

	// Set rasterizer state
	ID3D11RasterizerState* rasterState;
	{
		D3D11_RASTERIZER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));

		desc.FillMode = D3D11_FILL_SOLID;
		desc.CullMode = D3D11_CULL_BACK;
		desc.FrontCounterClockwise = TRUE;
		desc.MultisampleEnable = TRUE;
		desc.AntialiasedLineEnable = TRUE;

		hr = device->CreateRasterizerState(&desc, &rasterState);
		if (FAILED(hr))
		{
			depthStencilView->Release();
			depthStencilBuffer->Release();
			swapChain->Release();
			device->Release();
			deviceContext->Release();
			renderTargetView->Release();
			std::stringstream ss;
			ss << "Failed to create init D3D11Context:\nFailed to create rasterizer state:\nError: " << hr;
			throw RenderDeviceError(ss.str());
		}
		deviceContext->RSSetState(rasterState);
	}

	// Store Direct3D stuff on void pointers
	m_swapChain = swapChain;
	m_device = device;
	m_deviceContext = deviceContext;
	m_defaultRenderTargetView = renderTargetView;
	m_renderTargetView = renderTargetView;
	m_depthStencilBuffer = depthStencilBuffer;
	m_defaultDepthStencilView = depthStencilView;
	m_depthStencilView = depthStencilView;
	m_rasterState = rasterState;

#else
	throw RenderDeviceError("Failed to call function on D3D11RenderDevice:\nMAGMA_FRAMEWORK_USE_DIRECTX must be defined");
#endif
}

void Magma::Framework::Graphics::D3D11RenderDevice::Terminate()
{
#if defined(MAGMA_FRAMEWORK_USE_DIRECTX)
	
#else
	throw RenderDeviceError("Failed to call function on D3D11RenderDevice:\nMAGMA_FRAMEWORK_USE_DIRECTX must be defined");
#endif
}

VertexShader * Magma::Framework::Graphics::D3D11RenderDevice::CreateVertexShader(const ShaderData & data)
{
#if defined(MAGMA_FRAMEWORK_USE_DIRECTX)
	return new D3D11VertexShader(this, data);
#else
	throw RenderDeviceError("Failed to call function on D3D11RenderDevice:\nMAGMA_FRAMEWORK_USE_DIRECTX must be defined");
#endif
}

void Magma::Framework::Graphics::D3D11RenderDevice::DestroyVertexShader(VertexShader * vertexShader)
{
#if defined(MAGMA_FRAMEWORK_USE_DIRECTX)
	delete vertexShader;
#else
	throw RenderDeviceError("Failed to call function on D3D11RenderDevice:\nMAGMA_FRAMEWORK_USE_DIRECTX must be defined");
#endif
}

PixelShader * Magma::Framework::Graphics::D3D11RenderDevice::CreatePixelShader(const ShaderData & data)
{
#if defined(MAGMA_FRAMEWORK_USE_DIRECTX)
	return new D3D11PixelShader(this, data);
#else
	throw RenderDeviceError("Failed to call function on D3D11RenderDevice:\nMAGMA_FRAMEWORK_USE_DIRECTX must be defined");
#endif
}

void Magma::Framework::Graphics::D3D11RenderDevice::DestroyPixelShader(PixelShader * pixelShader)
{
#if defined(MAGMA_FRAMEWORK_USE_DIRECTX)
	delete pixelShader;
#else
	throw RenderDeviceError("Failed to call function on D3D11RenderDevice:\nMAGMA_FRAMEWORK_USE_DIRECTX must be defined");
#endif
}

Pipeline * Magma::Framework::Graphics::D3D11RenderDevice::CreatePipeline(VertexShader * vertexShader, PixelShader * pixelShader)
{
#if defined(MAGMA_FRAMEWORK_USE_DIRECTX)
	return new D3D11Pipeline((D3D11VertexShader*)vertexShader, (D3D11PixelShader*)pixelShader);
#else
	throw RenderDeviceError("Failed to call function on D3D11RenderDevice:\nMAGMA_FRAMEWORK_USE_DIRECTX must be defined");
#endif
}

void Magma::Framework::Graphics::D3D11RenderDevice::DestroyPipeline(Pipeline * pipeline)
{
#if defined(MAGMA_FRAMEWORK_USE_DIRECTX)
	delete pipeline;
#else
	throw RenderDeviceError("Failed to call function on D3D11RenderDevice:\nMAGMA_FRAMEWORK_USE_DIRECTX must be defined");
#endif
}

void Magma::Framework::Graphics::D3D11RenderDevice::SetPipeline(Pipeline * pipeline)
{
#if defined(MAGMA_FRAMEWORK_USE_DIRECTX)
	((ID3D11DeviceContext*)m_deviceContext)->VSSetShader(static_cast<D3D11Pipeline*>(pipeline)->vertexShader->vs, NULL, 0);
	((ID3D11DeviceContext*)m_deviceContext)->PSSetShader(static_cast<D3D11Pipeline*>(pipeline)->pixelShader->ps, NULL, 0);
#else
	throw RenderDeviceError("Failed to call function on D3D11RenderDevice:\nMAGMA_FRAMEWORK_USE_DIRECTX must be defined");
#endif
}

VertexBuffer * Magma::Framework::Graphics::D3D11RenderDevice::CreateVertexBuffer(size_t size, const void * data, BufferUsage usage)
{
#if defined(MAGMA_FRAMEWORK_USE_DIRECTX)
	return new D3D11VertexBuffer(this, size, data, usage);
#else
	throw RenderDeviceError("Failed to call function on D3D11RenderDevice:\nMAGMA_FRAMEWORK_USE_DIRECTX must be defined");
#endif
}

void Magma::Framework::Graphics::D3D11RenderDevice::DestroyVertexBuffer(VertexBuffer * vertexBuffer)
{
#if defined(MAGMA_FRAMEWORK_USE_DIRECTX)
	delete vertexBuffer;
#else
	throw RenderDeviceError("Failed to call function on D3D11RenderDevice:\nMAGMA_FRAMEWORK_USE_DIRECTX must be defined");
#endif
}

VertexLayout * Magma::Framework::Graphics::D3D11RenderDevice::CreateVertexLayout(size_t elementCount, const VertexElement * elements, VertexShader * vertexShader)
{
#if defined(MAGMA_FRAMEWORK_USE_DIRECTX)
	return new D3D11VertexLayout(this, elementCount, elements, (D3D11VertexShader*)vertexShader);
#else
	throw RenderDeviceError("Failed to call function on D3D11RenderDevice:\nMAGMA_FRAMEWORK_USE_DIRECTX must be defined");
#endif
}

void Magma::Framework::Graphics::D3D11RenderDevice::DestroyVertexLayout(VertexLayout * vertexLayout)
{
#if defined(MAGMA_FRAMEWORK_USE_DIRECTX)
	delete vertexLayout;
#else
	throw RenderDeviceError("Failed to call function on D3D11RenderDevice:\nMAGMA_FRAMEWORK_USE_DIRECTX must be defined");
#endif
}

VertexArray * Magma::Framework::Graphics::D3D11RenderDevice::CreateVertexArray(size_t bufferCount, VertexBuffer ** buffers, VertexLayout * layout)
{
#if defined(MAGMA_FRAMEWORK_USE_DIRECTX)
	return new D3D11VertexArray(this, bufferCount, (D3D11VertexBuffer**)buffers, (D3D11VertexLayout*)layout);
#else
	throw RenderDeviceError("Failed to call function on D3D11RenderDevice:\nMAGMA_FRAMEWORK_USE_DIRECTX must be defined");
#endif
}

void Magma::Framework::Graphics::D3D11RenderDevice::DestroyVertexArray(VertexArray * vertexArray)
{
#if defined(MAGMA_FRAMEWORK_USE_DIRECTX)
	delete vertexArray;
#else
	throw RenderDeviceError("Failed to call function on D3D11RenderDevice:\nMAGMA_FRAMEWORK_USE_DIRECTX must be defined");
#endif
}

void Magma::Framework::Graphics::D3D11RenderDevice::SetVertexArray(VertexArray * vertexArray)
{
#if defined(MAGMA_FRAMEWORK_USE_DIRECTX)
	auto va = static_cast<D3D11VertexArray*>(vertexArray);
	auto l = va->layout;

	((ID3D11DeviceContext*)m_deviceContext)->IASetInputLayout(l->inputLayout);
	((ID3D11DeviceContext*)m_deviceContext)->IASetVertexBuffers(0, va->bufferCount, va->buffers, l->strides, l->offsets);
#else
	throw RenderDeviceError("Failed to call function on D3D11RenderDevice:\nMAGMA_FRAMEWORK_USE_DIRECTX must be defined");
#endif
}

IndexBuffer * Magma::Framework::Graphics::D3D11RenderDevice::CreateIndexBuffer(IndexType type, size_t size, const void * data, BufferUsage usage)
{
#if defined(MAGMA_FRAMEWORK_USE_DIRECTX)
	return new D3D11IndexBuffer(this, type, size, data, usage);
#else
	throw RenderDeviceError("Failed to call function on D3D11RenderDevice:\nMAGMA_FRAMEWORK_USE_DIRECTX must be defined");
#endif
}

void Magma::Framework::Graphics::D3D11RenderDevice::DestroyIndexBuffer(IndexBuffer * indexBuffer)
{
#if defined(MAGMA_FRAMEWORK_USE_DIRECTX)
	delete indexBuffer;
#else
	throw RenderDeviceError("Failed to call function on D3D11RenderDevice:\nMAGMA_FRAMEWORK_USE_DIRECTX must be defined");
#endif
}

void Magma::Framework::Graphics::D3D11RenderDevice::SetIndexBuffer(IndexBuffer * indexBuffer)
{
#if defined(MAGMA_FRAMEWORK_USE_DIRECTX)
	auto b = static_cast<D3D11IndexBuffer*>(indexBuffer);
	((ID3D11DeviceContext*)m_deviceContext)->IASetIndexBuffer(b->buffer, b->format, 0);
#else
	throw RenderDeviceError("Failed to call function on D3D11RenderDevice:\nMAGMA_FRAMEWORK_USE_DIRECTX must be defined");
#endif
}

Texture2D * Magma::Framework::Graphics::D3D11RenderDevice::CreateTexture2D(size_t width, size_t height, TextureFormat format, const void * data, BufferUsage usage, bool isRenderTarget)
{
#if defined(MAGMA_FRAMEWORK_USE_DIRECTX)
	return new D3D11Texture2D(this, width, height, format, data, usage, isRenderTarget);
#else
	throw RenderDeviceError("Failed to call function on D3D11RenderDevice:\nMAGMA_FRAMEWORK_USE_DIRECTX must be defined");
#endif
}

void Magma::Framework::Graphics::D3D11RenderDevice::DestroyTexture2D(Texture2D * texture)
{
#if defined(MAGMA_FRAMEWORK_USE_DIRECTX)
	delete texture;
#else
	throw RenderDeviceError("Failed to call function on D3D11RenderDevice:\nMAGMA_FRAMEWORK_USE_DIRECTX must be defined");
#endif
}

Sampler2D * Magma::Framework::Graphics::D3D11RenderDevice::CreateSampler2D(const Sampler2DDesc & desc)
{
#if defined(MAGMA_FRAMEWORK_USE_DIRECTX)
	return new D3D11Sampler2D(this, desc);
#else
	throw RenderDeviceError("Failed to call function on D3D11RenderDevice:\nMAGMA_FRAMEWORK_USE_DIRECTX must be defined");
#endif
}

void Magma::Framework::Graphics::D3D11RenderDevice::DestroySampler2D(Sampler2D * sampler)
{
#if defined(MAGMA_FRAMEWORK_USE_DIRECTX)
	delete sampler;
#else
	throw RenderDeviceError("Failed to call function on D3D11RenderDevice:\nMAGMA_FRAMEWORK_USE_DIRECTX must be defined");
#endif
}

RasterState * Magma::Framework::Graphics::D3D11RenderDevice::CreateRasterState(const RasterStateDesc & desc)
{
#if defined(MAGMA_FRAMEWORK_USE_DIRECTX)
	return new D3D11RasterState(this, desc);
#else
	throw RenderDeviceError("Failed to call function on D3D11RenderDevice:\nMAGMA_FRAMEWORK_USE_DIRECTX must be defined");
#endif
}

void Magma::Framework::Graphics::D3D11RenderDevice::DestroyRasterState(RasterState * rasterState)
{
#if defined(MAGMA_FRAMEWORK_USE_DIRECTX)
	delete rasterState;
#else
	throw RenderDeviceError("Failed to call function on D3D11RenderDevice:\nMAGMA_FRAMEWORK_USE_DIRECTX must be defined");
#endif
}

void Magma::Framework::Graphics::D3D11RenderDevice::SetRasterState(RasterState * rasterState)
{
#if defined(MAGMA_FRAMEWORK_USE_DIRECTX)
	((ID3D11DeviceContext*)m_deviceContext)->RSSetState(static_cast<D3D11RasterState*>(rasterState)->state);
#else
	throw RenderDeviceError("Failed to call function on D3D11RenderDevice:\nMAGMA_FRAMEWORK_USE_DIRECTX must be defined");
#endif
}

DepthStencilState * Magma::Framework::Graphics::D3D11RenderDevice::CreateDepthStencilState(const DepthStencilStateDesc & desc)
{
#if defined(MAGMA_FRAMEWORK_USE_DIRECTX)
	return new D3D11DepthStencilState(this, desc);
#else
	throw RenderDeviceError("Failed to call function on D3D11RenderDevice:\nMAGMA_FRAMEWORK_USE_DIRECTX must be defined");
#endif
}

void Magma::Framework::Graphics::D3D11RenderDevice::DestroyDepthStencilState(DepthStencilState * depthStencilState)
{
#if defined(MAGMA_FRAMEWORK_USE_DIRECTX)
	delete depthStencilState;
#else
	throw RenderDeviceError("Failed to call function on D3D11RenderDevice:\nMAGMA_FRAMEWORK_USE_DIRECTX must be defined");
#endif
}

void Magma::Framework::Graphics::D3D11RenderDevice::SetDepthStencilState(DepthStencilState * depthStencilState)
{
#if defined(MAGMA_FRAMEWORK_USE_DIRECTX)
	((ID3D11DeviceContext*)m_deviceContext)->OMSetDepthStencilState(static_cast<D3D11DepthStencilState*>(depthStencilState)->state, static_cast<D3D11DepthStencilState*>(depthStencilState)->stencilRef);
#else
	throw RenderDeviceError("Failed to call function on D3D11RenderDevice:\nMAGMA_FRAMEWORK_USE_DIRECTX must be defined");
#endif
}

void Magma::Framework::Graphics::D3D11RenderDevice::Clear(glm::vec4 color, float depth, int stencil)
{
#if defined(MAGMA_FRAMEWORK_USE_DIRECTX)
	((ID3D11DeviceContext*)m_deviceContext)->ClearRenderTargetView((ID3D11RenderTargetView*)m_renderTargetView, &color[0]);
	((ID3D11DeviceContext*)m_deviceContext)->ClearDepthStencilView((ID3D11DepthStencilView*)m_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, depth, stencil);
#else
	throw RenderDeviceError("Failed to call function on D3D11RenderDevice:\nMAGMA_FRAMEWORK_USE_DIRECTX must be defined");
#endif
}

void Magma::Framework::Graphics::D3D11RenderDevice::DrawTriangles(size_t offset, size_t count)
{
#if defined(MAGMA_FRAMEWORK_USE_DIRECTX)
	((ID3D11DeviceContext*)m_deviceContext)->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	((ID3D11DeviceContext*)m_deviceContext)->Draw(offset, count);
#else
	throw RenderDeviceError("Failed to call function on D3D11RenderDevice:\nMAGMA_FRAMEWORK_USE_DIRECTX must be defined");
#endif
}

void Magma::Framework::Graphics::D3D11RenderDevice::DrawTrianglesIndexed(size_t offset, size_t count)
{
#if defined(MAGMA_FRAMEWORK_USE_DIRECTX)
	((ID3D11DeviceContext*)m_deviceContext)->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	((ID3D11DeviceContext*)m_deviceContext)->DrawIndexed(count, offset, 0);
#else
	throw RenderDeviceError("Failed to call function on D3D11RenderDevice:\nMAGMA_FRAMEWORK_USE_DIRECTX must be defined");
#endif
}

void Magma::Framework::Graphics::D3D11RenderDevice::SetRenderTargets(Texture2D ** textures, size_t count)
{
#if defined(MAGMA_FRAMEWORK_USE_DIRECTX)
	// TO DO
#else
	throw RenderDeviceError("Failed to call function on D3D11RenderDevice:\nMAGMA_FRAMEWORK_USE_DIRECTX must be defined");
#endif
}

void Magma::Framework::Graphics::D3D11RenderDevice::SwapBuffers()
{
#if defined(MAGMA_FRAMEWORK_USE_DIRECTX)
	((IDXGISwapChain*)m_swapChain)->Present(m_settings.enableVSync ? 1 : 0, 0);
#else
	throw RenderDeviceError("Failed to call function on D3D11RenderDevice:\nMAGMA_FRAMEWORK_USE_DIRECTX must be defined");
#endif
}

ConstantBuffer * Magma::Framework::Graphics::D3D11RenderDevice::CreateConstantBuffer(size_t size, const void * data, BufferUsage usage)
{
#if defined(MAGMA_FRAMEWORK_USE_DIRECTX)
	return new D3D11ConstantBuffer(this, size, data, usage);
#else
	throw RenderDeviceError("Failed to call function on D3D11RenderDevice:\nMAGMA_FRAMEWORK_USE_DIRECTX must be defined");
#endif
}

void Magma::Framework::Graphics::D3D11RenderDevice::DestroyConstantBuffer(ConstantBuffer * constantBuffer)
{
#if defined(MAGMA_FRAMEWORK_USE_DIRECTX)
	delete constantBuffer;
#else
	throw RenderDeviceError("Failed to call function on D3D11RenderDevice:\nMAGMA_FRAMEWORK_USE_DIRECTX must be defined");
#endif
}

BlendState * Magma::Framework::Graphics::D3D11RenderDevice::CreateBlendState(const BlendStateDesc & desc)
{
#if defined(MAGMA_FRAMEWORK_USE_DIRECTX)
	return new D3D11BlendState(this, desc);
#else
	throw RenderDeviceError("Failed to call function on D3D11RenderDevice:\nMAGMA_FRAMEWORK_USE_DIRECTX must be defined");
#endif
}

void Magma::Framework::Graphics::D3D11RenderDevice::DestroyBlendState(BlendState * blendState)
{
#if defined(MAGMA_FRAMEWORK_USE_DIRECTX)
	delete blendState;
#else
	throw RenderDeviceError("Failed to call function on D3D11RenderDevice:\nMAGMA_FRAMEWORK_USE_DIRECTX must be defined");
#endif
}

void Magma::Framework::Graphics::D3D11RenderDevice::SetBlendState(BlendState * blendState)
{
#if defined(MAGMA_FRAMEWORK_USE_DIRECTX)
	auto bs = static_cast<D3D11BlendState*>(blendState);
	float color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	((ID3D11DeviceContext*)m_deviceContext)->OMSetBlendState(bs->state, color, 0xFFFFFFFF);
#else
	throw RenderDeviceError("Failed to call function on D3D11RenderDevice:\nMAGMA_FRAMEWORK_USE_DIRECTX must be defined");
#endif
}
