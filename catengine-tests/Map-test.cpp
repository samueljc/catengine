#include "stdafx.h"

#include "Map.h"

using Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace catenginetest {
TEST_CLASS(Map_test) {
  TEST_METHOD(constructor_test)
  {
    catengine::Map<std::string, std::string> map;
    auto copy_map = map;
  }

  TEST_METHOD(set_override_test)
  {
    catengine::Map<std::string, std::string> map;
    map.set("key", "value");
    map.set("key", "val");
    Assert::AreEqual(std::string("val"), map.get("key"));
  }

  TEST_METHOD(defined_test)
  {
    catengine::Map<std::string, std::string> map;
    map.set("key", "value");
    Assert::AreEqual(true, map.defined("key"));
    Assert::AreEqual(false, map.defined("not key"));
  }

  TEST_METHOD(get_test)
  {
    catengine::Map<std::string, std::string> map;
    map.set("key", "value");
    Assert::AreEqual(std::string("value"), map.get("key"));
  }

  TEST_METHOD(unset_test)
  {
    catengine::Map<std::string, std::string> map;
    map.set("key", "value");
    map.unset("key");
    Assert::AreEqual(0u, map.size());
  }

  TEST_METHOD(size_test)
  {
    catengine::Map<std::string, std::string> map;
    map.set("key1", "value");
    map.set("key2", "value");
    map.set("key3", "value");
    Assert::AreEqual(3u, map.size());
  }

  TEST_METHOD(clear_test)
  {
    catengine::Map<std::string, std::string> map;
    map.set("key", "value");
    map.clear();
    Assert::AreEqual(0u, map.size());
  }

  TEST_METHOD(iterator_test)
  {
    catengine::Map<std::string, std::string> map;
    map.set("key1", "value");
    map.set("key2", "value");
    map.set("key3", "value");
    catengine::_unsigned i = 0u;
    for (auto const& pair : map)
      ++i;
    Assert::AreEqual(3u, i);
  }
};
}