#version 330 core

in		vec4			interpolatedColour;
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
		fragColor = interpolatedColour;
	else
	if (colouringSelector == 1u)
		fragColor = texture(textureUnit0, texcoord);
	else
		fragColor = vec4(1.0f, 0.0f, 1.0f, 1.0f);

}


// 62010006282296
