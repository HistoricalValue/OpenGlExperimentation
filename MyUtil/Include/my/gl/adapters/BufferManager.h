#ifndef __MY_UTIL__MY__GL__ADAPTERS__BUFFER_MANAGER__H__
#define __MY_UTIL__MY__GL__ADAPTERS__BUFFER_MANAGER__H__

#include <my/gl/adapters/Buffer.h>
#include <MyUtil.h>
#include <nmutil/SharedResourceManager.h>

#pragma warning( push, 0 )
#	include <ufunctors.h>
#pragma warning( pop )

namespace my { namespace gl { namespace adapters {

// linkage specified in forward declaration
class BufferManager {
public:

	bool		IsCreated (GLuint id) const;
	Buffer*		Create (void);
	Buffer*		Get (GLuint id) const;
	void		Release (Buffer*);

	void		Create (Buffer** buffers, size_t n);
	template <const size_t N>
	void		Create (Buffer* (&buffers)[N])
					{ Create(&buffers[0], N); }

	void		Release (Buffer** buffers, size_t n);
	template <const size_t N>
	void		Release (Buffer* (&buffers)[N])
					{ Release(&buffers[0], N); }

	USINGLETON_APISTYLE_DECLARE_PUBLICSTDMETHODS
	USINGLETON_APISTYLE_DECLARE_GETTER(BufferManager)

	BufferManager (void);
private:

	static Buffer*	NewBuffer (GLuint id);

	typedef	GLuint	K;
	typedef Buffer	T;

	static T*		CreateBuffer (K);	// Never used, buffers always registered
	static void		DeleteBuffer (T*);
	static bool		ValidateBuffer (K, T*);

	typedef uspecific_pointer_to_unary_function<T*, K, &CreateBuffer>			Creator;
	typedef uspecific_pointer_to_unary_function<void, T*, &DeleteBuffer>		Deleter;
	typedef uspecific_pointer_to_binary_function<bool, K, T*, &ValidateBuffer>	Validator;

	typedef nmutil::SharedResourceManager<T, K, Creator, Deleter, Validator> SRM;

	SRM	srm;
};

}}} // my::gl::adapters

#endif // __MY_UTIL__MY__GL__ADAPTERS__BUFFER_MANAGER__H__
