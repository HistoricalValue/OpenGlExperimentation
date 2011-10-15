#pragma once

#include <PTypes.h>
#include <string>

namespace my {

	class String: public _T_STR {
	public:
														String (void):
															_T_STR()
															{}
														String (String const& other):
															_T_STR(other)
															{}
														String (_T_STR const& other):
															_T_STR(other)
															{}
														String (TCHAR const* const str):
															_T_STR(str)
															{}
														~String (void)
															{}
	};

} // namespace my
