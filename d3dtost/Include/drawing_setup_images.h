#ifndef __DRAWING__SETUP_IMAGES__H__
#define __DRAWING__SETUP_IMAGES__H__

#include <drawing.h>

///////////////////////////////////////////////////////

namespace _ {

///////////////////////////////////////////////////////

static
void LoadTehStonets (ImagesArray& images)
{
	// Save LOTS of setup time
	if (_::WITH_DRAW_TEXTURED) {
		using namespace ankh::images;

		ImageLoader& il(ImageLoader::GetSingleton());

		images[0] = il.LoadFromPaths("../textures/digitframes", 11, "png");	// gets loaded with Devil

		size_t bufbytesize;
	#if 0
		void* const buf(uloadbinaryfile("../textures/paccy.png", &bufbytesize));
		images[1] = il.Load3DFromData(32, "pacco", "png", buf, bufbytesize);
	#else
		void* const buf(uloadbinaryfile("../textures/1508-stark-winter-is-coming-wallpaper-wallchan-1920x1200.jpg", &bufbytesize));
		images[1] = il.LoadFromData("pacco", "jpg", buf, bufbytesize);
	#endif
		delete[] buf;
	}
}

static
void UnloadTehStonets (ImagesArray& images) {
	if (WITH_DRAW_TEXTURED)
		foreach(images, std::bind1st(std::mem_fun(&ankh::images::ImageLoader::Unload), ankh::images::ImageLoader::GetSingletonPtr()));
}

///////////////////////////////////////////////////////

}

///////////////////////////////////////////////////////

#endif // __DRAWING__SETUP_IMAGES__H__

