#include <stdafx.h>
#include <my/gl/adapters/Buffer.h>
#include <nmutil/OpenGlExtensionManager.h>

using namespace ::gl::ext;

namespace my { namespace gl { namespace adapters {

size_t Buffer::Add (Entry const& entry) {
	DASSERT(!IsCommitted());
	entries.push_back(entry);

	size_t const elementBytesize(entry.bytesize / entry.numberOfElements);
	PASSERT(elementBytesize * entry.numberOfElements == entry.bytesize)	// no interger rounding

	size_t const alignmentPadding(totalBytesize % elementBytesize);
	totalBytesize += alignmentPadding;

	size_t const offset(totalBytesize);
	totalBytesize += entry.bytesize;

	return offset;
}


void Buffer::Commit (void) {
	PASSERT(glIsBuffer(id))
};


}}} // my::gl::adapters
