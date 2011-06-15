#ifndef __MY__GLOBAL_INSTANCES__H__
#define __MY__GLOBAL_INSTANCES__H__

#include "PError.h"
#include "MyLogger.h"
#include "MyUtil.h"


MYUTIL_API extern int n;

namespace my {
	namespace global {


		namespace errorHandler {
			MYUTIL_API codeshare::ErrorHolder&			Get (void);
			MYUTIL_API void								Set (codeshare::ErrorHolder*);
		}

		namespace logger {
			MYUTIL_API GenericLogger&					Get (void);
			MYUTIL_API void								Set (GenericLogger*);
		}



	} // namespace global
} // namespace my

#endif // __MY__GLOBAL_INSTANCES__H__
