#ifndef POINT_2D_H
#define POINT_2D_H

#include "Types.h"
#include "MathF.h"
#include "Vector2d.h"
#include "Point3d.h"

namespace catengine {
struct Point2d {
  _decimal x;
  _decimal y;

  Point2d() :
    x(0.f),
    y(0.f)
  {}

  Point2d(_decimal x, _decimal y) :
    x(x),
    y(y)
  {}

  inline bool operator==(Point2d const& p) const
  {
    return x == p.x && y == p.y;
  }
  inline bool operator!=(Point2d const& p) const
  {
    return !(*this == p);
  }

  inline Point2d& operator+=(Vector2d const& v)
  {
    x += v.x;
    y += v.y;
    return *this;
  }
  inline Point2d& operator-=(Vector2d const& v)
  {
    x -= v.x;
    y -= v.y;
    return *this;
  }

  inline Point2d& operator=(Point2d const& p)
  {
    x = p.x;
    y = p.y;
    return *this;
  }

  inline explicit operator Vector2d() const { return Vector2d(x, y); }
  inline explicit operator Point3d() const { return Point3d(x, y, 0.0f); }

  friend Point2d operator+(Point2d const& lhs, Vector2d const& rhs);
  friend Point2d operator-(Point2d const& lhs, Vector2d const& rhs);

  friend Vector2d operator-(Point2d const& lhs, Point2d const& rhs);
};
}

#endif