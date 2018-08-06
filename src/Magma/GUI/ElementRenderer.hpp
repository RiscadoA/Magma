#pragma once

#include <string>
#include <typeindex>

namespace Magma
{
	namespace GUI
	{
		class Element;

		/// <summary>
		///		Abstract class to render elements
		/// </summary>
		class ElementRenderer
		{
		public:
			ElementRenderer(std::type_index type);
			virtual ~ElementRenderer() = default;

			/// <summary>
			///		Renders an element whose type matches this renderer
			/// </summary>
			/// <param name="element">Pointer to element</param>
			virtual void Render(Element* element) = 0;

			/// <summary>
			///		Gets this element renderer's type
			/// </summary>
			/// <returns>Element renderer's type</returns>
			inline std::type_index GetType() { return m_type; }

		private:
			std::type_index m_type;
		};
	}
}
