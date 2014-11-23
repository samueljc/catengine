#include "stdafx.h"

#include <sstream>
#include <iomanip>

#include "Color.h"

using Microsoft::VisualStudio::CppUnitTestFramework::Assert;

template<> static 
std::wstring Microsoft::VisualStudio::CppUnitTestFramework::ToString<catengine::Color>(catengine::Color const& t) {
  std::wostringstream os;
  os << std::hex << "0x" << std::setfill(L'0') << std::setw(8) << static_cast<catengine::_unsigned>(t);
  return os.str();
}

namespace catenginetest {
  TEST_CLASS(Color_test) {
    TEST_METHOD(default_constructor_test) {
      catengine::Color c;
      Assert::AreEqual(catengine::Color::BLACK, c);
    }

    TEST_METHOD(copy_constructor_test) {
      catengine::Color c(catengine::Color::BLUE);
      Assert::AreEqual(catengine::Color::BLUE, c);
    }

    TEST_METHOD(float_constructor_test) {
      catengine::Color c(420.0f, 69.0f, 1.0f); // should bound it
      Assert::AreEqual(catengine::Color::WHITE, c);
    }

    TEST_METHOD(rgb_int_constructor_test) {
      catengine::Color c(420u, 8080u, 0u); // should bound it
      Assert::AreEqual(catengine::Color::YELLOW, c);
    }

    TEST_METHOD(int_constructor_test) {
      catengine::Color c(0xcfff00ff);
      Assert::AreEqual(catengine::Color::ELECTRIC_LIME, c);
    }

    TEST_METHOD(unsigned_cast_test) {
      catengine::Color c(catengine::Color::ELECTRIC_LIME);
      catengine::_unsigned u = static_cast<catengine::_unsigned>(c);
      Assert::AreEqual(0xcfff00ff, u);
    }

    TEST_METHOD(string_cast_test) {
      std::string s = catengine::Color::RED;
      Assert::AreEqual(std::string("0xff0000ff"), s);
    }
  };
}