#include "stdafx.h"

#include "Point2d.h"
#include "Vector2d.h"

using Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace catenginetests {
TEST_CLASS(Point2d_test) {
public:
  TEST_METHOD(default_constructor_test) 
  {
    catengine::Point2d point;
    Assert::AreEqual(0.0f, point.x, d_);
    Assert::AreEqual(0.0f, point.y, d_);
  }

  TEST_METHOD(x_y_constructor_test) 
  {
    catengine::Point2d point(5.0f, 10.0f);
    Assert::AreEqual(5.0f, point.x, d_);
    Assert::AreEqual(10.0f, point.y, d_);
  }

  TEST_METHOD(addition_vector_test)
  {
    catengine::Point2d point_a(1.0f, 1.0f);
    catengine::Vector2d vector_a(5.0f, 10.0f);
    catengine::Point2d point_b = point_a + vector_a;
    Assert::AreEqual(6.0f, point_b.x, d_);
    Assert::AreEqual(11.0f, point_b.y, d_);
  }

  TEST_METHOD(addition_assignment_vector_test)
  {
    catengine::Point2d point(1.0f, 1.0f);
    catengine::Vector2d vector(5.0f, 10.0f);
    point += vector;
    Assert::AreEqual(6.0f, point.x, d_);
    Assert::AreEqual(11.0f, point.y, d_);
  }

  TEST_METHOD(subtraction_assignment_vector_test)
  {
    catengine::Point2d point(1.0f, 1.0f);
    catengine::Vector2d vector(5.0f, 10.0f);
    point -= vector;
    Assert::AreEqual(-4.0f, point.x, d_);
    Assert::AreEqual(-9.0f, point.y, d_);
  }

  TEST_METHOD(subtraction_vector_test)
  {
    catengine::Point2d point_a(1.0f, 1.0f);
    catengine::Vector2d vector_a(5.0f, 10.0f);
    catengine::Point2d point_b = point_a - vector_a;
    Assert::AreEqual(-4.0f, point_b.x, d_);
    Assert::AreEqual(-9.0f, point_b.y, d_);
  }

  TEST_METHOD(subtraction_point_test)
  {
    catengine::Point2d point_a(1.0f, 1.0f);
    catengine::Point2d point_b(5.0f, 10.0f);
    catengine::Vector2d dif = point_a - point_b;
    Assert::AreEqual(-4.0f, dif.x, d_);
    Assert::AreEqual(-9.0f, dif.y, d_);
  }

private:
  float d_ = 0.00001f;
};
}