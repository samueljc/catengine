#ifndef MATHF_H
#define MATHF_H

#include <cmath>

#include "Types.h"

namespace catengine {
  static _decimal const PI = 3.14159265359f;
  static _decimal const PI2 = 6.28318530718f;

  /**
   * Calculate the sqrt of x. Currently wraps the existing std::math sqrt
   * function, but this will allow me to easily modify it later to include
   * caching or any other optimizations.
   *
   * @param x The value we want to get the square root of.
   * @return The square root.
   */
  inline _decimal sqrt(_decimal x)
  {
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
}

#endif