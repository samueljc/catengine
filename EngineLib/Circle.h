#ifndef CIRCLE_H
#define CIRCLE_H

namespace catengine {
  struct Circle {
    float radius;
    float x;
    float y;

    Circle() : 
      Circle(0.0f, 0.0f, 0.0f)
    {
    }

    Circle(float radius, float x, float y) :
      radius(radius),
      x(x),
      y(y)
    {
    }
  };
}

#endif