#include <stdafx.h>
#include <my/gl/adapters/Buffer.h>
#include <nmutil/OpenGlExtensionManager.h>

using namespace ::gl::ext;

namespace my { namespace gl { namespace adapters {

void Buffer::Bind (void) const
	{ glBindBuffer(GL_ARRAY_BUFFER, id); }

void Buffer::Unbind (void) const
	{ PASSERT(IsBound()) glBindBuffer(GL_ARRAY_BUFFER, 0); }

bool Buffer::IsBound (void) const {
	GLint boundBufferId(-1);
	glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &boundBufferId);
	PASSERT(boundBufferId > -1);
	return GLuint(boundBufferId) == id;
}

size_t Buffer::Add (Entry const& entry) {
	PASSERT(InvariantsHold())
	PASSERT(!IsCommitted())

	size_t const elementBytesize(entry.bytesize / entry.numberOfElements);
	PASSERT(elementBytesize * entry.numberOfElements == entry.bytesize)	// no interger rounding

	size_t const alignmentPaddingComplement(totalBytesize % elementBytesize);
	if (alignmentPaddingComplement != 0)
		totalBytesize += elementBytesize - alignmentPaddingComplement;
	PASSERT(totalBytesize % elementBytesize == 0)

	size_t const offset(totalBytesize);
	totalBytesize += entry.bytesize;

	entries.push_back(entry.WithOffset(offset));

	return offset;
}

void Buffer::Clear (void)  {
	PASSERT(InvariantsHold())
	if (!IsCommitted()) {
		Entries::const_iterator const entries_end(entries.end());
		for (Entries::const_iterator i(entries.begin()); i != entries_end; ++i)
			i->deleter->operator ()(i->data);
		entries.clear();
	}
}

void Buffer::Commit (void) {
	PASSERT(InvariantsHold())
	PASSERT(!IsCommitted())

	Bind();

	glBufferData(GL_ARRAY_BUFFER, totalBytesize, NULL, GL_STATIC_DRAW);

	Entries::const_iterator const entries_end(entries.end());
	for (Entries::const_iterator i(entries.begin()); i != entries_end; ++i) {
		PASSERT(i->offset + i->bytesize <= totalBytesize)

		glBufferSubData(GL_ARRAY_BUFFER, i->offset, i->bytesize, i->data);

		(*i->deleter)(i->data);
	}

	entries.clear();

	Unbind();

	committed = true;
};


}}} // my::gl::adapters
