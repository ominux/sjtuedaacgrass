#ifndef _ANALYSIS_H_
#define _ANALYSIS_H_
#include<map>
#include<string>
#include<complex>

typedef std::complex<double> complex_double;

/// A mimic of a symbolic analysis
/**
 * CAUTION: The private memebers are just for testing purpose.

 * Sample circuit: Sallen-Key low-pass filter

 * http://en.wikipedia.org/wiki/Sallen%E2%80%93Key_topology, Figure 2.
 * \f[
       H(s) = \frac{a_0}{b_0 + b_1s + b_2s^2},
 * \f]
 * where
 * \f[
 *     a_0 = \frac{1}{C_1C_2R_1R_2},
 *     b_0 = \frac{1}{C_1C_2R_1R_2},
 *     b_1 = \frac{R_1 + R_2}{C_1R_1R_2},
 *     b_2 = 1.
 * \f]
 */
class Analysis
{
  /// Mathematical constant \f$\pi\f$.
  static const double PI_;
public:
  Analysis(); 
  ~Analysis();
  /// Evaluate the coefficients of the Transfer Function (TF) after s-expanding.
  void
  EvaluateSCoeff();
  /**
   * \brief Get the complex value of the transfer function (TF) of the TPDD
	 * with any frequency.
	 *
   * The transfer function is s-variable coefficients extracted. That means
   * the TF is
	 *
   * \f[
	 *    H(s) = \frac{a_0 + a_1s + a_2s^2 + ... + a_ms^m}
	 *                {b_0 + b_1s + b_2s^2 + ... + b_ns^n}.
	 * \f]
   * The TF with s-variable coefficients extracted is built already before
   * invoking this function.
	 *
   * All of the s-variable coefficients is recorded in the vector named
   * sCoeffList in the two children nodes of the root of the sexpTpddNode
   * in the TPDD.
	 *
   * The order of the s-variable is recored in the vector named sPowerList
   * in the two children nodes of the root of the sexpTpddNode in the TPDD.
	 *
   * \param freq is the frequency (Hz) of the s-variable, where
	 * \f$ s = 2 \pi j \mbox{freq} \f$.
   */
  complex_double
  EvaluateSCoeffFreq(const double freq) const;

private:
  struct {
    const char *name;
    double value;
  } c1, //!< Parameter of the transfer function: \f$C_1\f$
	 	c2, //!< Parameter of the transfer function: \f$C_2\f$
	 	r1, //!< Parameter of the transfer function: \f$R_1\f$
	 	r2; //!< Parameter of the transfer function: \f$R_2\f$

  double a0, //!< Coefficient of the s-expanded transfer function: \f$a_0\f$
				 b0, //!< Coefficient of the s-expanded transfer function: \f$b_0\f$
				 b1, //!< Coefficient of the s-expanded transfer function: \f$b_1\f$
				 b2, //!< Coefficient of the s-expanded transfer function: \f$b_2\f$
				 b3; //!< Coefficient of the s-expanded transfer function: \f$b_3\f$
  /// \brief This function initializes all parameters.
  //! It MUST be invoked in the constructor.
  void
  init();
public:
  /// Generate a map from each parameter name to the pointer to its value
  void
  generate_parname2ptrvalue_map(
    std::map<std::string, double *> &pn2ptrv_map);

};

#endif // _ANALYSIS_H_
