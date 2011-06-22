#ifdef __MINIUTIL__LIST__H__BEING_INCLUDED__
#	error "Circular inclusion"
#endif
#define __MINIUTIL__LIST__H__BEING_INCLUDED__

#ifndef __MINIUTIL__LIST__H__INCLUDED__
#define __MINIUTIL__LIST__H__INCLUDED__

#include <list>

#include "miniutil/memory.h"

namespace miniutil {

	template <typename T, typename Allocator = allocator>
	class list: public std::list<T, Allocator> {
	public:
														list (void):
															std::list()
															{ }
	};
	
} // namespace miniutil

#endif //	__MINIUTIL__LIST__H__INCLUDED__
#undef		__MINIUTIL__LIST__H__BEING_INCLUDED__
