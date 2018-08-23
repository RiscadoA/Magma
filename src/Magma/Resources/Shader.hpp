#pragma once

#include "Importer.hpp"

#include <Magma/Framework/Graphics/1.X/RenderDevice.hpp>
#include <Magma/Framework/Memory/PoolAllocator.hpp>

namespace Magma
{
	namespace Resources
	{
		/// <summary>
		///		Shader resource data
		/// </summary>
		class Shader : public ResourceData
		{
		public:
			using ResourceData::ResourceData;

			/// <summary>
			///		Shader type
			/// </summary>
			Framework::Graphics_V1X::ShaderType type = Framework::Graphics_V1X::ShaderType::Invalid;

			union
			{
				/// <summary>
				///		Vertex shader handle
				/// </summary>
				Framework::Graphics_V1X::VertexShader* vertexShader = nullptr;

				/// <summary>
				///		Pixel shader handle
				/// </summary>
				Framework::Graphics_V1X::PixelShader* pixelShader;
			};
		};

		/// <summary>
		///		Used to import shader resources
		/// </summary>
		class ShaderImporter final : public Importer
		{
		public:
			ShaderImporter(Manager* manager, Framework::Graphics_V1X::RenderDevice* device);
			virtual ~ShaderImporter() final;

			// Inherited via Importer
			virtual void Update() final;
			virtual void Import(Resource * resource) final;
			virtual void Destroy(Resource * resource) final;
			virtual ResourceMode GetMode(Resource* resources) final;

		private:
			Framework::Graphics_V1X::RenderDevice* m_device;
			Framework::Memory::PoolAllocator m_pool;
		};
	}
}
