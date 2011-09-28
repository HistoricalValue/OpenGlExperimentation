#version 330 core

uniform vec4			pooplight;

in		vec4			interpolatedColour;
flat in	vec4			baseColour;
in		vec4			interpolatedNormal;
in		vec4			textureCoordinate;
uniform	sampler2DArray	textureUnit0;
uniform	sampler2DArray	textureUnit1;
uniform	sampler2DArray	textureUnit2;
uniform	sampler2DArray	textureUnit3;
uniform	uint			colouringSelector;
out		vec4			fragColor;

void main (void) {
	vec3 texcoord = textureCoordinate.stp;

	if (colouringSelector == 0u)
		fragColor =		baseColour
						* 2.0f
						* dot(normalize(vec4(pooplight.xyz * 1000.0f, 1.0f)), normalize(interpolatedNormal))
						;
	else
	if (colouringSelector == 1u)
		fragColor = texture(textureUnit0, texcoord);
	else
	if (colouringSelector == 2u)
		fragColor = interpolatedColour;
	else
		fragColor = vec4(1.0f, 0.0f, 1.0f, 1.0f);

}


// 62010006282296
