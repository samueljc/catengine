#ifndef LINE_SEGMENT_H
#define LINE_SEGMENT_H

#include "Types.h"
#include "Point2d.h"

namespace catengine {
  struct LineSegment {
    Point2d start;
    Point2d end;

    LineSegment() :
      start(),
      end()
    {}

    LineSegment(Point2d const& start, Point2d const& end) :
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