// Li Ji
// April 22, 2009
// GLUT+OpenGL on both linux and windows platforms
#ifndef _SURFACE_H_
#define _SURFACE_H_

// The stdlib.h which ships with the recent versions of Visual Studio
// has a different (and conflicting) definition of the exit() function.
// It clashes with the definition in glut.h.
//
// Solution:
// Override the definition in glut.h with that in stdlib.h,
// i.e. always place the stdlib.h line above the glut.h line in your code.
// For example,
// 1) C style:
// #include <stdlib.h>
// #include <GL/glut.h>
//
// 2) C++ style:
// #include <cstdlib> 
// #include <GL/glut.h>

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
//#include "GL/glut.h"

#include <vector>
#include <utility> // std::pair
#include <limits>  // std::numeric_limits<T>::min()
#include <cstdio>  // std::printf(), std::sprintf(), std::fflush()
#include <cmath>
#include <cstring> // std::memcpy(), std::memset()
#include <cstddef> // std::size_t
#include <cassert>

////////////////////
// class Vector3D //
////////////////////
// Typename ElementType will be GLfloat, GLdouble, and so on.
// Forward declaration
template<typename ElementType> class Vector3D;

template<typename ElementType>
const Vector3D<ElementType> dot_product(const Vector3D<ElementType> &v1,
																				const Vector3D<ElementType> &v2);

template<typename ElementType = GLdouble>
class Vector3D
{
	typedef ElementType VertexType_[3];
	typedef const ElementType ConstVertxType_[3];
public:
	Vector3D(const ElementType &x = ElementType(0.0),
					 const ElementType &y = ElementType(0.0),
					 const ElementType &z = ElementType(0.0))
	{
		VertexType_ tmp_vertex = {x, y, z};
		(void) std::memcpy(vertex_, tmp_vertex, sizeof(VertexType_));
	}

	Vector3D(const Vector3D<ElementType> &v3d)
	{ (void) std::memcpy(vertex_, v3d.vertex_, sizeof(VertexType_)); }

	~Vector3D() {}

	const ElementType &x() const { return vertex_[0]; }
	const ElementType &y() const { return vertex_[1]; }
	const ElementType &z() const { return vertex_[2]; }
	void x(const ElementType &x) { vertex_[0] = x; }
	void y(const ElementType &y) { vertex_[1] = y; }
	void z(const ElementType &z) { vertex_[2] = z; }
	const ElementType *v() const { return vertex_; } // useful when using OpenGL function

	Vector3D<ElementType> &operator=(const Vector3D<ElementType> &v3d)
	{
		if (this == &v3d) return *this; // avoid self assignment
		(void) std::memcpy(vertex_, v3d.vertex_, sizeof(VertexType_));
		return *this;
	}

	Vector3D<ElementType> &operator+=(const Vector3D<ElementType> &v3d)
	{
		vertex_[0] += v3d.vertex_[0];
		vertex_[1] += v3d.vertex_[1];
		vertex_[2] += v3d.vertex_[2];
		return *this;
	}

	Vector3D<ElementType> &operator-=(const Vector3D<ElementType> &v3d)
	{
		vertex_[0] -= v3d.vertex_[0];
		vertex_[1] -= v3d.vertex_[1];
		vertex_[2] -= v3d.vertex_[2];
		return *this;
	}

	// vector cross product
	Vector3D<ElementType> &operator*=(const Vector3D<ElementType> &v3d)
	{
		VertexType_ cross_product = {
			vertex_[1] * v3d.vertex_[2] - v3d.vertex_[1] * vertex_[2],
			vertex_[2] * v3d.vertex_[0] - v3d.vertex_[2] * vertex_[0],
			vertex_[0] * v3d.vertex_[1] - v3d.vertex_[0] * vertex_[1]
		};
		(void) std::memcpy(vertex_, cross_product, sizeof(VertexType_));
		return *this;
	}

	// opposite
	const Vector3D<ElementType> operator-() const
	{
		return Vector3D<ElementType>(-vertex_[0], -vertex_[1], -vertex_[2]);
	}

	// v /= |v|
	void normalize()
	{
		ElementType mag = std::sqrt(vertex_[0]*vertex_[0] +
															 	vertex_[1]*vertex_[1] +
															 	vertex_[2]*vertex_[2]);
		if (mag < std::numeric_limits<ElementType>::min())
		{
			vertex_[0] = ElementType(0.0);
			vertex_[1] = ElementType(0.0);
			vertex_[2] = ElementType(0.0);
		}
		else
		{
			vertex_[0] /= mag;
			vertex_[1] /= mag;
			vertex_[2] /= mag;
		}
	}

	// v1 . v2
	friend const Vector3D<ElementType> dot_product<ElementType>(
		const Vector3D<ElementType> &v1,
		const Vector3D<ElementType> &v2);
private:
	VertexType_ vertex_;
};

///////////////////////////////
// Vector Algebra Operations //
///////////////////////////////

// Calculate vector addition, v1 + v2,
// in terms of operator +=
template<typename ElementType>
const Vector3D<ElementType> operator+(const Vector3D<ElementType> &v1,
																		 	const Vector3D<ElementType> &v2)
{
	return Vector3D<ElementType>(v1) += v2;
}

