#ifndef __DRAWING__SETUP_TEXTURES__H__
#define __DRAWING__SETUP_TEXTURES__H__

#include <drawing.h>

///////////////////////////////////////////////////////

namespace _ {

///////////////////////////////////////////////////////

extern void        PlayWithTextureUnitsForTesting (void);
extern void        CreateTextures (
                        ImagesArray const&    images,
                        TexturesArray&        textures,
                        size_t&                previousTextureIndex);
extern void        DestroyTextures (TexturesArray& textures);

///////////////////////////////////////////////////////

}

///////////////////////////////////////////////////////

#endif // __DRAWING__SETUP_TEXTURES__H__
