#pragma once

#include "Allocator.hpp"
#include "Exception.hpp"

namespace Magma
{
	namespace Framework
	{
		namespace Memory
		{
			/// <summary>
			///		Functor object.
			/// </summary>
			template <typename T>
			class Functor;

			template <typename T, typename ... TArgs>
			class Functor<T (TArgs...)>
			{
			public:
				class ICallable
				{
				public:
					virtual ~ICallable() = default;
					virtual T Call(TArgs ... args) = 0;
					virtual ICallable* Clone(HAllocator alloc) = 0;
				};

				class FuncCallable : public ICallable
				{
				public:
					FuncCallable(T(*func)(TArgs...))
					{
						m_func = func;
					}

					virtual T Call(TArgs ... args) final
					{
						return m_func(args...);
					}

					virtual ICallable* Clone(HAllocator alloc) final
					{
						auto ret = new (alloc.Allocate(sizeof(FuncCallable))) FuncCallable(m_func);
						return ret;
					}

					T(*m_func)(TArgs...);
				};

				template <typename Obj>
				class MethodCallable : public ICallable
				{
				public:
					MethodCallable(Obj* obj, T(Obj::*func)(TArgs...))
					{
						m_obj = obj;
						m_func = func;
					}

					virtual T Call(TArgs ... args) final
					{
						return (m_obj->*m_func)(args...);
					}

					virtual ICallable* Clone(HAllocator alloc) final
					{
						auto ret = new (alloc.Allocate(sizeof(MethodCallable))) MethodCallable(m_obj, m_func);
						return ret;
					}

					Obj* m_obj;
					T(Obj::*m_func)(TArgs...);
				};

				inline Functor(Functor&& func)
					: m_allocator(func.m_allocator), m_callable(func.m_callable)
				{
					func.m_allocator.Release();
					func.m_callable = nullptr;
				}

				inline Functor(const Functor& func)
					: m_allocator(func.m_allocator)
				{
					if (func.m_callable == nullptr)
						m_callable = nullptr;
					else
						m_callable = func.m_callable->Clone(m_allocator);
				}

				inline Functor(HAllocator allocator = StandardAllocator)
					: m_allocator(allocator)
				{
					m_callable = nullptr;
				}

				inline Functor(T(*func)(TArgs...), HAllocator allocator = StandardAllocator)
					: m_allocator(allocator)
				{
					m_callable = new (m_allocator.Allocate(sizeof(FuncCallable))) FuncCallable(func);
				}

				inline Functor& operator=(T(*func)(TArgs...))
				{
					if (m_callable != nullptr)
					{
						m_callable->~ICallable();
						m_allocator.Deallocate(m_callable);
					}
					m_callable = new (m_allocator.Allocate(sizeof(FuncCallable))) FuncCallable(func);
					return *this;
				}

				inline Functor& operator=(const Functor& func)
				{
					if (m_callable != nullptr)
					{
						m_callable->~ICallable();
						m_allocator.Deallocate(m_callable);
					}
					if (func.m_callable == nullptr)
						m_callable = nullptr;
					else
						m_callable = func.m_callable->Clone(m_allocator);
					return *this;
				}

				inline ~Functor()
				{
					if (m_callable != nullptr)
					{
						m_callable->~ICallable();
						m_allocator.Deallocate(m_callable);
					}
				}

				inline T Invoke(TArgs ... args)
				{
					return m_callable->Call(args...);
				}

				inline T operator()(TArgs ... args)
				{
					return this->Invoke(args...);
				}

				HAllocator m_allocator;
				ICallable * m_callable;
			};

			/// <summary>
			///		Gets a functor from a method.
			/// </summary> 
			template <typename Obj, typename T, typename ... TArgs>
			inline Functor<T(TArgs...)> GetMethod(Obj& obj, T(Obj::*func)(TArgs...), HAllocator allocator = StandardAllocator)
			{
				Functor<T(TArgs...)> f(allocator);
				f.m_callable = new (allocator.Allocate(sizeof(typename Functor<T(TArgs...)>::MethodCallable<Obj>))) typename Functor<T(TArgs...)>::MethodCallable<Obj>(&obj, func);
				return f;
			}
		}
	}
}
