#pragma once

#include "Type.h"

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

#ifdef __cplusplus
}
#endif
