#include "stdafx.h"
#include <drawing_setup_textures.h>
#include <drawing_setup_geometrical.h>

///////////////////////////////////////////////////////

namespace _ {

///////////////////////////////////////////////////////

void PlayWithTextureUnitsForTesting (void) {
	using namespace ankh;
	textures::TextureUnitManager& tum(textures::TextureUnitManager::GetSingleton());

	textures::TextureUnit& tu00(tum.Get(textures::TextureUnitIds::TEXTURE0 ));
	textures::TextureUnit& tu17(tum.Get(textures::TextureUnitIds::TEXTURE17));
	textures::TextureUnit& tu23(tum.Get(textures::TextureUnitIds::TEXTURE23));
	textures::TextureUnit& tu30(tum.Get(textures::TextureUnitIds::TEXTURE30));
	// Error
//	textures::TextureUnit& tu37(tum.Get(textures::TextureUnitId(37)));

	tu17.Activate();
	DASSERT(!tu23.IsActive());

	tu30.Activate();
	DASSERT(!tu17.IsActive());

	tu23.Activate();
	DASSERT(!tu17.IsActive());
	DASSERT(!tu30.IsActive());

	tu23.Deactivate();
	DASSERT(!tu23.IsActive());

	tu17.Activate();
	DASSERT(!tu23.IsActive());

	tu30.Activate();
	DASSERT(!tu17.IsActive());

	tu23.Activate();
	DASSERT(!tu17.IsActive());
	DASSERT(!tu30.IsActive());
	// Error
//	tu17.Deactivate();

	tu23.Deactivate();
	DASSERT(tu00.IsActive());
	// active-by-default texture ID can be deactivated as many times
	// as one wants -- it will still be active.
	tu00.Deactivate();
	tu00.Deactivate();
	tu00.Deactivate();
	DASSERT(tu00.IsActive());
//	tu00.Activate();
	tu17.Activate();
	tu00.Activate();
	tu00.Deactivate();
}

///////////////////////////////////////////////////////

void CreateTextures (
		ImagesArray const&	images,
		TexturesArray&		textures,
		size_t&				previousTextureIndex)
{
	// Save setup time
	if (_::WITH_DRAW_TEXTURED) {
		using namespace ankh::textures;

		TextureUnitManager&	tum	(TextureUnitManager	::GetSingleton());
		TextureManager&		tm	(TextureManager		::GetSingleton());

		textures[0] = tm.New("Numberwng", images[0]);
		textures[1] = tm.New("Pacco", images[1]);

		textures[0]->BindTo(tum.Get(_::TexturesUnits[0]));
		textures[1]->BindTo(tum.Get(_::TexturesUnits[1]));

		textures[1]->SetMinFilter(GL_NEAREST);

		previousTextureIndex = 0;
	}
}

///////////////////////////////////////////////////////

void DestroyTextures (TexturesArray& textures) {
	if (WITH_DRAW_TEXTURED)
		foreach(textures, std::bind1st(std::mem_fun(&ankh::textures::TextureManager::Delete), ankh::textures::TextureManager::GetSingletonPtr()));
}

///////////////////////////////////////////////////////

}

///////////////////////////////////////////////////////