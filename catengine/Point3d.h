// Point3d.h
// This is a simple container that can be passed to a shader. Point2d and 
// Vector2d should be able to convert into this structure, but this structure
// has little purpose outside of passing values to the shader.

#ifndef POINT3D
#define POINT3D

#include "Types.h"

namespace catengine {
struct Point3d {
  _decimal x, y, z;

  Point3d() :
    Point3d(0.0f, 0.0f, 0.0f)
  {
  }

  Point3d(_decimal x, _decimal y, _decimal z = 0.0f) :
    x(x),
    y(y),
    z(z)
  {
  }
};
}

#endif