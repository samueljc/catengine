#include "Circle.h"

namespace catengine {
Circle operator+(Circle const& lhs, Vector2d const& rhs)
{
  return{ lhs.x + rhs.x, lhs.x + rhs.x, lhs.radius };
}

Circle operator-(Circle const& lhs, Vector2d const& rhs)
{
  return{ lhs.x - rhs.x, lhs.x - rhs.x, lhs.radius };
}
}