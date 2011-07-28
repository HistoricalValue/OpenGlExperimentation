#ifndef __MY__GLOBAL_INSTANCES__H__
#define __MY__GLOBAL_INSTANCES__H__

#include "PError.h"
#include "MyLogger.h"
#include "MyUtil.h"


MYUTIL_API extern int n;

namespace my {
	namespace global {


		namespace errorHandler {
			typedef void (*ErrorHandler) (LPCTSTR msg);

			MYUTIL_API ErrorHandler						Get (void);
			MYUTIL_API void								Set (ErrorHandler);
		}

		namespace errorHolder {
			using codeshare::utilities::ErrorHolder;

			MYUTIL_API ErrorHolder&						Get (void);
			MYUTIL_API void								Set (ErrorHolder*);
		}

		namespace logger {
			MYUTIL_API GenericLogger&					Get (void);
			MYUTIL_API void								Set (GenericLogger*);
		}

	} // namespace global
} // namespace my

#endif // __MY__GLOBAL_INSTANCES__H__
