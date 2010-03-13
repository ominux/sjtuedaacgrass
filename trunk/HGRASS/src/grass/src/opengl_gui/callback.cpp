// Li Ji
// April 22, 2009
// GLUT+OpenGL on both linux and windows platforms
#include "callback.h"
#include "surface.h"
#include "analyser.h"
#include "Analysis.h"
#include "main_wrapper.h"
#include "parameter_group.h"

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
//#include <GL/glut.h>

#include <vector>
#include <string>
#include <cstring> // strlen, strcpy
#include <cstdio>  // printf
#include <cmath>   // fabs, sqrt
#include <cstdarg> // va_start, va_end
#include <cstddef>
#include <cassert> // assert
using std::printf;
using std::exit;
using std::vector;
using std::vsprintf;
using std::fabs;
using std::sqrt;
using std::string;
using std::strlen;
using std::strcpy;

// Global variable declaration
// Phase Margin Parameter Group
extern ParameterGroup *ptr_parameter_group_pm_x, *ptr_parameter_group_pm_y;
// Phase Margin Parameter Group
extern ParameterGroup *ptr_parameter_group_bw_x, *ptr_parameter_group_bw_y;
// extern string par_name[];
// Global Wrapper Pointers
extern MainWrapper *main_wrapper_global_phase_margin;
extern MainWrapper *main_wrapper_global_phase_margin_sens;
extern MainWrapper *main_wrapper_global_band_width;
extern MainWrapper *main_wrapper_global_band_width_sens;
extern MainWrapper *main_wrapper_global_normal_pdf_2D;
extern GLdouble mu1, mu2;
extern GLdouble sigma1, sigma2;
extern GLdouble normal_pdf_2D(const GLdouble x, const GLdouble y, void *);
// Declare these global variables just
// for destructing them when the pragram is terminated by exit().
class ToGraph;
extern ToGraph	*toGraph;
#ifdef PARALLEL_COMPUTING
extern ToGraph	*toGraph2;
#endif // PARALLEL_COMPUTING
extern Analysis *ptr_analysis1;
extern Analyser *ptr_analyser1;
extern tpdd *mytpdd1;
extern Analysis *ptr_analysis2;
extern Analyser *ptr_analyser2;
extern tpdd *mytpdd2;

namespace Callback
{
namespace
{
// Subwindows
int surface_window, contour_window, status_window;

// Glabol variables
MainWrapper *main_wrapper_global = 0;
const GLdouble drawing_center[3] = {0.0, 0.0, -200.0};
const GLdouble view_angle_variation = 2.5; // Degree
bool show_grid = false;
bool show_design_center = false; // show data pointon the contour window
bool show_legend = false; // show legend of contour ellipses on the contour window
GLint contour_window_width = 0, contour_window_height = 0; // Size of the contour window
const GLdouble contour_display_ratio = 3.0 / 2.0;
GLdouble contour_x = 0, contour_y = 0;

bool show_contour_data_pointer = false; // for contour window
bool show_highlight_contour_on_surface = true; // for surface window
bool show_highlight_data_pointer_on_surface = false; // for surface window
bool enable_absorption = true; // bind to key 'm'

// Project Square Plain
GLdouble square_z = 0.0;
GLdouble square_z_inc = 0.0;
GLdouble square_z_min = 0.0;
GLdouble square_z_max = 0.0;
GLdouble square_z_init = 0.0;

// Mouse postion
GLdouble Px = 0.0, Py = 0.0;
// Partial Drivatives
GLdouble pdx = 0.0, pdy = 0.0;

// Global Display List
GLint surface, surface_grid, surface_box, square, contour_coordinate;

const Surface<GLdouble> *psurface = 0; // Data pointer

bool show_contour_line_boundary = true; // show contour line boundary or not
vector<Surface<GLdouble>::LineType> contour_line; // contour line container
vector<Surface<GLdouble>::LineType> contour_line_upper_bound;
vector<Surface<GLdouble>::LineType> contour_line_lower_bound;

enum {
	PHASE_MARGIN = 0, PHASE_MARGIN_SENS, BAND_WIDTH, BAND_WIDTH_SENS, NUM_TARGET
};
bool show_contour_line_constraint = true;
// constraint contour lines' container 
vector<vector<Surface<GLdouble>::LineType> >contour_line_constraint[NUM_TARGET];
// color vectors corresponding to constraint contour lines' container
const double color_contour_line_constraint[NUM_TARGET][3] =
{ {0.8, 0.8, 0.3}, {0.8, 0.3, 0.8}, {0.3, 0.8, 0.8}, {0.816, 0.816, 0.816}};

// 2D normal distribution contour ellipses
const int NUM_ELLIPSE = 11;
const GLdouble ellipse[NUM_ELLIPSE] = {
	0.5, 0.75, 1.0, 1.25, 1.5, 1.75, 2.0, 2.25, 2.5, 2.75, 3.0};
// percentage
const GLdouble ellipse_label[NUM_ELLIPSE] = {
	38.3, 54.68, 68.26, 78.88, 86.64, 91.98, 95.44, 97.56, 98.76, 99.4, 99.74};
const GLfloat ellipse_color[NUM_ELLIPSE][3] = {
	{1.0, 0.0, 0.0},
	{0.0, 1.0, 0.0},
	{0.0, 0.0, 1.0},
	{1.0, 0.0, 1.0},
	{0.0, 1.0, 1.0},
	{0.0, 0.5, 0.0},
	{1.0, 0.5, 0.5},
	{0.5, 1.0, 0.5},
	{0.5, 0.5, 1.0},
	{0.5, 0.0, 0.0},
	{0.0, 0.0, 0.0}
};
vector<Surface<GLdouble>::LineType> contour_ellipse[NUM_ELLIPSE];

void
redisplay_all()
{
	glutPostWindowRedisplay(surface_window);
	glutPostWindowRedisplay(contour_window);
	glutPostWindowRedisplay(status_window);
}

// With a variable-length parameter table like printf()
void
draw_string(GLvoid *font, const GLdouble position[3], const char *format, ...)
{
	assert(font);
	assert(format);

	va_list args;
	char buffer[1024] = {0}; // the buffer size should be large enough
	va_start(args, format);
	vsprintf(buffer, format, args);
	va_end(args);

	glPushAttrib(GL_LIST_BIT);
	glRasterPos3dv(position);
	for (const char *ps = buffer; *ps; ++ps)
		glutBitmapCharacter(font, *ps);
	glPopAttrib();
}

/**
 *  \brief Draw a 3D surface strip by strip.
 *
 *  You'll probably want to call this function when
 *  building a display list since we do a lot of trig here.
 *  \param  vertexmat the surface vertex matrix
 *  \param  normalmat the normal matrix related to the surface vertex matrix
 *  \param  num_xpoints the number of columns of both vertex and normal matrices
 *  \param  num_ypoints the number of rows of both vertex and normal matrices
 **/
void draw_surface(const VertexMatrix<GLdouble> &vertexmat,
									const NormalMatrix<GLdouble> &normalmat,
									const size_t &num_xpoints,
									const size_t &num_ypoints)
{
	// Material
	const GLfloat front_material_specular[4] = {1.0, 1.0, 1.0, 1.0};
	const GLfloat front_material_shininess[] = {60.0};
	const GLfloat back_material_specular[4] = {0.5, 0.5, 0.5, 1.0};
	const GLfloat back_material_shininess[] = {30.0};

	glShadeModel(GL_SMOOTH);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glMaterialfv(GL_FRONT, GL_SPECULAR, front_material_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, front_material_shininess);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, front_material_specular);
	glMaterialfv(GL_BACK, GL_SPECULAR, back_material_specular);
	glMaterialfv(GL_BACK, GL_SHININESS, back_material_shininess);
	glMaterialfv(GL_BACK, GL_AMBIENT_AND_DIFFUSE, back_material_specular);
	/* Draw the surface strip by strip. */
	for (size_t i = 0; i < num_ypoints - 1; ++i)
	{
		/*
		 * Here is a sample triangle strip and its plotting order
		 * which keeps each triangle plane positive,
		 * i.e. draw each triangle clockwisely.
		 * e.g.: 0-1-2, 2-1-3, 2-3-4, 4-3-5, 4-5-6, 6-5-7,, 6-7-8, 8-7-9, ...
		 *
		 * (0,0) --------------------- x -- +INF
		 *   |
		 *   |    i -> 1--3--5--7--9 ......
		 *   |         |\ |\ |\ |\ |
		 *   |         | \| \| \| \|
		 *   |  i+1 -> 0--2--4--6--8 ......
		 *   y
		 *   |
		 *  +INF
		 **/
		glBegin(GL_TRIANGLE_STRIP);
		for (size_t j = 0; j < num_xpoints; ++j)
		{
			glNormal3dv(normalmat[i+1][j].v());
			glVertex3dv(vertexmat[i+1][j].v());
			glNormal3dv(normalmat[i][j].v());
			glVertex3dv(vertexmat[i][j].v());
		}
		glEnd();
	}
}

/*
 * Draw the surface grid, a mesh.
 **/
void draw_surface_grid(const VertexMatrix<GLdouble> &vertexmat,
											 const NormalMatrix<GLdouble> &normalmat,
											 const size_t &num_xpoints,
											 const size_t &num_ypoints)
{
	const GLfloat grid_material_specular[4] = {0.0, 0.0, 0.0, 1.0};
	glShadeModel(GL_SMOOTH);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, grid_material_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, grid_material_specular);
	// Upper mesh
	// lines parallel with y axis
	for (size_t i = 0; i < num_ypoints; ++i)
	{
		glBegin(GL_LINE_STRIP);
		for (size_t j = 0; j < num_xpoints; ++j)
		{
			glNormal3dv(normalmat[i][j].v());
			glVertex3dv(vertexmat[i][j].v());
		}
		glEnd();
	}
	// lines parallel with x axis
	for (size_t j = 0; j < num_xpoints; ++j)
	{
		glBegin(GL_LINE_STRIP);
		for (size_t i = 0; i < num_ypoints; ++i)
		{
			glNormal3dv(normalmat[i][j].v());
			glVertex3dv(vertexmat[i][j].v());
		}
		glEnd();
	}
}

