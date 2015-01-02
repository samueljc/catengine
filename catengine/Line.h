#ifndef LINE_SEGMENT_H
#define LINE_SEGMENT_H

#include "Types.h"
#include "Vector2d.h"

namespace catengine {
struct Line {
  _decimal intercept;
  Vector2d direction;

  Line() :
    intercept(0.f),
    direction()
  {}

  Line(_decimal intercept, Vector2d const& direction) :
    intercept(intercept),
    direction(direction)
  {}
};
}

#endif