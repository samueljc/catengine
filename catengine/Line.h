#ifndef LINE_SEGMENT_H
#define LINE_SEGMENT_H

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

    LineSegment(float x1, float y1, float x2, float y2) :
      start(x1, y1),
      end(x2, y2)
    {}
  };
}

#endif