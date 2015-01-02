#include "Point2d.h"

namespace catengine {
Point2d operator+(Point2d const& lhs, Vector2d const& rhs)
{
  return Point2d(lhs.x + rhs.x, lhs.y + rhs.y);
}
Point2d operator-(Point2d const& lhs, Vector2d const& rhs)
{
  return Point2d(lhs.x - rhs.x, lhs.y - rhs.y);
}

Vector2d operator-(Point2d const& lhs, Point2d const& rhs)
{
  return Vector2d(lhs.x - rhs.x, lhs.y - rhs.y);
}
}