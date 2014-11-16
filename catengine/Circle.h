#ifndef CIRCLE_H
#define CIRCLE_H

#include "Types.h"

namespace catengine {
  struct Circle {
    _decimal radius;
    _decimal x;
    _decimal y;

    Circle() : 
      Circle(0.0f, 0.0f, 0.0f)
    {
    }

    Circle(_decimal radius, _decimal x, _decimal y) :
      radius(radius),
      x(x),
      y(y)
    {
    }
  };
}

#endif