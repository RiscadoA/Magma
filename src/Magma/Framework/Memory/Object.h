#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

typedef void (*mfmDestructor)(void*);

typedef struct
{
	mfmDestructor destructorFunc;
} mfmObject;

#ifdef __cplusplus
}
#endif
