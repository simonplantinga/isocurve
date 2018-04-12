#ifndef TYPEINTERVAL_H
#define TYPEINTERVAL_H
#include <boost/numeric/interval.hpp>
#include <fstream>
#include<math.h>
#include <gmp.h>
#include <mpfr.h>

//#########################for trancendental functions############################
struct full_rounding:
  boost::numeric::interval_lib::rounded_arith_opp<double>
{
private:
  typedef int mpfr_func(mpfr_t, const __mpfr_struct*, mp_rnd_t);
  double invoke_mpfr(double x, mpfr_func f, mp_rnd_t r) {
    mpfr_t xx;
    mpfr_init_set_d(xx, x, GMP_RNDN);
    f(xx, xx, r);
    double res = mpfr_get_d(xx, r);
    mpfr_clear(xx);
    return res;
  }
public:
# define GENR_FUNC(name) \
  double name##_down(double x) { return invoke_mpfr(x, mpfr_##name, GMP_RNDD); } \
  double name##_up  (double x) { return invoke_mpfr(x, mpfr_##name, GMP_RNDU); }
  GENR_FUNC(exp)
  GENR_FUNC(log)
  GENR_FUNC(sin)
  GENR_FUNC(cos)
  GENR_FUNC(tan)
  GENR_FUNC(asin)
  GENR_FUNC(acos)
  GENR_FUNC(atan)
  GENR_FUNC(sinh)
  GENR_FUNC(cosh)
  GENR_FUNC(tanh)
  GENR_FUNC(asinh)
  GENR_FUNC(acosh)
  GENR_FUNC(atanh)
};

namespace dummy {
  using namespace boost;
  using namespace numeric;
  using namespace interval_lib;
  typedef save_state<full_rounding> R;
  typedef checking_strict<double> P;
  typedef interval<double, policies<R, P> > I;
};

typedef dummy::I Interval;

#endif