/*
 * Draw a cube skeleton embracing the whole 3D surface.
 *
 * Input: Vector3D *pvmin, *pvmax.
 *      z
 *	    |
 *	  (0,0) - y
 *	   /
 *    x
 *
 * Vertex map:
 *     Z
 *     |
 *     1----------6
 *    /|         /|
 *   2----------5 |
 *   | |        | |
 *   | 0--------|-7-- Y
 *   |/         |/
 *   3----------4
 *  /
 * X
 **/
void draw_cube_skeleton(const GLdouble vmin[3], const GLdouble vmax[3])
{
	GLdouble v0[3] = {vmin[0], vmin[1], vmin[2]};
	GLdouble v5[3] = {vmax[0], vmax[1], vmax[2]};
	GLdouble dv[3] = {v5[0] - v0[0], v5[1] - v0[1], v5[2] - v0[2]};

	GLdouble v1[3] = {v0[0],         v0[1],         v0[2] + dv[2]};
	GLdouble v3[3] = {v0[0] + dv[0], v0[1],         v0[2]};
	GLdouble v7[3] = {v0[0],         v0[1] + dv[1], v0[2]};

	GLdouble v2[3] = {v5[0],         v5[1] - dv[1], v5[2]};
	GLdouble v4[3] = {v5[0],         v5[1],         v5[2] - dv[2]};
	GLdouble v6[3] = {v5[0] - dv[1], v5[1],         v5[2]};

	GLdouble vx[3] = {v3[0] + 0.2 * dv[0], v3[1],               v3[2]};
	GLdouble vy[3] = {v7[0],               v7[1] + 0.2 * dv[1], v7[2]};
	GLdouble vz[3] = {v1[0],               v1[1],               v1[2] + 0.2 * dv[2]};

	const GLfloat material_specular[4] = {0.0, 0.0, 0.0, 1.0};
	glShadeModel(GL_FLAT);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, material_specular);

	/* X,Y,Z labels*/
	draw_string(GLUT_BITMAP_HELVETICA_12, vx, "%s", ptr_parameter_group_pm_x->group_name().c_str());
	draw_string(GLUT_BITMAP_HELVETICA_12, vy, "%s", ptr_parameter_group_pm_y->group_name().c_str());
	draw_string(GLUT_BITMAP_HELVETICA_12, vz, "%s", main_wrapper_global->target_label.c_str());

	/* 0-1-2-3-0 */
	glBegin(GL_LINE_LOOP);
	glVertex3dv(v0);
	glVertex3dv(v1);
	glVertex3dv(v2);
	glVertex3dv(v3);
	glEnd();

	/* 4-5-6-7-4 */
	glBegin(GL_LINE_LOOP);
	glVertex3dv(v4);
	glVertex3dv(v5);
	glVertex3dv(v6);
	glVertex3dv(v7);
	glEnd();

	/* 0-7, 1-6, 2-5, 3-4 */
	glBegin(GL_LINES);
	glVertex3dv(v0); glVertex3dv(v7);
	glVertex3dv(v1); glVertex3dv(v6);
	glVertex3dv(v2); glVertex3dv(v5);
	glVertex3dv(v3); glVertex3dv(v4);
	glEnd();

	/* X,Y,Z axises */
	glBegin(GL_LINES);
	glVertex3dv(v3); glVertex3dv(vx);
	glVertex3dv(v7); glVertex3dv(vy);
	glVertex3dv(v1); glVertex3dv(vz);
	glEnd();
}

/*
 * Draw a square on the XOY plane.
 * (0,0) ---------- x - +INF
 *   |  v1 --- v2
 *   |  |      |
 *   |  |      |
 *   y  v4 --- v3
 *   |
 * +INF
 * Order: v1 -> v2 -> v3 -> v4
 **/
