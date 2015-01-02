#ifndef COLOR_H
#define COLOR_H

#include <iomanip>
#include <iostream>
#include <sstream>

#include "Types.h"
#include "MathF.h"

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
    inline void r(_decimal r) { this->r_ = bound(r); }
    inline void g(_decimal g) { this->g_ = bound(g); }
    inline void b(_decimal b) { this->b_ = bound(b); }
    inline void a(_decimal a) { this->a_ = bound(a); }

    inline _decimal r() const { return r_; }
    inline _decimal g() const { return g_; }
    inline _decimal b() const { return b_; }
    inline _decimal a() const { return a_; }

    // bounded integer setters
    inline void r_u(_unsigned r) { this->r_ = from_int(r); }
    inline void g_u(_unsigned g) { this->g_ = from_int(g); }
    inline void b_u(_unsigned b) { this->b_ = from_int(b); }
    inline void a_u(_unsigned a) { this->a_ = from_int(a); }

    inline _unsigned r_u() const { return to_int(r_); }
    inline _unsigned g_u() const { return to_int(g_); }
    inline _unsigned b_u() const { return to_int(b_); }
    inline _unsigned a_u() const { return to_int(a_); }

    inline bool operator==(Color const& rhs) const {
      return abserr(r_, rhs.r_) &&
        abserr(g_, rhs.g_) &&
        abserr(b_, rhs.b_) &&
        abserr(a_, rhs.a_);
    }
    inline bool operator!=(Color const& rhs) const { return !(*this == rhs); }

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