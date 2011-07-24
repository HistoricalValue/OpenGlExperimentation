// MyUtil.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"


// This is an example of an exported variable
MYUTIL_API int nMyUtil=0;

// This is an example of an exported function.
MYUTIL_API int fnMyUtil(void)
{
	return 42;
}

// This is the constructor of a class that has been exported.
// see MyUtil.h for the class definition
CMyUtil::CMyUtil()
{
	return;
}

///////////////////////////////////////////////////

namespace my {

	void Initialise (void) {
		ilInit();
		ilEnable(IL_ORIGIN_SET);
		ilSetInteger(IL_ORIGIN_MODE, IL_ORIGIN_LOWER_LEFT);

		image_decoders::DevilImageDecoder::Initialise();
	}

	void CleanUp (void) {
		image_decoders::DevilImageDecoder::CleanUp();
		// ilCleanup()
	}

} // namespace my
