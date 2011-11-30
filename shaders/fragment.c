#version 330 core

// uniform inputs
uniform vec4			pooplight;
uniform	sampler2DArray	textureUnit0;
uniform	sampler2DArray	textureUnit1;
uniform	sampler2DArray	textureUnit2;
uniform	sampler2DArray	textureUnit3;
uniform	uint			colouringSelector;

// inputs from previous stages
in		vec4			interpolatedColour;
flat in	vec4			baseColour;
in		vec4			interpolatedNormal;
in		vec4			textureCoordinate;
in		float			interpolatedAmbientOcclusionFactor;

// ouputs
out		vec4[]			fragData;

vec4	ColourFromRgb (uint r, uint g, uint b)
			{ return vec4(float(r)/255.0f, float(g)/255.0f, float(b)/255.0f, 1.0f); }

void main (void) {
	vec3 texcoord = textureCoordinate.stp;

	if (colouringSelector == 0u)	// Light + Colour
		fragData[0] =	interpolatedColour
						* 2.0f
						* dot(normalize(vec4(pooplight.xyz * 1000.0f, 1.0f)), normalize(interpolatedNormal))
						;
	else
	if (colouringSelector == 2u)	// Colour
		fragData[0] = interpolatedColour;
	else
	if (colouringSelector == 3u)	// Colour + AO
		fragData[0] = interpolatedColour * interpolatedAmbientOcclusionFactor;
	else
	if (colouringSelector == 4u)	// AO
		fragData[0] = vec4(1, 1, 1, 1) * interpolatedAmbientOcclusionFactor;
	else
	if (colouringSelector == 5u)	// Light + Colour + AO
		fragData[0] =	interpolatedColour
						* 2.0f
						* dot(normalize(vec4(pooplight.xyz * 1000.0f, 1.0f)), normalize(interpolatedNormal))
						* interpolatedAmbientOcclusionFactor
						;
	else
	if (colouringSelector == 6u)	// Black
		fragData[0] = vec4(0, 0, 0, 1);
	else
	if (colouringSelector == 7u)	// Neon
		fragData[0] = ColourFromRgb(0x4fu, 0x4fu, 0xffu);

	// Texture colouring
	else
	if (colouringSelector == 8u)	// Texture + AO
		fragData[0] = texture(textureUnit0, texcoord) * interpolatedAmbientOcclusionFactor;
	else
	if (colouringSelector == 1u)	// Texture
		fragData[0] = texture(textureUnit0, texcoord);
	else
	if (colouringSelector == 9u)	// Light + Texture + AO
		fragData[0] =	texture(textureUnit0, texcoord)
						* 2.0f
						* dot(normalize(vec4(pooplight.xyz * 1000.0f, 1.0f)), normalize(interpolatedNormal))
						* interpolatedAmbientOcclusionFactor
						;
	else
	if (colouringSelector == 10u)	// Light + Texture
		fragData[0] =	texture(textureUnit0, texcoord)
						* 2.0f
						* dot(normalize(vec4(pooplight.xyz * 1000.0f, 1.0f)), normalize(interpolatedNormal))
						;
	else
		fragData[0] = vec4(0.0f, 1.0f, 1.0f, 1.0f);
}
