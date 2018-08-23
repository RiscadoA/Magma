#pragma once

#include "Error.h"

#ifdef __cplusplus
extern "C"
{
#endif

	typedef void (*mfmDestructor)(void*);

	typedef struct
	{
		mfmDestructor destructorFunc;
		volatile mfmI32 m_referenceCount;
	} mfmObject;

	mfError mfmInitObject(mfmObject* obj);

	mfError mfmDestroyObject(mfmObject* obj);

	mfError mfmIncObjectRef(mfmObject* obj);

	mfError mfmDecObjectRef(mfmObject* obj);

	mfError mfmGetObjectRefCount(mfmObject* obj, mfmI32* refCount);

#ifdef __cplusplus
}
#endif
