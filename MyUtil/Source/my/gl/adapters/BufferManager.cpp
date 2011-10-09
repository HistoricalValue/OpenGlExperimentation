#include <stdafx.h>
#include <my/gl/adapters/BufferManager.h>
#include <nmutil/SharedResourceManager_inl.h>
#include <nmutil/OpenGlExtensionManager.h>

using namespace ::my::gl::adapters;

struct BufferPlaceholder {
	char bytes[sizeof(my::gl::adapters::Buffer)];
};
static BufferManager* singleton = NULL;

namespace my { namespace gl { namespace adapters {

// Singleton
void BufferManager::SingletonCreate (void)
	{ unew(singleton); }

void BufferManager::SingletonDestroy (void)
	{ udelete(singleton); }

BufferManager& BufferManager::GetSingleton (void)
	{ return *DPTR(DNULLCHECK(singleton)); }

// Constructors/Destructors
BufferManager::BufferManager (void):
	srm	(Deleter(), Validator())
	{}

// Resource Manager 
void BufferManager::DeleteBuffer (T* const v)
	{ v->~Buffer(); DDELETE(reinterpret_cast<BufferPlaceholder* const>(v)); }

bool BufferManager::ValidateBuffer (K const k, T* const v)
	{ return v->GetId() == k; }

// BufferManager
Buffer* BufferManager::Create (void) {
	GLuint id(-1);
	::gl::ext::glGenBuffers(1, &id);
	PASSERT(id != -1)

	Buffer* const buffer(reinterpret_cast<Buffer* const>(DNEW(BufferPlaceholder)));
	new(buffer) Buffer(id);

	srm.Register(id, buffer);

	return buffer;
}

void BufferManager::Create (Buffer** const buffers, size_t const n) {
	for (Buffer** buffer = buffers; buffer < buffers + n; ++buffer)
		*buffer = Create();
}

void BufferManager::Release (Buffer* const buffer)
	{ srm.Unuse(buffer); }

void BufferManager::Release (Buffer** const buffers, size_t const n) {
	for (Buffer** buffer = buffers; buffer < buffers + n; ++buffer)
		Release(*buffer);
}

}}} // my::gl::adapters