void draw_square(const VertexMatrix<GLdouble> &vertexmat)
{
	const GLfloat front_material_specular[4] = {1.0, 1.0, 1.0, 0.5};
	const GLfloat front_material_shininess[] = {60.0};
	const GLfloat back_material_specular[4] = {0.5, 0.5, 0.5, 0.5};
	const GLfloat back_material_shininess[] = {30.0};

	glShadeModel(GL_SMOOTH);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glMaterialfv(GL_FRONT, GL_SPECULAR, front_material_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, front_material_shininess);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, front_material_specular);
	glMaterialfv(GL_BACK, GL_SPECULAR, back_material_specular);
	glMaterialfv(GL_BACK, GL_SHININESS, back_material_shininess);
	glMaterialfv(GL_BACK, GL_AMBIENT_AND_DIFFUSE, back_material_specular);

	static GLdouble normal[3] = {0.0, 0.0, 1.0};
	GLdouble v1[3] = {vertexmat.xmin(), vertexmat.ymin(), 0.0};
	GLdouble v3[3] = {vertexmat.xmax(), vertexmat.ymax(), 0.0};
	GLdouble v2[3] = {v3[0], v1[1], 0.0};
	GLdouble v4[3] = {v1[0], v3[1], 0.0};

	glBegin(GL_QUADS);
	glNormal3dv(normal);
	glVertex3dv(v1);
	glVertex3dv(v2);
	glVertex3dv(v3);
	glVertex3dv(v4);
	glEnd();
}
} // namespace

void display_surface()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glCallList(surface_box);

	glCallList(surface);

	// Draw two grids so as to display a clearer mesh.
	if (show_grid)
	{
		glPushMatrix();
		glTranslated(0.0, 0.0, 1.0e-1);
		glCallList(surface_grid);
		glPopMatrix();

		glPushMatrix();
		glTranslated(0.0, 0.0, -1.0e-1);
		glCallList(surface_grid);
		glPopMatrix();
	}

	// draw cut-off plane
	glPushMatrix();
	glTranslated(0.0, 0.0, square_z);
	glCallList(square);
	glPopMatrix();

	// Draw contour lines
	if (show_highlight_contour_on_surface)
	{
		const GLfloat material_specular[4] = {0.0, 0.0, 0.0, 1.0};
		glLineWidth(4);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material_specular);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, material_specular);
		glBegin(GL_LINES);
		for (vector<Surface<GLdouble>::LineType>::const_iterator
				 cit = contour_line.begin();
				 cit != contour_line.end();
				 ++cit)
		{
			glVertex3dv(cit->first.v());
			glVertex3dv(cit->second.v());
		}
		if (show_contour_line_boundary)
		{
			for (vector<Surface<GLdouble>::LineType>::const_iterator
					 cit = contour_line_upper_bound.begin();
					 cit != contour_line_upper_bound.end();
					 ++cit)
			{
				glVertex3dv(cit->first.v());
				glVertex3dv(cit->second.v());
			}
			for (vector<Surface<GLdouble>::LineType>::const_iterator
					 cit = contour_line_lower_bound.begin();
					 cit != contour_line_lower_bound.end();
					 ++cit)
			{
				glVertex3dv(cit->first.v());
				glVertex3dv(cit->second.v());
			}
		}
		glEnd();
		glLineWidth(1); // Reset the line width to the default value.
	}

	// Draw the data point and its project point
	if (show_highlight_data_pointer_on_surface)
	{
		const static GLfloat material_specular[4] = {0.0, 1.0, 1.0, 1.0};
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material_specular);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, material_specular);
		if (enable_absorption)
		{
			glPushMatrix();
			glTranslated(mu1, mu2, square_z);
			glutSolidSphere(1, 4, 4);
			glPopMatrix();
		}
		else
		{
			const GLdouble zmin = psurface->recover_z(
				psurface->vertex_matrix().zmin());
			const GLdouble zmax = psurface->recover_z( 
				psurface->vertex_matrix().zmax());
			GLdouble z0 = psurface->vertex_matrix().evaluate(
				psurface->recover_x(mu1),
				psurface->recover_y(mu2));
			z0 = ((z0 - zmin) / (zmax - zmin) - 0.5) * psurface->zscale();
			// draw data point
			glPushMatrix();
			glTranslated(mu1, mu2, z0);
			glutSolidSphere(1, 4, 4);
			glPopMatrix();
			// draw projection point
			glPushMatrix();
			glTranslated(mu1, mu2, square_z);
			glutSolidSphere(1, 4, 4);
			glPopMatrix();
			// link both points with a dashed line
			glLineStipple(1, 0xAAAA); 
			glEnable(GL_LINE_STIPPLE);
			glBegin(GL_LINES);
			glVertex3d(mu1, mu2, z0);
			glVertex3d(mu1, mu2, square_z);
			glEnd();
			glDisable(GL_LINE_STIPPLE);
		}
	}

	glPopMatrix();

	glutSwapBuffers();
}

namespace
{
void find_x_boundary_of_contour_ellipses(
	Surface<GLdouble>::VertexMatrixElemetType *xboudary[NUM_ELLIPSE])
{
	Surface<GLdouble>::VertexMatrixElemetType *pxmin = 0;
	Surface<GLdouble>::VertexMatrixElemetType *pxmax = 0;
	GLdouble xmin = 0.0;
	GLdouble xmax = 0.0;
	GLdouble x1 = 0.0;
	GLdouble x2 = 0.0;
	Surface<GLdouble>::VertexMatrixElemetType *pmin = 0;
	Surface<GLdouble>::VertexMatrixElemetType *pmax = 0;
	GLdouble min = 0.0;
	GLdouble max = 0.0;

	for (int i = 0; i < NUM_ELLIPSE; ++i)
	{
		pxmin = &contour_ellipse[i][0].first;
		pxmax = pxmin;
		xmin = contour_ellipse[i][0].first.x();
		xmax = xmin;
		x1 = 0.0;
		x2 = 0.0;
		pmin = 0;
		pmax = 0;
		min = 0.0;
		max = 0.0;
		for (vector<Surface<GLdouble>::LineType>::size_type
				 index = 0;
				 index < contour_ellipse[i].size();
				 ++index)
		{
			x1 = contour_ellipse[i][index].first.x();
			x2 = contour_ellipse[i][index].second.x();
			if (x1 < x2)
			{
				min = x1;
				max = x2;
				pmin = &contour_ellipse[i][index].first;
				pmax = &contour_ellipse[i][index].second;
			}
			else
			{
				min = x2;
				max = x1;
				pmin = &contour_ellipse[i][index].second;
				pmax = &contour_ellipse[i][index].first;
			}
			if (min < xmin)
			{
				xmin = min;
				pxmin = pmin;
			}
			if (xmax < max)
			{
				xmax = max;
				pxmax = pmax;
			}
		}
		xboudary[i] = (i % 2) ? pxmin : pxmax;
	}
}
}

