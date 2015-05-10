#include "stdafx.h"

#include <sstream>
#include <iomanip>

#include "Matrix4x4.h"

using Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace catenginetest {
TEST_CLASS(Matrix4x4_test) {
public:
  TEST_METHOD(access_test) {
    for (int i = 0; i < 16; ++i) {
      float value = static_cast<float>(i + 1);
      Assert::AreEqual(value, test_[i / 4][i % 4], d_);
      Assert::AreEqual(value, test_.get(i / 4, i % 4));
      Assert::AreEqual(value, test_.get(i));
    }
  }

  TEST_METHOD(tranpose_test) {
    const catengine::Matrix4x4 transposed = {
      { 1.f, 5.f, 9.f, 13.f },
      { 2.f, 6.f, 10.f, 14.f },
      { 3.f, 7.f, 11.f, 15.f },
      { 4.f, 8.f, 12.f, 16.f }
    };

    catengine::Matrix4x4 mat = test_.transpose();

    Assert::IsTrue(transposed == mat, L"transpose fail");
    Assert::IsTrue(test_ == mat.transpose(), L"reverting tranpose failed");
  }

  TEST_METHOD(multiply) {
    const catengine::Matrix4x4 squared = {
      { 90.f, 100.f, 110.f, 120.f },
      { 202.f, 228.f, 254.f, 280.f },
      { 314.f, 356.f, 398.f, 440.f },
      { 426.f, 484.f, 542.f, 600.f }
    };
    catengine::Matrix4x4 mat = test_ * test_;

    Assert::IsTrue(squared == mat, L"multiplication is invalid");
  }

  TEST_METHOD(identity_multiply) {
    catengine::Matrix4x4 mat = test_ * catengine::Matrix4x4::create_identity();
    Assert::IsTrue(mat == test_, L"Multiplying by the identity should not modify the matrix.");
  }

private:
  const float d_ = 0.0001f;
  const catengine::Matrix4x4 test_ = {
    { 1.f, 2.f, 3.f, 4.f },
    { 5.f, 6.f, 7.f, 8.f },
    { 9.f, 10.f, 11.f, 12.f },
    { 13.f, 14.f, 15.f, 16.f }
  };
};
}