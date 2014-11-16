#include "stdafx.h"

#include <vector>

#include "Polygon.h"
#include "Vector2.h"

using Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace catenginetests {
  TEST_CLASS(Polygon_test) {
  public:
    TEST_METHOD(constructor_test) 
    {
      catengine::Polygon a;
      std::vector<catengine::Vector2> vertices;
      vertices.push_back(catengine::Vector2(0.0f, 0.0f));
      vertices.push_back(catengine::Vector2(1.0f, 0.0f));
      vertices.push_back(catengine::Vector2(1.0f, 1.0f));
      vertices.push_back(catengine::Vector2(0.0f, 1.0f));
      catengine::Polygon b(vertices);
      a = b;
      b.clear();
      Assert::AreEqual((size_t)4, a.size());
      Assert::AreEqual((size_t)0, b.size());
    }

    TEST_METHOD(add_test)
    {
      catengine::Polygon a;
      a.add(catengine::Vector2(0.0f, 0.0f));
      a.add(catengine::Vector2(1.0f, 0.0f));
      a.add(catengine::Vector2(1.0f, 1.0f));
      a.add(catengine::Vector2(0.0f, 1.0f));

      int i = 0;
      for (catengine::Vector2 v : a) {
        ++i;
      }
      Assert::AreEqual((size_t)4, a.size());

      a.insert(3, a[2]);
      Assert::AreEqual((size_t)5, a.size());
    }

    TEST_METHOD(remove_test)
    {
      catengine::Polygon a;
      a.add(catengine::Vector2(0.0f, 0.0f));
      a.add(catengine::Vector2(1.0f, 0.0f));
      a.add(catengine::Vector2(1.0f, 1.0f));
      a.add(catengine::Vector2(0.0f, 1.0f));
      a.remove(2);
      Assert::AreEqual((size_t)3, a.size());
      a.remove(0);
      Assert::AreEqual((size_t)2, a.size());
    }
  };
}