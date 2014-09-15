#ifndef __MY__LOCAL_GLOBAL_INSTANCES__H__
#define __MY__LOCAL_GLOBAL_INSTANCES__H__

#include "TheCursed.h"

namespace my {
    namespace global {


        namespace log {
            extern void info (TCHAR const*);
            extern void infoA (char const*);
            extern void warning (TCHAR const*);
            extern void error (TCHAR const*);
        } // namespace log

        my::Console& GetConsole (void);

    } // namespace global
} // namespace my

#endif // __MY__LOCAL_GLOBAL_INSTANCES__H__
