#ifndef __DRAWING__SETUP_ANKH__H__
#define __DRAWING__SETUP_ANKH__H__

#include <ImageLoader.h>
#include <TextureManager.h>

///////////////////////////////////////////////////////

namespace _	{

///////////////////////////////////////////////////////

static
void InitialiseAnkh (void) {
// initialise Images and Textures lib
	{	bool const success(ankh::images::Initialise());
		PASSERT(success) }
	{	bool const success(ankh::textures::Initialise());
		PASSERT(success) }

	ankh::images::InstallDefaultImageDecoders();
}

static
void CleanUpAnkh (void) {
	ankh::textures::CleanUp();
	ankh::images::CleanUp();
}

///////////////////////////////////////////////////////

}	//	_

///////////////////////////////////////////////////////

#endif // __DRAWING__SETUP_ANKH__H__
