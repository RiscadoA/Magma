#include "Entry.h"

#include "../String/Stream.h"

void mfEntry(int argc, char** argv)
{
	// Init stream library
	mfsInitStream();

	// Run program
	Main(argc, argv);

	// Terminate stream library
	mfsTerminateStream();
}
