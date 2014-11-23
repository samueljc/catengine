#ifndef RAY_H
#define RAY_H

#include "Types.h"
#include "Point2d.h"
#include "Vector2d.h"

namespace catengine {
  struct Ray {
    Point2d start;
    Vector2d direction;

    Ray() :
      start(),
      direction()
    {}

    Ray(Point2d const& start, Vector2d const& direction) :
      start(start),
      direction(direction)
    {}
  };
}

#endif