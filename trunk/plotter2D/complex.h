#ifndef _COMPLEX_H_
#define _COMPLEX_H_

#include <complex>

class Complex : public std::complex<double>
{
	static const double PI_;
public:
	typedef double value_type;

	Complex(double re = 0, double im = 0); //!< default constructor
	Complex(const std::complex<double> &z); //!< copy constructor
	virtual ~Complex(); //!< destructor
	double mag() const; //!< return the magnitude of complex
	double arg() const; //!< return phase angle of complex: degree/s
};

#endif // _COMPLEX_H_
