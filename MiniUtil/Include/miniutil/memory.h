#ifdef __MINIUTIL__MEMORY__H__BEING_INCLUDED__
#	error "Circular inclusion"
#endif
#define __MINIUTIL__MEMORY__H__BEING_INCLUDED__

#ifndef __MINIUTIL__MEMORY__H__INCLUDED__
#define __MINIUTIL__MEMORY__H__INCLUDED__

#include <memory>

namespace miniutil {

	template <typename T>
	class allocator: public std::allocator<T> {	
	public:
														allocator (void):
															std::allocator()
															{ }
	}; // class allocator

} // namespace miniutil

#endif //	__MINIUTIL__MEMORY__H__INCLUDED__
#undef		__MINIUTIL__MEMORY__H__BEING_INCLUDED__

