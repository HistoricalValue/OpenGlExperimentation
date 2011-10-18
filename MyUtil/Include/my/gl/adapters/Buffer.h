#ifndef __MY_UTIL__MY__GL__ADAPTERS__BUFFER__H__
#define __MY_UTIL__MY__GL__ADAPTERS__BUFFER__H__

#include <MyUtil.h>
#include <usingleton.h>
#include <MyOpenGLUtils.h>

namespace my { namespace gl { namespace adapters {


class MYUTIL_API BufferManager;

class MYUTIL_API Buffer {
public:

	class MYUTIL_API Deleter {
	public:
		virtual void operator () (GLvoid*) = 0;
		virtual ~Deleter (void) {}
	};

	struct MYUTIL_API Entry {
		void* const		data;
		size_t const	numberOfElements;
		size_t const	bytesize;
		Deleter* const	deleter;
		ptrdiff_t const	offset;		// offset in buffer, after aligning

		Entry	WithOffset(ptrdiff_t const offset) const
					{ return Entry(data, numberOfElements, bytesize, deleter, offset); }

		Entry (void* const _data, size_t const _numberOfElements, size_t const _bytesize, Deleter* const _deleter, ptrdiff_t const _offset = -1):
			data(_data), numberOfElements(_numberOfElements), bytesize(_bytesize), deleter(_deleter), offset(_offset) {}
		Entry (Entry const& e): data(e.data), numberOfElements(e.numberOfElements), bytesize(e.bytesize), deleter(e.deleter), offset(e.offset) {}
		UOVERLOADED_VOID_ASSIGN_VIA_COPY_CONSTRUCTOR(Entry)
	};

	size_t		Add (Entry const& entry);
	size_t		Add (GLvoid* const data, size_t const numberOfElements, size_t const bytesize, Deleter* const deleter)
					{ return Add(Entry(data, numberOfElements, bytesize, deleter)); }

	bool		IsCommitted (void) const
					{ return committed; }

	void		Commit (void);

	bool		InvariantsHold (void) const
					{ return (!IsCommitted() || entries.size() == 0); }

	void		Clear (void);

	GLuint		GetId (void) const
					{ return id; }

	void		Bind (void) const;
	bool		IsBound (void) const;
	void		Unbind (void) const;

private:
	friend class BufferManager;
	Buffer (GLuint const _id):
		id(_id),
		entries(),
		committed(false),
		totalBytesize(0u)
		{}
	Buffer (Buffer const&);				// no copying
	void operator = (Buffer const&);	// no assigning

	typedef std::list<Entry>	Entries;

	GLuint const			id;
	#pragma warning( push )
	#pragma warning( disable: 4251 )
	Entries					entries;
	#pragma warning( pop )
	bool					committed;
	size_t					totalBytesize;

	~Buffer (void) {
		PASSERT(InvariantsHold())

		if (!IsCommitted())
			Clear();
	}
};

}}} // my::gl::adapters

#endif // __MY_UTIL__MY__GL__ADAPTERS__BUFFER__H__
