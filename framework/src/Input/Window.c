#include <Magma/Framework/Input/Window.h>

#include <string.h>

struct
{
	mfmBool active;
	mfiWindowCreatorFunction func;
	mfsUTF8CodeUnit type[16];
} mfiWindowCreatorRegisterEntries[MFI_MAX_WINDOW_CREATOR_REGISTER_ENTRIES];

mfError mfiInitWindows()
{
	// Init all entries

	for (mfmU64 i = 0; i < MFI_MAX_WINDOW_CREATOR_REGISTER_ENTRIES; ++i)
	{
		mfiWindowCreatorRegisterEntries[i].active = MFM_FALSE;
		mfiWindowCreatorRegisterEntries[i].func = NULL;
	}

	return MF_ERROR_OKAY;
}

void mfiTerminateWindows()
{
	// Reset all entries

	for (mfmU64 i = 0; i < MFI_MAX_WINDOW_CREATOR_REGISTER_ENTRIES; ++i)
	{
		mfiWindowCreatorRegisterEntries[i].active = MFM_FALSE;
		mfiWindowCreatorRegisterEntries[i].func = NULL;
	}
}

mfError mfiRegisterWindowCreator(const mfsUTF8CodeUnit* type, mfiWindowCreatorFunction func)
{
	for (mfmU64 i = 0; i < MFI_MAX_WINDOW_CREATOR_REGISTER_ENTRIES; ++i)
		if (mfiWindowCreatorRegisterEntries[i].active == MFM_FALSE)
		{
			mfiWindowCreatorRegisterEntries[i].active = MFM_TRUE;
			mfiWindowCreatorRegisterEntries[i].func = func;

			for (mfmU32 j = 0; j < 16; ++j)
			{
				mfiWindowCreatorRegisterEntries[i].type[j] = type[j];
				if (type[j] == '\0')
					break;
			}

			return MF_ERROR_OKAY;
		}

	return MFI_ERROR_NO_REGISTER_ENTRIES;
}

mfError mfiCreateWindow(const mfsUTF8CodeUnit * type, mfiWindow ** window, mfmU32 width, mfmU32 height, mfiEnum mode, const mfsUTF8CodeUnit * title)
{
	for (mfmU64 i = 0; i < MFI_MAX_WINDOW_CREATOR_REGISTER_ENTRIES; ++i)
		if (mfiWindowCreatorRegisterEntries[i].active == MFM_TRUE &&
			strcmp(type, mfiWindowCreatorRegisterEntries[i].type) == 0)
			return mfiWindowCreatorRegisterEntries[i].func(window, width, height, mode, title);
	return MFI_ERROR_TYPE_NOT_REGISTERED;
}

void mfiDestroyWindow(void * window)
{
	((mfmObject*)window)->destructorFunc(window);
}
