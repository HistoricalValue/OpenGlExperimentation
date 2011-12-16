#ifndef __DRAWING__SETUP_GEOMETRICAL__H__
#define __DRAWING__SETUP_GEOMETRICAL__H__

#include "stdafx.h"
#include <drawing.h>

///////////////////////////////////////////////////////////

namespace _ {

///////////////////////////////////////////////////////////

///////////////////////////////////////////////////////

static const GLuint TexturesUnits[NUMBER_OF_TEXTURES] =
	{ 12, 23 };

///////////////////////////////////////////////////////

extern float GetRotationAngle (unsigned long int const dt_milli);
extern GLuint GetTextureZ (unsigned long int const dt_milli);
extern size_t GetTextureIndex (unsigned long int const dt_milli);
extern void SetupCamera (void);
extern void SetupFrustrum (float const n, float const f, float const l, float const r, float const b, float const t);

///////////////////////////////////////////////////////////

}

///////////////////////////////////////////////////////////

#endif // __DRAWING__SETUP_GEOMETRICAL__H__
