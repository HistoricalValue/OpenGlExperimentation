#include "stdafx.h"

namespace _ {
	
	// Types
	struct Format {
		ILenum	il;
		char*	str;
	};

	// Constants
	static const unsigned int NumberOfFormats(2);
	
	// State
	static bool							initialised(false);
	//
	static char							_formatId_memory[sizeof(ankh::images::ImageFormatId)];
	static ankh::images::ImageFormatId&	formatId(*reinterpret_cast<ankh::images::ImageFormatId* const>(_formatId_memory));
	//
	static char							_Formats_memory[sizeof(Format) * NumberOfFormats];
	static Format						(&Formats)[NumberOfFormats](reinterpret_cast<Format (&)[NumberOfFormats]>(_Formats_memory));
	//
#	define FOREACH_FORMAT				for (_::Format* i(&_::Formats[0]); i < &_::Formats[_::NumberOfFormats]; ++i)

	// Functions
	static ILenum TranslateFormat (char const* const fmt) {
		DASSERT(initialised);

		FOREACH_FORMAT
			if (strcmp(fmt, i->str) == 0)
				return i->il;
		DASSERT(false);
		return ILenum(-1);
	}
} // namespace _

#define FORMAT_INIT(I,IL,STR)				\
	{										\
		DASSERT( (I) < _::NumberOfFormats);	\
		_::Format& fmt(_::Formats[(I)]);	\
		fmt.il = IL;						\
		fmt.str = _strdup(STR);				\
	}										\


// static
void my::image_decoders::DevilImageDecoder::Initialise (void) {
	DASSERT(!_::initialised);

	new(_::_formatId_memory) ankh::images::ImageFormatId("");

	FORMAT_INIT(0, IL_TGA, "tga")
	FORMAT_INIT(1, IL_PNG, "png")

	_::initialised = true;
}

void my::image_decoders::DevilImageDecoder::CleanUp (void) {
	DASSERT(_::initialised);

	FOREACH_FORMAT {
		free(i->str);
		memset(&(*i), 0x00, sizeof(_::Format));
	}

	nmutil::Destruct(_::formatId);

	_::initialised = false;
}

my::image_decoders::DevilImageDecoder::DevilImageDecoder (void):
	ankh::images::ImageDecoder("Devil Image Decoder")
	{ }

my::image_decoders::DevilImageDecoder::~DevilImageDecoder (void) {
}

ankh::images::ImageFormatId const& my::image_decoders::DevilImageDecoder::GetFormatId (void) const {
	return _::formatId;
}

bool my::image_decoders::DevilImageDecoder::CanHandleFormat (ankh::images::ImageFormatId const& fmt) const {
	FOREACH_FORMAT
		if (strcmp(fmt.c_str(), i->str) == 0)
			return true;
	return false;
}

ankh::images::Image* my::image_decoders::DevilImageDecoder::Decode(
		ankh::images::GenericReader&			r,
		ankh::images::ImageId const&			imgid,
		ankh::images::ImageFormatId const&		fmtid,
		ankh::images::ImageSourceType const&	src) {

	void* buf(NULL);
	size_t bufLength(0);
	nmutil::ReadBuffer(r, std::ptr_fun(&nmutil::dmemalloc), buf, bufLength);
	
	// decoder image with DevIL
	ankh::images::Image* image(NULL);
	{
		ILuint ilImage(ilGenImage());
		ilBindImage(ilImage);
		ILboolean ilImageLoaded(ilLoadL(_::TranslateFormat(fmtid.c_str()), buf, bufLength));
		DASSERT(ilImageLoaded == IL_TRUE);

		ILuint width(ilGetInteger(IL_IMAGE_WIDTH));
		ILuint height(ilGetInteger(IL_IMAGE_HEIGHT));

		image = (DNEWCLASS(ankh::images::Image, (width, height, 1, imgid, fmtid, ankh::images::PixelFormats::RGB, src)));

		ilCopyPixels(0, 0, 0, width, height, 1, IL_RGB, IL_UNSIGNED_BYTE, image->GetBytes());

		ilDeleteImage(ilImage);
	}

	nmutil::dfree(buf);

	return image;
}