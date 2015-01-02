#include "Rectangle.h"

namespace catengine {
Rectangle operator+(Rectangle const& lhs, Vector2d const& rhs)
{
  return Rectangle(lhs.x + rhs.x, lhs.y + rhs.y, lhs.width, lhs.height);
}

Rectangle operator-(Rectangle const& lhs, Vector2d const& rhs)
{
  return Rectangle(lhs.x - rhs.x, lhs.y - rhs.y, lhs.width, lhs.height);
}
}