#ifndef CIRCLE_H
#define CIRCLE_H

#include "Types.h"
#include "Vector2d.h"
#include "Rectangle.h"

namespace catengine {
struct Circle {
  _decimal x;
  _decimal y;
  _decimal radius;

  Circle() : 
    Circle(0.0f, 0.0f, 0.0f)
  {
  }

  Circle(_decimal x, _decimal y, _decimal radius) :
    x(x),
    y(y),
    radius(radius)
  {
  }

  inline _decimal left() const { return x - radius; }
  inline _decimal right() const { return x + radius; }
  inline _decimal top() const { return y - radius; }
  inline _decimal bottom() const { return y + radius; }

  inline Rectangle bounds() const
  {
    return{ x - radius, y - radius, radius + radius, radius + radius };
  }

  inline Circle& operator+=(Vector2d const& rhs)
  {
    x += rhs.x;
    y += rhs.y;
    return *this;
  }

  inline Circle& operator-=(Vector2d const& rhs)
  {
    x -= rhs.x;
    y -= rhs.y;
    return *this;
  }

  friend Circle operator+(Circle const& lhs, Vector2d const& rhs);
  friend Circle operator-(Circle const& lhs, Vector2d const& rhs);
};
}

#endif