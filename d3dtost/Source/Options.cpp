#include <stdafx.h>
#include <Options.h>

static bool* initialised = NULL;
static inline bool Option_ValidateInvariants (void) { DASSERT(*DPTR(DNULLCHECK(initialised))); return true; }
#define H DASSERT(Option_ValidateInvariants);

static const GLuint COLOUR_WITH_LIGHTING_AND_COLOUR				(0u);
static const GLuint COLOUR_WITH_COLOUR							(2u);
static const GLuint COLOUR_WITH_COLOUR_AND_AMBIENT_OCCLUSION	(3u);
static const GLuint COLOUR_WITH_AMBIENT_OCCLUSION				(4u);
static const GLuint	COLOUR_WITH_LIGHTING_AND_AO_AND_COLOUR		(5u);
static const GLuint COLOUR_WITH_BLACK							(6u);
static const GLuint COLOUR_WITH_NEON							(7u);
// Texture Colouring
static const GLuint COLOUR_WITH_TEXTURE							(1u);
static const GLuint COLOUR_WITH_TEXTURE_AND_AO					(8u);
static const GLuint COLOUR_WITH_TEXTURE_LIGHT_AND_AO			(9u);
static const GLuint COLOUR_WITH_TEXTURE_AND_LIGHT				(10u);

size_t	Options::PointShapeColouringMethod (void)					{ return COLOUR_WITH_COLOUR					; }
size_t	Options::LineShapeColouringMethod (void)					{ return COLOUR_WITH_COLOUR					; }
size_t	Options::TriangleShapeColouringMethod (void)				{ return
																			//	COLOUR_WITH_COLOUR
																			//	COLOUR_WITH_LIGHTING_AND_COLOUR
																				COLOUR_WITH_AMBIENT_OCCLUSION
																			//	COLOUR_WITH_COLOUR_AND_AMBIENT_OCCLUSION
																			//	COLOUR_WITH_LIGHTING_AND_AO_AND_COLOUR
																			; }
size_t	Options::TriangleShapeGridColouringMethod (void)			{ return COLOUR_WITH_NEON					; }
size_t	Options::TexturedTriangleShapeColouringMethod (void)		{ return COLOUR_WITH_TEXTURE_LIGHT_AND_AO	; }
