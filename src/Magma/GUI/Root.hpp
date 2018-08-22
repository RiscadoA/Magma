#pragma once

#include "Element.hpp"

namespace Magma
{
	namespace GUI
	{
		/// <summary>
		///		This class represents the GUI element tree root and is used to allocate and create new elements.
		/// </summary>
		class Root final : public Element
		{
		public:
			/// <summary>
			///		Constructs a new GUI element tree
			/// </summary>
			/// <param name="poolSize">Element pool48 allocator capacity</param>
			/// <param name="maxElementSize">Maximum element size in bytes</param>
			Root(size_t poolSize = 4096, size_t maxElementSize = 1024);

			/// <summary>
			///		Deconstructs a GUI element tree
			/// </summary>
			virtual ~Root() final;

			/// <summary>
			///		Creates a new GUI element
			/// </summary>
			/// <param name="parent">Element parent</param>
			/// <returns>GUI element pointer</returns>
			template <typename T, typename ... TArgs>
			T* CreateElement(Element* parent, TArgs ... args);

			/// <summary>
			///		Deallocates the invalid elements
			/// </summary>
			/// <returns>The number of elements that were claned</returns>
			size_t Clean();

		private:
			virtual void UpdateTransform() const final;

			Element* Allocate(size_t size);

			size_t m_poolSize;
			size_t m_maxElementSize;

			char* m_pool;
			bool* m_free;
		};

		template<typename T, typename ...TArgs>
		inline T* Root::CreateElement(Element* parent, TArgs ... args)
		{
			static_assert(std::is_base_of<Element, T>::value);
			static_assert(!std::is_same<Root, T>::value);

			auto loc = Allocate(sizeof(T));
			
			auto t = new (loc) T(args...);
			if (parent == nullptr)
				t->SetParent(this);
			else
				t->SetParent(parent);
			return t;
		}
	}
}