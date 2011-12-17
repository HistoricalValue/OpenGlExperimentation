#include "stdafx.h"
#include <drawing_setup_ankh.h>
#include "DevilImageDecoder.h"

///////////////////////////////////////////////////////

namespace _	{

using ankh::textures::TextureUnitManager;
using ankh::images::ImageLoader;
using ankh::third::images::DevilImageDecoder;
using ankh::images::ImageDecoder;
using ankh::textures::TextureManager;

///////////////////////////////////////////////////////

struct DevilImageDecoderPlaceholder { unsigned char data[sizeof(DevilImageDecoder)]; } _devil;
DevilImageDecoder& devil (void) { return reinterpret_cast<DevilImageDecoder&>(_devil); }

///////////////////////////////////////////////////////

void InitialiseAnkh (void) {
	TextureUnitManager::SingletonCreate();
	
	DevilImageDecoder::Initialise();
	DevilImageDecoder& devil(_::devil());
	new(&devil) DevilImageDecoder;

	ImageLoader::SingletonCreate();
	ImageLoader& il(ImageLoader::GetSingleton());
	il.InstallDecoder(devil.GetBufferImageDecoder());
	il.InstallDecoder(devil.GetFilePointerImageDecoder());

	TextureManager::SingletonCreate();
}

void CleanUpAnkh (void) {
	TextureManager::SingletonDestroy();

	ImageLoader::SingletonDestroy();

	devil().~DevilImageDecoder();
	DevilImageDecoder::CleanUp();

	TextureUnitManager::SingletonDestroy();
}

///////////////////////////////////////////////////////

}	//	_

///////////////////////////////////////////////////////