// Calculate vector substraction, v1 - v2,
// in terms of operator -=
template<typename ElementType>
const Vector3D<ElementType> operator-(const Vector3D<ElementType> &v1,
																			const Vector3D<ElementType> &v2)
{
	return Vector3D<ElementType>(v1) -= v2;
}

// Calculate vector cross product, v1 x v2,
// in terms of operator *=
template<typename ElementType>
const Vector3D<ElementType> operator*(const Vector3D<ElementType> &v1,
																			const Vector3D<ElementType> &v2)
{
	return Vector3D<ElementType>(v1) *= v2;
}

// Calculate vector dot product, v1 . v2,
template<typename ElementType>
const Vector3D<ElementType> dot_product(const Vector3D<ElementType> &v1,
																				const Vector3D<ElementType> &v2)
{
	return Vector3D<ElementType>(v1.vertex_[0] * v2.vertex_[0],
															 v1.vertex_[1] * v2.vertex_[1],
															 v1.vertex_[2] * v2.vertex_[2]);
}

/////////////
// Utility //
/////////////
namespace
{
inline void progress_bar(const double total,
												 const double current)
{
#ifndef PARALLEL_COMPUTING
	assert(current >= 0.0);
	assert(total >= current);
	const std::size_t length = 60; // length of a progress bar (# of char)
	const std::size_t progress = static_cast<std::size_t>(current / total * length);
	const char bar_char = '#';
	// the size of bar must be large enough
 	// to hold 60 bar_char and a percentage numerical lable.
	const std::size_t size_bar = length + 7; // lenght + sizeof("100.0%")
	char bar[size_bar] = {0};
	// Character Label
	for (std::size_t i = 0; i < size_bar - 1; ++i)
	{
		bar[i] = i < progress ? bar_char : '_';
	}
	// Percentage Label
	sprintf(bar+progress, "%.1f%%", 100.0 * current / total);
	// Print the progress bar
	std::printf("\r%s", bar);
	std::fflush(stdout);
#endif // PARALLEL_COMPUTING
}
}

/////////////////////
// class RawMatrix //
/////////////////////
// The base class of both surface vertex and normal matrices.
// Here typename T can be any type.
template<typename T>
class RawMatrix
{
public:
	typedef T ElementType;
	RawMatrix(std::size_t rows=1, std::size_t cols=1) : 
		num_rows_(rows > 0 ? rows : 1), num_cols_(cols > 0 ? cols : 1),
	 	matrix_(new ElementType *[rows])
	{
		for (std::size_t i = 0; i < num_rows_; ++i)
			matrix_[i] = new ElementType[num_cols_](); // use default constructor
#ifdef DEBUG
		std::printf("RawMatrix constructor: this = %p\n", this);
#endif // DEBUG
	}

	RawMatrix(const RawMatrix<T> &rawmat) :
		num_rows_(rawmat.num_rows_),
		num_cols_(rawmat.num_cols_),
		matrix_(new ElementType *[num_rows_])
	{
		for (std::size_t i = 0; i < num_rows_; ++i)
		{
			matrix_[i] = new ElementType[num_cols_](); // use default constructor
			(void) std::memcpy(matrix_[i],
												 rawmat.matrix_[i],
												 num_cols_ * sizeof(ElementType));
		}
#ifdef DEBUG
		std::printf("RawMatrix copy constructor: this = %p\n", this);
#endif // DEBUG
	}

	virtual ~RawMatrix()
	{
		for (std::size_t i = 0; i < num_rows_; ++i)
		{
			delete []matrix_[i];
			matrix_[i] = 0;
		}
		delete []matrix_;
		matrix_ = 0;
#ifdef DEBUG
		std::printf("RawMatrix destructor: this = %p\n", this);
#endif // DEBUG
	}

	virtual std::size_t row_size() const {return num_rows_;}
	virtual std::size_t col_size() const {return num_cols_;}
	virtual ElementType *const &operator[] (const std::size_t row) const
	{ return matrix_[row]; }
protected:
	std::size_t num_rows_, num_cols_;
	ElementType **matrix_;
};

////////////////////////
// class VertexMatrix //
////////////////////////
// Surface Vertex Matrix
// Typename GLType will be GLfloat, GLdouble, and so on.
template<typename GLType = GLdouble, typename ObjPtrType = void *,
 	typename EvaluatorType = GLType(*)(const GLType, const GLType, ObjPtrType)>
