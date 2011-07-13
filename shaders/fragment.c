#version 330 core

in		vec4			interpolatedColour;
in		vec4			textureCoordinate;
uniform	sampler2DArray	textureUnit0;
uniform	sampler2DArray	textureUnit1;
uniform	sampler2DArray	textureUnit2;
uniform	sampler2DArray	textureUnit3;
uniform	sampler2DArray	textureUnit4;
out		vec4			fragColor;

void main (void) {
	fragColor =  texture(textureUnit4, textureCoordinate.stp);
}
