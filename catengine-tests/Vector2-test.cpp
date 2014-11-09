#include "stdafx.h"
#include "Vector2.h"

#define Test Microsoft::VisualStudio::CppUnitTestFramework

namespace catenginetests {
  TEST_CLASS(Vector2_test) {
  public:

    TEST_METHOD(construction_test)
    {
      catengine::Vector2 a;
      Test::Assert::AreEqual(0.0f, a.x);
      Test::Assert::AreEqual(0.0f, a.y);

      catengine::Vector2 b(5.0f, 10.0f);
      Test::Assert::AreEqual(5.0f, b.x);
      Test::Assert::AreEqual(10.0f, b.y);

      catengine::Vector2 c(b);
      Test::Assert::AreEqual(5.0f, c.x);
      Test::Assert::AreEqual(10.0f, c.y);
    }

    TEST_METHOD(addition_test)
    {
      // scalar addition
      catengine::Vector2 a;
      a += 5.0f;
      Test::Assert::AreEqual(5.0f, a.x);
      Test::Assert::AreEqual(5.0f, a.y);

      // vector addition
      catengine::Vector2 b;
      b += a;
      Test::Assert::AreEqual(5.0f, b.x);
      Test::Assert::AreEqual(5.0f, b.y);

      catengine::Vector2 c = a + b;
      Test::Assert::AreEqual(a.x + b.x, c.x);
      Test::Assert::AreEqual(a.y + b.y, c.y);
    }

    TEST_METHOD(subtraction_test)
    {
      // scalar addition
      catengine::Vector2 a;
      a -= 5.0f;
      Test::Assert::AreEqual(-5.0f, a.x);
      Test::Assert::AreEqual(-5.0f, a.y);

      // vector addition
      catengine::Vector2 b;
      b -= a;
      Test::Assert::AreEqual(5.0f, b.x);
      Test::Assert::AreEqual(5.0f, b.y);

      catengine::Vector2 c = a - b;
      Test::Assert::AreEqual(a.x - b.x, c.x);
      Test::Assert::AreEqual(a.y - b.y, c.y);
    }

    TEST_METHOD(multiplication_test)
    {
      catengine::Vector2 a(1.0f, 2.0f);
      a *= 5.0f;
      Test::Assert::AreEqual(5.0f, a.x);
      Test::Assert::AreEqual(10.0f, a.y);

      catengine::Vector2 b = a * a;
      Test::Assert::AreEqual(25.0f, b.x);
      Test::Assert::AreEqual(100.0f, b.y);
    }

    TEST_METHOD(magnitude_test)
    {
      catengine::Vector2 a(4.0f, -3.0f);
      float mag = a.mag();
      float sqmag = a.sqmag();
      Test::Assert::AreEqual(mag, 5.0f, d_);
      Test::Assert::AreEqual(sqmag, 25.0f, d_);
    }

    TEST_METHOD(normalize_test)
    {
      catengine::Vector2 a(1.0f, 0.0f);
      catengine::Vector2 norm = a.normal();
      Test::Assert::AreEqual(a.x, norm.x, d_);
      Test::Assert::AreEqual(a.y, norm.y, d_);

      a.x = 5.0f;
      a.normalize();
      Test::Assert::AreEqual(norm.x, a.x);
      Test::Assert::AreEqual(norm.y, a.y);

      catengine::Vector2 b(4.0f, -3.0f);
      b.normalize();
      Test::Assert::AreEqual(4.0f / 5.0f, b.x, d_);
      Test::Assert::AreEqual(-3.0f / 5.0f, b.y, d_);
    }

    TEST_METHOD(equality_test)
    {
      catengine::Vector2 a(1.0f, 1.0f);
      catengine::Vector2 b(2.0f, 1.0f);
      Test::Assert::IsFalse(a == b);
      Test::Assert::IsTrue(a == a);
      b.x = a.x;
      b.y = a.y;
      Test::Assert::IsTrue(a == b);
    }

    TEST_METHOD(cross_product_test)
    {
      catengine::Vector2 a(5.0f, 3.0f);
      catengine::Vector2 b(5.0f, 3.0f);
      Test::Assert::AreEqual(0.0f, a.cross(b), d_);
    }

    TEST_METHOD(dot_product_test)
    {
      catengine::Vector2 a(1.0f, 0.0f);
      catengine::Vector2 b(0.0f, 1.0f);
      Test::Assert::AreEqual(0.0f, a.dot(b), d_);

      b = a * 5.0f;
      Test::Assert::AreEqual(b.dot(a), a.mag() * b.mag(), d_);
    }

  private:
    float d_ = 0.00001f;
  };
}