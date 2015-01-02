#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "Types.h"

#include "Vector2d.h"
#include "MathF.h"

namespace catengine {
struct Rectangle {
  _decimal x;
  _decimal y;
  _decimal width;
  _decimal height;

  Rectangle() :
    x(0.0f),
    y(0.0f),
    width(0.0f),
    height(0.0f)
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
    return y + height / 2.0f;
  }

  inline Rectangle& operator+=(Vector2d const& rhs)
  {
    x += rhs.x;
    y += rhs.y;
    return *this;
  }

  inline Rectangle& operator-=(Vector2d const& rhs)
  {
    x -= rhs.x;
    y -= rhs.y;
    return *this;
  }

  inline bool operator==(Rectangle const& rhs)
  {
    return abserr(x, rhs.x) && 
      abserr(y, rhs.y) &&
      abserr(width, rhs.width) &&
      abserr(height, rhs.height);
  }

  inline bool operator!=(Rectangle const& rhs)
  {
    return !(*this == rhs);
  }

  friend Rectangle operator+(Rectangle const& lhs, Vector2d const& rhs);
  friend Rectangle operator-(Rectangle const& lhs, Vector2d const& rhs);
};
}

#endif