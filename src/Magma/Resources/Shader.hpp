#pragma once

#include "Importer.hpp"

#include <Magma/Framework/Graphics/RenderDevice.hpp>
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
			Framework::Graphics::ShaderType type = Framework::Graphics::ShaderType::Invalid;

			union
			{
				/// <summary>
				///		Vertex shader handle
				/// </summary>
				Framework::Graphics::VertexShader* vertexShader = nullptr;

				/// <summary>
				///		Pixel shader handle
				/// </summary>
				Framework::Graphics::PixelShader* pixelShader;
			};
		};

		/// <summary>
		///		Used to import shader resources
		/// </summary>
		class ShaderImporter final : public Importer
		{
		public:
			ShaderImporter(Manager* manager, Framework::Graphics::RenderDevice* device);
			virtual ~ShaderImporter() final;

			// Inherited via Importer
			virtual void Update() final;
			virtual void Import(Resource * resource) final;
			virtual void Destroy(Resource * resource) final;

		private:
			Framework::Graphics::RenderDevice* m_device;
			Framework::Memory::PoolAllocator m_pool;
		};
	}
}