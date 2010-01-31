#include "Analysis.h"

const double Analysis::PI_ = 3.1415926535897932384626433832795028841971;

Analysis::Analysis()
{
  init();
}

Analysis::~Analysis()
{}

void
Analysis::EvaluateSCoeff()
{
  a0 = 1 / (c1.value * c2.value * r1.value * r2.value);
  b0 = 1 / (c1.value * c2.value * r1.value * r2.value);
  b1 = (r1.value + r2.value) / (c1.value * r1.value * r2.value);
  b2 = 1;
}

complex_double
Analysis::EvaluateSCoeffFreq(const double freq) const
{
  // s variable of the transfer function (TF), i.e. 0 + j * 2*pi*freq
  complex_double s(0.0, 2.0 * PI_ * freq);
  return a0 / (b0 + b1 * s + b2 * s * s);
}

void
Analysis::init()
{
  c1.name = "C1"; c1.value = 1e-9;
  c2.name = "C2"; c2.value = 1e-9;
  r1.name = "R1"; r1.value = 10e3;
  r2.name = "R2"; r2.value = 10e3;
}

void
Analysis::generate_parname2ptrvalue_map(
  std::map<std::string, double *> &pn2ptrv_map)
{
  pn2ptrv_map.clear();
  // Create a new parameter-name-to-value-pointer map
  pn2ptrv_map[c1.name] = &c1.value;
  pn2ptrv_map[c2.name] = &c2.value;
  pn2ptrv_map[r1.name] = &r1.value;
  pn2ptrv_map[r2.name] = &r2.value;
}

