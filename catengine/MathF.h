#ifndef MATHF_H
#define MATHF_H

#include <cmath>

#include "Types.h"
#include "Logging.h"

namespace catengine {
static _decimal const PI = 3.14159265359f;
static _decimal const PI2 = 6.28318530718f;
static _decimal const ABSOLUTE_ERR = 0.00001f;
static _decimal const RELATIVE_ERR = 0.00001f;

/**
  * Calculate the sqrt of x. Currently wraps the existing std::math sqrt
  * function, but this will allow me to easily modify it later to include
  * caching or any other optimizations. If you provide a negative number
  * we'll currently log an error and return the value passed in.
  *
  * @param x The value we want to get the square root of.
  * @return The square root.
  */
inline _decimal sqrt(_decimal x)
{
  if (x < 0.0f) {
    LOG(WARNING) << "attempted to calculate square root of negative value";
    return x;
  }
  return std::sqrt(x);
}

/**
  * Calculate the sine given an angle in radians.
  * @param x x
  * @return sin of x
  */
inline _decimal sin(_decimal x)
{
  return std::sin(x);
}

/**
* Calculate the cosine given an angle in radians.
* @param x x
* @return cos of x
*/
inline _decimal cos(_decimal x)
{
  return std::cos(x);
}

/**
* Calculate the tangent given an angle in radians.
* @param x x
* @return tan of x
*/
inline _decimal tan(_decimal x)
{
  return std::tan(x);
}

inline _decimal deg2rad(_decimal deg)
{
  return deg * PI / 180.0f;
}
inline _decimal rad2deg(_decimal rad)
{
  return rad * 180.0f / PI;
}

/**
 * Compares two decimal point numbers using the absolute error defined in MathF.
 * @param a a
 * @param b b
 * @return if the difference between a and b is less than an absolute error.
 */
inline bool abserr(_decimal a, _decimal b)
{
  return fabsf(a - b) < ABSOLUTE_ERR;
}


/**
* Compares two decimal point numbers using a relative error. This is going to 
* be slower than using abserr.
* @param a a
* @param b b
* @return if the difference between a and b is less than a relative error
*/
inline bool relerr(_decimal a, _decimal b)
{
  if (a == b) return true;
  if (fabsf(a - b) < RELATIVE_ERR) return true;
  float relerr = fabs((a - b) / (fabsf(a) < fabsf(b) ? b : a));
  return relerr < RELATIVE_ERR;
}
}

#endif