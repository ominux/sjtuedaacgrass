#include "complex.h"

const double Complex::
PI_ = 3.1415926535897932384626433832795028841971;

Complex::
Complex(double re, double im) :
	std::complex<double>(re, im)
{}

Complex::
Complex(const std::complex<double> &z) :
	std::complex<double>(z)
{}

Complex::
~Complex()
{}

double Complex::
mag() const
{
	return std::abs(*this);
}

double Complex::
arg() const
{
	return std::arg(*this) * 180 / PI_;
}
