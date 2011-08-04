#include <stdafx.h>

// TODO
// make all types of image decoders wih DevIL


namespace _ {

	// Types
	struct Format {
		ILenum	il;
		char*	str;
	};

	// Constants
	static const unsigned int				NumberOfFormats		(3);
	static const char* const				DevilDecoderId		("[ImageDecoder/DevIL & NM//File Pointer]");
	static const char* const				SupportedFormatId	("");

	// State
	static bool								initialised(false);

	// Utils
	static ankh::images::ImageFormatId*		formatId(NULL);
	static ankh::images::ImageDecoderId*	devilDecoderId(NULL);
	//
	static Format							Formats[NumberOfFormats];
	//
#	define FOREACH_FORMAT					for (_::Format* i(&_::Formats[0]); i < &_::Formats[_::NumberOfFormats]; ++i)
	//
	ucmpstringfunc							streql;

	// Functions
	static inline
	ILenum TranslateFormat (char const* const fmt) {
		DASSERT(initialised);

		FOREACH_FORMAT
			if (strcmp(fmt, i->str) == 0)
				return i->il;

		DASSERT(false);
		return ILenum(-1);
	}

	static inline
	ankh::images::PixelFormat PixelFormatFor (ILenum const fmt) {
		switch (fmt) {
			case IL_PNG:	return  ankh::images::PixelFormats::RGB;
			case IL_TGA:	return  ankh::images::PixelFormats::RGB;
			case IL_JPG:	return	ankh::images::PixelFormats::RGB;
			default:		DASSERT(false); return ankh::images::PixelFormat(-1);
		}
	}

	static inline
	ILenum IlPixelFormatFor (ankh::images::PixelFormat const fmt) {
		switch (fmt) {
			case ankh::images::PixelFormats::RGB:	return	IL_RGB;
			default:								DASSERT(false); return GLenum(-1);
		}
	}

	template <typename T>
	static inline
	void eradicate (T*& ptr) {
		uzeromemory(ptr);
		ucalldestructor(ptr);
		DDELETE(ptr);
		unullify(ptr);
	}

	template <typename _IlLoder>
	static inline
	ankh::images::Image* LoadImageFromIlImage (
			ankh::images::ImageCharacteristics const&	imgchar,
			_IlLoder									loader) {
		using ankh::images::PixelFormat;
		using ankh::images::Image;

		ILuint const		ilImage(ilGenImage());
		ILenum const		ilfmt(TranslateFormat(imgchar.fmt.c_str()));

		ilBindImage(ilImage);
		ILboolean const ilImageLoaded(loader(ilfmt));
		DASSERT(ilImageLoaded == IL_TRUE);

		ILuint const		width	(ilGetInteger(IL_IMAGE_WIDTH));
		ILuint const		height	(ilGetInteger(IL_IMAGE_HEIGHT));
		PixelFormat const	pxlfmt	(PixelFormatFor(ilfmt));			// Ankh image pixel format
		ILenum const		ilpxlfmt(IlPixelFormatFor(pxlfmt));			// IL image pixel format

		Image* const image(DNEWCLASS(Image, (width, height, 1, imgchar.id, imgchar.fmt, pxlfmt, imgchar.src)));
		DASSERT(image);

		ilCopyPixels(0, 0, 0, width, height, 1, ilpxlfmt, IL_UNSIGNED_BYTE, DPTR(image)->GetBytes());

		ilDeleteImage(ilImage);

		return DPTR(image);
	}

	static inline
	ILboolean LoadF (ILenum const ilfmt, FILE* const fp) {
		return ilLoadF(ilfmt, fp);
	}

	static inline
	ILboolean LoadL (ILenum const ilfmt, void* const buf, ILuint const bytesize) {
		return ilLoadL(ilfmt, buf, bytesize);
	}
} // namespace _

#define FORMAT_INIT(I,IL,STR)				\
	{										\
		DASSERT( (I) < _::NumberOfFormats);	\
		_::Format& fmt(_::Formats[(I)]);	\
		fmt.il = IL;						\
		fmt.str = ucopystr(STR);			\
	}										\


// static

namespace my				{
namespace image_decoders	{

void DevilImageDecoder::Initialise (void) {
	DASSERT(!_::initialised);

	_::formatId = DNEWCLASS(ankh::images::ImageFormatId, (""));
	_::devilDecoderId = DNEWCLASS(ankh::images::ImageDecoderId, (_::DevilDecoderId));

	FORMAT_INIT(0, IL_TGA, "tga")
	FORMAT_INIT(1, IL_PNG, "png")
	FORMAT_INIT(2, IL_JPG, "jpg")

	_::initialised = true;
}

void DevilImageDecoder::CleanUp (void) {
	DASSERT(_::initialised);

	FOREACH_FORMAT {
		DDELARR(i->str);
		memset(&(*i), 0x00, sizeof(_::Format));
	}

	_::eradicate(_::devilDecoderId);
	_::eradicate(_::formatId);

	_::initialised = false;
}

DevilImageDecoder::DevilImageDecoder (void):
	ankh::images::ImageDecoder(*DPTR(_::devilDecoderId)),
	ankh::images::FilePointerImageDecoder(*DPTR(_::devilDecoderId)),
	ankh::images::BufferImageDecoder(*DPTR(_::devilDecoderId))
	{ }

DevilImageDecoder::~DevilImageDecoder (void) {
	DASSERTPTR(_::devilDecoderId);
	DASSERTPTR(_::formatId);
}

ankh::images::ImageFormatId const& DevilImageDecoder::GetFormatId (void) const {
	return *DPTR(_::formatId);
}

bool DevilImageDecoder::CanHandleFormat (ankh::images::ImageFormatId const& fmt) const {
	FOREACH_FORMAT
		if (_::streql(fmt.c_str(), DPTR(i->str)))
			return true;
	return false;
}


ankh::images::Image* DevilImageDecoder::Decode (FILE* const fp, ankh::images::ImageCharacteristics const& imgchar) {
	return _::LoadImageFromIlImage(imgchar, ubind2nd(uptr_fun(&_::LoadF), fp));
}

ankh::images::Image* DevilImageDecoder::Decode (void* const buf, size_t const bytesize, ankh::images::ImageCharacteristics const& imgchar) {
	std::string sig(upig( uptr_fun(&_::LoadL) ) );

	return _::LoadImageFromIlImage(imgchar, ubind2nd(ubind2nd(uptr_fun(&_::LoadL), buf), bytesize));
}


}	// image_decoders
}	// my
