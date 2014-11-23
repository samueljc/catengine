#ifndef COLOR_H
#define COLOR_H

#include "Types.h"

namespace catengine {
  class Color {
  public:
    static Color BLACK;
    static Color WHITE;
    static Color RED;
    static Color GREEN;
    static Color BLUE;

    _decimal r;
    _decimal g;
    _decimal b;
    _decimal a;

    Color();
    inline Color(_decimal r, _decimal g, _decimal b) : 
      Color(r, g, b, 1.0f) 
    {
    }
    Color(_decimal r, _decimal g, _decimal b, _decimal a);
    inline Color(_integer r, _integer g, _integer b) : 
      Color(r, g, b, 255) 
    {
    }
    Color(_integer r, _integer g, _integer b, _integer a);
    Color(_unsigned rgba);

    // bounded setters
    inline void red(_decimal r) { this->r = bound(r); }
    inline void green(_decimal g) { this->g = bound(g); }
    inline void blue(_decimal b) { this->b = bound(b); }
    inline void alpha(_decimal a) { this->a = bound(a); }

    // bounded integer setters
    inline void red(_integer r) { this->r = from_int(r); }
    inline void green(_integer g) { this->g = from_int(g); }
    inline void blue(_integer b) { this->b = from_int(b); }
    inline void alpha(_integer a) { this->a = from_int(a); }

    // integer getters - does not check for boundedness
    inline _integer red() { return to_int(r); }
    inline _integer green() { return to_int(g); }
    inline _integer blue() { return to_int(b); }
    inline _integer alpha() { return to_int(a); }

  private:
    inline _decimal bound(_decimal v) { return (v <= 1.0f) ? (v >= 0.0f) ? v : 0.0f : 1.0f; }
    inline _integer to_int(_decimal v) { return _integer(v * 255.0f); }
    inline _decimal from_int(_integer v) { return bound(_decimal(v) / 255.0f); }
  };
}

#endif