class VertexMatrix : public RawMatrix< Vector3D<GLType> >
{
public:
	typedef Vector3D<GLType> ElementType;
	// Vertex matrix:
	// (O,0) ----------------------------------------------------------- x -- +INF
	//   | [ v00,             v01,             ..., v_0,num_xpoints          ]
	//   | [ v10,             v11,             ..., v_1,num_xpoints          ]
	//   |                             .
	//   |                             .
	//   |                             .
	//   | [ v_num_ypoints,0, v_num_ypoints,1, ..., v_num_ypoints,num_xpoints],
	//   y
	//   |
	//  +INF
	//
	// where vij = Vector3D {xij, yij, zij}
	VertexMatrix(const GLType &xmin, const GLType &xmax,
							 const GLType &ymin, const GLType &ymax,
							 const std::size_t num_xpoints,
							 const std::size_t num_ypoints,
							 EvaluatorType evaluator,
							 ObjPtrType obj_ptr = 0) :
		RawMatrix<ElementType>(num_ypoints, num_xpoints),
		vertex_min_(xmin, ymin),
		vertex_max_(xmax, ymax),
		evaluator_(evaluator),
		obj_ptr_(obj_ptr)
	{
		assert(xmax > xmin);
		assert(ymax > ymin);
		GLType xstep = (xmax - xmin) / (num_xpoints - 1);
		GLType ystep = (ymax - ymin) / (num_ypoints - 1);
		/* Create surface matrix row by row. */
		GLType x(xmin), y(ymin);
		GLType zmin((*evaluator_)(x, y, obj_ptr)), zmax(zmin);
		// Progress bar
		std::printf("Constructing the vertex matrix:\n");
		const std::size_t total =
		 	RawMatrix<ElementType>::row_size() * RawMatrix<ElementType>::col_size();
		std::size_t current = 0;
		for (std::size_t i = 0; i < RawMatrix<ElementType>::row_size(); ++i)
		{
			for (std::size_t j = 0; j < RawMatrix<ElementType>::col_size(); ++j)
			{
				++current;
				GLType z( (*evaluator_)(x, y, obj_ptr) );
				RawMatrix< Vector3D<GLType> >::matrix_[i][j].x(x);
				RawMatrix< Vector3D<GLType> >::matrix_[i][j].y(y);
				RawMatrix< Vector3D<GLType> >::matrix_[i][j].z(z);
				x += xstep;
				// Find the {min, max}(z)
				if (z < zmin)
					zmin = z;
				else if (z > zmax)
					zmax = z;
				else
					continue;
			}
			x = xmin;
			y += ystep;
			progress_bar(total, current);
		}
		// Set min(z) and max(z)
		vertex_min_.z(zmin);
		vertex_max_.z(zmax);
		std::printf("\n");
#ifdef DEBUG
		std::printf("VertexMatrix constructor: this = %p\n", this);
#endif // DEBUG
	}

	VertexMatrix(const VertexMatrix &vmat) :
		RawMatrix<ElementType>(vmat),
		vertex_min_(vmat.vertex_min_),
		vertex_max_(vmat.vertex_max_),
		evaluator_(vmat.evaluator_),
		obj_ptr_(vmat.obj_ptr_)
	{
#ifdef DEBUG
	 	std::printf("VertexMatrix copy constructor: this = %p\n", this);
#endif // DEBUG
 	}

	virtual ~VertexMatrix()
 	{
#ifdef DEBUG
	 	std::printf("VertexMatrix destructor: this = %p\n", this);
#endif // DEBUG
 	}

	const GLType &xmin() const { return vertex_min_.x(); }
	const GLType &ymin() const { return vertex_min_.y(); }
	const GLType &zmin() const { return vertex_min_.z(); }
	const GLType &xmax() const { return vertex_max_.x(); }
	const GLType &ymax() const { return vertex_max_.y(); }
	const GLType &zmax() const { return vertex_max_.z(); }
	const ElementType &vmin() const { return vertex_min_; }
	const ElementType &vmax() const { return vertex_max_; }
	void vmin(const ElementType &vertex) { vertex_min_ = vertex; }
	void vmax(const ElementType &vertex) { vertex_max_ = vertex; }
	const GLType evaluate(const GLType &x, const GLType &y) const
	{ return (*evaluator_)(x, y, obj_ptr_); }
private:
	ElementType vertex_min_, vertex_max_;
	EvaluatorType evaluator_;
	ObjPtrType obj_ptr_;
};

////////////////////////
// class NormalMatrix //
////////////////////////
// Typename GLType will be GLfloat, GLdouble, and so on.
template<typename GLType = GLdouble, typename ObjPtrType = void *,
typename EvaluatorType = GLType(*)(const GLType, const GLType, ObjPtrType)>
class NormalMatrix : public RawMatrix< Vector3D<GLType> >
{
public:
	typedef Vector3D<GLType> ElementType;
	explicit NormalMatrix(const VertexMatrix<GLType, ObjPtrType, EvaluatorType> &vmat);
	virtual ~NormalMatrix()
 	{
#ifdef DEBUG
	 	std::printf("NormalMatrix destructor: this = %p\n", this);
#endif // DEBUG
 	}
};

