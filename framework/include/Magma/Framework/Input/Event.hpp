#pragma once

#include <functional>
#include <unordered_map>

namespace Magma
{
	namespace Framework
	{
		namespace Input
		{
			/// <summary>
			///		Event pattern implementation class.
			/// </summary>
			/// <typeparam name="TArgs">Event arguments</typeparam>
			template <typename ... TArgs>
			class Event
			{
			public:
				inline Event() { m_nextID = 1; }
				inline ~Event() { }

				/// <summary>
				///		Adds a listener to this event.
				/// </summary>
				/// <param name="listener">Litener</param>
				/// <returns>Listener ID</returns>
				inline size_t AddListener(std::function<void(TArgs...)> listener) { m_listeners.try_emplace(m_nextID, listener); return m_nextID++; }

				/// <summary>
				///		Removes a listener from this event.
				/// </summary>
				/// <param name="listener">Listener ID</param>
				inline void RemoveListener(size_t listener) { m_listeners.erase(listener); }

				/// <summary>
				///		Fires this event (calls all listener functions added).
				/// </summary>
				/// <param name="args">Event arguments</param>
				inline void Fire(TArgs ... args) { for (auto& l : m_listeners) l.second(args...); }

			private:
				size_t m_nextID;
				std::unordered_map<size_t, std::function<void(TArgs...)>> m_listeners;
			};
		}
	}
}