#include "Vector2d.h"

namespace catengine {
  Vector2d operator+(Vector2d const& lhs, Vector2d const& rhs)
  {
    return Vector2d(lhs.x + rhs.x, lhs.y + rhs.y);
  }
  Vector2d operator-(Vector2d const& lhs, Vector2d const& rhs)
  {
    return Vector2d(lhs.x - rhs.x, lhs.y - rhs.y);
  }
  Vector2d operator*(Vector2d const& lhs, Vector2d const& rhs)
  {
    return Vector2d(lhs.x * rhs.x, lhs.y * rhs.y);
  }

  Vector2d operator+(Vector2d const& lhs, _decimal rhs)
  {
    return Vector2d(lhs.x + rhs, lhs.y + rhs);
  }
  Vector2d operator-(Vector2d const& lhs, _decimal rhs)
  {
    return Vector2d(lhs.x - rhs, lhs.y - rhs);
  }
  Vector2d operator*(Vector2d const& lhs, _decimal rhs)
  {
    return Vector2d(lhs.x * rhs, lhs.y * rhs);
  }
  Vector2d operator/(Vector2d const& lhs, _decimal rhs)
  {
    return Vector2d(lhs.x / rhs, lhs.y / rhs);
  }

  Vector2d operator+(_decimal a, Vector2d const& b)
  {
    return Vector2d(a + b.x, a + b.y);
  }
  Vector2d operator*(_decimal a, Vector2d const& b)
  {
    return Vector2d(a * b.x, a * b.y);
  }

  Vector2d lerp(Vector2d const& a, Vector2d const& b, _decimal s)
  {
    return Vector2d(a.x + s * (b.x - a.x), b.y + s * (b.y - a.y));
  }
}