template<typename GLType, typename ObjPtrType, typename EvaluatorType>
NormalMatrix<GLType, ObjPtrType, EvaluatorType>::NormalMatrix(
	const VertexMatrix<GLType, ObjPtrType, EvaluatorType> &vmat) :
	RawMatrix<ElementType>(vmat.row_size(), vmat.col_size())
{
	/*
	 * Evaluate normal of each vertex of a left-bottom triangles:
	 * (0,0) ----- x -- +INF
	 *   |
	 *   |  i-1 -> 1
	 *   |         |\
	 *   |         | \
	 *   |    i -> 0--2
	 *   y         ^  ^
	 *   |         j j+1
	 * +INF
	 * where v0 = (i, j), v1 = (i-1, j), v2 = (i, j+1).
	 * Normal = (2->0) x (0->1).
	 **/
	// Progress bar
	printf("Constructing the normal matrix:\n");
	const std::size_t total =
	 	(RawMatrix<ElementType>::row_size() - 1) *
	 	(RawMatrix<ElementType>::col_size() - 1) * 2 +
		RawMatrix<ElementType>::row_size() *
		RawMatrix<ElementType>::col_size();
	std::size_t current = 0;
	for (std::size_t i = 1; i < RawMatrix<ElementType>::row_size(); ++i)
	{
		for (std::size_t j = 0; j < RawMatrix<ElementType>::col_size() - 1; ++j)
		{
			++current;
			Vector3D<GLType> normal =
				(vmat[i][j] - vmat[i][j+1]) * (vmat[i-1][j] - vmat[i][j]);
			// normal.normalize();
			// matrix_[i][j] has been initialized by RawMatrix's constructor.
			RawMatrix<ElementType>::matrix_[i  ][j  ] += normal;
			RawMatrix<ElementType>::matrix_[i-1][j  ] += normal;
			RawMatrix<ElementType>::matrix_[i  ][j+1] += normal;
		}
		progress_bar(total, current);
	}
	/*
	 * Evaluate normal of each vertex of a right-top triangle:
	 * (0,0) ----- x -- +INF
	 *   |
	 *   |    i -> 2--0
	 *   |          \ |
	 *   |           \|
	 *   |  i+1 ->    1
	 *   y         ^  ^
	 *   |        j-1 j
	 * +INF
	 * where v0 = (i, j), v1 = (i+1, j), v2 = (i, j-1).
	 * Normal = (2->0) x (0->1).
	 **/
	for (std::size_t i = 0; i < RawMatrix<ElementType>::row_size() - 1; ++i)
	{
		for (std::size_t j = 1; j < RawMatrix<ElementType>::col_size(); ++j)
		{
			++current;
			Vector3D<GLType> normal =
				(vmat[i][j] - vmat[i][j-1])*(vmat[i+1][j] - vmat[i][j]);
			normal.normalize();
			// matrix_[i][j] has been initialized by RawMatrix's constructor.
			RawMatrix<ElementType>::matrix_[i  ][j  ] += normal;
			RawMatrix<ElementType>::matrix_[i+1][j  ] += normal;
			RawMatrix<ElementType>::matrix_[i  ][j-1] += normal;
		}
		progress_bar(total, current);
	}
	for (std::size_t i = 0; i < RawMatrix<ElementType>::row_size(); ++i)
	{
		for (std::size_t j = 0; j < RawMatrix<ElementType>::col_size(); ++j)
		{
			++current;
			RawMatrix<ElementType>::matrix_[i][j].normalize();
		}
		progress_bar(total, current);
	}
	std::printf("\n");
#ifdef DEBUG
	std::printf("NormalMatrix constructor: this = %p\n", this);
#endif // DEBUG
}

////////////////////////
// class SurfaceMatrix //
////////////////////////
// Move original surface to the center of the 3D coordinate system
// and scale X,Y,Z so as to constrain the surface in a fixed box,
// (0,0,0) -> (100,100,61.8).
// Then move the surface to (50, 50, 30.9) so as to set the origin 
// at the center of the fixed box.
// As a result, -50 <= x <= 50, -50 <= y <= 50, -30.9 <= z <= 30.9.
template<typename GLType = GLdouble, typename ObjPtrType = void *,
 	typename EvaluatorType = GLType(*)(const GLType, const GLType, ObjPtrType)>
class Surface
{
	typedef VertexMatrix<GLType, ObjPtrType, EvaluatorType> VertexMatrixType;

	typedef NormalMatrix<GLType> NormalMatrixType;
	typedef typename NormalMatrixType::ElementType NormalMatrixElementType;
public:
	typedef typename VertexMatrixType::ElementType VertexMatrixElemetType;

	explicit Surface(const VertexMatrixType &orig_vmat);
	~Surface()
	{
		delete pnormalmat_;
		pnormalmat_ = 0;
		delete pvertexmat_;
		pvertexmat_ = 0;
#ifdef DEBUG
		std::printf("Surface destructor: this = %p\n", this);
#endif // DEBUG
	}

	const VertexMatrixType &vertex_matrix() const
	{ return (*pvertexmat_); }

	const NormalMatrixType &normal_matrix() const
	{ return (*pnormalmat_); }

	// Get the vector of contour lines.
	typedef std::pair<VertexMatrixElemetType, VertexMatrixElemetType> LineType;
	void contour(const GLType z, std::vector<LineType> &contour_line) const;
	// Recover the original data points
	const GLType recover_x(const GLType &scaled_x) const
	{ return origin_.x() + (scaled_x + center_offset_.x()) / vratio_.x(); }

	const GLType recover_y(const GLType &scaled_y) const
	{ return origin_.y() + (scaled_y + center_offset_.y()) / vratio_.y(); }

	const GLType recover_z(const GLType &scaled_z) const
	{ return origin_.z() + (scaled_z + center_offset_.z()) / vratio_.z(); }

