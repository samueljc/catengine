#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "Types.h"

namespace catengine {
  struct Rectangle {
    _decimal x;
    _decimal y;
    _decimal width;
    _decimal height;

    Rectangle() :
      Rectangle(0.0f, 0.0f, 0.0f, 0.0f)
    {
    }

    Rectangle(_decimal x, _decimal y, _decimal width, _decimal height) :
      x(x),
      y(y),
      width(width),
      height(height)
    {
    }

    inline _decimal left() const
    {
      return x;
    }
    inline _decimal right() const
    {
      return x + width;
    }
    inline _decimal top() const
    {
      return y;
    }
    inline _decimal bottom() const
    {
      return y + height;
    }

    inline _decimal center_x() const 
    {
      if (width == 0.0f) return x;
      return x + width / 2.0f;
    }
    inline _decimal center_y() const 
    {
      if (height == 0.0f) return y;
      return y + width / 2.0f;
    }
  };
}

#endif