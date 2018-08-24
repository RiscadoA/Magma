#pragma once

#include <Magma/Resources/Shader.hpp>
#include <Magma/Framework/Graphics/1.X/RenderDevice.hpp>

#include <vector>

#include "Root.hpp"
#include "ElementRenderer.hpp"

namespace Magma
{
	namespace GUI
	{
		/// <summary>
		///		Used to render the graphical user interface element tree
		/// </summary>
		class Renderer final
		{
		public:
			/// <summary>
			///		Constructs a new graphical user interface Renderer
			/// </summary>
			Renderer(Framework::Graphics::V1X::RenderDevice* device);
			~Renderer();

			/// <summary>
			///		Renders the graphical user interface element tree
			/// </summary>
			/// <param name="root">Tree root pointer</param>
			 void Render(Root* root);

			 /// <summary>
			 ///	Adds an element renderer to this renderer.
			 /// </summary>
			 /// <typeparam name="T">Element renderer type</typeparam>
			 template <typename T, typename ... TArgs>
			 void AddRenderer(TArgs ... args);

		private:
			void RenderRecursive(Element* element);

			std::vector<ElementRenderer*> m_renderers;

			Framework::Graphics::V1X::RenderDevice* m_device;
			Framework::Graphics::V1X::DepthStencilState* m_depthStencilState;
			Framework::Graphics::V1X::BlendState* m_blendState;
		};

		template<typename T, typename ...TArgs>
		inline void Renderer::AddRenderer(TArgs ...args)
		{
			static_assert(std::is_base_of<ElementRenderer, T>::value);
			auto renderer = new T(args...);
			m_renderers.push_back(renderer);
		}
	}
}