	// Partial deriviatives
	const GLType Sens_x(const GLType &scaled_x0, const GLType &scaled_y0) const;

	const GLType Sens_y(const GLType &scaled_x0, const GLType &scaled_y0) const;

	// Static members
	static const GLType &xscale() { return xscale_; }
	static const GLType &yscale() { return yscale_; }
	static const GLType &zscale() { return zscale_; }
private:
	// Original vertex matrix can be recovered completely
	// with member origin, vratio and center_offset_

	// origin corresponds to orig_vmat.vmin
	const VertexMatrixElemetType origin_;
	//              vscale
	// vratio = ---------------
	//             max - min
	const VertexMatrixElemetType vratio_;
	const VertexMatrixElemetType center_offset_;

	// main data members
	VertexMatrixType *pvertexmat_;
	NormalMatrixType *pnormalmat_;

	// static member
	static const GLType xscale_, yscale_, zscale_;
};

template<typename GLType, typename ObjPtrType, typename EvaluatorType>
const GLType Surface<GLType, ObjPtrType, EvaluatorType>::xscale_(GLType(100.0));

template<typename GLType, typename ObjPtrType, typename EvaluatorType>
const GLType Surface<GLType, ObjPtrType, EvaluatorType>::yscale_(GLType(100.0));

template<typename GLType, typename ObjPtrType, typename EvaluatorType>
const GLType Surface<GLType, ObjPtrType, EvaluatorType>::zscale_(GLType(61.8));

template<typename GLType, typename ObjPtrType, typename EvaluatorType>
Surface<GLType, ObjPtrType, EvaluatorType>::Surface(
	const VertexMatrixType &orig_vmat) :
	origin_(orig_vmat.vmin()),
	vratio_(xscale_ / (orig_vmat.xmax()-orig_vmat.xmin()),
					yscale_ / (orig_vmat.ymax()-orig_vmat.ymin()),
					zscale_ / (orig_vmat.zmax()-orig_vmat.zmin())),
	center_offset_(xscale_ / 2.0, yscale_ / 2.0, zscale_ / 2.0),
	pvertexmat_(new VertexMatrixType(orig_vmat)),
	pnormalmat_(0)
{
	// Modify *pvertexmat_ to constrain the surface in a fixed box.
	for (std::size_t i = 0; i < pvertexmat_->row_size(); ++i)
		for (std::size_t j = 0; j < pvertexmat_->col_size(); ++j)
		{
			(*pvertexmat_)[i][j] =
			 	dot_product<GLType>(vratio_, (*pvertexmat_)[i][j] - origin_)
			 	- center_offset_;
		}
	// Set new vertex_min_ and vertex_max_.
	pvertexmat_->vmin(-center_offset_);
	pvertexmat_->vmax(center_offset_);
	// Create the normal matrix of the scaled vertex matrix.
	pnormalmat_ = new NormalMatrix<GLType, ObjPtrType, EvaluatorType>(*pvertexmat_);
#ifdef DEBUG
	std::printf("Surface constructor: this = %p\n", this);
#endif // DEBUG
}

/*
 * Marching Triangles Approach: Get the contour lines.
 * There are 8 cases for each triangle as follows,
 * Legend: *: z(x,y) >= z0,
 *         O: z(x,y) <  z0,
 *				 @: an ending of a contour line
  
       *             O              *              O
      / \           / \            / \            / \
     /   \         /   \          /   \          /   \
		/     \       @-----@        @     \        /     @
   /       \     /       \      / \     \      /     / \
  *---------*   *---------*    O---@-----*    O-----@---*
      (0)           (1)            (2)            (3)

       *             O              *              O
      / \           / \            / \            / \
     /   \         /   \          /   \          /   \
		/     @       @     \        @-----@        /     \
   /     / \     / \     \      /       \      /       \
  *-----@---O   *---@-----O    O---------O    O---------O
      (4)           (5)            (6)            (7)
 **/
template<typename GLType>
int triangle_cell_case(const GLType z0, // countour value
											 const Vector3D<GLType> &a,
											 const Vector3D<GLType> &b,
											 const Vector3D<GLType> &c)
{
	/*    a
	 *   / \
	 *  /   \
	 * b-----c
	 **/
	int case_num = 0; // case_num = 0, 1, ..., 7.
	// Binary encoding
	if (a.z() < z0) case_num += 1;
	if (b.z() < z0) case_num += 2;
	if (c.z() < z0) case_num += 4;
	return case_num;
}

