#include "Object.h"

#include "../Thread/Atomic.h"

mfError mfmInitObject(mfmObject * obj)
{
	if (obj == NULL)
		return MFM_ERROR_INVALID_ARGUMENTS;
	obj->destructorFunc = NULL;
	mfError err = mftAtomic32Store(&obj->m_referenceCount, 0);
	if (err != MFT_ERROR_OKAY)
		return err;
	return MFM_ERROR_OKAY;
}

mfError mfmDestroyObject(mfmObject* obj)
{
	if (obj == NULL)
		return MFM_ERROR_INVALID_ARGUMENTS;
	mfmI32 refCount = 0;
	mfError err = mftAtomic32Load(&obj->m_referenceCount, &refCount);
	if (err != MFT_ERROR_OKAY)
		return err;
	if (refCount != 0)
		return MFM_ERROR_STILL_HAS_REFERENCES;
	return MFM_ERROR_OKAY;
}

mfError mfmIncObjectRef(mfmObject* obj)
{
	if (obj == NULL)
		return MFM_ERROR_INVALID_ARGUMENTS;
	mfError err = mftAtomic32Add(&obj->m_referenceCount, 1);
	if (err != MFT_ERROR_OKAY)
		return err;
	return MFM_ERROR_OKAY;
}

mfError mfmDecObjectRef(mfmObject* obj)
{
	if (obj == NULL)
		return MFM_ERROR_INVALID_ARGUMENTS;
	mfError err = mftAtomic32Add(&obj->m_referenceCount, -1);
	if (err != MFT_ERROR_OKAY)
		return err;
	return MFM_ERROR_OKAY;
}

mfError mfmGetObjectRefCount(mfmObject* obj, mfmI32* refCount)
{
	if (obj == NULL || refCount == NULL)
		return MFM_ERROR_INVALID_ARGUMENTS;
	mfError err = mftAtomic32Load(&obj->m_referenceCount, refCount);
	if (err != MFT_ERROR_OKAY)
		return err;
	return MFM_ERROR_OKAY;
}
