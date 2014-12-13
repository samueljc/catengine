#include "stdafx.h"

#include <vector>

#include "QuadTree.h"
#include "GameObject.h"
#include "Rectangle.h"

using Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace catenginetest {
  TEST_CLASS(QuadTree_test) {
    TEST_METHOD(constructor_test) 
    {
      catengine::QuadTree tree(catengine::Rectangle(0, 0, 100, 100));
    }

    TEST_METHOD(insertion_test)
    {
      catengine::QuadTree tree(catengine::Rectangle(0, 0, 100, 100));
      std::vector<catengine::GameObject> objects;
      for (int i = 0; i < 100; ++i) {
        objects.emplace_back();
        tree.insert(&(objects.back()));
      }
      Assert::AreEqual(100, (int)tree.count());
      tree.clear();
    }

    TEST_METHOD(collection_test)
    {
      catengine::QuadTree tree(catengine::Rectangle(0, 0, 100, 100));
    }

    TEST_METHOD(rebalance_test)
    {
      catengine::QuadTree tree(catengine::Rectangle(0, 0, 100, 100));
    }
  };
}