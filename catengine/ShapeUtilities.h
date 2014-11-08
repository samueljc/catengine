#ifndef SHAPE_UTILITIES_H
#define SHAPE_UTILITIES_H

#include "Rectangle.h"

namespace catengine {
  Rectangle intersection(Rectangle const& a, Rectangle const& b) {
    if (a.width == 0.0f || a.height == 0.0f || b.width == 0.0f || b.height == 0.0f)
      return Rectangle();

  }
}

#endif