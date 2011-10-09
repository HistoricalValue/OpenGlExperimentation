#include "stdafx.h"

#include <my/gl/shapes/ShapeUtilities.h>

#define WITH_LAME_THINGS 0

#define ASSERT_INITIALISED	PASSERT(*DPTR(_DNOTNULL(_::initialised)))

namespace	{
namespace _	{

static bool*	initialised(NULL);

#if WITH_LAME_THINGS == 1
size_t const	CHUNKLEN(1 << 29); // 512MB
static size_t	allocations(0);

static struct	allocation {
	allocation* next;
	void* mymem;
	bool allocated;
}* head(NULL), * next(NULL);
static size_t left(0);
#ifdef _DEBUG
size_t const sz_allocation(sizeof(allocation));
#endif

static inline allocation* alInit (void* const self)
	{ return	reinterpret_cast<allocation* const>(self)->next = NULL,
				reinterpret_cast<allocation* const>(self)->mymem = codeshare::utilities::pointer_utilities::offset(self, sizeof(allocation)),
				reinterpret_cast<allocation* const>(self)->allocated = false,
				reinterpret_cast<allocation* const>(self); }
static inline allocation* alGet (void* const mem)
	{ return	reinterpret_cast<allocation* const>(codeshare::utilities::pointer_utilities::offset(mem, -ptrdiff_t(sizeof(allocation)))); }
static inline bool alValidate (void* const mem)
	{ return	PASSERT_EXPR(codeshare::utilities::pointer_utilities::offset(head, sizeof(allocation)) <= mem),
				PASSERT_EXPR(mem < codeshare::utilities::pointer_utilities::offset(head, CHUNKLEN)),
				PASSERT_EXPR(alGet(mem)->mymem == mem),
				true; }
#endif // WITH_LAME_THINGS

}	// _
}	//

namespace my				{
namespace gl				{
namespace shapes			{
namespace ShapeUtilities	{

void Initialise (void) {
	PASSERT(_::initialised == NULL)

#if WITH_LAME_THINGS == 1
	PASSERT(_::head == NULL)
	PASSERT(_::next == NULL)
	PASSERT(_::left == 0)

	_::head = reinterpret_cast<_::allocation*>(DNEW(util_ui8[_::CHUNKLEN]));
	
	P_STATIC_ASSERT(sizeof(*_::head) < _::CHUNKLEN * sizeof(util_ui8))
#endif // WITH_LAME_THINGS

	*DPTR(_DNOTNULL(_::initialised = DNEW(bool))) = true;
	Reset();
}

void CleanUp (void) {
	ASSERT_INITIALISED
	Reset();

#if WITH_LAME_THINGS == 1
	_::left = 0;
	_::next = NULL;
	udelete(_::head);
#endif // WITH_LAME_THINGS

	*_::initialised = false;
	udelete(_::initialised);
}

void Reset (void) {
	ASSERT_INITIALISED

#if WITH_LAME_THINGS == 1
	PASSERT(_::allocations == 0)

	_::next = _::alInit(_::head);
	_::left = _::CHUNKLEN - sizeof(_::allocation);
#endif // WITH_LAME_THINGS
}

void* _ShapeAlloc (size_t const bytesize) {
	ASSERT_INITIALISED
#if WITH_LAME_THINGS == 1
	PASSERT(_DNOTNULL(_::head))
	PASSERT(_DNOTNULL(_::next))
	PASSERT(_::next->next == NULL)

	using codeshare::utilities::pointer_utilities::offset;

	bool const fits(bytesize <= _::left);
	PASSERT(fits)

	void* result(NULL);
	if (fits) {
		result = _::next->mymem;
		_::next->allocated = true;
		_::next = _::next->next = _::alInit(offset(_::next, sizeof(_::allocation) + bytesize));
		_::left -= bytesize + sizeof(_::allocation);
		++_::allocations;
	}

	return result;
#else // !WITH_LAME_THINGS
	PASSERT(*DPTR(_DNOTNULL(_::initialised)))
	return DNEWARR(util_ui8, bytesize);
#endif // WITH_LAME_THINGS
}

void DisposeClonedShape (Shape* const shape) {
	ASSERT_INITIALISED

#if WITH_LAME_THINGS == 1
	// Memory validation
	PASSERT(_::alValidate(shape))
	PASSERT(_::alGet(shape)->allocated)
	
	shape->~Shape();
	_::alGet(shape)->allocated = false;

	if (--_::allocations == 0)
		Reset();
#else // !WITH_LAME_THINGS
	DPTR(_DNOTNULL(shape))->~Shape();
	DDELARR(reinterpret_cast<util_ui8* const>(shape));
#endif // WITH_LAME_THINGS
}

}	// ShapeUtilities
}	// shapes
}	// gl
}	// my

