#ifndef OPTIONS_H
#define OPTIONS_H

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

#endif // OPTIONS_H
