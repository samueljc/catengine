#include "stdafx.h"
#include "MathF.h"
#include <cmath>
#include <random>

#define Test Microsoft::VisualStudio::CppUnitTestFramework

namespace catenginetests {
  TEST_CLASS(MathF_test) {
  public:
    TEST_METHOD(sqrt_test)
    {
      float s = catengine::sqrt(0.0f);
      Test::Assert::AreEqual(0.0f, s, d_);
      s = catengine::sqrt(4.0f);
      Test::Assert::AreEqual(2.0f, s, d_);
    }

    TEST_METHOD(trig_test)
    {
      size_t count = 8;
      float ad[] = { 0.0f, 15.0f, 30.0f, 45.0f, 90.0f, 180.0f, 270.0f, 360.0f };
      float ar[] = { 0.0f, 0.261799388f, 0.523598776f, 0.785398163f, 1.57079633f, 3.14159265f, 4.71238898f, 6.28318531f };

      for (size_t i = 0; i < count; ++i) {
        Test::Assert::AreEqual(ad[i], catengine::rad2deg(ar[i]), d_);
        Test::Assert::AreEqual(ar[i], catengine::deg2rad(ad[i]), d_);
      }

      for (float a : ar) {
        Test::Assert::AreEqual(std::sin(a), catengine::sin(a), d_);
        Test::Assert::AreEqual(std::cos(a), catengine::cos(a), d_);
        Test::Assert::AreEqual(std::tan(a), catengine::tan(a), d_);
      }
    }

  private:
    float d_ = 0.00001f;
  };
}