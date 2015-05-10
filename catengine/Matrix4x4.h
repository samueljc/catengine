#ifndef MATRIX_4x4_H
#define MATRIX_4x4_H

#include "Logging.h"
#include "Vector4.h"

#include <stdexcept>
#include <array>

namespace catengine {
///
// Matrix4x4 for use in place of the DirectX library DXMATRIX
// 
struct Matrix4x4 {
private:
  typedef std::array<Vector4, 4> data_type;

public:
  data_type data;
  
  inline Matrix4x4() = default;

  inline Matrix4x4(Vector4 row1, Vector4 row2, Vector4 row3, Vector4 row4)
  {
    // fixme: this is a hack to get around the fact that the VC++ compiler is garbage
    data = data_type{{ row1, row2, row3, row4 }};
  }

  inline Matrix4x4 transpose() const
  {
    return {
      { data[0][0], data[1][0], data[2][0], data[3][0] },
      { data[0][1], data[1][1], data[2][1], data[3][1] },
      { data[0][2], data[1][2], data[2][2], data[3][2] },
      { data[0][3], data[1][3], data[2][3], data[3][3] }
    };
  }

  inline float get(int row, int column) const {
    return this->operator[](row)[column];
  }
  inline float get(int index) const {
    return get(index / 4, index % 4);
  }

  inline Vector4 const& operator[](_unsigned i) const
  {
    if (i >= 4) {
      LOG(FATAL) << "Matrix4x4 access out of bounds";
    }
    return data[i];
  }
  inline Vector4& operator[](_unsigned i) 
  {
    if (i >= 4) {
      LOG(FATAL) << "Matrix4x4 access out of bounds";
    }
    return data[i];
  }

  inline bool operator==(Matrix4x4 const& rhs) const {
    return memcmp(this, &rhs, sizeof(Matrix4x4)) == 0;
  }
  inline bool operator!=(Matrix4x4 const& rhs) const {
    return !(this->operator==(rhs));
  }

  friend Vector4 operator*(Matrix4x4 const& matrix, Vector4 const& vector);
  friend Matrix4x4 operator*(Matrix4x4 const& lhs, Matrix4x4 const& rhs);

  static Matrix4x4 const& create_identity() {
    return identity_;
  }

private:
  static const Matrix4x4 identity_;
};
}

#endif