template<typename GLType, typename VectorType>
void get_contour_line(const GLType z0, const GLType dx, const GLType dy,
											const Vector3D<GLType> &a,
											const Vector3D<GLType> &b,
											const Vector3D<GLType> &c,
											VectorType &contour_line)
{
	// ending pairs of a contour line
	Vector3D<GLType> v1(0.0, 0.0, 0.0), v2(0.0, 0.0, 0.0);
	switch (triangle_cell_case<GLType>(z0, a, b, c))
	{
	case 0: case 7: // No contour line
		break;
	case 1: case 6: // a--v1--b, a--v2--c
		v1.x(a.x() + dx * ((z0 - a.z()) / (b.z() - a.z())));
		v1.y(a.y());
		v2.x(a.x() + dx * ((z0 - a.z()) / (c.z() - a.z())));
		v2.y(a.y() + dy * ((z0 - a.z()) / (c.z() - a.z())));
		break;
	case 2: case 5: // a--v1--b, b--v2--c
		v1.x(a.x() + dx * ((z0 - a.z()) / (b.z() - a.z())));
		v1.y(a.y());
		v2.x(b.x());
		v2.y(b.y() + dy * ((z0 - b.z()) / (c.z() - b.z())));
		break;
	case 3: case 4: // b--v1--c, a--v2--c
		v1.x(b.x());
		v1.y(b.y() + dy * ((z0 - b.z()) / (c.z() - b.z())));
		v2.x(a.x() + dx * ((z0 - a.z()) / (c.z() - a.z())));
		v2.y(a.y() + dy * ((z0 - a.z()) / (c.z() - a.z())));
		break;
	default:
		assert(0); // Unkown triangle cell cases
	}
	contour_line.push_back(std::make_pair(v1, v2));
}

template<typename GLType, typename ObjPtrType, typename EvaluatorType>
void Surface<GLType, ObjPtrType, EvaluatorType>::
contour(const GLType z0, std::vector<LineType> &contour_line) const
{
	// ending pairs of a contour line segment
	Vector3D<GLType> v1(0.0, 0.0, z0), v2(0.0, 0.0, z0);
	const GLType dx = (*pvertexmat_)[0][1].x() - (*pvertexmat_)[0][0].x();
 	const GLType dy = (*pvertexmat_)[1][0].y() - (*pvertexmat_)[0][0].y();
	for (std::size_t i = 1; i < pvertexmat_->row_size(); ++i)
		for (std::size_t j = 1; j < pvertexmat_->col_size(); ++j)
		{
			/*
			 * (0,0) ----- x -- +INF
			 *   |
			 *   |  i-1 -> c1
			 *   |         |\
			 *   |         | \
			 *   |         |  \
			 *   |    i -> b1--a1
			 *   y         ^   ^
			 *   |        j-1  j
			 * +INF
			 **/
			const Vector3D<GLType> &a1 = (*pvertexmat_)[i  ][j  ];
			const Vector3D<GLType> &b1 = (*pvertexmat_)[i  ][j-1];
			const Vector3D<GLType> &c1 = (*pvertexmat_)[i-1][j-1];
			switch (triangle_cell_case<GLType>(z0, a1, b1, c1))
			{
			case 0: case 7: // No contour line
				break;
			case 1: case 6: // a1--v1--b1, a1--v2--c1
				v1.x(b1.x() + dx * ((z0 - b1.z()) / (a1.z() - b1.z())));
				v1.y(b1.y());
				v1.z(z0);
				v2.x(c1.x() + dx * ((z0 - c1.z()) / (a1.z() - c1.z())));
				v2.y(c1.y() + dy * ((z0 - c1.z()) / (a1.z() - c1.z())));
				v2.z(z0);
				contour_line.push_back(std::make_pair(v1, v2));
				break;
			case 2: case 5: // a1--v1--b1, b1--v2--c1
				v1.x(b1.x() + dx * ((z0 - b1.z()) / (a1.z() - b1.z())));
				v1.y(b1.y());
				v1.z(z0);
				v2.x(c1.x());
				v2.y(c1.y() + dy * ((z0 - c1.z()) / (b1.z() - c1.z())));
				v2.z(z0);
				contour_line.push_back(std::make_pair(v1, v2));
				break;
			case 3: case 4: // b1--v1--c1, a1--v2--c1
				v1.x(c1.x());
				v1.y(c1.y() + dy * ((z0 - c1.z()) / (b1.z() - c1.z())));
				v1.z(z0);
				v2.x(c1.x() + dx * ((z0 - c1.z()) / (a1.z() - c1.z())));
				v2.y(c1.y() + dy * ((z0 - c1.z()) / (a1.z() - c1.z())));
				v2.z(z0);
				contour_line.push_back(std::make_pair(v1, v2));
				break;
			default:
				assert(0); // Unkown triangle cell cases
			}
			/*
			 * (0,0) ----- x -- +INF
			 *   |
			 *   |   b2--c2 <- i-1
			 *   |    \  |
			 *   |     \ |
			 *   |      \|
			 *   |      a2  <- i
			 *   y   ^   ^
			 *   |  j-1  j
			 * +INF
			 **/
			const Vector3D<GLType> &a2 = (*pvertexmat_)[i  ][j  ];
			const Vector3D<GLType> &b2 = (*pvertexmat_)[i-1][j-1];
			const Vector3D<GLType> &c2 = (*pvertexmat_)[i-1][j  ];
			switch (triangle_cell_case<GLType>(z0, a2, b2, c2))
			{
			case 0: case 7: // No contour line
				break;
			case 1: case 6: // a2--v1--b2, a2--v2--c2
				v1.x(b2.x() + dx * (z0 - b2.z()) / (a2.z() - b2.z()));
				v1.y(b2.y() + dy * (z0 - b2.z()) / (a2.z() - b2.z()));
				v1.z(z0);
				v2.x(c2.x());
				v2.y(c2.y() + dy * (z0 - c2.z()) / (a2.z() - c2.z()));
				v2.z(z0);
				contour_line.push_back(std::make_pair(v1, v2));
				break;
			case 2: case 5: // a2--v1--b2, b2--v2--c2
				v1.x(b2.x() + dx * (z0 - b2.z()) / (a2.z() - b2.z()));
				v1.y(b2.y() + dy * (z0 - b2.z()) / (a2.z() - b2.z()));
				v1.z(z0);
				v2.x(b2.x() + dx * (z0 - b2.z()) / (c2.z() - b2.z()));
				v2.y(b2.y());
				v2.z(z0);
				contour_line.push_back(std::make_pair(v1, v2));
				break;
			case 3: case 4: // b2--v1--c2, a2--v2--c2
				v1.x(b2.x() + dx * (z0 - b2.z()) / (c2.z() - b2.z()));
				v1.y(b2.y());
				v1.z(z0);
				v2.x(c2.x());
				v2.y(c2.y() + dy * (z0 - c2.z()) / (a2.z() - c2.z()));
				v2.z(z0);
				contour_line.push_back(std::make_pair(v1, v2));
				break;
			default:
				assert(0); // Unkown triangle cell cases
			}
		}
}

