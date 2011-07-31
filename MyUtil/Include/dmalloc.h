#ifndef __DMALLOC_H__
#define __DMALLOC_H__

#include <DDebug.h>

template <typename T>
void dfree (T** const ptr) {
	{
		T* const	inst(*ptr);
		void* const	mem(POFFS(inst, -DDEBUG_USERSIZE_BYTES));

		DASSERT(!*((util_ui32*) mem));
		DASSERT(dprivatememsize(mem) >= DDEBUG_USERSIZE_BYTES + sizeof(*inst));

		//inst->~T();
		memset(mem, 0x00, sizeof(*inst) + DDEBUG_USERSIZE_BYTES);
		dprivatefree(mem);	// Memory disposal.
	}

	*ptr = NULL;
}

#endif // __DMALLOC_H__