void display_contour()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslated(drawing_center[0], drawing_center[1], drawing_center[2]);

	// Draw the coordinate
	glCallList(contour_coordinate);
	// Draw contour line
	glColor3f(1.0, 0.0, 1.0);
	glBegin(GL_LINES);
	for (vector<Surface<GLdouble>::LineType>::const_iterator
			 cit = contour_line.begin();
			 cit != contour_line.end();
			 ++cit)
	{
		glVertex2dv(cit->first.v());
		glVertex2dv(cit->second.v());
	}
	glEnd();
	if (show_design_center)
	{
		// Draw contour ellipses at the design center
		for (int i = 0; i < NUM_ELLIPSE; ++i)
		{
			glColor3fv(ellipse_color[i]);
			glBegin(GL_LINES);
			for (vector<Surface<GLdouble>::LineType>::const_iterator
					 cit = contour_ellipse[i].begin();
					 cit != contour_ellipse[i].end();
					 ++cit)
			{
				glVertex2dv(cit->first.v());
				glVertex2dv(cit->second.v());
			}
			glEnd();
		}
		// show the design center
		const GLdouble design_center[3] = {mu1, mu2, 0.0};
		draw_string(GLUT_BITMAP_HELVETICA_12, design_center,
								"(%.3e, %.3e)",
								psurface->recover_x(design_center[0]),
								psurface->recover_y(design_center[1]));
		// drraw design center point
		glPointSize(3.37);
		glBegin(GL_POINTS);
		glVertex2d(design_center[0], design_center[1]);
		glEnd();
		// show legend of contour ellipses
		if (show_legend)
		{
			Surface<GLdouble>::VertexMatrixElemetType *legend_pos_x[NUM_ELLIPSE] = {0};
			find_x_boundary_of_contour_ellipses(legend_pos_x);
			const GLdouble ymax = psurface->vertex_matrix().ymax();
			const GLdouble height = ymax - psurface->vertex_matrix().ymin();
			GLdouble legend_pos_y = 0.0;
			for (int i = 0; i < NUM_ELLIPSE; ++i)
			{
				const GLdouble legend_pos[2] = {
					legend_pos_x[i]->x(),
					(ymax + height * (0.02 * (i%2 ? i+1 : i) + 0.002))};
				glColor3fv(ellipse_color[i]);
				draw_string(GLUT_BITMAP_HELVETICA_12, legend_pos,
										"%.2f%%", ellipse_label[i]);
				glBegin(GL_LINES);
				glVertex2dv(legend_pos_x[i]->v());
				glVertex2dv(legend_pos);
				glEnd();
			}
		}
	}
	// Draw bounding contour line
	if (show_contour_line_boundary)
	{
		// Draw upper bound
		glColor3f(0.0, 1.0, 0.0);
		glBegin(GL_LINES);
		for (vector<Surface<GLdouble>::LineType>::const_iterator
				 cit = contour_line_upper_bound.begin();
				 cit != contour_line_upper_bound.end();
				 ++cit)
		{
			glVertex2dv(cit->first.v());
			glVertex2dv(cit->second.v());
		}
		glEnd();
		// Draw lower bound
		glColor3f(1.0, 0.5, 0.5);
		glBegin(GL_LINES);
		for (vector<Surface<GLdouble>::LineType>::const_iterator
				 cit = contour_line_lower_bound.begin();
				 cit != contour_line_lower_bound.end();
				 ++cit)
		{
			glVertex2dv(cit->first.v());
			glVertex2dv(cit->second.v());
		}
		glEnd();
	}
	if (show_contour_line_constraint)
	{
		for (unsigned i = 0; i < NUM_TARGET; ++i)
		{
			glColor3f(color_contour_line_constraint[i][0],
								color_contour_line_constraint[i][1],
								color_contour_line_constraint[i][2]);
			glBegin(GL_LINES);
			for (vector<vector<Surface<GLdouble>::LineType> >::const_iterator
					 cit1 = contour_line_constraint[i].begin();
					 cit1 != contour_line_constraint[i].end();
					 ++cit1)
			{
				for (vector<Surface<GLdouble>::LineType>::const_iterator
						 cit = cit1->begin();
						 cit != cit1->end();
						 ++cit)
				{
					glVertex2dv(cit->first.v());
					glVertex2dv(cit->second.v());
				}
			}
			glEnd();
		}
	}

	glutSwapBuffers();
}

void display_status()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3ub(0, 0, 0); // Background color

	// Scale back the data
	assert(psurface);
	GLdouble zmax = psurface->recover_z(square_z_max);
	GLdouble zmin = psurface->recover_z(square_z_min);
	// Text positions
	const GLdouble first_line = 20.0;
	const GLdouble line_height = 20.0;
	GLdouble z_label_pos[3] = {20.0, first_line, 0.0};
	draw_string(GLUT_BITMAP_HELVETICA_12, z_label_pos,
						 	"%s: max = %e, min = %e",
						 	main_wrapper_global->target_label.c_str(), zmax, zmin);

	GLdouble z = psurface->recover_z(square_z);
	GLdouble z_pos[3] = {20.0, first_line + line_height, 0.0};
	draw_string(GLUT_BITMAP_HELVETICA_12, z_pos,
						 	"%s = %e", main_wrapper_global->target_label.c_str(), z);

	GLdouble mouse_pos[3] = {20.0, first_line + line_height*2, 0.0};
	GLdouble z_Sens_pos1[3] = {20.0, first_line + line_height*3, 0.0};
	GLdouble z_Sens_pos2[3] = {20.0, first_line + line_height*4, 0.0};
	GLdouble z_grad_pos[3] = {20.0, first_line + line_height*5, 0.0};
	const GLdouble design_center[3] = {mu1, mu2, 0.0};
	if (show_contour_data_pointer)
	{
		draw_string(GLUT_BITMAP_HELVETICA_12, mouse_pos, "(%s, %s) = (%e, %e)", 
								ptr_parameter_group_pm_x->group_name().c_str(), ptr_parameter_group_pm_y->group_name().c_str(),
								psurface->recover_x(design_center[0]), psurface->recover_y(design_center[1]));
		draw_string(GLUT_BITMAP_HELVETICA_12, z_Sens_pos1,
								"Sens(%s, %s) = %.2e",
								main_wrapper_global->target_label.c_str(),
								ptr_parameter_group_pm_x->group_name().c_str(),
								pdx);
								
		draw_string(GLUT_BITMAP_HELVETICA_12, z_Sens_pos2,
								"Sens(%s, %s) = %.2e",
								main_wrapper_global->target_label.c_str(),
								ptr_parameter_group_pm_y->group_name().c_str(),
								pdy);
		draw_string(GLUT_BITMAP_HELVETICA_12, z_grad_pos,
								"|grad(%s)| = %.2e",
								main_wrapper_global->target_label.c_str(),
								sqrt(pdx*pdx + pdy*pdy));
	}
	else
	{
	draw_string(GLUT_BITMAP_HELVETICA_12, mouse_pos, "(%s, %s) =", 
							ptr_parameter_group_pm_x->group_name().c_str(), ptr_parameter_group_pm_y->group_name().c_str(),
							psurface->recover_x(contour_x), psurface->recover_y(contour_y));
		draw_string(GLUT_BITMAP_HELVETICA_12, z_Sens_pos1,
								"Sens(%s, %s) =",
								main_wrapper_global->target_label.c_str(),
								ptr_parameter_group_pm_x->group_name().c_str());
		draw_string(GLUT_BITMAP_HELVETICA_12, z_Sens_pos2,
								"Sens(%s, %s) =",
								main_wrapper_global->target_label.c_str(),
								ptr_parameter_group_pm_y->group_name().c_str());
		draw_string(GLUT_BITMAP_HELVETICA_12, z_grad_pos,
								"|grad(%s)| =",
								main_wrapper_global->target_label.c_str());
	}
	glutSwapBuffers();
}

