// mesh tool ni

#version 330 core

in vec4			position;
in vec4			colour;
in vec4			axyc;
in vec4			texcoord;

uniform uint	textureZ;

out vec4		interpolatedColour;
out	vec4		textureCoordinate;

const float WORLD_WIDTH		= 2000.f;
const float WW				= WORLD_WIDTH;
const float width			= ( WORLD_WIDTH * 0.125f * 2);	// 2/8
const float near			= WORLD_WIDTH * 4 * 2 * 0.125;

float sine = sin(radians(axyc[0]));
float cosine = cos(radians(axyc[0]));

void main (void) {
	vec4 angles = vec4(
			radians(axyc[0] + 90.0f),
			radians(axyc[3] + 90.0f),
			radians(axyc[0]),
			radians(axyc[3]));
	
	mat4 rotx = mat4(					
				1.f,	0.f,		0.f,		0.f,
				0.f,	cosine,		sine,		0.f,
				0.f,	-sine,		cosine,		0.f,
				0.f,	0.f,		0.f,		1.f);
	mat4 roty = mat4(
				cosine,	0.f,	-sine,		0.f,
				0.f,	1.f,	0.f,		0.f,
				sine,	0.f,	cosine,		0.f,
				0.f,	0.f,	0.f,		1.f);
	mat4 rotz = mat4(
				cosine,		sine,	0.f,	0.f,
				-sine,		cosine,	0.f,	0.f,
				0.f,		0.f,	1.f,	0.f,
				0.f,		0.f,	0.f,	1.f);

	vec4 trans = vec4(-sine * width, 0, 0, 0);

	mat4 cam = mat4(	1,					0,	0,				0,
						0,		sin(angles[1]),	cos(angles[1]),	0,
						0,		sin(angles[3]),	cos(angles[3]),	0,
						0,					0,	0,				1) ;

	vec4 pos = position;
	pos *=  rotx *
			mat4(
				cos(radians(30.f)),	0.f,	-sin(radians(30.f)),		0.f,
				0.f,	1.f,	0.f,		0.f,
				sin(radians(30.f)),	0.f,	cos(radians(30.f)),		0.f,
				0.f,	0.f,	0.f,		1.f);;
//	pos += trans;
	//pos *=  cam;
	float mod = near/(pos.z + near);
	//float zmod = (far + near - 2*far*near/zdist) / (zdist * (far - near));

	mat4 proj = mat4(	mod,	0,		0,		0,
						0,		mod,	0,		0,
						0,		0,		1,		0,
						0,		0,		0,		(2 * width) / WW);

	pos *= proj;
//	pos.x = (2*pos.x)/(right-left) + (left+right)/(left-right);
//	pos.y = (2*pos.y)/(top-bottom) + (bottom+top)/(bottom-top);
//	pos.x = (2*pos.x)/(back-front) + (front+back)/(front-back);
	pos.w = width;
	gl_Position = pos;


	interpolatedColour = colour;

	textureCoordinate = texcoord;
	textureCoordinate.p = textureZ;
	textureCoordinate.q = 1;
}

void main0 (void) {
	gl_Position = position;

	interpolatedColour = colour;

	textureCoordinate = texcoord;
	textureCoordinate.p = textureZ;
	textureCoordinate.q = 1;
}

