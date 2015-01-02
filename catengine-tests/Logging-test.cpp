#include "stdafx.h"

#include "Logging.h"

using Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace catenginetest {
TEST_CLASS(Logging_test) {
  TEST_METHOD(info_test) {
    LOG(INFO) << "info test passed";
  }

  TEST_METHOD(warning_test) {
    LOG(WARNING) << "warning test passed";
  }

  TEST_METHOD(error_test) {
    LOG(ERROR) << "error test passed";
  }
};
}