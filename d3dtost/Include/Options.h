#ifndef OPTIONS_H
#define OPTIONS_H

///////////////////////////////////////////////////////

#define WITH_NORMALS	1
#define	WITH_GRID		1
#define WITH_INVERSE_IT	0

///////////////////////////////////////////////////////

namespace _ {

///////////////////////////////////////////////////////
// Test [what] flags
static const bool	TEST_ALL			(!true);
static const bool	TEST_TEXTURES		(!true);

///////////////////////////////////////////////////////
// With draw [what] flags
static const bool	WITH_DRAW_POINTS	(!TEST_TEXTURES || TEST_ALL);
static const bool	WITH_DRAW_LINES		(true || !TEST_TEXTURES);
static const bool	WITH_DRAW_TRIANGLES	(!TEST_TEXTURES || TEST_ALL);
static const bool	WITH_DRAW_TEXTURED	(TEST_TEXTURES  || TEST_ALL);
//
static const bool	WITH_CAMERA			(true);

///////////////////////////////////////////////////////
// Number of [what]
static const size_t NUMBER_OF_VAOs		(4);
static const size_t NUMBER_OF_VBOs		(6);
static const size_t NUMBER_OF_VTOs		(1);
static const size_t NUMBER_OF_TEXTURES	(2);
static const size_t NUMBER_OF_IMAGES	(2);

///////////////////////////////////////////////////////
// Indeces for colour renderbuffer bindings
static const size_t COLOUR_CHANNEL_RED_INDEX	(1u);
static const size_t COLOUR_CHANNEL_GREEN_INDEX	(2u);
static const size_t COLOUR_CHANNEL_BLUE_INDEX	(3u);
static const size_t COLOUR_CHANNEL_ALL_INDEX	(4u);

///////////////////////////////////////////////////////
// False constants

///////////////////////////////////////////////////////
// #define NURBS_LOAD_FROM ...
static struct {
	operator std::string const (void) const {
		std::ifstream ifs("../shaders/vertex.c");
		std::string line1;
		getline(ifs, line1);
		getline(ifs, line1);
		return line1.substr(3);
	}
} NURBS_LOAD_FROM;

///////////////////////////////////////////////////////
// class Config
struct Config {
	std::string const& operator [] (std::string const& k) const
		{ return configmap.at(k); }

	Config (void): configmap() { load(configmap); }
private:
	typedef std::map<std::string, std::string>		Configmap;
	Configmap	configmap;

	static Configmap& load (Configmap& m);
};

///////////////////////////////////////////////////////

}

///////////////////////////////////////////////////////

class Options {
public:
	static void		Initialise (void);
	static void		CleanUp (void);

	static size_t	PointShapeColouringMethod (void);
	static size_t	LineShapeColouringMethod (void);
	static size_t	TriangleShapeColouringMethod (void);
	static size_t	TriangleShapeGridColouringMethod (void);
	static size_t	TexturedTriangleShapeColouringMethod (void);
};

///////////////////////////////////////////////////////

#endif // OPTIONS_H
