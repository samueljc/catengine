#include "Vector2.h"

namespace catengine {
  Vector2 lerp(Vector2 const& a, Vector2 const& b, _decimal s)
  {
    return Vector2(a.x + s * (b.x - a.x), b.y + s * (b.y - a.y));
  }

  Vector2 operator+(_decimal a, Vector2 const& b)
  {
    return Vector2(a + b.x, a + b.y);
  }

  Vector2 operator*(_decimal a, Vector2 const& b)
  {
    return Vector2(a * b.x, a * b.y);
  }
}