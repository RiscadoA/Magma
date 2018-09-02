#include "ErrorString.h"

#include "File/ErrorString.h"
#include "Graphics/ErrorString.h"
#include "Input/ErrorString.h"
#include "Memory/ErrorString.h"
#include "String/ErrorString.h"
#include "Thread/ErrorString.h"
#include "VM/ErrorString.h"

const mfsUTF8CodeUnit * mfErrorToString(mfError err)
{
	const mfsUTF8CodeUnit* errStr = NULL;

	errStr = mffErrorToString(err);
	if (errStr != NULL)
		return errStr;

	errStr = mfgErrorToString(err);
	if (errStr != NULL)
		return errStr;

	errStr = mfiErrorToString(err);
	if (errStr != NULL)
		return errStr;

	errStr = mfmErrorToString(err);
	if (errStr != NULL)
		return errStr;

	errStr = mfsErrorToString(err);
	if (errStr != NULL)
		return errStr;

	errStr = mftErrorToString(err);
	if (errStr != NULL)
		return errStr;

	errStr = mfvErrorToString(err);
	if (errStr != NULL)
		return errStr;

	return errStr;
}
