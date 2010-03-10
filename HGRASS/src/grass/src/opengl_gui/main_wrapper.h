// The C++ language guarantees that
// the static members, i.e. globals' destructors will be called
// when the pragram was terminated either by a return clause in main()
// or by the standard C library function exit() anywhere.
// So class MainWrapper is used to hold the variables
// and only one is there global MainWrapper object.
#include "surface.h"
#include "analyser.h"
#include <string>
#ifdef _WIN32
#ifndef _STDCALL_SUPPORTED
#define _STDCALL_SUPPORTED
#endif // _STDCALL_SUPPORTED
#ifndef _M_IX86
#define _M_IX86
#endif // _M_IX86
// #include <windows.h>
#endif // _WIN32
#include <cstdlib>
#include <GL/glut.h>
class MainWrapper
{
public:
	MainWrapper(const std::string &label,
							Analyser *analyser,
						 	double xmin, double xmax, size_t num_xpoints,
						 	double ymin, double ymax, size_t num_ypoints,
							GLdouble (*target)(const GLdouble, const GLdouble, void *)):
		target_label(label),
		// Initial settings of OpenGL GUI
		xmin(xmin), xmax(xmax),
		ymin(ymin), ymax(ymax),
		num_xpoints(num_xpoints), num_ypoints(num_ypoints),
		vertexmat(xmin, xmax, ymin, ymax, num_xpoints, num_ypoints,
						 	target, analyser),
		surface(vertexmat),
		surface_window(0), contour_window(0), status_window(0)
 	{
#ifdef DEBUG
	 	printf("MainWrapper constructor: this = %p\n", this);
#endif // DEBUG
 	}

	~MainWrapper()
 	{
#ifdef DEBUG
	 	printf("MainWrapper destructor: this = %p\n", this);
#endif // DEBUG
 	}

	const std::string target_label;
	GLdouble xmin, xmax;
	GLdouble ymin, ymax;
	size_t num_xpoints, num_ypoints;

private:
	VertexMatrix<GLdouble> vertexmat;

public:
	Surface<GLdouble> surface;
	// GLUT windows id
	GLuint surface_window;
	GLuint contour_window;
	GLuint status_window;
};
