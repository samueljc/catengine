#include "stdafx.h"

#include "QuadTree.h"
#include "GameObject.h"
#include "Rectangle.h"

namespace catenginetest {
  TEST_CLASS(QuadTree_test) {
    TEST_METHOD(constructor_test) 
    {
      catengine::QuadTree tree(catengine::Rectangle(0, 0, 100, 100));
    }

    TEST_METHOD(insertion_test)
    {
      catengine::QuadTree tree(catengine::Rectangle(0, 0, 100, 100));
      catengine::GameObject o;
      tree.insert(&o);
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