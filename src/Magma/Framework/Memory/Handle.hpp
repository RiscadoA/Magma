#pragma once

#include "Object.h"

namespace Magma
{
	namespace Framework
	{
		namespace Memory
		{
			/// <summary>
			///		Used as an object handle.
			///		Destroys the object automatically when there are no more references to it.
			/// </summary>
			class Handle
			{
			public:
				Handle();
				Handle(mfmObject& obj);
				Handle(void* obj);
				Handle(const Handle& rhs);
				Handle(Handle&& rhs);
				virtual ~Handle();

				/// <summary>
				///		Sets a new object to where this handle will point.
				///		Releases the previously set object, if any.
				/// </summary>
				/// <param name="obj">New object</param>
				void Set(mfmObject& obj);

				/// <summary>
				///		Releases the previously set object, if any.
				/// </summary>
				/// <returns>True if there was an object set in this handle, otherwise false</returns>
				bool Release();

				/// <summary>
				///		Gets the currently set object (throws error when there isn't an object set).
				/// </summary>
				/// <returns>Reference to the currently set object</returns>
				mfmObject& Get() const;

				Handle& operator=(mfmObject& obj);
				Handle& operator=(Handle obj);
				mfmObject* operator->() const;

				/// <summary>
				///		Gets a pointer to the currently set object without checking if it is NULL or not.
				///		USE WITH CAUTION.
				/// </summary>
				/// <returns>Currently set pointer</returns>
				inline mfmObject* GetNoChecks() const { return m_obj; }

			private:
				mfmObject* m_obj;
			};
		}
	}
}