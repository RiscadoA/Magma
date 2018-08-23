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
		mfmU64 referenceCount;
	} mfmObject;

	mfError mfmInitObject(mfmObject* obj);

	mfError mfmDestroyObject(mfmObject* obj);

#ifdef __cplusplus
}
#endif
