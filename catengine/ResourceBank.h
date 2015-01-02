#ifndef RESOURCE_BANK_H
#define RESOURCE_BANK_H

#include <string>

#include "Exception.h"
#include "Texture.h"
#include "Map.h"

namespace catengine {
class ResourceBank {
public:
  inline Texture get_texture(std::string const& key) const
  {

  }

  inline void set_texture(std::string key, Texture val)
  {

  }

  inline void release_texture(std::string const& key)
  {

  }

private:
  Map<std::string, Texture> textures_;
};
}

#endif