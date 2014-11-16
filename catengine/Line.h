#ifndef LINE_SEGMENT_H
#define LINE_SEGMENT_H

#include "Types.h"

#include "Vector2.h"

namespace catengine {
  struct LineSegment {
    Vector2 start;
    Vector2 end;

    LineSegment() :
      start(),
      end()
    {
    }

    LineSegment(Vector2 start, Vector2 end) :
      start(start),
      end(end)
    {}

    LineSegment(_decimal x1, _decimal y1, _decimal x2, _decimal y2) :
      start(x1, y1),
      end(x2, y2)
    {}
  };
}

#endif