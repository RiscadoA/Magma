#pragma once

#include <Magma/Framework/Graphics/RenderDevice.hpp>
#include <Magma/Resources/Shader.hpp>

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
			Renderer();
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

			Framework::Graphics::RenderDevice* m_device;

			std::vector<ElementRenderer*> m_renderers;
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
