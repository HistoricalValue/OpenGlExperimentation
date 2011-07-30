#version 330 core

in		vec4			interpolatedColour;
in		vec4			textureCoordinate;
uniform	sampler2D		textureUnit0;
uniform	sampler2D		textureUnit1;
uniform	sampler2D		textureUnit2;
uniform	sampler2D		textureUnit3;
uniform	uint			colouringSelector;
out		vec4			fragColor;

void main (void) {
	vec2 texcoord = textureCoordinate.st;

	if (colouringSelector == 0u)
		fragColor = interpolatedColour;
	else
	if (colouringSelector == 1u)
		fragColor = texture(textureUnit0, texcoord);
	else
		fragColor = vec4(1.0f, 0.0f, 1.0f, 1.0f);

}


// 62010006282296
