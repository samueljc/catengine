#include "stdafx.h"

#include <sstream>
#include <iomanip>

#include "Camera.h"
#include "Matrix4x4.h"

using Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace catenginetest {
TEST_CLASS(Camera_test) {
public:
  TEST_METHOD(constructor_test_1) {
    catengine::Camera camera(width_, height_);

    Assert::AreEqual(0.f, camera.get_viewport().x, d_);
    Assert::AreEqual(0.f, camera.get_viewport().y, d_);
    Assert::AreEqual(width_, camera.get_viewport().width, d_);
    Assert::AreEqual(height_, camera.get_viewport().height, d_);
  }

  TEST_METHOD(constructor_test_2) {
    float x = 50.;
    float y = 100.f;
    catengine::Camera camera(x, y, width_, height_);

    Assert::AreEqual(x, camera.get_viewport().x, d_);
    Assert::AreEqual(y, camera.get_viewport().y, d_);
    Assert::AreEqual(width_, camera.get_viewport().width, d_);
    Assert::AreEqual(height_, camera.get_viewport().height, d_);
  }

  TEST_METHOD(reposition_test) {
    catengine::Camera cam1(width_, height_);
    cam1.reposition(50.f, 100.f);

    catengine::Camera cam2(50.f, 100.f, width_, height_);
    Assert::IsTrue(cam1.wvp_matrix() == cam2.wvp_matrix(), L"operations should be equivalent");
  }

  TEST_METHOD(resize_test) {
    catengine::Camera cam1(width_, height_);
    cam1.resize(1280.f, 900.f);

    catengine::Camera cam2(1280.f, 900.f);
    Assert::IsTrue(cam1.wvp_matrix() == cam2.wvp_matrix(), L"operations should be equivalent");
  }

  TEST_METHOD(vp_test) {
    catengine::Camera cam(50.f, 100.f, width_, height_);

    catengine::Matrix4x4 view = cam.view_matrix();
    catengine::Matrix4x4 proj = cam.proj_matrix();
    Assert::IsTrue((proj * view) == cam.vp_matrix(), L"operations should be equivalent");
  }

  TEST_METHOD(wvp_test) {
    catengine::Camera cam(50.f, 100.f, width_, height_);

    catengine::Matrix4x4 vp = cam.vp_matrix();
    catengine::Matrix4x4 world = cam.world_matrix();
    Assert::IsTrue((vp * world) == cam.wvp_matrix(), L"operations should be equivalent");
  }

private:
  const float width_ = 480.f;
  const float height_ = 640.f;
  const float d_ = 0.001f;
};
}