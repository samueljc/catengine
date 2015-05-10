#ifndef VECTOR4_H
#define VECTOR4_H

#include "Logging.h"
#include "Types.h"

#include <array>

namespace catengine {
///
// Vector4 for use in place of the DirectX library implementation. For this reason
// the class has remained feature sparse and is primarily just for use with the
// Matrix4x4 for computing transformations.
struct Vector4 {
private:
  typedef std::array<_decimal, 4> data_type;

public:
  data_type data;

  inline Vector4()
  {
    // fixme: this is a hack to get around the fact that the VC++ compiler is garbage
    data = data_type{{ 0.f, 0.f, 0.f, 0.f }};
  }
  inline Vector4(_decimal x, _decimal y, _decimal z, _decimal w)
  {
    // fixme: this is a hack to get around the fact that the VC++ compiler is garbage
    data = data_type{{ x, y, z, w }};
  }
    
  inline _decimal operator[](_unsigned i) const
  {
    if (i >= 4) {
      LOG(FATAL) << "Vector4 access out of bounds";
    }
    return data[i];
  }
  inline _decimal& operator[](_unsigned i) 
  {
    if (i >= 4) {
      LOG(FATAL) << "Vector4 access out of bounds";
    }
    return data[i];
  }
};
}

#endif