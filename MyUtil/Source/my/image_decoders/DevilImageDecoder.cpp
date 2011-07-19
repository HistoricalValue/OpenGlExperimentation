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
	static ankh::images::ImageFormatId	formatId("everything");
	static char							_Formats_memory[sizeof(Format) * NumberOfFormats];
	static Format						(&Formats)[NumberOfFormats](reinterpret_cast<Format (&)[NumberOfFormats]>(_Formats_memory));
#	define FOREACH_FORMAT				for (_::Format* i(&_::Formats[0]); i < &_::Formats[_::NumberOfFormats]; ++i)

	// Functions
	static ILenum TranslateFormat (char const* const fmt) {
		FOREACH_FORMAT
			if (strcmp(fmt, i->str) == 0)
				return i->il;
		DASSERT(false);
		return ILenum(-1);
	}
}

#define FORMAT_INIT(I,IL,STR)				\
	{										\
		DASSERT( (I) < _::NumberOfFormats);	\
		Format& fmt(_::Formats[(I)]);		\
		fmt.il = IL;						\
		fmt.str = strdup(STR);				\
	}										\


// static
void my::image_decoders::DevilImageDecoder::Initialise (void) {
	DASSERT(!initialised);

	FORMAT_INIT(0, IL_TGA, "tga")
	FORMAT_INIT(1, IL_PNG, "png")

	initialised = true;
}

void my::image_decoders::DevilImageDecoder::CleanUp (void) {
	DASSERT(initialised);

	FOREACH_FORMAT {
		free(i->str);
		memset(&(*i), 0x00, sizeof(_::Format));
	}

	initialised = false;
}

my::image_decoders::DevilImageDecoder::DevilImageDecoder (void):
	ankh::images::ImageDecoder("Devil Image Decoder")
	{ }


ankh::images::ImageFormatId const& my::image_decoders::DevilImageDecoder::GetFormatId (void) const {
	return _::formatId;
}

bool my::image_decoders::DevilImageDecoder::CanHandleFormat (ankh::images::ImageFormatId const& fmt) const {
	return true;
}

ankh::images::Image* my::image_decoders::DevilImageDecoder::Decode(
		ankh::images::GenericReader&			r,
		ankh::images::ImageId const&			imgid,
		ankh::images::ImageFormatId const&		fmtid,
		ankh::images::ImageSourceType const&	src) {

	r.read(
	
}