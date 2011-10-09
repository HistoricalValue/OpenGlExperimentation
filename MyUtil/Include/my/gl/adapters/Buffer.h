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

		Entry (void* const _data, size_t const _numberOfElements, size_t const _bytesize, Deleter* const _deleter):
			data(_data), numberOfElements(_numberOfElements), bytesize(_bytesize), deleter(_deleter) {}
		Entry (Entry const& e): data(e.data), numberOfElements(e.numberOfElements), bytesize(e.bytesize), deleter(e.deleter) {}
	};

	size_t		Add (Entry const& entry);
	size_t		Add (GLvoid* const data, size_t const numberOfElements, size_t const bytesize, Deleter* const deleter)
					{ return Add(Entry(data, numberOfElements, bytesize, deleter)); }

	bool		IsCommitted (void) const
					{ return committed; }

	void		Commit (void);
	

private:
	friend class BufferManager;
	Buffer (GLuint const _id):
		id(_id),
		offset(0),
		entries(),
		committed(false),
		totalBytesize(0u)
		{}
	Buffer (Buffer const&);				// no copying
	void operator = (Buffer const&);	// no assigning

	typedef std::list<Entry>	Entries;

	GLuint const			id;
	size_t					offset;
	#pragma warning( push )
	#pragma warning( disable: 4251 )
	Entries					entries;
	#pragma warning( pop )
	bool					committed;
	size_t					totalBytesize;

	~Buffer (void) {
		Entries::const_iterator const entries_end(entries.end());
		for (Entries::const_iterator i(entries.begin()); i != entries_end; ++i)
			i->deleter->operator ()(i->data);
	}
};

// linkage specified in forward declaration
class BufferManager {
public:
	USINGLETON_APISTYLE_DECLARE_PUBLICSTDMETHODS
	USINGLETON_APISTYLE_DECLARE_GETTER(BufferManager)
private:
};

}}} // my::gl::adapters

#endif // __MY_UTIL__MY__GL__ADAPTERS__BUFFER__H__
