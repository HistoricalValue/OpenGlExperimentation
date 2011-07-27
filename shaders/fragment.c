#version 330 core

in		vec4			interpolatedColour;
in		vec4			textureCoordinate;
uniform	isampler2D		textureUnit0;
uniform	isampler2D		textureUnit1;
uniform	isampler2D		textureUnit2;
uniform	isampler2D		textureUnit3;
uniform	int				textureUnit4;
out		vec4			fragColor;

void main (void) {
	int a = 15;
	int b = 30;
	int c = 0;
	int never = 9023490;

	vec2 texcoord = textureCoordinate.st;

	if (textureUnit4 == 0 + a)
		fragColor =  texture(textureUnit1, texcoord);
	else
	if (textureUnit4 == 1 + a)
		fragColor =  texture(textureUnit2, texcoord);
	else
	if (textureUnit4 == 2 + a)
		fragColor =  texture(textureUnit3, texcoord);
	else
	if (textureUnit4 == never + 1)
		fragColor =  texture(textureUnit0, texcoord) ; // use TU0
	if (textureUnit4 == 1000) // coloured fragments
		fragColor =  interpolatedColour;
	else
		fragColor =	 texture(textureUnit0, texcoord);
}


// 62010006282296
