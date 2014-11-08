#ifndef VECTOR2_H
#define VECTOR2_H

namespace catengine {
  /**
   * A 2d vector for use with our 2d matrices.
   */
  struct Vector2 {
    float x;
    float y;

    Vector2() : 
      Vector2(0.0f, 0.0f) 
    {
    }

    Vector2(float a, float b) :
      x(a),
      y(b) 
    {
    }

    Vector2(Vector2 const& v) :
      Vector2(v.x, v.y) 
    {
    }

    Vector2 operator+(Vector2 const& v) const 
    {
      return Vector2(x + v.x, y + v.y);
    }
    Vector2 operator-(Vector2 const& v) const
    {
      return Vector2(x + v.x, y + v.y);
    }
    Vector2 operator*(Vector2 const& v) const 
    {
      return Vector2(x * v.x, y * v.y);
    }

    Vector2 operator+(float f) const 
    {
      return Vector2(x + f, y + f);
    }
    Vector2 operator-(float f) const 
    {
      return Vector2(x - f, y - f);
    }
    Vector2 operator*(float f) const 
    {
      return Vector2(x * f, y * f);
    }
    Vector2 operator/(float f) const
    {
      return Vector2(x / f, y / f);
    }

    Vector2& operator+=(Vector2 const& v)
    {
      x += v.x;
      y += v.y;
      return *this;
    }
    Vector2& operator-=(Vector2 const& v) 
    {
      x -= v.x;
      y -= v.y;
      return *this;
    }

    Vector2 operator-() const 
    {
      return Vector2(-x, -y);
    }

    // TODO: maybe make this accept values that aren't exactly equal
    inline bool operator==(Vector2 const& v) const
    {
      return x == v.x && y == v.y;
    }
    inline bool operator!=(Vector2 const& v) const 
    {
      return x != v.x || y != v.y;
    }

    Vector2& operator=(Vector2 const& v) 
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
    inline float cross(Vector2 const& v) const 
    {
      return x * v.y - y * v.x;
    }

    /**
     * The dot product of this vector and v.
     * @param Vector on the right hand side of the dot product equation.
     * @return Scalar value of the dot product.
     */
    inline float dot(Vector2 const& v) const 
    {
      return x * v.x + y * v.y;
    }

    /**
     * Normalize in place. This method transforms this vector into its normal
     * vector.
     * @return This vector itself.
     */
    Vector2& normalize() 
    {
      Vector2 n = normal();
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
    Vector2 normal() const
    {
      float m = mag();
      if (m > 0.0f) {
        float d = 1.0f / m;
        return Vector2(x * d, y * d);
      }
      else {
        return Vector2();
      }
    }

    /**
     * The magnitude of the vector.
     */
    float mag() const 
    {
      return sqrtf(x * x + y * y);
    }

    /**
    * The squared magnitude of the vector.
    */
    inline float sqmag() const 
    {
      return x * x + y * y;
    }

    friend Vector2 lerp(Vector2 const& a, Vector2 const& b, float s);
    friend Vector2 operator+(float a, Vector2 const& b);
    friend Vector2 operator*(float a, Vector2 const& b);
  };

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

#endif