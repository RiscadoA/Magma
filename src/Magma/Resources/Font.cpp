#include "Font.hpp"
#include "Manager.hpp"
#include "Exception.hpp"

Magma::Resources::Font::Font(
	Resource * rsc,
	Framework::Graphics_V1X::RenderDevice * device,
	const unsigned char * data,
	size_t size,
	size_t charWidth,
	size_t charHeight,
	size_t atlasWidth,
	size_t atlasHeight,
	size_t maxAtlasCount) :

	ResourceData(rsc),
	font(device, data, size, charWidth, charHeight, atlasWidth, atlasHeight, maxAtlasCount)
{

}

Magma::Resources::FontImporter::FontImporter(Manager* manager, Framework::Graphics_V1X::RenderDevice* device)
	: Importer("font", manager), m_pool(sizeof(Font), 256), m_device(device)
{
	
}

Magma::Resources::FontImporter::~FontImporter()
{

}

void Magma::Resources::FontImporter::Update()
{
	// Do nothing (not necessary here)
}

void Magma::Resources::FontImporter::Import(Resource * resource)
{
	auto path = resource->GetDataPath();

	// Check extension
	if (path.GetExtension() == "ttf" || path.GetExtension() == "ttc")
	{
		// Read font data from file
		auto fs = this->GetManager()->GetFileSystem();
		auto file = fs->OpenFile(Framework::Files::FileMode::Read, path);
		auto size = fs->GetSize(file);
		auto buff = new unsigned char[size];
		fs->Read(file, buff, size);
		fs->CloseFile(file);

		auto data = new (m_pool.Allocate(sizeof(Font))) Font(
			resource,
			m_device,
			buff,
			size,
			std::stoull(resource->GetParam("char-width")),
			std::stoull(resource->GetParam("char-height")),
			std::stoull(resource->GetParam("atlas-width")),
			std::stoull(resource->GetParam("atlas-height")),
			std::stoull(resource->GetParam("max-atlas-count")));
		resource->SetData(data);

		delete[] buff;
	}
	else
	{
		std::stringstream ss;
		ss << "Failed to import font resource on path '" << path.ToString() << "':" << std::endl;
		ss << "Unsupported extension '" << path.GetExtension() << "':" << std::endl;
		ss << "Supported extensions: 'ttf', 'ttc'";
		throw ImporterError(ss.str());
	}
}

void Magma::Resources::FontImporter::Destroy(Resource * resource)
{
	auto data = resource->GetData<Font>();
	data->~Font();
	m_pool.Deallocate(data);
	resource->SetData(nullptr);
}

Magma::Resources::ResourceMode Magma::Resources::FontImporter::GetMode(Resource * resource)
{
	auto mode = resource->GetParam("storage");

	if (mode == "permanent")
		return ResourceMode::GPUPermanent;
	else if (mode == "temporary")
		return ResourceMode::GPUTemporary;
	else
	{
		std::stringstream ss;
		ss << "Failed to get font resource on path '" << resource->GetDataPath().ToString() << "' storage mode:" << std::endl;
		ss << "Unsupported storage mode '" << mode << "':" << std::endl;
		ss << "Supported storage modes: 'permament', 'temporary'";
		throw ImporterError(ss.str());
	}
}
