#include <Magma/Framework/ErrorString.h>

#include <Magma/Framework/Audio/ErrorString.h>
#include <Magma/Framework/File/ErrorString.h>
#include <Magma/Framework/Graphics/ErrorString.h>
#include <Magma/Framework/Input/ErrorString.h>
#include <Magma/Framework/Memory/ErrorString.h>
#include <Magma/Framework/String/ErrorString.h>
#include <Magma/Framework/Thread/ErrorString.h>
#include <Magma/Framework/VM/ErrorString.h>

const mfsUTF8CodeUnit * mfErrorToString(mfError err)
{
	const mfsUTF8CodeUnit* errStr = NULL;

	errStr = mfaErrorToString(err);
	if (errStr != NULL)
		return errStr;

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
