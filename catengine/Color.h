#ifndef COLOR_H
#define COLOR_H

#include <iomanip>
#include <iostream>
#include <sstream>

#include "Types.h"

namespace catengine {
  class Color {
  public:
    static Color BLACK;
    static Color WHITE;
    static Color RED;
    static Color GREEN;
    static Color BLUE;
    static Color YELLOW;
    static Color MAGENTA;
    static Color TEAL;
    static Color ELECTRIC_LIME;

    inline Color() :
      r_(0.0f),
      g_(0.0f),
      b_(0.0f),
      a_(1.0f)
    {
    }

    inline Color(_decimal r, _decimal g, _decimal b) : 
      Color(r, g, b, 1.0f) 
    {
    }

    inline Color(_decimal r, _decimal g, _decimal b, _decimal a) :
      r_(bound(r)),
      g_(bound(g)),
      b_(bound(b)),
      a_(bound(a))
    {
    }

    inline Color(_unsigned r, _unsigned g, _unsigned b) :
      Color(r, g, b, 255) 
    {
    }

    inline Color(_unsigned r, _unsigned g, _unsigned b, _unsigned a) :
      r_(from_int(r)),
      g_(from_int(g)),
      b_(from_int(b)),
      a_(from_int(a))
    {
    }

    inline Color(_unsigned rgba) :
      r_(from_int((rgba >> 24) & 0xff)),
      g_(from_int((rgba >> 16) & 0xff)),
      b_(from_int((rgba >> 8) & 0xff)),
      a_(from_int(rgba & 0xff))
    {
    }

    // bounded setters
    inline void red(_decimal r) { this->r_ = bound(r); }
    inline void green(_decimal g) { this->g_ = bound(g); }
    inline void blue(_decimal b) { this->b_ = bound(b); }
    inline void alpha(_decimal a) { this->a_ = bound(a); }

    // bounded integer setters
    inline void red(_unsigned r) { this->r_ = from_int(r); }
    inline void green(_unsigned g) { this->g_ = from_int(g); }
    inline void blue(_unsigned b) { this->b_ = from_int(b); }
    inline void alpha(_unsigned a) { this->a_ = from_int(a); }

    inline _unsigned red() { return to_int(r_); }
    inline _unsigned green() { return to_int(g_); }
    inline _unsigned blue() { return to_int(b_); }
    inline _unsigned alpha() { return to_int(a_); }

    inline bool operator==(Color const& other) const {
      return (r_ == other.r_ && g_ == other.g_ && b_ == other.b_ && a_ == other.a_);
    }
    inline bool operator!=(Color const& other) const { return !(*this == other); }

    // don't want implicit conversion as it probably won't make sense most of the time
    explicit inline operator _unsigned() const {
      _unsigned c =  to_int(r_) << 24;
      c |= to_int(g_) << 16;
      c |= to_int(b_) << 8;
      c |= to_int(a_);
      return c;
    }

    inline operator std::string() const {
      std::ostringstream os;
      os << *this;
      return os.str();
    }

    friend std::ostream& operator<<(std::ostream& os, Color const& color) {
      os << std::hex << "0x" << std::setfill('0') << std::setw(8) << static_cast<catengine::_unsigned>(color);
      return os;
    }

  private:
    _decimal r_;
    _decimal g_;
    _decimal b_;
    _decimal a_;

    inline static _decimal bound(_decimal v) { return (v <= 1.0f) ? (v >= 0.0f) ? v : 0.0f : 1.0f; }
    inline static _unsigned to_int(_decimal v) { return _unsigned(v * 255.0f); }
    inline static _decimal from_int(_unsigned v) { return (v > 255 ? 255.0f : _decimal(v)) / 255.0f; }
  };
}

#endif