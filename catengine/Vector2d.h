#ifndef VECTOR_2D_H
#define VECTOR_2D_H

#include "Types.h"
#include "MathF.h"

namespace catengine {
struct Vector2d {
  _decimal x;
  _decimal y;

  Vector2d() : 
    Vector2d(0.0f, 0.0f) 
  {
  }

  Vector2d(_decimal a, _decimal b) :
    x(a),
    y(b) 
  {
  }

  Vector2d& operator+=(Vector2d const& v)
  {
    x += v.x;
    y += v.y;
    return *this;
  }
  Vector2d& operator+=(_decimal f)
  {
    x += f;
    y += f;
    return *this;
  }
  Vector2d& operator-=(Vector2d const& v)
  {
    x -= v.x;
    y -= v.y;
    return *this;
  }
  Vector2d& operator-=(_decimal f)
  {
    x -= f;
    y -= f;
    return *this;
  }
  Vector2d& operator*=(_decimal f)
  {
    x *= f;
    y *= f;
    return *this;
  }

  Vector2d operator-() const 
  {
    return Vector2d(-x, -y);
  }

  // TODO: maybe make this accept values that aren't exactly equal
  inline bool operator==(Vector2d const& v) const
  {
    return x == v.x && y == v.y;
  }
  inline bool operator!=(Vector2d const& v) const 
  {
    return x != v.x || y != v.y;
  }

  Vector2d& operator=(Vector2d const& v) 
  {
    x = v.x;
    y = v.y;
    return *this;
  }

  /**
    * Get the cross product of 2d vectors by treating them as 3d vectors
    * with the z-component set to 0.
    * @param The right vector.
    * @return Scalar value of the 2d cross product.
    */
  inline _decimal cross(Vector2d const& v) const 
  {
    return x * v.y - y * v.x;
  }

  /**
    * The dot product of this vector and v.
    * @param Vector on the right hand side of the dot product equation.
    * @return Scalar value of the dot product.
    */
  inline _decimal dot(Vector2d const& v) const 
  {
    return x * v.x + y * v.y;
  }

  /**
    * Normalize in place. This method transforms this vector into its normal
    * vector.
    * @return This vector itself.
    */
  Vector2d& normalize() 
  {
    Vector2d n = normal();
    x = n.x;
    y = n.y;
    return *this;
  }

  /**
    * Create a normal vector from the current vector. Does not modify the
    * current vector.
    * @return A new vector containing the normalized data of the calling
    *         vector.
    */
  Vector2d normal() const
  {
    _decimal m = mag();
    if (m > 0.0f) {
      _decimal d = 1.0f / m;
      return Vector2d(x * d, y * d);
    }
    else {
      return Vector2d();
    }
  }

  /**
    * The magnitude of the vector.
    */
  _decimal mag() const 
  {
    return sqrt(x * x + y * y);
  }

  /**
  * The squared magnitude of the vector.
  */
  inline _decimal sqmag() const 
  {
    return x * x + y * y;
  }

  friend Vector2d operator+(Vector2d const& lhs, Vector2d const& rhs);
  friend Vector2d operator-(Vector2d const& lhs, Vector2d const& rhs);
  friend Vector2d operator*(Vector2d const& lhs, Vector2d const& rhs);

  friend Vector2d operator+(Vector2d const& lhs, _decimal rhs);
  friend Vector2d operator-(Vector2d const& lhs, _decimal rhs);
  friend Vector2d operator*(Vector2d const& lhs, _decimal rhs);
  friend Vector2d operator/(Vector2d const& lhs, _decimal rhs);

  friend Vector2d operator+(_decimal a, Vector2d const& b);
  friend Vector2d operator*(_decimal a, Vector2d const& b);

  friend Vector2d lerp(Vector2d const& a, Vector2d const& b, _decimal s);
};
}

#endif