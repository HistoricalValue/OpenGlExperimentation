#pragma once

#include "MyWindows.h"

namespace d3dtost {
	void Initialise (void);
	void CleanUp (void);

	void MakeWindow (void);

	LPWSTR ConvertErrorMessage (char const* const msg);
} // namespace d3dtost
