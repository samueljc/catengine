#ifndef RECTANGLE_H
#define RECTANGLE_H

namespace catengine {
  struct Rectangle {
    float x;
    float y;
    float width;
    float height;

    Rectangle() :
      Rectangle(0.0f, 0.0f, 0.0f, 0.0f)
    {
    }

    Rectangle(float x, float y, float width, float height) :
      x(x),
      y(y),
      width(width),
      height(height)
    {
    }

    inline float left() const
    {
      return x;
    }
    inline float right() const
    {
      return x + width;
    }
    inline float top() const
    {
      return y;
    }
    inline float bottom() const
    {
      return y + height;
    }

    inline float center_x() const 
    {
      if (width == 0.0f) return x;
      return x + width / 2.0f;
    }
    inline float center_y() const 
    {
      if (height == 0.0f) return y;
      return y + width / 2.0f;
    }
  };
}

#endif