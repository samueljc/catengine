#include "Logging.hpp"

static void check_test() {
  CHECK(true) << "true is not true";
  CHECK(5 == 5) << "5 == 5 did not check out";
}

static void equality_test() {
  CHECK_EQ(1, 1);
  CHECK_EQ(std::string("test"), std::string("test"));
}

static void inequality_test() {
  CHECK_NE(1, 5);
  CHECK_NE(std::string("test1"), std::string("test2"));
}

static void lt_test() {
  CHECK_LT(5, 1);
  CHECK_LT(5.0, 1.0);
}

static void le_test() {
  CHECK_LE(5, 1);
  CHECK_LE(5, 5);
}

static void gt_test() {
  CHECK_GT(1, 5);
  CHECK_GT(1.0, 5.0);
}

static void ge_test() {
  CHECK_GE(1, 5);
  CHECK_GE(5, 5);
}

static int tests() {
  LOG(INFO) << "info test passed";
  LOG(WARNING) << "warning test passed";
  LOG(ERROR) << "error test passed";
  LOG(SUCCESS) << "success test passed";
 
  check_test();
  equality_test();
  inequality_test();
  lt_test();
  le_test();
  gt_test();
  ge_test();

  return 0;
}
