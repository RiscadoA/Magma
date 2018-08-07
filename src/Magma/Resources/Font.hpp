#pragma once

#include "Importer.hpp"

#include <Magma/Framework/Graphics/RenderDevice.hpp>
#include <Magma/Framework/Memory/PoolAllocator.hpp>
#include <Magma/Framework/Graphics/Font.hpp>

namespace Magma
{
	namespace Resources
	{
		/// <summary>
		///		Shader resource data
		/// </summary>
		class Font : public ResourceData
		{
		public:
			Font(Resource* rsc, Framework::Graphics::RenderDevice* device, const unsigned char* data, size_t size, size_t charWidth, size_t charHeight, size_t atlasWidth, size_t atlasHeight, size_t maxAtlasCount);

			Framework::Graphics::Font font;
		};

		/// <summary>
		///		Used to import shader resources
		/// </summary>
		class FontImporter final : public Importer
		{
		public:
			FontImporter(Manager* manager, Framework::Graphics::RenderDevice* device);
			virtual ~FontImporter() final;

			// Inherited via Importer
			virtual void Update() final;
			virtual void Import(Resource * resource) final;
			virtual void Destroy(Resource * resource) final;
			virtual ResourceMode GetMode(Resource* resources) final;

		private:
			Framework::Graphics::RenderDevice* m_device;
			Framework::Memory::PoolAllocator m_pool;
		};
	}
}
