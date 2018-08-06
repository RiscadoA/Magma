#pragma once

#include "BoundingBox.hpp"

#include <string>
#include <typeindex>

#include <Magma/Framework/Input/Event.hpp>
#include <Magma/Framework/Input/Mouse.hpp>

namespace Magma
{
	namespace GUI
	{
		class ElementRenderer;

		/// <summary>
		///		Represents an element in the graphical user interface.
		/// </summary>
		class Element
		{
		public:
			/// <summary>
			///		Constructs a new graphical user interface element (starts as invalid, must be validated).
			/// </summary>
			/// <param name="type">Element type</param>
			/// <param name="isRenderable">Is this element renderable?</param>
			Element(std::type_index type, bool isRenderable);

			/// <summary>
			///		Deconstructs an element
			/// </summary>
			virtual ~Element();

			/// <summary>
			///		Sets the parent to nullptr, removes all children and invalidates the element.
			/// </summary>
			void Destroy();

			/// <summary>
			///		Sets this element's parent and removes this element from its parent (calls RemoveChild on old parent and calls AddChild on new parent).
			/// </summary>
			void SetParent(Element* parent);

			/// <summary>
			///		Adds a child to this element and sets this element as the child's parent (doesn't call SetParent on child).
			/// </summary>
			/// <param name=""></param>
			void AddChild(Element* child);

			/// <summary>
			///		Removes a child from this element and sets the child's parent to nullptr (doesn't call SetParent on child).
			/// </summary>
			/// <param name="child">Child element pointer</param>
			void RemoveChild(Element* child);

			/// <summary>
			///		Checks if this element is the parent of another element.
			/// </summary>
			/// <param name="child">Pointer to element to check</param>
			/// <returns>True if is child, otherwise false</returns>
			bool IsChild(Element* child) const;

			/// <summary>
			///		Gets a pointer to this element's parent.
			/// </summary>
			/// <returns>Pointer to this element's parent</returns>
			inline const Element* GetParent() const { return m_parent; }

			/// <summary>
			///		Gets a pointer to this element's parent.
			/// </summary>
			/// <returns>Pointer to this element's parent</returns>
			inline Element* GetParent() { return m_parent; }

			/// <summary>
			///		Gets a pointer to this element's first child.
			/// </summary>
			/// <returns>Pointer to this element's first child</returns>
			inline const Element* GetFirstChild() const { return m_child; }

			/// <summary>
			///		Gets a pointer to this element's first child.
			/// </summary>
			/// <returns>Pointer to this element's first child</returns>
			inline Element* GetFirstChild() { return m_child; }

			/// <summary>
			///		Gets a pointer to the next child of this element's parent.
			/// </summary>
			/// <returns>Pointer to the next child of this element's parent</returns>
			inline const Element* GetNext() const { return m_next; }

			/// <summary>
			///		Gets a pointer to the next child of this element's parent.
			/// </summary>
			/// <returns>Pointer to the next child of this element's parent</returns>
			inline Element* GetNext() { return m_next; }

			/// <summary>
			///		Checks if this element is valid.
			/// </summary>
			/// <returns>True if is valid, otherwise false</returns>
			inline bool IsValid() const { return m_valid; }

			/// <summary>
			///		Disables this element.
			/// </summary>
			inline void Disable() { m_enabled = false; }

			/// <summary>
			///		Enables this element.
			/// </summary>
			inline void Enable() { m_enabled = true; }

			/// <summary>
			///		Checks if this element is enabled.
			/// </summary>
			inline bool IsEnabled() const { return m_enabled; }

			/// <summary>
			///		Sets the bounding box for this element.
			/// </summary>
			/// <param name="box">Bounding box</param>
			inline void SetBox(const BoundingBox& box) { m_box = box; m_dirty = true; }

			/// <summary>
			///		Gets the bounding box being used by this element.
			/// </summary>
			/// <returns>Reference to the bounding box being used by this element</returns>
			inline const BoundingBox& GetBox() const { return m_box; }

			/// <summary>
			///		Gets this element's type.
			/// </summary>
			/// <returns>This element's type</returns>
			inline std::type_index GetType() const { return m_type; }
			
			/// <summary>
			///		Sets this element's renderer.
			/// </summary>
			/// <param name="renderer">Pointer to new renderer</param>
			inline void SetRenderer(ElementRenderer* renderer) { m_renderer = renderer; }

			/// <summary>
			///		Gets this element's render.
			/// </summary>
			/// <returns>This element's renderer</returns>
			inline ElementRenderer* GetRenderer() const { return m_renderer; }

			/// <summary>
			///		Gets this element's bounding box in pixels (absolute coordinates).
			/// </summary>
			/// <returns>This element's bounding box in pixels (absolute coordinates)</returns>
			BoundingBox GetAbsoluteBoundingBox() const;

			/// <summary>
			///		Checks if this element contains a point inside its bounding box (point with relative coordinates to <paramref name="relativeElement"></paramref>)
			/// </summary>
			bool Contains(Point point, Element* relativeElement = nullptr) const;

			/// <summary>
			///		Updates this element's transform.
			/// </summary>
			virtual void UpdateTransform() const;

			/// <summary>
			///		Gets this element's transform.
			/// </summary>
			const glm::mat4& GetTransform() const;

			/// <summary>
			///		Checks if this element is renderable.
			/// </summary>
			/// <returns>True if it is renderable, otherwise false</returns>
			inline bool IsRenderable() const { return m_isRenderable; }

			/// <summary>
			///		Fired when the mouse enters the element bounding box.
			/// </summary>
			Framework::Input::Event<> OnMouseEnter;

			/// <summary>
			///		Fired when the mouse leaves the element bounding box.
			/// </summary>
			Framework::Input::Event<> OnMouseLeave;

			/// <summary>
			///		Fired when the mouse moves inside the element bounding box.
			///		Params: { mouse position X relative to bounding box; mouse position Y relative to bounding box; }
			/// </summary>
			Framework::Input::Event<float, float> OnMouseMove;

			/// <summary>
			///		Fired when the mouse wheel is scrolled inside the element bounding box.
			///		Params: { mouse scroll delta; }
			/// </summary>
			Framework::Input::Event<float> OnMouseScroll;

			/// <summary>
			///		Fired when a mouse button goes up inside the element bounding box.
			///		Params: { mouse button; }
			/// </summary>
			Framework::Input::Event<Framework::Input::Mouse> OnMouseUp;

			/// <summary>
			///		Fired when a mouse button goes down inside the element bounding box.
			///		Params: { mouse button; }
			/// </summary>
			Framework::Input::Event<Framework::Input::Mouse> OnMouseDown;

		protected:
			/// <summary>
			///		Sets this element's transform matrix.
			///		Only use this if you know what you are doing!
			/// </summary>
			inline void SetTransform(const glm::mat4& transform) const { m_transform = transform; m_dirty = false; }

		private:
			bool m_enabled;
			bool m_valid;

			Element* m_parent;	// Element parent
			Element* m_child; // First element child
			Element* m_next; // Next child of the element's parent


			BoundingBox m_box;

			std::type_index m_type;
			ElementRenderer* m_renderer;

			bool m_isRenderable;

			mutable bool m_dirty;
			mutable glm::mat4 m_transform;
		};
	}
}
