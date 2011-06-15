#version 330 core

in		vec4		interpolatedColour;
in		vec4		textureCoordinate;
uniform	sampler2D	textureUnit0;
out		vec4		fragColor;

void main (void) {
	fragColor =  interpolatedColour * texture2D(textureUnit0, textureCoordinate.xy);
}
