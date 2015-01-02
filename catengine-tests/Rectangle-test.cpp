#include "stdafx.h"

#include "Rectangle.h"
#include "Vector2d.h"

using Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace catenginetests {
TEST_CLASS(Rectangle_test) {
public:
  TEST_METHOD(default_constructor_test)
  {
    catengine::Rectangle r;
    Assert::AreEqual(0.0f, r.x, d_);
    Assert::AreEqual(0.0f, r.y, d_);
    Assert::AreEqual(0.0f, r.width, d_);
    Assert::AreEqual(0.0f, r.height, d_);
  }

  TEST_METHOD(full_constructor_test)
  {
    catengine::Rectangle r(0.0f, 0.0f, 10.0f, 10.0f);
    Assert::AreEqual(0.0f, r.x, d_);
    Assert::AreEqual(0.0f, r.y, d_);
    Assert::AreEqual(10.0f, r.width, d_);
    Assert::AreEqual(10.0f, r.height, d_);
  }

  TEST_METHOD(left_test)
  {
    catengine::Rectangle r(5.0f, 5.0f, 10.0f, 10.0f);
    Assert::AreEqual(5.0f, r.left(), d_);
  }

  TEST_METHOD(right_test)
  {
    catengine::Rectangle r(5.0f, 5.0f, 10.0f, 10.0f);
    Assert::AreEqual(15.0f, r.right(), d_);
  }

  TEST_METHOD(top_test)
  {
    catengine::Rectangle r(5.0f, 5.0f, 10.0f, 10.0f);
    Assert::AreEqual(5.0f, r.top(), d_);
  }

  TEST_METHOD(bottom_test)
  {
    catengine::Rectangle r(5.0f, 5.0f, 10.0f, 10.0f);
    Assert::AreEqual(15.0f, r.bottom(), d_);
  }

  TEST_METHOD(center_x_test)
  {
    catengine::Rectangle r(5.0f, 5.0f, 10.0f, 10.0f);
    Assert::AreEqual(7.5f, r.bottom(), d_);
  }

  TEST_METHOD(center_y_test)
  {
    catengine::Rectangle r(5.0f, 5.0f, 10.0f, 10.0f);
    Assert::AreEqual(7.5f, r.bottom(), d_);
  }

  TEST_METHOD(equality_test)
  {
    catengine::Rectangle r1(5.0f, 5.0f, 10.0f, 10.0f);
    catengine::Rectangle r2(5.0f, 5.0f, 10.0f, 10.0f);
    Assert::IsTrue(r1 == r2);
    r2.width = -r2.width;
    Assert::IsFalse(r1 == r2);
  }

  TEST_METHOD(inequality_test)
  {
    catengine::Rectangle r1(5.0f, 5.0f, 10.0f, 10.0f);
    catengine::Rectangle r2(5.0f, 5.0f, 10.0f, 10.0f);
    Assert::IsFalse(r1 != r2);
    r2.width = -r2.width;
    Assert::IsTrue(r1 != r2);
  }

  TEST_METHOD(vector_addition_test)
  {
    catengine::Rectangle r(5.0f, 5.0f, 10.0f, 10.0f);
    catengine::Vector2d v(10.0f, 10.0f);
    catengine::Rectangle r2 = r + v;
    Assert::AreEqual(15.0f, r2.x, d_);
    Assert::AreEqual(15.0f, r2.y, d_);
    Assert::AreEqual(r.width, r2.width, d_);
    Assert::AreEqual(r.height, r2.height, d_);
  }

  TEST_METHOD(vector_addition_assignment_test)
  {
    catengine::Rectangle r(5.0f, 5.0f, 10.0f, 10.0f);
    catengine::Vector2d v(10.0f, 10.0f);
    r += v;
    Assert::AreEqual(15.0f, r.x, d_);
    Assert::AreEqual(15.0f, r.y, d_);
    Assert::AreEqual(10.0f, r.width, d_);
    Assert::AreEqual(10.0f, r.height, d_);
  }

  TEST_METHOD(vector_subtraction_test)
  {
    catengine::Rectangle r(5.0f, 5.0f, 10.0f, 10.0f);
    catengine::Vector2d v(10.0f, 10.0f);
    catengine::Rectangle r2 = r - v;
    Assert::AreEqual(-5.0f, r2.x, d_);
    Assert::AreEqual(-5.0f, r2.y, d_);
    Assert::AreEqual(r.width, r2.width, d_);
    Assert::AreEqual(r.height, r2.height, d_);
  }

  TEST_METHOD(vector_subtraction_assignment_test)
  {
    catengine::Rectangle r(5.0f, 5.0f, 10.0f, 10.0f);
    catengine::Vector2d v(10.0f, 10.0f);
    r -= v;
    Assert::AreEqual(-5.0f, r.x, d_);
    Assert::AreEqual(-5.0f, r.y, d_);
    Assert::AreEqual(10.0f, r.width, d_);
    Assert::AreEqual(10.0f, r.height, d_);
  }

private:
  float d_ = 0.00001f;
};
}