// new window size or exposure
void reshape_surface(int width, int height)
{
	GLdouble fovy = 50.0;
	GLdouble near_pos = psurface->xscale() / 3.0;
	GLdouble far_pos = psurface->xscale() * 3.0;
	GLdouble aspect = (GLdouble) width / height;
	glViewport(0, 0, (GLint) width, (GLint) height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fovy, aspect, near_pos, far_pos);
}

// new window size or exposure
void reshape_contour(int width, int height)
{
	 GLdouble left   = -contour_display_ratio * psurface->xscale() / 2.0;
 	 GLdouble right  =  contour_display_ratio * psurface->xscale() / 2.0;
	 GLdouble bottom = -contour_display_ratio * psurface->yscale() / 2.0;
 	 GLdouble top    =  contour_display_ratio * psurface->yscale() / 2.0;
	// near < |drawing_center[2]| < far
	 GLdouble near_pos   = fabs(drawing_center[2]) - 5.0;
	 GLdouble far_pos    = fabs(drawing_center[2]) + 5.0;
	contour_window_width  = width;
	contour_window_height = height;
	glViewport(0, 0, contour_window_width, contour_window_height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(left, right, bottom, top, near_pos, far_pos);
}

void reshape_status(int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);   
	glLoadIdentity();     
	gluOrtho2D(0, width, height, 0);
	glMatrixMode(GL_MODELVIEW);    
	glLoadIdentity();     
	glClearColor(1.0, 1.0, 1.0, 0.0);
}

/// Right Click Menu
void menu_surface(int value)
{
	switch (value)
	{
	case 1:
		if (main_wrapper_global != main_wrapper_global_phase_margin)
		{
			main_wrapper_global = main_wrapper_global_phase_margin;
			break;
		}
		else // Do nothing if selecting the same option as the last one
		{
			glutPostWindowRedisplay(surface_window);
			return ;
		}
	case 2:
		if (main_wrapper_global != main_wrapper_global_phase_margin_sens)
		{
			main_wrapper_global = main_wrapper_global_phase_margin_sens;
			break;
		}
		else // Do nothing if selecting the same option as the last one
		{
			glutPostWindowRedisplay(surface_window);
			return ;
		}
	case 3:
		if (main_wrapper_global != main_wrapper_global_band_width)
		{
			main_wrapper_global = main_wrapper_global_band_width;
			break;
		}
		else // Do nothing if selecting the same option as the last one
		{
			glutPostWindowRedisplay(surface_window);
			return ;
		}
	case 4:
		if (main_wrapper_global != main_wrapper_global_band_width_sens)
		{
			main_wrapper_global = main_wrapper_global_band_width_sens;
			break;
		}
		else // Do nothing if selecting the same option as the last one
		{
			glutPostWindowRedisplay(surface_window);
			return ;
		}
	case 5:
		if (main_wrapper_global != main_wrapper_global_normal_pdf_2D)
		{
			main_wrapper_global = main_wrapper_global_normal_pdf_2D;
			break;
		}
		else // Do nothing if selecting the same option as the last one
		{
			glutPostWindowRedisplay(surface_window);
			return ;
		}
	default:
		return;
	}
	contour_line.clear();
	show_contour_line_boundary = true;
	contour_line_upper_bound.clear();
	contour_line_lower_bound.clear();
	init(); // Re-init all
	show_contour_data_pointer = false;
	show_highlight_data_pointer_on_surface = false;
	redisplay_all();
}

namespace
{
// Init drawing position, i.e. the origin
// and translate the 3D coordinates from
//      y               z  y
//	    |               | /
//	  (0,0) -- x  to  (0,0) -- x
//	   /
//    z
void init_drawing_position()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslated(drawing_center[0], drawing_center[1], drawing_center[2]);
	glRotated(-45.0, 1.0, 0.0, 0.0);
}
}

void keyboard_surface(unsigned char key, int /*mouse_x*/, int /*mouse_y*/)
{
	switch (key)
	{
	case 'I': square_z = square_z_init; // Reset the reference sqare plane
						show_contour_data_pointer = false;
						show_highlight_data_pointer_on_surface = false;
						redisplay_all(); // Refresh
						break;
	case 'i': init_drawing_position();
						// Refresh the surface window
						glutPostWindowRedisplay(surface_window);
						break;
	case 'G':
	case 'g': show_grid = !show_grid;
						// Refresh the surface window
						glutPostWindowRedisplay(surface_window);
						break;
	case ' ': square_z = square_z_init;
						show_contour_data_pointer = false;
						show_highlight_data_pointer_on_surface = false;
						init_drawing_position();
						redisplay_all(); // Refresh
						break;
	case 27: // ESC 
#ifdef DEBUG
						printf("+-------------------------+\n");
						printf("| Destruction Information |\n");
						printf("+-------------------------+\n");
#endif // DEBUG
						delete ptr_analyser1;
						delete ptr_analysis1;
						delete ptr_analyser2;
						delete ptr_analysis2;
						delete ptr_parameter_group_pm_x;
						delete ptr_parameter_group_pm_y;
						delete ptr_parameter_group_bw_x;
						delete ptr_parameter_group_bw_y;
						delete main_wrapper_global_normal_pdf_2D;
						delete main_wrapper_global_band_width_sens;
						delete main_wrapper_global_band_width;
						delete main_wrapper_global_phase_margin_sens;
						delete main_wrapper_global_phase_margin;
						if (mytpdd2 != mytpdd1)
							delete mytpdd2;
						delete mytpdd1;
#ifdef PARALLEL_COMPUTING
						delete toGraph2;
#endif // PARALLEL_COMPUTING
						delete toGraph;
						printf("\nGRASS(GUI3D) Exit normally.\n");
						exit(0);
						break;
	case 'l': // Save current contour line as lower bound.
						// show_contour_line_boundary = false;
						contour_line_lower_bound = contour_line;
						redisplay_all();
						break;
	case 'L': // Show lower bound info.
						break;
	case 'u': // Save current contour line as upper bound.
						// show_contour_line_boundary = false;
						contour_line_upper_bound = contour_line;
						redisplay_all();
						break;
	case 'U': // Show upper bound info.
						break;
	case 'a': // Show previous contour line saved or not.
						show_contour_line_boundary = ~show_contour_line_boundary;
						// contour_line_lower_bound.clear();
						// contour_line_upper_bound.clear();
						redisplay_all();
						break;
	case 'p': // Save current contour line as a phase margin copy.
						contour_line_constraint[PHASE_MARGIN].push_back(contour_line);
						if (show_contour_line_constraint)
							redisplay_all();
						break;
	case 'P': // Save current contour line as a sensitivity of phase margin copy.
						contour_line_constraint[PHASE_MARGIN_SENS].push_back(contour_line);
						if (show_contour_line_constraint)
							redisplay_all();
						break;
	case 'b': // Save current contour line as a band width copy.
						contour_line_constraint[BAND_WIDTH].push_back(contour_line);
						if (show_contour_line_constraint)
							redisplay_all();
						break;
	case 'B': // Save current contour line as a sensitivity of band width copy.
						contour_line_constraint[BAND_WIDTH_SENS].push_back(contour_line);
						if (show_contour_line_constraint)
							redisplay_all();
						break;
	default:
						break;
	}
}

