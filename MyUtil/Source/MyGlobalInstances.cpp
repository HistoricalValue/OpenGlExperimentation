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
			typedef ErrorHandler						Value;

			static Value								value(NULL);
			void Set (Value const _value) {
				value = _value;
			}

			Value Get (void) {
				return value;
			}
		} // namesace errorHandler

		namespace errorHolder {
			typedef codeshare::utilities::ErrorHolder	Value;
			__TRIVIAL_IMPLEMENTATION
		} // namespace errorHolder

	} // namespace global

} // namespace my
