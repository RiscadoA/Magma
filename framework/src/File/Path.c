#include <Magma/Framework/File/Path.h>
#include <Magma/Framework/String/StringStream.h>
#include "Config.h"

#include <string.h>
#include <stdlib.h>

const mfsUTF8CodeUnit* mffMagmaRootDirectory = MAGMA_ROOT_DIRECTORY;

const mfsUTF8CodeUnit * mffGoDownPath(const mfsUTF8CodeUnit * path)
{
	const mfsUTF8CodeUnit* it = path;

	if (*it == '/')
		++it;

	while (*it != '\0')
	{
		if (*it == '/')
			return it;
		++it;
	}

	return NULL;
}

const mfsUTF8CodeUnit * mffGetPathName(const mfsUTF8CodeUnit * path)
{
	const mfsUTF8CodeUnit* nameStart = NULL;
	const mfsUTF8CodeUnit* newNameStart = NULL;

	if (path != NULL)
		while (*path != '\0')
		{
			if (newNameStart != NULL)
			{
				nameStart = newNameStart;
				newNameStart = NULL;
			}

			if (*path == '/')
				newNameStart = path + 1;
			++path;
		}

	return nameStart;
}

const mfsUTF8CodeUnit * mffGetPathExtension(const mfsUTF8CodeUnit * path)
{
	const mfsUTF8CodeUnit* extStart = NULL;

	if (path != NULL)
		while (*path != '\0')
		{
			if (*path == '.')
				extStart = path + 1;
			++path;
		}

	return extStart;
}