inline void find_contour_lines()
{
	contour_line.clear();
	assert(psurface);
	psurface->contour(square_z, contour_line); // get the contour lines
}

/// Keys to control the surface position
void special_keyboard(int key, int /*mouse_x*/, int /*mouse_y*/)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:
		glRotated(view_angle_variation, 1.0, 0.0, 0.0);
		break;
	case GLUT_KEY_RIGHT:
		glRotated(-view_angle_variation, 1.0, 0.0, 0.0);
		break;
	case GLUT_KEY_UP:
		glRotated(-view_angle_variation, 0.0, 1.0, 0.0);
		break;
	case GLUT_KEY_DOWN:
		glRotated(view_angle_variation, 0.0, 1.0, 0.0);
		break;
	case GLUT_KEY_PAGE_UP:
		glRotated(view_angle_variation, 0.0, 0.0, 1.0);
		break;
	case GLUT_KEY_PAGE_DOWN:
		glRotated(-view_angle_variation, 0.0, 0.0, 1.0);
		break;
	case GLUT_KEY_HOME:
		square_z += square_z_inc;
		find_contour_lines();
		show_contour_data_pointer = false;
		show_highlight_data_pointer_on_surface = false;
		break;
	case GLUT_KEY_END:
		square_z -= square_z_inc;
		find_contour_lines();
		show_contour_data_pointer = false;
		show_highlight_data_pointer_on_surface = false;
		break;
	default:
		break;
	}
	if (square_z > square_z_max)
	 	square_z = square_z_max;
	else if (square_z < square_z_min)
		square_z = square_z_min;
	redisplay_all(); // Refresh
}

/*
 * Pick up a data point among the contour lines.
 *
 * First, find the nearest contour line to the mouse point.
 *
 * Second, find the foot of the perpendicular
 * from the mouse point to the nearest contour line.
 *               * P
 *               |
 *               |
 * P1 *----------+---------*P2
 *              (Q)
 * where P is the mouse point, Q is the foot of the perpendicular,
 * and P1P2 is the nearest contour line to point P
 * with min(|PP1|^2 + |PP2|^2).
 * 
 * If the foot Q is outside of line P1P2, choose the point between P1 and P2
 * which is nearer to P than the other. Otherwise Q is the right point.
 *                  * P
 *                  |
 *                  |
 * P1 *-----*-------+
 *         (P2)     Q
 */
void contour_mouse_left_click(int mouse_x, int mouse_y)
{
	if (!contour_line.size()) return; // do nothing else if no contour line
	// Change Px and Py only if this function is called
 	// by the mouse callback function
	if (mouse_x >= 0 && mouse_y >= 0)
	{
		Px = contour_display_ratio * Surface<GLdouble>::xscale() *
			(-0.5 + GLdouble(mouse_x) / contour_window_width);
		Py = contour_display_ratio * Surface<GLdouble>::yscale() *
			(-0.5 + GLdouble(contour_window_height - mouse_y) / contour_window_height);
	}
	// Find the nearest contour line
	GLdouble min_measurement =
		(contour_line[0].first.x()  - Px) * (contour_line[0].first.x()  - Px) +
		(contour_line[0].first.y()  - Py) * (contour_line[0].first.y()  - Py) +
		(contour_line[0].second.x() - Px) * (contour_line[0].second.x() - Px) +
		(contour_line[0].second.y() - Py) * (contour_line[0].second.y() - Py);
	GLdouble measurement = min_measurement;
	typedef vector<Surface<GLdouble>::LineType>::const_iterator
		contour_line_const_it_type;
	contour_line_const_it_type nearest_iter = contour_line.begin();
	GLdouble dx = 0, dy = 0;
	for (contour_line_const_it_type cit = contour_line.begin();
			 cit != contour_line.end();
			 ++cit)
	{
		// |PP1|^2
		dx = cit->first.x() - Px;
		dy = cit->first.y() - Py;
		measurement = dx * dx + dy * dy;
		// |PP2|^2
		dx = cit->second.x() - Px;
		dy = cit->second.y() - Py;
		measurement += dx * dx + dy * dy;
		if (measurement < min_measurement)
		{
			nearest_iter = cit;
			min_measurement = measurement;
		}
	}
	// Find the foot of the prependicular, Q, from point P to line P1P2
	GLdouble P1x = nearest_iter->first.x(), P1y = nearest_iter->first.y();
	GLdouble P2x = nearest_iter->second.x(), P2y = nearest_iter->second.y();
	GLdouble dx12 = P1x - P2x, dy12 = P1y - P2y;
	GLdouble dy1  = Py - P1y,  dy2  = Py - P2y;
	GLdouble Qx = (Px * dx12 * dx12 + (P1x * dy2 - P2x * dy1) * dy12) /
		(dx12 * dx12 + dy12 * dy12);
	GLdouble Qy = (P1x*P2y*dx12 - P2x*P1y*dx12 + Px*dx12*dy12 + Py*dy12*dy12) /
		(dx12 * dx12 + dy12 * dy12);
	GLdouble xmin = P1x < P2x ? P1x : P2x, xmax = P1x + P2x - xmin;
	if (Qx < xmin || Qx > xmax) // Q is outside of P1P2
	{
		if (fabs(Qx - P1x) < fabs(Qx - P2x))
		{
			contour_x = P1x;
			contour_y = P1y;
		}
		else
		{
			contour_x = P2x;
			contour_y = P2y;
		}
	}
	else // Q is inside of P1P2
	{
		contour_x = Qx;
		contour_y = Qy;
	}
	// get the design center
	if (enable_absorption)
	{
		mu1 = contour_x;
		mu2 = contour_y;
	}
	else
	{
		mu1 = Px;
		mu2 = Py;
		//mu1 = contour_display_ratio * Surface<GLdouble>::xscale() *
		//	(-0.5 + GLdouble(mouse_x) / contour_window_width);
		//mu2 = contour_display_ratio * Surface<GLdouble>::yscale() *
		//	(-0.5 + GLdouble(contour_window_height - mouse_y) / contour_window_height);
	}
	// get a new 2D normal pdf graph at the design cernter
	MainWrapper *tmp = new MainWrapper(
		"2D Normal pdf",
		0,
		-0.5 * Surface<GLdouble>::xscale(),
		+0.5 * Surface<GLdouble>::xscale(),
		ptr_parameter_group_bw_x->num_samples(),
		-0.5 * Surface<GLdouble>::yscale(),
		+0.5 * Surface<GLdouble>::yscale(),
		ptr_parameter_group_bw_y->num_samples(),
		normal_pdf_2D);
	tmp->status_window =
		main_wrapper_global_phase_margin->status_window;
	tmp->contour_window =
		main_wrapper_global_phase_margin->contour_window;
	tmp->surface_window =
		main_wrapper_global_phase_margin->surface_window;
	if (main_wrapper_global == main_wrapper_global_normal_pdf_2D)
	{
		main_wrapper_global = tmp;
		psurface = &main_wrapper_global->surface;
	}
	delete main_wrapper_global_normal_pdf_2D;
	main_wrapper_global_normal_pdf_2D = tmp;
	// find a group of contour ellipses from the 2D normal pdf
	for (int i = 0; i < NUM_ELLIPSE; ++i)
		contour_ellipse[i].clear(); // clear old ellipses
	const Surface<GLdouble> *tmp_psurface =
		&main_wrapper_global_normal_pdf_2D->surface;
	const GLdouble zmax = tmp_psurface->recover_z(
		tmp_psurface->vertex_matrix().zmax());
	const GLdouble zmin = tmp_psurface->recover_z(
		tmp_psurface->vertex_matrix().zmin());
	for (int i = 0; i < NUM_ELLIPSE; ++i)
	{
		const GLdouble z0 = normal_pdf_2D(
			mu1 + ellipse[i]*sigma1, mu2 + ellipse[i]*sigma2, 0);
#ifdef DEBUG
		printf("(mu1, sigma11) = (%6e, %6e), (mu2, sigma12) = (%6e, %6e), z0 = %6e\n",
					 mu1, sigma1, mu2, sigma2, z0);
#endif // DEBUG
		tmp_psurface->contour(
			((z0 - zmin) / (zmax - zmin) - 0.5) * tmp_psurface->zscale(), 
			contour_ellipse[i]);
	}

	// Evaluate Sens drivatives
	if (!psurface) return;
	pdx = psurface->Sens_x(mu1, mu2);
	pdy = psurface->Sens_y(mu1, mu2);
	// Refresh
	show_contour_data_pointer = true;
	show_highlight_data_pointer_on_surface = true;
	glutPostWindowRedisplay(contour_window);
	glutPostWindowRedisplay(status_window);
	glutPostWindowRedisplay(surface_window);
}

