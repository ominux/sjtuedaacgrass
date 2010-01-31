/*
 *    $RCSfile: complex_t.h,v $
 *    $Revision: 1.1 $
 *    $Date: 1999/04/29 19:16:38 $
 */

#ifndef complex_H_
#define complex_H_

#include <cmath>
#include <errno.h>

#include <iostream>

class complex_t {
public:
	complex_t() : re(0.0), im(0.0) {}
	explicit complex_t(const double d) : re(d), im(0.0) { }
	complex_t(const complex_t &c) : re(c.re), im(c.im) { }
	complex_t(double _v, double _i) : re(_v), im(_i) { }
	~complex_t() {}

	double real() const { return re; }
	double imag() const { return im; };

	complex_t &operator= (const complex_t &_z);
	void operator+= (const complex_t &);
	void operator+= (double);
	void operator-= (const complex_t &);
	void operator-= (double);
	void operator*= (const complex_t &);
	void operator*= (double);
	void operator/= (const complex_t &);
	void operator/= (double);

	friend double  real(const complex_t&);    // real part 
	friend double  imag(const complex_t&);    // imaginary part
	friend const complex_t conj(const complex_t &);        // complex_t conjugate
	friend double  norm(const complex_t &);        // square of the magnitude
	friend double  magn(const complex_t &);        // magnitude  
	friend double  db20(const complex_t &);        // voltage db(=20*log10(z))  
	friend double  arg(const complex_t &);        // angle in the complex_t plane

	// Create a complex_t object given polar coordinates
	friend const complex_t polar(double _mag, double _arg=0.0);

	// Overloaded ANSI C math functions
	friend double  abs(const complex_t &);
	friend const complex_t acos(const complex_t &);
	friend const complex_t asin(const complex_t &);
	friend const complex_t atan(const complex_t &);
	friend const complex_t cos(const complex_t &);
	friend const complex_t cosh(const complex_t &);
	friend const complex_t exp(const complex_t &); 
	friend const complex_t log(const complex_t &); 
	friend const complex_t log10(const complex_t &);
	friend const complex_t sin(const complex_t &);
	friend const complex_t sinh(const complex_t &);
	friend const complex_t sqrt(const complex_t &);
	friend const complex_t tan(const complex_t &);
	friend const complex_t tanh(const complex_t &);

	// Unary Operator Functions
	friend const complex_t operator- (const complex_t &);

	// Binary Operator Functions
	friend const complex_t operator+ (const complex_t&, const complex_t&);
	friend const complex_t operator+ (double, const complex_t&);
	friend const complex_t operator+ (const complex_t&, double);

	friend const complex_t operator- (const complex_t&, const complex_t&);
	friend const complex_t operator- (double, const complex_t&);
	friend const complex_t operator- (const complex_t&, double);

	friend const complex_t operator* (const complex_t&, const complex_t&);
	friend const complex_t operator* (const complex_t&, double);
	friend const complex_t operator* (double, const complex_t&);
	friend const complex_t operator/ (const complex_t&, const complex_t&);
	friend const complex_t operator/ (const complex_t&, double);
	friend const complex_t operator/ (double, const complex_t&);

	friend int operator== (const complex_t&, const complex_t&);
	friend int operator!= (const complex_t&, const complex_t&);

private:
	double re, im;
};

inline complex_t &complex_t::operator= (const complex_t &_z)
{
	if (this != &_z)
	{
		re = _z.re;
		im = _z.im;
	}
	return *this;
}
inline void complex_t::operator+= (const complex_t &_z) { re += _z.re; im += _z.im; }
inline void complex_t::operator+= (double _v2) { re += _v2; }
inline void complex_t::operator-= (const complex_t &_z) { re -= _z.re; im -= _z.im; }
inline void complex_t::operator-= (double _z) { re -= _z; }
inline void complex_t::operator*= (double _v) { re *= _v; im *= _v; }

// Friend functions
inline double real(const complex_t& _z) { return _z.re; }
inline double imag(const complex_t& _z) { return _z.im; }
inline const complex_t conj(const complex_t &_z) { return complex_t(_z.re, -_z.im); }
inline double norm(const complex_t &_z) { return _z.re*_z.re + _z.im*_z.im; }
// norm() is the sqrt of it, same as magn(). GShi.
inline double magn(const complex_t &_z) { return std::sqrt(norm(_z)); }
inline double db20(const complex_t &_z) { return 20.0*std::log10(magn(_z)); }
inline double arg(const complex_t &_z) { return std::atan2(_z.im, _z.re); }

// negative
inline const complex_t operator- (const complex_t &_z) { return complex_t(-_z.re, -_z.im); }

// binary +
inline const complex_t operator+ (const complex_t &_z1, const complex_t &_z2)
{ return complex_t(_z1.re + _z2.re, _z1.im + _z2.im); }
inline const complex_t operator+ (double _v1, const complex_t &_z2)
{ return complex_t(_v1 + _z2.re, _z2.im); }
inline const complex_t operator+ (const complex_t &_z1, double _v2)
{ return complex_t(_z1.re + _v2, _z1.im); }

// binary -
inline const complex_t operator- (const complex_t &_z1, const complex_t &_z2) {
	return complex_t(_z1.re - _z2.re, _z1.im - _z2.im);
}
inline const complex_t operator- (double _v1, const complex_t &_z2) {
	return complex_t(_v1 - _z2.re, -_z2.im);
}
inline const complex_t operator- (const complex_t &_z1, double _v2) {
	return complex_t(_z1.re - _v2, _z1.im);
}

// binary *
inline const complex_t operator* (const complex_t &_z1, const complex_t &_z2){
	return complex_t(_z1.re * _z2.re - _z1.im * _z2.im, _z1.re * _z2.im + _z1.im * _z2.re);
}
inline const complex_t operator* (const complex_t &_z1, double _v2) {
	return complex_t(_z1.re*_v2, _z1.im*_v2);
}
inline const complex_t operator* (double _v1, const complex_t &_z2) {
	return complex_t(_z2.re*_v1, _z2.im*_v1);
}
inline const complex_t operator/ (const complex_t &z1, double v2) {
	return complex_t(z1.re/v2, z1.im/v2);
}

inline const complex_t operator/ (const complex_t &z1, const complex_t &z2) {
	return complex_t((z1.re * z2.re + z1.im * z2.im) / (z2.re * z2.re + z2.im * z2.im),
									 (z1.im * z2.re - z1.re* z2.im) / (z2.re * z2.re + z2.im * z2.im));
}

inline int operator== (const complex_t &_z1, const complex_t &_z2) {
	return _z1.re == _z2.re && _z1.im == _z2.im;
}
inline int operator!= (const complex_t &_z1, const complex_t &_z2) {
	return _z1.re != _z2.re || _z1.im != _z2.im;
}
// inline const complex_t pow(const complex_t &_b, int _exp) { return std::pow(_b, (double)_exp); }

/*
 * complex_t stream I/O
 */


std::ostream& operator<< (std::ostream& out, const complex_t& data);
/*std::ostream& operator<< (std::ostream& out, const complex_t& data)
	{
	out << data.re << " + j" << data.im;
	return out;
	};*/    // user may override
//istream& operator>> (istream&, complex_t&);    // user may override


#endif /* _complex_H_ */

