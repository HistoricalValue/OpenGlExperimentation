#include "stdafx.h"

#define ASSERT_INITIALISED	PASSERT(*DPTR(_DNOTNULL(_::initialised)))

namespace	{
namespace _	{

size_t const	CHUNKLEN(1 << 29); // 512MB

static bool*	initialised(NULL);
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

}	// _
}	//

namespace my				{
namespace gl				{
namespace shapes			{
namespace ShapeUtilities	{

void Initialise (void) {
	PASSERT(_::initialised == NULL)
	PASSERT(_::head == NULL)
	PASSERT(_::next == NULL)
	PASSERT(_::left == 0)

	*DPTR(_DNOTNULL(_::initialised = DNEW(bool))) = true;

	_::head = reinterpret_cast<_::allocation*>(DNEW(util_ui8[_::CHUNKLEN]));
	
	P_STATIC_ASSERT(sizeof(*_::head) < _::CHUNKLEN * sizeof(util_ui8))
	Reset();
}

void CleanUp (void) {
	ASSERT_INITIALISED
	Reset();

	_::left = 0;
	_::next = NULL;
	udelete(_::head);

	*_::initialised = false;
	udelete(_::initialised);
}

void Reset (void) {
	ASSERT_INITIALISED
	PASSERT(_::allocations == 0)

	_::next = _::alInit(_::head);
	_::left = _::CHUNKLEN - sizeof(_::allocation);
}

void* _ShapeAlloc (size_t const bytesize) {
	ASSERT_INITIALISED
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
//	return PASSERT_EXPR(*DPTR(_DNOTNULL(_::initialised))), DNEWARR(util_ui8, bytesize);
}

void DisposeClonedShape (Shape* const shape) {
	ASSERT_INITIALISED

	// Memory validation
	PASSERT(_::alValidate(shape))
	PASSERT(_::alGet(shape)->allocated)
	
	shape->~Shape();
	_::alGet(shape)->allocated = false;

	if (--_::allocations == 0)
		Reset();

//	DPTR(_DNOTNULL(shape))->~Shape();
//	DDELARR(reinterpret_cast<util_ui8* const>(shape));
}

}	// ShapeUtilities
}	// shapes
}	// gl
}	// my