// Normalized Partial Derivatives
// Sens_x: x0 / F0 * dF/dx, x=x0, y=y0, F0=F(x0,y0)
template<typename GLType, typename ObjPtrType, typename EvaluatorType>
const GLType Surface<GLType, ObjPtrType, EvaluatorType>::
Sens_x(const GLType &scaled_x0, const GLType &scaled_y0) const
{
	assert(pvertexmat_);
	const GLType xmin = recover_x(pvertexmat_->xmin());
	const GLType xmax = recover_x(pvertexmat_->xmax());
	const GLType scaled_dx( 
		((*pvertexmat_)[0][1].x() - (*pvertexmat_)[0][0].x()));
	assert(scaled_dx);
	// factor should be less than 1.0 so as to get a smaller dx.
	const GLType factor(0.01);
	GLType dx(factor * (recover_x(scaled_dx) - (xmin + xmax) / 2.0));
	assert(dx);
	GLType x0(0.0), z0(0.0), z_forward(0.0), z_backward(0.0);
	const GLType y0(recover_y(scaled_y0));
	GLType pdx(0.0); // Partial Derivative

#ifdef DEBUG
	int choice_branch = 0;
#endif // DEBUG
	assert(dx <= factor * (xmax - xmin));

	if (pvertexmat_->xmin() < scaled_x0 - scaled_dx &&
			scaled_x0 + scaled_dx < pvertexmat_->xmax())
	{
#ifdef DEBUG
		choice_branch = 1;
#endif // DEBUG
		x0 = recover_x(scaled_x0);
		z0 = pvertexmat_->evaluate(x0, y0);
		z_forward =  pvertexmat_->evaluate(x0 - dx, y0);
		z_backward = pvertexmat_->evaluate(x0 + dx, y0);
		pdx = x0 / z0 * (z_backward - z_forward) / dx / 2.0;
	}
	else if (pvertexmat_->xmin() >= scaled_x0 - scaled_dx &&
					 pvertexmat_->xmin() < scaled_x0)
	{
#ifdef DEBUG
		choice_branch = 2;
#endif // DEBUG
		x0 = recover_x(scaled_x0);
		z0 = pvertexmat_->evaluate(x0, y0);
		dx = x0 - xmin;
		z_forward =  pvertexmat_->evaluate(xmin, y0);
		z_backward = pvertexmat_->evaluate(x0 + dx, y0);
		pdx = x0 / z0 * (z_backward - z_forward) / dx / 2.0;
	}
	else if (scaled_x0 + scaled_dx >= pvertexmat_->xmax() &&
					 scaled_x0 < pvertexmat_->xmax())
	{
#ifdef DEBUG
		choice_branch = 3;
#endif // DEBUG
		x0 = recover_x(scaled_x0);
		z0 = pvertexmat_->evaluate(x0, y0);
		dx = xmax - x0;
		z_forward =  pvertexmat_->evaluate(x0 - dx, y0);
		z_backward = pvertexmat_->evaluate(xmax, y0);
		pdx = x0 / z0 * (z_backward - z_forward) / dx / 2.0;
	}
	else if (pvertexmat_->xmin() >= scaled_x0)
	{
#ifdef DEBUG
		choice_branch = 4;
#endif // DEBUG
		z0 = pvertexmat_->evaluate(xmin, y0);
		z_backward = pvertexmat_->evaluate(xmin + dx, y0);
		pdx = xmin / z0 * (z_backward - z0) / dx;
	}
	else if (pvertexmat_->xmax() <= scaled_x0)
	{
#ifdef DEBUG
		choice_branch = 5;
#endif // DEBUG
		z0 = pvertexmat_->evaluate(xmax, y0);
		z_forward = pvertexmat_->evaluate(xmax - dx, y0);
		pdx = xmax / z0 * (z0 - z_forward) / dx;
	}
	else
	{
		// Unkown Error Case
		assert(0);
	}
#ifdef DEBUG
	std::printf("DEBUG Partial X:\n");
	std::printf("choice_branch = %d, xmin = %e, xmax = %e\n", choice_branch, xmin, xmax);
	std::printf("x0 = %e, y0 = %e, z0 = %e, dx = %e\n", x0, y0, z0, dx);
	std::printf("scaled_x0 = %e, scaled_y0 = %e, scaled_dx = %e\n", scaled_x0, scaled_y0, scaled_dx);
	std::printf("z_forward = %e, z_backward = %e, pdx = %e\n", z_forward, z_backward, pdx);
	std::fflush(stdout);
#endif // DEBUG
	return pdx;
}

