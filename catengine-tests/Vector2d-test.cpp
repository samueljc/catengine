#include "stdafx.h"

#include "Vector2d.h"

using Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace catenginetests {
TEST_CLASS(Vector2d_test) {
public:

  TEST_METHOD(default_constructor_test)
  {
    catengine::Vector2d a;
    Assert::AreEqual(0.0f, a.x, d_);
    Assert::AreEqual(0.0f, a.y, d_);
  }

  TEST_METHOD(x_y_constructor_test)
  {
    catengine::Vector2d b(5.0f, 10.0f);
    Assert::AreEqual(5.0f, b.x, d_);
    Assert::AreEqual(10.0f, b.y, d_);
  }

  TEST_METHOD(copy_constructor_test)
  {
    catengine::Vector2d b(5.0f, 10.0f);
    catengine::Vector2d c(b);
    Assert::AreEqual(5.0f, c.x, d_);
    Assert::AreEqual(10.0f, c.y, d_);
  }

  TEST_METHOD(addition_test)
  {
    // scalar addition
    catengine::Vector2d a;
    a += 5.0f;
    Assert::AreEqual(5.0f, a.x, d_);
    Assert::AreEqual(5.0f, a.y, d_);

    // vector addition
    catengine::Vector2d b;
    b += a;
    Assert::AreEqual(5.0f, b.x, d_);
    Assert::AreEqual(5.0f, b.y, d_);

    catengine::Vector2d c = a + b;
    Assert::AreEqual(a.x + b.x, c.x, d_);
    Assert::AreEqual(a.y + b.y, c.y, d_);
  }

  TEST_METHOD(subtraction_test)
  {
    // scalar addition
    catengine::Vector2d a;
    a -= 5.0f;
    Assert::AreEqual(-5.0f, a.x, d_);
    Assert::AreEqual(-5.0f, a.y, d_);

    // vector addition
    catengine::Vector2d b;
    b -= a;
    Assert::AreEqual(5.0f, b.x, d_);
    Assert::AreEqual(5.0f, b.y, d_);

    catengine::Vector2d c = a - b;
    Assert::AreEqual(a.x - b.x, c.x, d_);
    Assert::AreEqual(a.y - b.y, c.y, d_);
  }

  TEST_METHOD(multiplication_test)
  {
    catengine::Vector2d a(1.0f, 2.0f);
    a *= 5.0f;
    Assert::AreEqual(5.0f, a.x, d_);
    Assert::AreEqual(10.0f, a.y, d_);

    catengine::Vector2d b = a * a;
    Assert::AreEqual(25.0f, b.x, d_);
    Assert::AreEqual(100.0f, b.y, d_);
  }

  TEST_METHOD(magnitude_test)
  {
    catengine::Vector2d a(4.0f, -3.0f);
    float mag = a.mag();
    float sqmag = a.sqmag();
    Assert::AreEqual(mag, 5.0f, d_);
    Assert::AreEqual(sqmag, 25.0f, d_);
  }

  TEST_METHOD(normalize_test)
  {
    catengine::Vector2d a(1.0f, 0.0f);
    catengine::Vector2d norm = a.normal();
    Assert::AreEqual(a.x, norm.x, d_);
    Assert::AreEqual(a.y, norm.y, d_);

    a.x = 5.0f;
    a.normalize();
    Assert::AreEqual(norm.x, a.x);
    Assert::AreEqual(norm.y, a.y);

    catengine::Vector2d b(4.0f, -3.0f);
    b.normalize();
    Assert::AreEqual(4.0f / 5.0f, b.x, d_);
    Assert::AreEqual(-3.0f / 5.0f, b.y, d_);
  }

  TEST_METHOD(equality_test)
  {
    catengine::Vector2d a(1.0f, 1.0f);
    catengine::Vector2d b(2.0f, 1.0f);
    Assert::IsFalse(a == b);
    Assert::IsTrue(a == a);
    b.x = a.x;
    b.y = a.y;
    Assert::IsTrue(a == b);
  }

  // todo: better cross product test
  TEST_METHOD(cross_product_test)
  {
    catengine::Vector2d a(5.0f, 3.0f);
    catengine::Vector2d b(5.0f, 3.0f);
    Assert::AreEqual(0.0f, a.cross(b), d_);
  }

  TEST_METHOD(dot_product_test)
  {
    catengine::Vector2d a(1.0f, 0.0f);
    catengine::Vector2d b(0.0f, 1.0f);
    Assert::AreEqual(0.0f, a.dot(b), d_);

    b = a * 5.0f;
    Assert::AreEqual(b.dot(a), a.mag() * b.mag(), d_);
  }

private:
  float d_ = 0.00001f;
};
}