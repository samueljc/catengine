#include "Vector2.h"

namespace catengine {
  Vector2 lerp(Vector2 const& a, Vector2 const& b, float s)
  {
    return Vector2(a.x + s * (b.x - a.x), b.y + s * (b.y - a.y));
  }

  Vector2 operator+(float a, Vector2 const& b)
  {
    return Vector2(a + b.x, a + b.y);
  }

  Vector2 operator*(float a, Vector2 const& b)
  {
    return Vector2(a * b.x, a * b.y);
  }
}