// Sens_y: y0 / F0 * dF/dy, x=x0, y=y0, F0=F(x0,y0)
template<typename GLType, typename ObjPtrType, typename EvaluatorType>
const GLType Surface<GLType, ObjPtrType, EvaluatorType>::
Sens_y(const GLType &scaled_x0, const GLType &scaled_y0) const
{
	assert(pvertexmat_);
	const GLType ymin = recover_y(pvertexmat_->ymin());
	const GLType ymax = recover_y(pvertexmat_->ymax());
	const GLType scaled_dy(
		((*pvertexmat_)[1][0].y()-(*pvertexmat_)[0][0].y()));
	assert(scaled_dy);
	// factor should be less than 1.0 so as to get a smaller dy.
	const GLType factor(0.01);
	GLType dy(factor * (recover_y(scaled_dy) - (ymin + ymax) / 2.0));
	const GLType x0(recover_x(scaled_x0));
	GLType y0(0.0), z0(0.0), z_forward(0.0), z_backward(0.0);
	GLType pdy(0.0); // Partial Derivative

#ifdef DEBUG
	int choice_branch = 0;
#endif // DEBUG
	assert(dy <= factor * (ymax - ymin));

	if (pvertexmat_->ymin() < scaled_y0 - scaled_dy &&
			scaled_y0 + scaled_dy < pvertexmat_->ymax())
	{
#ifdef DEBUG
	choice_branch = 1;
#endif // DEBUG
		y0 = recover_y(scaled_y0);
		z0 = pvertexmat_->evaluate(x0, y0);
		z_forward =  pvertexmat_->evaluate(x0, y0 - dy);
		z_backward = pvertexmat_->evaluate(x0, y0 + dy);
		pdy = y0 / z0 * (z_backward - z_forward) / dy / 2.0;
	}
	else if (pvertexmat_->ymin() >= scaled_y0 - scaled_dy &&
					 pvertexmat_->ymin() < scaled_y0)
	{
#ifdef DEBUG
	choice_branch = 2;
#endif // DEBUG
		y0 = recover_y(scaled_y0);
		z0 = pvertexmat_->evaluate(x0, y0);
		dy = y0 - ymin;
		z_forward =  pvertexmat_->evaluate(x0, ymin);
		z_backward = pvertexmat_->evaluate(x0, y0 + dy);
		pdy = y0 / z0 * (z_backward - z_forward) / dy / 2.0;
	}
	else if (scaled_y0 + scaled_dy >= pvertexmat_->ymax() &&
					 scaled_y0 < pvertexmat_->ymax())
	{
#ifdef DEBUG
	choice_branch = 3;
#endif // DEBUG
		y0 = recover_y(scaled_y0);
		z0 = pvertexmat_->evaluate(x0, y0);
		dy = ymax - y0;
		z_forward =  pvertexmat_->evaluate(x0, y0 - dy);
		z_backward = pvertexmat_->evaluate(x0, ymax);
		pdy = y0 / z0 * (z_backward - z_forward) / dy / 2.0;
	}
	else if (pvertexmat_->ymin() >= scaled_y0)
	{
#ifdef DEBUG
	choice_branch = 4;
#endif // DEBUG
		z0 = pvertexmat_->evaluate(x0, ymin);
		z_backward = pvertexmat_->evaluate(x0, ymin + dy);
		pdy = ymin / z0 * (z_backward - z0) / dy;
	}
	else if (pvertexmat_->ymax() <= scaled_y0)
	{
#ifdef DEBUG
	choice_branch = 5;
#endif // DEBUG
		z0 = pvertexmat_->evaluate(x0, ymax);
		z_forward = pvertexmat_->evaluate(x0, ymax - dy);
		pdy = ymax / z0 * (z0 - z_forward) / dy;
	}
	else
	{
		// Unknow Error Case
		assert(0);
	}
#ifdef DEBUG
	std::printf("DEBUG Partial Y:\n");
	std::printf("choice_branch = %d, ymin = %e, ymax = %e\n", choice_branch, ymin, ymax);
	std::printf("x0 = %e, y0 = %e, z0 = %e, dy = %e\n", x0, y0, z0, dy);
	std::printf("scaled_x0 = %e, scaled_y0 = %e, scaled_dy = %e\n", scaled_x0, scaled_y0, scaled_dy);
	std::printf("z_forward = %e, z_backward = %e, pdy = %e\n", z_forward, z_backward, pdy);
	std::fflush(stdout);
#endif // DEBUG
	return pdy;
}

#endif // _SURFACE_H_