void mouse_contour(int button, int state, int mouse_x, int mouse_y)
{
	if (state == GLUT_DOWN && button == GLUT_RIGHT_BUTTON)
	{
		// Cancel the data pointer when dragging mouse
		show_contour_data_pointer = false;
		show_legend = !show_legend;
		glutPostWindowRedisplay(contour_window);
	}
	if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON)
	{
		show_legend = false; // always close lengend if left click happens
		contour_mouse_left_click(mouse_x, mouse_y);
	}
}

void keyboard_contour(unsigned char key, int /*mouse_x*/, int /*mouse_y*/)
{
	show_legend = false; // always close lengend if any key is pressed
	switch (key)
	{
	case 'c': // Switch to display all saved contour lines as constraints or not.
						show_contour_line_constraint = !show_contour_line_constraint;
						glutPostWindowRedisplay(contour_window);
						break;
	case 'C': // Clear all saved contourlines
						for (int i = 0; i < NUM_TARGET; ++i)
							contour_line_constraint[i].clear();
						glutPostWindowRedisplay(contour_window);
						break;
	case 'd': // design center point display switch
						show_design_center = !show_design_center;
						glutPostWindowRedisplay(contour_window);
						break;
	case 'm': // data point adsorption switch
						enable_absorption = !enable_absorption;
						contour_mouse_left_click(-1, -1);
						glutPostWindowRedisplay(contour_window);
						break;
	default:
						break;
	}
}

void mouse_motion(int /*x*/, int y)
{
	static int last_y = y;
	if (last_y < y)
		square_z -= 10.0 * square_z_inc;
	else if (last_y > y)
		square_z += 10.0 * square_z_inc;
	else
		return;
	last_y = y;
	show_contour_data_pointer = false;

	if (square_z > square_z_max)
	 	square_z = square_z_max;
	else if (square_z < square_z_min)
		square_z = square_z_min;
	find_contour_lines();
	show_contour_data_pointer = false;
	show_highlight_data_pointer_on_surface = false;
	redisplay_all(); // Refresh
}

namespace
{
//
// Vertex map:
//     Z
//     |
//     1----------6
//    /|         /|
//   2----------5 |
//   | |        | |
//   | 0--------|-7-- Y
//   |/         |/
//   3----------4
//  /
// X
void init_surface_window(const VertexMatrix<GLdouble> &vertexmat,
												 const NormalMatrix<GLdouble> &normalmat,
												 const size_t &num_xpoints,
												 const size_t &num_ypoints)
{
	const GLdouble vertex_min[3] =
	{ vertexmat.xmin(), vertexmat.ymin(), vertexmat.zmin() };
	const GLdouble vertex_max[3] =
	{ vertexmat.xmax(), vertexmat.ymax(), vertexmat.zmax() };
	square_z_min = vertexmat.zmin();
	square_z_max = vertexmat.zmax();
	square_z_inc = 1e-3 * (vertexmat.zmax() - vertexmat.zmin());
	square_z = vertexmat.zmin();
	square_z_init = square_z;

	// Lighting
	const size_t NUM_LIGHTS = 4;
	const GLfloat light_diffuse[NUM_LIGHTS][4] = {
		{ 1.0, 1.0, 1.0, 1.0 },
		{ 1.0, 0.3, 0.0, 1.0 },
		{ 0.3, 0.0, 1.0, 1.0 },
		{ 0.0, 1.0, 0.6, 1.0 }
	};
	const GLfloat light_position[NUM_LIGHTS][4] = {
		{ 1.0, 0.0,  1.0, 0.0 },
		{-1.0, 0.0,  1.0, 0.0 },
		{-1.0, 0.0, -1.0, 0.0 },
		{ 1.0, 0.0, -1.0, 0.0 }
	};

	// Init the light posion
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	// Set the lighits
	// Note: GL_SPECULAR is not set because the default effect is good enough
 	// for GL_LIGHT_MODEL_TWO_SIDE lighting model:
	// the default specular of light0 is (1.0, 1.0, 1.0, 1.0)
	// while others' are (0.0, 0.0, 0.0, 1.0).
	for (size_t light_index = 0; light_index < NUM_LIGHTS; ++light_index)
	{
		glLightfv(GL_LIGHT0+light_index, GL_POSITION, light_position[light_index]);
		glLightfv(GL_LIGHT0+light_index, GL_DIFFUSE, light_diffuse[light_index]);
	}

	const GLfloat lmodel_ambient[4] = {0.2, 0.2, 0.2, 1.0};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
	glLightModeli(GL_LIGHT_MODEL_AMBIENT, GL_TRUE);

	// Enable lighting
	glEnable(GL_LIGHTING);
	for (size_t light_index = 0; light_index < NUM_LIGHTS; ++light_index)
		glEnable(GL_LIGHT0 + light_index);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

	// Enable transparecy
	glEnable(GL_LINE_SMOOTH); // Enable antialiasing
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

	// glPolygonOffset(1.0, 1.0);
	glEnable(GL_DEPTH_TEST);

	glClearColor(0.9, 0.9, 0.9, 0.0);

	init_drawing_position();
	
	// Create display lists
	// Make the surface_box skeleton display list.
	surface_box = glGenLists(1);
	glNewList(surface_box, GL_COMPILE);
	draw_cube_skeleton(vertex_min, vertex_max);
	glEndList();

	// Make the surface display list.
	surface = glGenLists(1);
	glNewList(surface, GL_COMPILE);
	draw_surface(vertexmat, normalmat, num_xpoints, num_ypoints);
	glEndList();

	// Make the surface display list.
	surface_grid = glGenLists(1);
	glNewList(surface_grid, GL_COMPILE);
	draw_surface_grid(vertexmat, normalmat, num_xpoints, num_ypoints);
	glEndList();

	// Make the cutting plane display list.
	square = glGenLists(1);
	glNewList(square, GL_COMPILE);
	draw_square(vertexmat);
	glEndList();
}

void init_contour_window(const VertexMatrix<GLdouble> &vertexmat,
												 const NormalMatrix<GLdouble> &normalmat)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClearColor(1.0, 1.0, 1.0, 0.0);
	glEnable(GL_LINE_STIPPLE);

