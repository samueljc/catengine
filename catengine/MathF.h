#ifndef MATHF_H
#define MATHF_H

#include <cmath>

namespace catengine {
  static float const PI = 3.14159265359f;
  static float const PI2 = 6.28318530718f;

  /**
   * Calculate the sqrt of x. Currently wraps the existing std::math sqrt
   * function, but this will allow me to easily modify it later to include
   * caching or any other optimizations.
   *
   * @param x The value we want to get the square root of.
   * @return The square root.
   */
  inline float sqrt(float x)
  {
    return std::sqrt(x);
  }

  /**
   * Calculate the sine given an angle in radians.
   * @param x x
   * @return sin of x
   */
  inline float sin(float x)
  {
    return std::sin(x);
  }

  /**
  * Calculate the cosine given an angle in radians.
  * @param x x
  * @return cos of x
  */
  inline float cos(float x)
  {
    return std::cos(x);
  }

  /**
  * Calculate the tangent given an angle in radians.
  * @param x x
  * @return tan of x
  */
  inline float tan(float x)
  {
    return std::tan(x);
  }

  inline float deg2rad(float deg)
  {
    return deg * PI / 180.0f;
  }
  inline float rad2deg(float rad)
  {
    return rad * 180.0f / PI;
  }
}

#endif