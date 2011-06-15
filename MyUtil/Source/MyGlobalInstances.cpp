#include "stdafx.h"


#define __TRIVIAL_IMPLEMENTATION												\
		static Value* value(NULL);												\
		void Set (Value* const _value) {										\
			value = _value;														\
		}																		\
		Value& Get (void) {														\
			return *value;														\
		}																		\


int n;

namespace my {

	namespace global {

		namespace logger {
			typedef my::GenericLogger					Value;
			__TRIVIAL_IMPLEMENTATION
		} // namespace logger

		namespace errorHandler {
			typedef codeshare::ErrorHolder				Value;
			__TRIVIAL_IMPLEMENTATION
		} // namesace errorHandler

	} // namespace global

} // namespace my