	// Make the contour square region display list.
	// Boundary Label Positions
	const GLdouble width = vertexmat.xmax() - vertexmat.xmin();
	const GLdouble height = vertexmat.ymax() - vertexmat.ymin();
	const GLdouble middle_x = (vertexmat.xmin() + vertexmat.xmax()) / 2.0;
	const GLdouble middle_y = (vertexmat.ymin() + vertexmat.ymax()) / 2.0;
	const GLdouble pos_xmin[3] = {
		vertexmat.xmin() - 0.1 * width,
		vertexmat.ymin() - 0.05 * height,
		0.0
	};
	const GLdouble pos_xmid[3] = {
		middle_x - 0.1 * width,
		vertexmat.ymin() - 0.05 * height,
		0.0
	};
	const GLdouble pos_xmax[3] = {
		vertexmat.xmax() - 0.1 * width,
		vertexmat.ymin() - 0.05 * height,
		0.0
	};
	const GLdouble pos_ymin[3] = {
		vertexmat.xmin() - 0.225 * width,
		vertexmat.ymin(),
		0.0
	};
	const GLdouble pos_ymid[3] = {
		vertexmat.xmin() - 0.225 * width,
		middle_y,
		0.0
	};
	const GLdouble pos_ymax[3] = {
		vertexmat.xmin() - 0.225 * width,
		vertexmat.ymax(),
		0.0
	};
	// Axies arrow positions
	const GLdouble vx[3] = {
		vertexmat.xmax() + 0.1 * width,
		vertexmat.ymin(),
		0.0
	};
	const GLdouble vy[3] = {
		vertexmat.xmin(),
		vertexmat.ymax() + 0.1 * height,
		0.0
	};
	contour_coordinate = glGenLists(1);
	glNewList(contour_coordinate, GL_COMPILE);
	glColor3f(0.0, 0.0, 1.0);
	glNormal3d(0.0, 0.0, 1.0); // z-axis
	// Square frame
	glLineStipple(1, 0xFFFF); // dotted line
	glBegin(GL_LINE_LOOP);
	glVertex3d(vertexmat.xmin(), vertexmat.ymin(), 0.0);
	glVertex3d(vertexmat.xmax(), vertexmat.ymin(), 0.0);
	glVertex3d(vertexmat.xmax(), vertexmat.ymax(), 0.0);
	glVertex3d(vertexmat.xmin(), vertexmat.ymax(), 0.0);
	glEnd();
	// Numerical Boundary Labels
	draw_string(GLUT_BITMAP_HELVETICA_12, pos_xmin, "%.2e",
							psurface->recover_x(vertexmat.xmin()));
	draw_string(GLUT_BITMAP_HELVETICA_12, pos_xmid, "%.2e",
							psurface->recover_x(middle_x));
	draw_string(GLUT_BITMAP_HELVETICA_12, pos_xmax, "%.2e",
							psurface->recover_x(vertexmat.xmax()));
	draw_string(GLUT_BITMAP_HELVETICA_12, pos_ymin, "%.2e",
							psurface->recover_y(vertexmat.ymin()));
	draw_string(GLUT_BITMAP_HELVETICA_12, pos_ymid, "%.2e",
							psurface->recover_y(middle_y));
	draw_string(GLUT_BITMAP_HELVETICA_12, pos_ymax, "%.2e",
							psurface->recover_y(vertexmat.ymax()));
	// X, Y Axises
	glBegin(GL_LINES);
	// X Axises
	glVertex3d(vertexmat.xmax(), vertexmat.ymin(), 0.0);
	glVertex3dv(vx);
	// X middle
	glVertex3d(middle_x, vertexmat.ymin(), 0.0);
	glVertex3d(middle_x, vertexmat.ymin() - 0.02 * height, 0.0);
	// Y Axises
	glVertex3d(vertexmat.xmin(), vertexmat.ymax(), 0.0);
	glVertex3dv(vy);
	// Y middle
	glVertex3d(vertexmat.xmin(), middle_y, 0.0);
	glVertex3d(vertexmat.xmin() - 0.02 * width, middle_y, 0.0);
	glEnd();
	// Text Labels
	draw_string(GLUT_BITMAP_8_BY_13, vx, "%s", ptr_parameter_group_pm_x->group_name().c_str());
	draw_string(GLUT_BITMAP_8_BY_13, vy, "%s", ptr_parameter_group_pm_y->group_name().c_str());
	glEndList();
}

void init_status_window()
{
	// Nothing to do now
}
}

void init()
{
	if (!main_wrapper_global) // Do it only once
		main_wrapper_global = main_wrapper_global_phase_margin; // default target
	assert(main_wrapper_global);
	psurface = &main_wrapper_global->surface;
	assert(psurface);
	// init windows
	surface_window = main_wrapper_global->surface_window;
	glutSetWindow(surface_window);
	init_surface_window(psurface->vertex_matrix(),
										 	psurface->normal_matrix(),
										 	main_wrapper_global->num_xpoints,
										 	main_wrapper_global->num_ypoints);

	contour_window = main_wrapper_global->contour_window;
	glutSetWindow(contour_window);
	init_contour_window(psurface->vertex_matrix(), psurface->normal_matrix());

	status_window = main_wrapper_global->status_window;
	glutSetWindow(status_window);
	init_status_window();
}
} // namespace Callback

