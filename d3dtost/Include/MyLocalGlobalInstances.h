#ifndef __MY__LOCAL_GLOBAL_INSTANCES__H__
#define __MY__LOCAL_GLOBAL_INSTANCES__H__

#include <MyConsole.h>

namespace my {
	namespace global {


		namespace log {
			extern void info (LPCTSTR);
			extern void infoA (char const*);
			extern void warning (LPCTSTR);
			extern void error (LPCTSTR);
		} // namespace log

		my::Console& GetConsole (void);

	} // namespace global
} // namespace my

#endif // __MY__LOCAL_GLOBAL_INSTANCES__H__
