#include "maskTest.h"
int maintest(int argc, char** argv)
{
	MaskTest mask;	// mask test lesson 66/67
	mask.maskSet(0x12, 0x34, 0x56);
	mask.maskSet(0x23, 0x45, 0x67);
	return EXIT_SUCCESS;

}	// end main()