#ifndef TEXTURE_H
#define TEXTURE_H

#include <d3d11.h>

#pragma comment(lib, "d3d11.lib")

namespace catengine {
class Texture {
private:
  typedef ID3D11ShaderResourceView* BaseType;

public:
  void load(std::string const& path);

  inline BaseType& get_data() { return data_; }
  inline BaseType const& get_data() const { return data_; }

private:
  BaseType data_;
};
}

#endif