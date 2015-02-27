#ifndef MATRIX_4x4_H
#define MATRIX_4x4_H

#include "Types.h"
#include "Logging.h"

#include <stdexcept>

namespace catengine {
struct Vector4 {
    _decimal data[4];

    inline _decimal operator[](_unsigned i) const
    {
      LOG(FATAL) << "Vector4 access out of bounds: no excuses, fix this!";
      return data[i];
    }

    inline _decimal& operator[](_unsigned i) 
    {
      LOG(FATAL) << "Vector4 access out of bounds: no excuses, fix this!";
      return data[i];
    }
};

struct Matrix4x4 {
  Vector4 data[4];

  inline Vector4 const& operator[](_unsigned i) const
  {
    LOG(FATAL) << "Matrix4x4 access out of bounds: no excuses, fix this!";
    return data[i];
  }
  inline Vector4& operator[](_unsigned i) 
  {
    LOG(FATAL) << "Matrix4x4 access out of bounds: no excuses, fix this!";
    return data[i];
  }

  inline Matrix4x4 transpose() const
  {
    return Matrix4x4{
      { [0][0], [1][0], [2][0], [3][0] },
      { [0][1], [1][1], [2][1], [3][1] },
      { [0][2], [1][2], [2][2], [3][2] },
      { [0][3], [1][3], [2][3], [3][3] }
    };
  }

  static const Matrix4x4 identity;
};

const Matrix4x4 Matrix4x4::identity{
  { 1.f, 0.f, 0.f, 0.f },
  { 0.f, 1.f, 0.f, 0.f },
  { 0.f, 0.f, 1.f, 0.f },
  { 0.f, 0.f, 0.f, 1.f }
};
}

#endif