#version 330 core

in		vec4			interpolatedColour;
in		vec4			textureCoordinate;
uniform	sampler2DArray	textureUnit0;
uniform	sampler2DArray	textureUnit1;
uniform	sampler2DArray	textureUnit2;
uniform	sampler2DArray	textureUnit3;
uniform	int				textureUnit4;
out		vec4			fragColor;

void main (void) {
	if (textureUnit4 == 0)
		fragColor =  texture(textureUnit1, textureCoordinate.stp);
	else
	if (textureUnit4 == 1)
		fragColor =  texture(textureUnit2, textureCoordinate.stp);
	else
	if (textureUnit4 == 2)
		fragColor =  texture(textureUnit3, textureCoordinate.stp);
	else
		fragColor =	 texture(textureUnit0, textureCoordinate.stp);
}
