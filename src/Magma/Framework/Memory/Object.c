#include "Object.h"

mfError mfmInitObject(mfmObject * obj)
{
	obj->destructorFunc = NULL;
	obj->referenceCount = 0;
	return MFM_ERROR_OKAY;
}

mfError mfmDestroyObject(mfmObject* obj)
{
	if (obj->referenceCount != 0)
		return MFM_ERROR_STILL_HAS_REFERENCES;
	return MFM_ERROR_OKAY;
}