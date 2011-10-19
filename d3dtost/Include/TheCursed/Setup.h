#pragma once

#include "PWindows.h"

namespace d3dtost {
	void Initialise (void);
	void CleanUp (void);

	void MakeWindow (void);

	wchar_t* ConvertErrorMessage (char const* const msg);
} // namespace d3dtost
