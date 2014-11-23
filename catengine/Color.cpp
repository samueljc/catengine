#include "Color.h"

using catengine::Color;

Color Color::BLACK(0.0f, 0.0f, 0.0f, 1.0f);
Color Color::WHITE(1.0f, 1.0f, 1.0f, 1.0f);
Color Color::RED(1.0f, 0.0f, 0.0f, 1.0f);
Color Color::GREEN(0.0f, 1.0f, 0.0f, 1.0f);
Color Color::BLUE(0.0f, 0.0f, 1.0f, 1.0f);

Color::Color() :
  r(0.0f),
  g(0.0f),
  b(0.0f),
  a(1.0f)
{
}

Color::Color(catengine::_decimal r, catengine::_decimal g,
             catengine::_decimal b, catengine::_decimal a) :
  r(bound(r)),
  g(bound(g)),
  b(bound(b)),
  a(bound(a))
{
}

Color::Color(catengine::_integer r, catengine::_integer g,
             catengine::_integer b, catengine::_integer a) :
  r(from_int(r)),
  g(from_int(g)),
  b(from_int(b)),
  a(from_int(a))
{
}

Color::Color(catengine::_unsigned rgba)
{

}