#ifndef __DEVIL_IMAGE_DECODER__H__
#define __DEVIL_IMAGE_DECODER__H__

#include "ImageDecoder.h"
#include "MyUtil.h"

namespace my				{
namespace image_decoders	{


class MYUTIL_API DevilImageDecoder: public ankh::images::ImageDecoder {
public:
	// ImageDecoder
	ankh::images::Image*				Decode (
												ankh::images::GenericReader&,
												ankh::images::ImageId const&,
												ankh::images::ImageFormatId const&,
												ankh::images::ImageSourceType const&);

	ankh::images::ImageFormatId const&	GetFormatId (void) const;
	bool								CanHandleFormat (ankh::images::ImageFormatId const&) const;

										DevilImageDecoder (void);
	virtual								~DevilImageDecoder (void);


	static void							Initialise (void);
	static void							CleanUp (void);
private:
	// No copying
										DevilImageDecoder (DevilImageDecoder const&);
	void								operator = (DevilImageDecoder const&);
};


} // image_decoders
} // my

#endif // __DEVIL_IMAGE_DECODER__H__