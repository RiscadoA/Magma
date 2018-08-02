#include "../Element.hpp"

namespace Magma
{
	namespace GUI
	{
		namespace Elements
		{
			/// <summary>
			///		Box graphical user interface element
			/// </summary>
			class Box : public Element
			{
			public:
				Box(glm::vec4 backgroundColor);

				/// <summary>
				///		Sets this box background color
				/// </summary>
				/// <param name="backgroundColor">New background color</param>
				inline void SetBackgroundColor(const glm::vec4& backgroundColor) { m_backgroundColor = backgroundColor; }

				/// <summary>
				///		Gets this box current background color
				/// </summary>
				/// <returns>Current background color</returns>
				inline const glm::vec4& GetBackgroundColor() const { return m_backgroundColor; }

			private:
				glm::vec4 m_backgroundColor;
			};
		}
	}
}