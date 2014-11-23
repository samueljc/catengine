#include "stdafx.h"
#include "MathF.h"
#include <cmath>
#include <random>

using Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace catenginetests {
  TEST_CLASS(MathF_test) {
  public:

    TEST_CLASS_INITIALIZE(MathF_test_initialize) {
      count_ = 8;
      ad_ = new float[8]{ 0.0f, 15.0f, 30.0f, 45.0f, 90.0f, 180.0f, 270.0f, 360.0f };
      ar_ = new float[8]{ 0.0f, 0.261799388f, 0.523598776f, 0.785398163f, 1.57079633f, 3.14159265f, 4.71238898f, 6.28318531f };
    }

    TEST_CLASS_CLEANUP(MathF_test_cleanup) {
      if (ad_ != nullptr) delete[] ad_;
      if (ar_ != nullptr) delete[] ar_;
    }

    TEST_METHOD(sqrt_test)
    {
      float s = catengine::sqrt(0.0f);
      Assert::AreEqual(0.0f, s, d_);
      s = catengine::sqrt(4.0f);
      Assert::AreEqual(2.0f, s, d_);
    }

    TEST_METHOD(rad2deg_test) {
      for (size_t i = 0; i < count_; ++i)
        Assert::AreEqual(ad_[i], catengine::rad2deg(ar_[i]), d_);
    }

    TEST_METHOD(deg2rad_test) {
      for (size_t i = 0; i < count_; ++i)
        Assert::AreEqual(ar_[i], catengine::deg2rad(ad_[i]), d_);
    }

    TEST_METHOD(sin_test) {
      for (size_t i = 0; i < count_; ++i)
        Assert::AreEqual(std::sin(ar_[i]), catengine::sin(ar_[i]), d_);
    }

    TEST_METHOD(cos_test) {
      for (size_t i = 0; i < count_; ++i)
        Assert::AreEqual(std::cos(ar_[i]), catengine::cos(ar_[i]), d_);
    }

    TEST_METHOD(tan_test) {
      for (size_t i = 0; i < count_; ++i)
        Assert::AreEqual(std::tan(ar_[i]), catengine::tan(ar_[i]), d_);
    }

  private:
    static size_t count_;
    static float* ad_;
    static float* ar_;
    static float d_;
  };

  size_t MathF_test::count_ = 8;
  float* MathF_test::ad_ = nullptr;
  float* MathF_test::ar_ = nullptr;
  float MathF_test::d_ = 0.00001f;
}