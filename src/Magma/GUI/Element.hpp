#pragma once

#include "BoundingBox.hpp"

namespace Magma
{
	namespace GUI
	{
		/// <summary>
		///		Represents an element type
		/// </summary>
		enum class ElementType
		{
			Invalid = -1,

			Null,
			Box,
			Text,

			Count
		};

		/// <summary>
		///		Represents an element in the graphical user interface
		/// </summary>
		class Element
		{
		public:
			/// <summary>
			///		Constructs a new graphical user interface element (starts as invalid, must be validated)
			/// </summary>
			/// <param name="type">Element type</param>
			Element(ElementType type);

			/// <summary>
			///		Deconstructs an element
			/// </summary>
			virtual ~Element();

			/// <summary>
			///		Sets the parent to nullptr, removes all children and invalidates the element
			/// </summary>
			void Destroy();

			/// <summary>
			///		Sets this element's parent and removes this element from its parent (calls RemoveChild on old parent and calls AddChild on new parent)
			/// </summary>
			void SetParent(Element* parent);

			/// <summary>
			///		Adds a child to this element and sets this element as the child's parent (calls SetParent on parent)
			/// </summary>
			/// <param name=""></param>
			void AddChild(Element* child);

			/// <summary>
			///		Removes a child from this element and sets the child's parent to nullptr
			/// </summary>
			/// <param name="child">Child element pointer</param>
			void RemoveChild(Element* child);

			/// <summary>
			///		Checks if this element is the parent of another element
			/// </summary>
			/// <param name="child">Pointer to element to check</param>
			/// <returns>True if is child, otherwise false</returns>
			bool IsChild(Element* child) const;

			/// <summary>
			///		Gets a pointer to this element's parent
			/// </summary>
			/// <returns>Pointer to this element's parent</returns>
			inline const Element* GetParent() const { return m_parent; }

			/// <summary>
			///		Gets a pointer to this element's parent
			/// </summary>
			/// <returns>Pointer to this element's parent</returns>
			inline Element* GetParent() { return m_parent; }

			/// <summary>
			///		Gets a pointer to this element's first child
			/// </summary>
			/// <returns>Pointer to this element's first child</returns>
			inline const Element* GetFirstChild() const { return m_child; }

			/// <summary>
			///		Gets a pointer to this element's first child
			/// </summary>
			/// <returns>Pointer to this element's first child</returns>
			inline Element* GetFirstChild() { return m_child; }

			/// <summary>
			///		Gets a pointer to the next child of this element's parent
			/// </summary>
			/// <returns>Pointer to the next child of this element's parent</returns>
			inline const Element* GetNext() const { return m_next; }

			/// <summary>
			///		Gets a pointer to the next child of this element's parent
			/// </summary>
			/// <returns>Pointer to the next child of this element's parent</returns>
			inline Element* GetNext() { return m_next; }

			/// <summary>
			///		Checks if this element is valid
			/// </summary>
			/// <returns>True if is valid, otherwise false</returns>
			inline bool IsValid() const { return m_valid; }

			/// <summary>
			///		Disables this element
			/// </summary>
			inline void Disable() { m_enabled = false; }

			/// <summary>
			///		Enables this element
			/// </summary>
			inline void Enable() { m_enabled = true; }

			/// <summary>
			///		Checks if this element is enabled
			/// </summary>
			inline bool IsEnabled() const { return m_enabled; }

			/// <summary>
			///		Sets the bounding box for this element
			/// </summary>
			/// <param name="box">Bounding box</param>
			inline void SetBox(const BoundingBox& box) { m_box = box; m_dirty = true; }

			/// <summary>
			///		Gets the bounding box being used by this element
			/// </summary>
			/// <returns>Reference to the bounding box being used by this element</returns>
			inline const BoundingBox& GetBox() const { return m_box; }

			/// <summary>
			///		Gets this element's bounding box in pixels (absolute coordinates)
			/// </summary>
			/// <returns>This element's bounding box in pixels (absolute coordinates)</returns>
			BoundingBox GetAbsoluteBoundingBox() const;

			/// <summary>
			///		Checks if this element contains a point inside its bounding box (point with relative coordinates to <paramref name="relativeElement"></paramref>)
			/// </summary>
			bool Contains(Point point, Element* relativeElement = nullptr) const;

		private:
			bool m_enabled;
			bool m_valid;

			Element* m_parent;	// Element parent
			Element* m_child; // First element child
			Element* m_next; // Next child of the element's parent

			BoundingBox m_box;

			bool m_dirty;
			glm::mat4 m_transform;
		};
	}
}
