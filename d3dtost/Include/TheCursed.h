// This header file includes all header files
// which directly or indirectly result in
// PWindows.h being included.
//
// PWindows.h includes Windows.h and then ruins some
// of its effects. So, if some file needs the original
// Windows functionality, it has to stay uncontaminated
// from the PWindows.h inclusion.
// 
// Therefore, The Cursed shoul never be included in
// other header files in fear of including them in 
// sources that want to aboid PWindows.h. Those who need
// The Cursed should only include them locally, in
// a source file.

#ifndef __THE_CURSED__H__
#define __THE_CURSED__H__

// Pointless including of those two, just to signify
// that they are not to be found elsewhere.
#include <Windows.h>

#include "../Include/TheCursed/MyDirect3D9.h"
#include "../Include/TheCursed/MyConsole.h"
#include "../Include/TheCursed/MyWindow.h"
#include "../Include/TheCursed/MyLocalGlobalInstances.h"
#include "../Include/TheCursed/MyOpenGL.h"
#include "../Include/TheCursed/Setup.h"

#endif // __THE_CURSED__H__

